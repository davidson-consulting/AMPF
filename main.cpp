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
#include "exact_solution.h"
#include "heuristic.h"

int main(int argc, char *argv[]){
IloEnv env;
cout<<"***cplex***"<<endl;
//############ read the substrate network ##########
char *name_file_network=new char[strlen(argv[1])+10];
name_file_network=strcpy(name_file_network, argv[1]);
Json::Value network;
read_instance(&network,name_file_network);
//############ read the slice ##########
char *name_file_slice=new char[strlen(argv[2])+10];
name_file_slice=strcpy(name_file_slice, argv[2]);
Json::Value slice;
read_instance(&slice,name_file_slice);
//###########read test_network_all_paths ###########
char *name_file_all_paths=new char[80];
name_file_all_paths=strcpy(name_file_all_paths, argv[1]);
if (std::stoi(argv[4])==1)
name_file_all_paths=strcat(name_file_all_paths, "_widest_paths ");
else
name_file_all_paths=strcat(name_file_all_paths, "_shortest_paths ");
name_file_all_paths=strcat(name_file_all_paths, argv[3]);
Json::Value all_paths;
read_instance(&all_paths,name_file_all_paths);
cout<<name_file_all_paths<<endl;
//#########################################################
int m= network["m"].asInt();
int m1= slice["m"].asInt();
int n=network["n"].asInt();
int n1=slice["n"].asInt();
cout<<"n=="<<n<<endl;
cout<<"n1=="<<n1<<endl;
float start = clock(); 
assert(minn(10, 20)==10);
int** d= NULL;
d=new int*[n];
for (int i=0; i<n; i++) {d[i]=new int[n];}
int** pp= NULL;
pp=new int*[n];
for (int i=0; i<n; i++) {pp[i]=new int[n];}
int** edge= NULL;
edge=new int*[n];
for (int i=0; i<n; i++) {edge[i]=new int[n];}
int * path;
path= new int [n];
int **paths;
int mm=int(n*(n-1)/2);
int max_paths=std::stoi(argv[3]);
int nbr_path=0;
int nbr=0;
nbr_path=all_paths["somme_len"].asInt();
paths=new int*[nbr_path];
for (int i=0; i<mm; i++) {
        for (int j=0; j<all_paths["all_paths"][i]["len"].asInt(); j++) {
            paths[nbr]=new int[all_paths["all_paths"][i]["simplePathslens"][j].asInt()+1];
           paths[nbr][0]=all_paths["all_paths"][i]["simplePathslens"][j].asInt();
            for (int k=0; k<paths[nbr][0]; k++) {
                paths[nbr][k+1]=all_paths["all_paths"][i]["simplePaths"][j][k].asInt();
                }
        nbr++;
    }}
//calculate costs
for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
        d[i][j]=0;}}
for (int i=0; i<m; i++) {
        d[network["edges"][i]["e"][0].asInt()][network["edges"][i]["e"][1].asInt()]=network["edges"][i]["weight"].asInt();
        d[network["edges"][i]["e"][1].asInt()][network["edges"][i]["e"][0].asInt()]=network["edges"][i]["weight"].asInt();}
int* costs=new int[nbr_path];
for (int i=0; i<nbr_path; i++) {
    costs[i]=0;
    for (int k=1; k<paths[i][0]; k++) {
    costs[i]=costs[i]+d[paths[i][k]][paths[i][k+1]];
    }
}
//lancer Cplex
IloModel model(env);
char *solution_name=new char[strlen(argv[2])+50];
solution_name=strcpy(solution_name, argv[2]);
solution_name=strcat(solution_name, " method_1");
int solution_realisable=2;
double Timelimit=std::stoi(argv[5]);

Json::Value edge_affectations;
int * affectation; 
save_solution (solution_name,affectation,edge_affectations,0, 0, start, n, n1,m1,0,slice);  
//**************************cplex************************
if (max_paths==1)
    phase2 (model,network,slice,all_paths,paths,nbr_path,solution_name,edge, &start,&solution_realisable,costs,Timelimit,0);
else
    exact_model (model,network,slice,all_paths,paths,nbr_path,solution_name, &start,&solution_realisable,costs,Timelimit,0);
for (int i=0; i<n; i++) {delete(d[i]);}
delete d;
for (int i=0; i<n; i++) {delete(pp[i]);}
delete(pp);
for (int i=0; i<n; i++) {delete(edge[i]);}
delete(edge);
delete(path);
for (int i=0; i<nbr_path; i++) {delete(paths[i]);}
delete(paths);
delete(solution_name);
delete (name_file_network);
delete(name_file_slice);
delete(name_file_all_paths);
delete(costs);
delete(affectation);
return 0;
}

