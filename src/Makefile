CXX                = g++ -std=c++20
OPTFLAGS	       = -O3 -DNDEBUG -ffast-math -march=native
CXXFLAGS          += -Wall 
INCLUDES           = -I. -I./lib -I ./lib/fastflow 
LIBS               = -pthread
SOURCES            = $(wildcard *.cpp)
TARGETS            = $(SOURCES:.cpp=)

.PHONY: all clean cleanall

# Rule to compile a single file
%: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) $(COREFLAGS) -o $@.o $< $(LIBS)

# Rule to compile all files
all: $(TARGETS)

mpi: mpi.cpp
	mpicxx -std=c++20 -Wall -O3 -DNDEBUG -o mpi.o mpi.cpp

# Clean rules
clean: 
	-rm -fr *.o *~

cleanall: clean
	-rm -fr $(TARGETS)