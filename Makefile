WARNINGS = -Wchar-subscripts -Wparentheses -Wreturn-type -Wmissing-braces -Wundef -Wshadow

CC = g++ -std=c++0x 
CC_OPTS = -O0 -g -c -Wfatal-errors -Werror $(WARNINGS)
LINKER = g++ -std=c++0x


MAZEROUTER = mazerouter
MAZEROUTER_OBJS = maze.o graph.o graph_tools.o dsets.o animation.o easypng.o

DSETS = dsets.h dsets.cpp

all: $(MAZEROUTER)

$(MAZEROUTER): $(MAZEROUTER_OBJS) mazerouter.cpp
	$(LINKER) mazerouter.cpp $(MAZEROUTER_OBJS) -lpng -o $(MAZEROUTER) 
    
demo.o: demo.cpp
	$(CC) $(CC_OPTS) demo.cpp

maze.o: input.cpp maze.cpp maze.h graph.h graph.cpp build_graph.h build_graph.cpp
	$(CC) $(CC_OPTS) maze.cpp

graph.o: random.h random.cpp graph.h graph.cpp build_graph.h build_graph.cpp
	$(CC) $(CC_OPTS) graph.cpp
	
graph_tools.o: graph_tools.h graph_tools.cpp $(DSETS)
	$(CC) $(CC_OPTS) graph_tools.cpp
	
dsets.o: dsets.cpp dsets.h
	$(CC) $(CC_OPTS) dsets.cpp

animation.o: animation.cpp animation.h
	$(CC) $(CC_OPTS) animation.cpp

easypng.o: easypng.cpp easypng.h
	$(CC) $(CC_OPTS) easypng.cpp
	
clean:
	-rm -f *.o $(MAZEROUTER) libdsets/*.gch

tidy: clean
	-rm -f *.png *.gif
