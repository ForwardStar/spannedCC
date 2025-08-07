OBJS=commonfunctions.o temporal_graph.o online_search.o baseline.o optimized_baseline.o kruskal.o differential_kruskal.o LCT_chunk.o C_TSF.o LCT_prt.o LCT.o main.o
CC=g++
CFLAGS=-c -std=c++11 -O3

main:$(OBJS)
	$(CC) $^ -o main -O3
%.o:source/%.cpp
	$(CC) $^ $(CFLAGS)

clean:
	$(RM) *.o main