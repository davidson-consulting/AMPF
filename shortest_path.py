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


################lire le graphe################
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
########################## shortest path ##########################
def k_shortest_paths(g, source, target, k, weight=None):
    return list(islice(nx.shortest_simple_paths(g, source, target, weight=weight), k))
def minimum (a,b):
    if a>b:
        return b
    else: 
        return a

def weight_value(g,path,d):
    w=999999
    for i in range(0,len(path)-1):
        w=minimum(w,d[path[i]][path[i+1]])
    return w
#############################################
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
        #max_path-1 others paths paths
        P=k_shortest_paths(go,i,j,max_paths-1)  
        for pt in P:
            weight=weight_value(go,pt,d)
            new_max=new_max+1
            selected_paths.append(pt)
            simplepathslens.append(len(pt))
            simplepathsmaxweight.append(weight)
    else:
        new_max=0
        P=k_shortest_paths(go,i,j,max_paths)  
        for pt in P:
            weight=weight_value(go,pt,d)
            new_max=new_max+1
            selected_paths.append(pt)
            simplepathslens.append(len(pt))
            simplepathsmaxweight.append(weight)
    if (new_max<max_paths):
        while(new_max<max_paths):
            pt=P[0]
            weight=weight_value(go,pt,d)
            new_max=new_max+1
            selected_paths.append(pt)
            simplepathslens.append(len(pt))
            simplepathsmaxweight.append(weight)
    rt={
            'i': i,'j': j,'simplePaths':selected_paths,'simplePathslens':simplepathslens,'simplePathsmaxweight':simplepathsmaxweight,
            'len': new_max
        }
    return rt

################read solution################
def open_solution(solution_path):
    with open(solution_path) as json_file:
        data = json.load(json_file)
        check=data["check"]
        solution_realisable=data["solution_realisable"]
    return solution_realisable,check
#######################main#####################
print("###########shortest##############")
assert(int(sys.argv[3])==2)
max_paths=int(sys.argv[2])
edgelist={}
d={}
instance=sys.argv[1]
n,go=open_file(instance,edgelist,d)
data = {}
data["all_paths"]=[]
data["somme_len"]=0
data["time"]=0
instance=sys.argv[1]+"_shortest_paths "+str(max_paths)
with open(instance, 'w') as outfile:
        json.dump(data, outfile,indent=4)

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

somme_len=0
for p in data["all_paths"]:
    somme_len=somme_len+p['len']
data["somme_len"]=somme_len
#CPU seconds elapsed (floating point)
data["time"]=elapsed
#save the file
with open(instance, 'w') as outfile:
        json.dump(data, outfile,indent=4)
