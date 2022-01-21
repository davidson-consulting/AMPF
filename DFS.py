"""Copyright (C) <2022>  <AIT ABA Massinissa>

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
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA."""

import sys
import networkx as nx
from itertools import islice
import json
import random
import time
################read the graph################
def open_file(instance,edgelist,d):
    print(instance)
    comp_edge=0
    with open(instance) as json_file:
        data = json.load(json_file)
        n=data["n"]
        for i in range(0,n):
            d[i]={}
        for i in range(0,n):
            for j in range(0,n):
                d[i][j]=-99999
        go=nx.empty_graph(n=n, create_using=None)
        for p in data['edges']:
            go.add_edge(p['e'][0], p['e'][1], weight= p['weight'])
            d[p['e'][0]][p['e'][1]]=p['weight']
            d[p['e'][1]][p['e'][0]]=p['weight']
        #initialiser edgelist 
        for e in go.edges:
            edgelist[comp_edge]={}
            edgelist[comp_edge][0]=e
            edgelist[comp_edge][1]='b'
            comp_edge=comp_edge+1
    return n,go
########################## widest path ##########################
def minimum (a,b):
    if a>b:
        return b
    else: 
        return a
def widest_path(s,t,n,weight_mat,number_path):
    P =[]
    count=[]
    done=[]
    B=[]
    pth={}
    pth["path"]=[s]
    pth["cost"]=999
    B.append(pth)
    #print (B[0]["path"])
    for i in range (0,n):
        count.append(0)
        done.append(0)
    step=0
    while (bool(B)==True and count[t]<number_path):
        best={}
        mx=-1
        for pt in B:
            if (pt["cost"]>mx):
                mx=pt["cost"]
                best=pt
        #B = B − {Pu }, countu = countu + 1
        B.remove(best)
        u=best["path"][len(best["path"])-1]
        if (u!=t):
            done[u]=1
        count[u]=count[u]+1
        #if u = t then P = P U {Pu}
        if (u==t):
            P.append(best)
        if(count[u]<=number_path and u!=t):
            for i in range (0,n):
                if (weight_mat[u][i]>=0 and done[i]==0):
                    new_pat={}
                    cst=best["cost"]
                    bpth=best["path"][:]  
                    new_pat["cost"]=minimum(cst,weight_mat[u][i])
                    new_pat["path"]=bpth
                    new_pat["path"].append(i)
                    B.append(new_pat)
        step=step+1
    return P
################read solution################
def open_solution(solution_path):
    with open(solution_path) as json_file:
        data = json.load(json_file)
        check=data["check"]
        solution_realisable=data["solution_realisable"]
    return solution_realisable,check
    

def threads_call(args):
    i=args[0]
    j=args[1]
    P =[]
    simplepathslens=[]
    simplepathsmaxweight=[]
    selected_paths=[]
    new_max=0
    if (d[i][j]>0): 
        edge=[]
        edge.append(i)
        edge.append(j)
        selected_paths.append(edge)
        simplepathslens.append(2)
        simplepathsmaxweight.append(d[i][j])
        new_max=1
        P=widest_path(i,j,n,d,max_paths-1)
        for pt in P:
            new_max=new_max+1
            selected_paths.append(pt["path"])
            simplepathslens.append(len(pt["path"]))
            simplepathsmaxweight.append(pt["cost"])
    else:
        new_max=0
        P=widest_path(i,j,n,d,max_paths)
        for pt in P:
            new_max=new_max+1
            selected_paths.append(pt["path"])
            simplepathslens.append(len(pt["path"]))
            simplepathsmaxweight.append(pt["cost"])
    if (new_max<max_paths):
        while(new_max<max_paths):
            if (len(P)==0):
                print("**************what !!!!")
            print("P[0]={}".format(P[0]))
            pt=P[0]
            new_max=new_max+1
            selected_paths.append(pt["path"])
            simplepathslens.append(len(pt["path"]))
            simplepathsmaxweight.append(pt["cost"])
    rt={
            'i': i,'j': j,'simplePaths':selected_paths,'simplePathslens':simplepathslens,'simplePathsmaxweight':simplepathsmaxweight,
            'len': new_max
        }
    return rt
#######################main#####################
#0 = all paths
print("***DFS***\n")
assert(int(sys.argv[3])==1)
max_paths=int(sys.argv[2])
edgelist={}
d={}
instance=sys.argv[1]
n,go=open_file(instance,edgelist,d)
data = {}
data["all_paths"]=[]
data["somme_len"]=0
data["time"]=0
instance=sys.argv[1]+"_widest_paths "+str(max_paths)
with open(instance, 'w') as outfile:
        json.dump(data, outfile,indent=4)
somme_len=0
start = time.time()
calls=[]
for i in range(0,n):
    for j in range(i+1,n):
        call=[]
        call.append(i)
        call.append(j)
        calls.append(call)   
import multiprocessing
from functools import partial
threads_number=multiprocessing.cpu_count()
pool = multiprocessing.Pool(threads_number-1) 
data["all_paths"]=pool.map(threads_call, calls) 

end = time.time()
elapsed = end - start

#print(resultat)
for p in data["all_paths"]:
    somme_len=somme_len+p['len']
data["somme_len"]=somme_len
#CPU seconds elapsed (floating point)
data["time"]=elapsed
#save the file
with open(instance, 'w') as outfile:
        json.dump(data, outfile,indent=4)







