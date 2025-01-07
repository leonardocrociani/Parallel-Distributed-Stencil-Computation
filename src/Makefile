CXX                = g++
MPICXX 			   = mpic++
STDFLAGS           = -std=c++20
OPTFLAGS	       = -O3 -DNDEBUG -ffast-math
CXXFLAGS          += -Wall 
INCLUDES           = -I. -I./lib -I ./lib/fastflow 
OMPFLAGS		   = -fopenmp
LIBS               = -pthread
SOURCES            = $(wildcard *.cpp)
TARGETS            = $(SOURCES:.cpp=)

.PHONY: all clean cleanall

%: %.cpp
	$(CXX) $(STDFLAGS) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -o $@ $< $(LIBS)

all: $(TARGETS)

mpi: mpi.cpp
	$(MPICXX) $(STDFLAGS) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -o mpi mpi.cpp $(OMPFLAGS)

clean: 
	-rm -fr mpi sequential fastflow
