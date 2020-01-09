CXX = $(shell wx-config --cxx) --std=c++14
INC = -I include -I deps/cereal/include

PROGRAM = Gandrgui

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp)) $(patsubst %.cpp,%.o,$(wildcard */*.cpp))


# implementation

.SUFFIXES:      .o .cpp

.cpp.o :
	$(CXX) $(INC) -c `wx-config --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):     $(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(INC) `wx-config --libs std,richtext`

clean:
	rm -f *.o $(PROGRAM) src/*o
