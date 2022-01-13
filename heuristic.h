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
#ifndef __HEURISTIC_h__
#define __HEURISTIC_h__
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
typedef IloArray<IloNumArray> DataMatrix;
typedef IloArray<IloIntArray> DataMatrix2;
typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<IloIntVarArray> IntVarMatrix;
typedef IloArray<IntVarMatrix> IntVarMatrix2;



void phase2 (IloModel modell,Json::Value network,Json::Value slice,Json::Value all_paths,int** paths, int nbr_path,
                string solution_name,int** edge,float* start,int* solution_realisable,int* costs, double Timelimit,double times_paths);                
#endif
