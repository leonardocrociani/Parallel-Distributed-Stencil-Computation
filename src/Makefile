CXX                = g++ -std=c++20
OPTFLAGS           = -O3
AVXFLAGS           = -mavx2
AUTOFLAGS          = -march=native -ffast-math -mavx2
CXXFLAGS          += -Wall 
INCLUDES           = -I. -I./include
LIBS               = -pthread
SOURCES            = $(wildcard *.cpp)
TARGETS            = $(SOURCES:.cpp=)

.PHONY: all clean cleanall

# Rule to compile a single file
%: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -o $@.o $< $(LIBS)

# Rule to compile all files
all: $(TARGETS)

# Clean rules
clean: 
	-rm -fr *.o *~

cleanall: clean
	-rm -fr $(TARGETS)