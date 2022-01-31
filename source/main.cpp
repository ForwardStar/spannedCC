#include "commonfunctions.h"
#include "temporal_graph.h"
#include "online_search.h"
#include "baseline.h"
#include "optimized.h"
#include "kruskal.h"

TemporalGraph * build(char * argv[]) {
    std::cout << "Building graph..." << std::endl;
    int build_graph_start_time = time(NULL);
    TemporalGraph * Graph = new TemporalGraph(argv[1], (char *)"Undirected");
    int build_graph_end_time = time(NULL);
    std::cout << "Build graph success in " << timeFormatting(difftime(build_graph_end_time, build_graph_start_time)).str() << std::endl;
    std::cout << "n = " << Graph->numOfVertices() << ", m = " << Graph->numOfEdges() << ", tmax = " << Graph->tmax << std::endl;
    return Graph;
}

int main(int argc, char * argv[]) {

    std::ios::sync_with_stdio(false);

    int start_time = time(NULL);

    if (argc != 5) {
        std::cout << "Parameter error. Please check the readme file." << std::endl;
    	return 0;
    }

    TemporalGraph * Graph = build(argv);
    int vertex_num = Graph->numOfVertices();

    if (std::strcmp(argv[4], "Online") == 0) {
        std::cout << "Running online search..." << std::endl;
        int online_search_start_time = time(NULL);
        online(Graph, argv[2], argv[3]);
        int online_search_end_time = time(NULL);
        std::cout << "Online search completed in " << timeFormatting(difftime(online_search_end_time, online_search_start_time)).str() << std::endl;
        delete Graph;
    }

    if (std::strcmp(argv[4], "Baseline") == 0) {
        std::cout << "Running baseline..." << std::endl;
        BaselineIndex *Index = new BaselineIndex();
        std::cout << "Constructing the index structure..." << std::endl;
        int index_construction_start_time = time(NULL);
        Index = new BaselineIndex(Graph);
        int index_construction_end_time = time(NULL);
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        delete Graph;
        std::cout << "Solving queries..." << std::endl;
        int query_start_time = time(NULL);
        baseline(Index, vertex_num, argv[2], argv[3]);
        int query_end_time = time(NULL);
        std::cout << "Query completed in " << timeFormatting(difftime(query_end_time, query_start_time)).str() << std::endl;
        std::cout << "Baseline completed!" << std::endl;
    }

    if (std::strcmp(argv[4], "Optimized") == 0) {
        std::cout << "Running optimized index..." << std::endl;
        OptimizedIndex *Index = new OptimizedIndex();
        std::cout << "Constructing the index structure..." << std::endl;
        int index_construction_start_time = time(NULL);
        Index = new OptimizedIndex(Graph);
        int index_construction_end_time = time(NULL);
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        std::cout << "Solving queries..." << std::endl;
        int query_start_time = time(NULL);
        optimized(Index, vertex_num, argv[2], argv[3]);
        int query_end_time = time(NULL);
        std::cout << "Query completed in " << timeFormatting(difftime(query_end_time, query_start_time)).str() << std::endl;
        std::cout << "Optimized completed!" << std::endl;
    }

    if (std::strcmp(argv[4], "Kruskal") == 0) {
        std::cout << "Running kruskal reconstruction tree..." << std::endl;
        KruskalReconstructionTree *Index = new KruskalReconstructionTree();
        std::cout << "Constructing the index structure..." << std::endl;
        int index_construction_start_time = time(NULL);
        Index = new KruskalReconstructionTree(Graph);
        int index_construction_end_time = time(NULL);
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        delete Graph;
        std::cout << "Solving queries..." << std::endl;
        int query_start_time = time(NULL);
        kruskal(Index, vertex_num, argv[2], argv[3]);
        int query_end_time = time(NULL);
        std::cout << "Query completed in " << timeFormatting(difftime(query_end_time, query_start_time)).str() << std::endl;
        std::cout << "Kruskal reconstruction tree completed!" << std::endl;
    }

    int end_time = time(NULL);
    std::cout << "Program finished in " << timeFormatting(difftime(end_time, start_time)).str() << std::endl;

    return 0;

}