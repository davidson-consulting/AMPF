#**********************************************************************************************************#
#This work was supported by the Agence Nationale de la Recherche (ANR) through the AIDY-F2N LabCom Project.#
#Ait Aba Massinissa                                                                                        #
#**********************************************************************************************************#
#folder name
network=Data/test/test_network

#number of slices
slice=Data/test/slice_0

#index widest if index=1 shortest if index=2
index=2

#max_paths
max_paths=1

#Timelimit
Timelimit=300

PFLAGS= $(name_floder) $(nbr_slices) $(min_paths) $(max_paths) $(objectif) $(update) $(Timelimit)

#################################cplex############################
CCC = g++ -fprofile-arcs  -ftest-coverage -O0 -w
#add the right directory fo cplex installation
CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio1210/cplex
CONCERTDIR=$(CPLEXDIR)/../concert
CPDIR=$(CPLEXDIR)/../cpoptimizer
CFLAGS = -DIL_STD -O -DNDEBUG -I$(CONCERTDIR)/include -fopenmp -ljsoncpp -ldl# -m64 -DILOUSEMT -D_REENTRANT -DILM_REENTRANT
CFLAGS+=-I$(CPDIR)/include
CFLAGS+=-I$(CPLEXDIR)/include
CFLAGS+=-g
LDFLAGS=-L$(CPDIR)/lib/x86-64_linux/static_pic -lcp 
LDFLAGS+=-L$(CPLEXDIR)/lib/x86-64_linux/static_pic -lilocplex -ljsoncpp -lcplex -L$(CONCERTDIR)/lib/x86-64_linux/static_pic -lconcert -ldl -lpthread 
LDFLAGS+=-g
#----------------------------------------------------------
# FILES
#---------------------------------------------------------
#Les fichiers source
SOURCES=main.cpp instance.cpp heuristic.cpp exact_solution.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE) widest shortest cplex
#all: $(SOURCES) $(EXECUTABLE) cplex

#cplex
$(EXECUTABLE): $(OBJECTS)
	$(CCC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CCC) $(CFLAGS) $< -o $@ -c 

widest:
	-python3 DFS.py $(network) $(max_paths) $(index)

shortest:
	-python3 shortest_path.py $(network) $(max_paths) $(index)

cplex:
	./main $(network) $(slice) $(max_paths) $(index) $(Timelimit)
remove:
	rm -f *.o *.gcda *.gcno 	
move:
	-mv *.gcno ./dev

gcov0:
	gcov -abcfu  instance.cpp
	gcov -abcfu  heuristic.cpp
	gcov -abcfu  exact_solution.cpp




