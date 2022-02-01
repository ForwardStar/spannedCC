OBJS=main.o commonfunctions.o temporal_graph.o online_search.o baseline.o optimized.o kruskal.o
CC=g++
CFLAGS=-O3 -c

main:$(OBJS)
	$(CC) $^ -o main
%.o:source/%.cpp
	$(CC) $^ $(CFLAGS) -o $@

clean:
	$(RM) *.o main