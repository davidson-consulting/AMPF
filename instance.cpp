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


string IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

string FloatToString (float a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

void read_instance(Json::Value* root,string file_name){
std::ifstream file_input(file_name);
Json::Reader reader;
reader.parse(file_input, *root);
}


void save_solution(string solution_name,int* affectation,Json::Value edge_affectations,int solution_realisable,int cost,float runtime,int n,int n1,int m1,int check,Json::Value slice){
    int revenue=0;

    Json::Value data;   
    cout<<"save_solution "<<solution_realisable<<endl;
    for (int  i = 0; i < m1; i++) {
    revenue=revenue+slice["edges"][i]["weight"].asInt();}
    for (int  i = 0; i < n1; i++) {
    revenue=revenue+slice["nodes_cap"][IntToString(i)].asInt();}
    data["n"]=n;
    data["n1"]=n1;
    data["m1"]=m1;
    if (solution_realisable==1){
    for (int  i = 0; i < n1; i++) {
        data["Affectation_nodes"][i]=affectation[i];
    }
    data["Affectation_edges"]=edge_affectations;}
    data["solution_realisable"]=solution_realisable;
    data["revenue"]=revenue;
    data["consumed_resources"]=cost;
    data["cost"]=1.0*revenue/cost;
    data["runtime"]=runtime;
    data["check"]=check;
    cout<<"checkkk= "<<check<<endl;
    std::ofstream file_id;
    file_id.open(solution_name);
    Json::StyledWriter styledWriter;
    file_id << styledWriter.write(data);
    file_id.close();
}

int minn(int a, int b){
    if (a<b) return a;
    return b;
}

int indexx(int i, int j ,int n){
    int tmp,i1=i,i2=j;
    if (i>j){
        i1=j;
        i2=i;
    }
    int somme=0;
    for (int k = 0; k <i1; k++) {
        for (int kk = k+1; kk <n; kk++) {
            somme+=1;

    }}
     for (int kk = i1+1; kk <i2; kk++) {
            somme+=1;}
    return somme;
}
int in_paths(int in1,int in2,int k,int ** paths){
    int rt=0;
    for (int i = 1; i <paths[k][0]; i++) {
            if((paths[k][i]==in1 && paths[k][i+1]==in2)||(paths[k][i]==in2 && paths[k][i+1]==in1)){
                rt=1;break;
            }
            }
            return rt;
}
//****************************check*************************************
int check(Json::Value edge_affectations, int * affectation,Json::Value network,Json::Value slice,int n,int n1,int m1,int m,int** paths,int* costs){
//nodes
int total_used_ressorces=0;
int * cap_node=new int[n];
for (int j = 0; j < n; j++) {
cap_node[j]=network["nodes_cap"][IntToString(j)].asInt();
}
for (int  i = 0; i < n1; i++) {
total_used_ressorces=total_used_ressorces+cap_node[affectation[i]];
cap_node[affectation[i]]=cap_node[affectation[i]]-slice["nodes_cap"][IntToString(i)].asInt();
if (cap_node[affectation[i]]<0){
   cout<<"problem affectation node "<<i<<endl; 
    return 0;}
}
//edges
int** d= NULL;
d=new int*[n];
for (int i=0; i<n; i++) {d[i]=new int[n];}
for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
        d[i][j]=0;
    }}
int in1,in2;    
for (int  i = 0; i < m; i++) {

    in1=network["edges"][i]["e"][0].asInt();
    in2=network["edges"][i]["e"][1].asInt();
    d[in1][in2]=network["edges"][i]["weight"].asInt();
    d[in2][in1]=d[in1][in2];}
int is1,is2;
int index;
for (int  i = 0; i < m1; i++) {
    index=edge_affectations[i]["path_index"].asInt();
    total_used_ressorces=total_used_ressorces+costs[index];
    is1=paths[index][1];
    is2=paths[index][paths[index][0]];
    in1=affectation[edge_affectations[i]["i"].asInt()]; 
    in2=affectation[edge_affectations[i]["j"].asInt()];

    if ((is1!=in1 && is2!=in2) && (is1!=in2 && is2!=in1)){
        cout<<"double affectation edge"<<i<<endl;
        cout<<is1<<" " <<is2<<" " << in1<<" " <<in2<<endl;
        return 0;
    }

    for (int  j = 0; j < paths[edge_affectations[i]["path_index"].asInt()][0]-1; j++) {
        in1=paths[edge_affectations[i]["path_index"].asInt()][1+j];
        in2=paths[edge_affectations[i]["path_index"].asInt()][j+2];
        d[in1][in2]=d[in1][in2]-edge_affectations[i]["weight"].asInt();
        d[in2][in1]=d[in1][in2];
    if (d[in2][in1]<0){
        cout<<"problem affectation edge "<<i+1<<" d[in2][in1]= "<<d[in2][in1]<<endl;    
        return 0;
    }
}}

cout<<"total_used_ressorces= "<<total_used_ressorces<<endl;
    return 1;
}