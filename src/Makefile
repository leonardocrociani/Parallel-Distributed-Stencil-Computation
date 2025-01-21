CXX                = g++
MPICXX 			   = mpic++
STDFLAGS           = -std=c++20
OPTFLAGS	       = -O3 -DNDEBUG -ffast-math -march=native
CXXFLAGS          += -Wall 
INCLUDES           = -I. -I./lib -I ./lib/fastflow
LIBS               = -pthread
SOURCES            = $(wildcard *.cpp)
TARGETS            = $(SOURCES:.cpp=)

.PHONY: all clean cleanall

%: %.cpp
	$(CXX) $(STDFLAGS) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -o $@ $< $(LIBS)

all: $(TARGETS)

mpi: mpi.cpp
	$(MPICXX) $(STDFLAGS) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -o mpi mpi.cpp

clean: 
	-rm -fr mpi sequential fastflow
