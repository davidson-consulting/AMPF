
[![codecov](https://codecov.io/gh/davidson-consulting/AMPF/branch/master/graph/badge.svg?token=5ZbtgZLoKh)](https://codecov.io/gh/davidson-consulting/AMPF)

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Example](#Example)

## General info
This code provides a solution for the VNE problem
	
## Technologies
Project is created with:
* Python 3
* C++
* CPLEX_Studio 12.10

## setup
* Install CPLEX_Studio 12.10 and add the right directory in makefile (CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio1210/cplex)
* Install lib json: sudo apt-get install libjsoncpp-dev

## Example
To run this project:
* First change parameters in makefile: 
  Add the physical network path : network=Data/test/test_network
  Add the slice path : slice=Data/test/slice_0
  Add an index to use widest paths with index=1, and to use shortest paths put index=2: index=2
  Add the number of maximum used paths: max_paths=3
  Set a runtime limit for cplex: Timelimit=300
  
* Start the execution: make
