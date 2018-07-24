CC = /usr/bin/g++ #path to g++
FLAGS = -isystem ~/devel/seqan/include -std=c++14 -O3 -DNDEBUG -W -Wall -pedantic -fopenmp -lpthread -pthread -ggdb -Wno-unused-parameter -Wno-sequence-point #flags necessary to use Seqan and its particular implementation in this source code
SOURCES = sequence_finder.cpp FragmentLibrary.cpp ContextFragment.cpp IndexedFragment.cpp #source files
EXE = sequence_finder #executable name
OBJS = $(SOURCES:.cpp = .o)

$(EXE): $(OBJS) 
	$(CC) -o $(EXE) $(FLAGS) $(OBJS) -lrt #the lrt MUST be placed after the object names in order for aio.h to work
%.o: %.cpp
	$(CC) $(FLAGS) -c $(.SOURCE)

clean:
	rm -i $(OBJS) $(EXE)

 
