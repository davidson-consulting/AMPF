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
    
#ifndef __INSTANCE_h__
#define __INSTANCE_h__
#include <algorithm>
#include <iterator>
#include <cmath>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <time.h> 
#include <ctime>
#include <stdio.h>  
#include <stdlib.h> 
#include <math.h>
//#include <json/value.h>
#include <cstdio>
#include <string.h>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <iostream>
#include <chrono>
#include <unistd.h>
//parallel 
#include <omp.h>
#include "omp.h"
using namespace std;





void read_instance(Json::Value *root,string file_name);
string IntToString (int a);
string FloatToString (float a);
void save_solution(string solution_name,int* affectation,Json::Value edge_affectations,int solution_realisable,int cost,float runtime,int n,int n1,int m1,int check,Json::Value slice);
int in_paths(int in1,int in2,int k,int ** paths);
int indexx(int i, int j ,int n);
int minn(int a, int b);
int check(Json::Value edge_affectations, int * affectation,Json::Value network,Json::Value slice,int n,int n1,int m1,int m,int** paths,int* costs);
#endif
