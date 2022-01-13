/*Copyright (C) <2022>  <AIT ABA Massinissa>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
#include "instance.h"
#include "heuristic.h"


void phase2 (IloModel model,Json::Value network,Json::Value slice,Json::Value all_paths,int** paths, int nbr_path, string solution_name,int** edge,float* start, int* solution_realisable,int* costs, double Timelimit, double times_paths){
try {
IloEnv env = model.getEnv();
//############################Variables######################################
int n= network["n"].asInt();
int n1= slice["n"].asInt();
int m_complete=int(n*(n-1)/2);
int m1= slice["m"].asInt();

IntVarMatrix x(env, n1);
for (int j = 0; j < n1; j++){
    x[j] = IloIntVarArray(env, n, 0, 1);}

IntVarMatrix y(env, m1);
for (int j = 0; j < m1; j++) {
    y[j] = IloIntVarArray(env, nbr_path, 0, 1);}

IloIntVarArray z = IloIntVarArray(env, nbr_path, 0, 1);
IloIntVarArray p = IloIntVarArray(env, n, 0, 1);

IloNumVar cost;
cost=IloNumVar(env, 0, IloInfinity);
//################################constraints############################################
 for (int  i = 0; i < n1; i++) {
    IloExpr Pe_affectation(env);
    for (int j = 0; j < n; j++) {
        Pe_affectation+=x[i][j];
            }
    model.add(Pe_affectation == 1);
        }
 for (int  j = 0; j < n; j++) {
    IloExpr Pe_affectation_max(env);
    for (int i = 0; i < n1; i++) {
        Pe_affectation_max+=x[i][j];
            }
    model.add(Pe_affectation_max <= 1);
        }
 for (int  i = 0; i < n1; i++) {
    for (int j = 0; j < n; j++) {
        model.add(x[i][j]*slice["nodes_cap"][IntToString(i)].asInt() <=network["nodes_cap"][IntToString(j)].asInt());
        }}
int m= network["m"].asInt();

int in1,in2,is1,is2,indx;
indx=0;
for (int  i = 0; i < m1; i++) {
    is1=slice["edges"][i]["e"][0].asInt();
    is2=slice["edges"][i]["e"][1].asInt();
    for (int  j = 0; j < nbr_path; j++) {
        in1=paths[j][1];
        in2=paths[j][paths[j][0]];
        model.add((x[is1][in1]+x[is2][in2])-1<= y[i][j]);
        model.add((x[is1][in2]+x[is2][in1])-1<= y[i][j]);
        }}
for (int  i = 0; i < m1; i++) {
    IloExpr e_affectation(env);
    for (  indx = 0; indx < nbr_path; indx++) {
        e_affectation+=y[i][indx];}
    model.add(e_affectation==1);}


int **edge_in = (int **)malloc(m * sizeof(int*));
int total=0;
for (int  i = 0; i < m; i++) {
    edge_in[i] = (int *)malloc(sizeof(int) );
    in1=network["edges"][i]["e"][0].asInt();
    in2=network["edges"][i]["e"][1].asInt();
    total=0;
    for (int  j = 0; j < nbr_path; j++) {
        if (in_paths(in1,in2,j,paths)==1){
            total=total+1;
            edge_in[i] = (int *)realloc( edge_in[i], (total+1) * sizeof(int) );
            edge_in[i][total]=j; 
        }}
    edge_in[i][0]=total; 
}
int weight;
for (int  i = 0; i < m; i++) {
    in1=network["edges"][i]["e"][0].asInt();
    in2=network["edges"][i]["e"][1].asInt();
    weight=network["edges"][i]["weight"].asInt();
    IloExpr consumed(env);
    for (int  j = 0; j < m1; j++) {
        for (int  k = 0; k <edge_in[i][0] ; k++) {  
            consumed+=y[j][edge_in[i][k+1]]*slice["edges"][j]["weight"].asInt();
    }}
    model.add(consumed<=weight);}

// ##################  objectif  ###################
IloExpr total_cost(env); 
for (int  i = 0; i < n1; i++) {
    for (int  j = 0; j < n; j++) {
        model.add(x[i][j]<=p[j]);}}
for (int  i = 0; i < n; i++) {
    total_cost+= p[i]*network["nodes_cap"][IntToString(i)].asInt();
        }         
for (int  in1 = 0; in1 < n; in1++) {
    for (int  in2 = in1+1; in2 < n; in2++) {
        indx=indexx(in1,in2,n);
        IloExpr affected(env);
        for (int  k = 0; k < m1; k++) {
            affected+=y[k][indx];}
        model.add(affected<=z[indx]);
        total_cost+=z[indx]*costs[indx];
                 }}
model.add(total_cost<=cost);
    //lunch cplex
    IloObjective obj = IloMinimize(env, cost);
    model.add(obj);
    IloCplex cplex(model);
    cplex.setParam(IloCplex::Param::TimeLimit, Timelimit);
    auto S = chrono::steady_clock::now();
    cplex.solve();
    auto end = chrono::steady_clock::now();
    auto runtime=chrono::duration_cast<chrono::milliseconds>(end - S).count()/1000.0;
    cout<<"runtime== "<<runtime<<endl;
    *start=runtime;
    double xij=0;
    //get and show nodes affectations 
    int * affectation=new int[n1];
     for (int  i = 0; i < n1; i++) {
        for (int  j = 0; j < n; j++) {
            xij=cplex.getValue(x[i][j]);
            if (xij>0){
                if (xij<0.99) {
                    *solution_realisable=0;}
                else {
                    *solution_realisable=1;
                    affectation[i]=j;}
                        }
            }}
    cout<<endl;
    cout<<"cost= "<<cplex.getValue(cost)<<endl;
    //get edge affectations 
      Json::Value edge_affectations; 
    double you;
    for (int  i = 0; i < m1; i++) {
        is1=slice["edges"][i]["e"][0].asInt();
        is2=slice["edges"][i]["e"][1].asInt();
        edge_affectations[i]["i"]=is1;
        edge_affectations[i]["j"]=is2;
        if (*solution_realisable>0){  
            edge_affectations[i]["i aff"]= affectation[is1];
            edge_affectations[i]["j aff"]= affectation[is2];}
        edge_affectations[i]["weight"]=slice["edges"][i]["weight"].asInt();
        Json::Value vec(Json::arrayValue);
        for (int  j = 0; j < nbr_path; j++) {
            you=cplex.getValue(y[i][j]);
            if (you>0){
                cout<<i<<" eme edge to paths["<<j<<"][1] "<<paths[j][1]<<" y[i][j]="<<you<<" len= "<<paths[j][0]<<endl;
                edge_affectations[i]["path_index"]=j;
                edge_affectations[i]["path_length"]=paths[j][0];
                for (int k=0; k<paths[j][0]; k++) {
                    vec.append(Json::Value(paths[j][k+1]));
                }
            }
        }
        edge_affectations[i]["path"]=vec;
    }                   
int check_sol=0;
if (*solution_realisable==1){
check_sol=check(edge_affectations, affectation,network,slice, n, n1, m1, m,paths,costs);
assert(check_sol==1);
cout<<endl<<"****** check = "<<check_sol<<" *********"<<endl<<endl;}       
    if (*solution_realisable==1 && check_sol==1){
    save_solution (solution_name, affectation,edge_affectations,*solution_realisable, cplex.getValue(cost),times_paths+ runtime, n, n1,m1,check_sol,slice);}

//memory
    for (int  k = 0; k < m; k++) {
    free (edge_in[k]);}
    free(edge_in);
    delete(affectation);
    obj.end();
    cplex.end();
    cost.end();
    x.end();
    y.end();
    p.end();
    z.end();
    env.end();
    //model.end();
}
catch (IloException& e) {
cerr << "Concert exception caught: " << e << endl;
}
catch (...) {
cerr << "Unknown exception caught" << endl;
}
}