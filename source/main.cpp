#include "commonfunctions.h"
#include "temporal_graph.h"
#include "online_search.h"
#include "baseline.h"
#include "kruskal.h"
#include "differential_kruskal.h"
#include "optimized_baseline.h"

TemporalGraph * build(char * argv[]) {

    std::cout << "Building graph..." << std::endl;
    unsigned long long build_graph_start_time = currentTime();
    TemporalGraph * Graph = new TemporalGraph(argv[1], (char *)"Undirected", 1);
    unsigned long long build_graph_end_time = currentTime();
    std::cout << "Build graph success in " << timeFormatting(difftime(build_graph_end_time, build_graph_start_time)).str() << std::endl;
    std::cout << "n = " << Graph->numOfVertices() << ", m = " << Graph->numOfEdges() << ", tmax = " << Graph->tmax << ", size = " << Graph->size() << " bytes" << std::endl;
    return Graph;
    
}

int main(int argc, char * argv[]) {

    std::ios::sync_with_stdio(false);

    unsigned long long start_time = currentTime();

    if (argc != 5) {
        std::cout << "Parameters are non-standard. Please check the readme file." << std::endl;
    }

    TemporalGraph * Graph = build(argv);
    Graph->shrink_to_fit();
    int vertex_num = Graph->numOfVertices();

    if (std::strcmp(argv[argc - 1], "Online") == 0) {
        for (int i = 2; i < argc - 2; i++) {
            std::cout << "Running online search..." << std::endl;
            unsigned long long online_search_start_time = currentTime();
            online(Graph, argv[i], argv[argc - 2]);
            unsigned long long online_search_end_time = currentTime();
            std::cout << "Online search completed in " << timeFormatting(online_search_end_time - online_search_start_time).str() << std::endl;
        }
        delete Graph;
    }

    if (std::strcmp(argv[argc - 1], "Baseline") == 0) {
        std::cout << "Running baseline..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        BaselineIndex *Index = new BaselineIndex(Graph);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        for (int i = 2; i < argc - 2; i++) {
            std::cout << "Solving queries..." << std::endl;
            unsigned long long query_start_time = currentTime();
            baseline(Index, vertex_num, argv[i], argv[argc - 2]);
            unsigned long long query_end_time = currentTime();
            std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        }
        std::cout << "Baseline completed!" << std::endl;
    }

    // if (std::strcmp(argv[4], "OBaseline") == 0) {
    //     std::cout << "Running optimized baseline..." << std::endl;
    //     std::cout << "Constructing the index structure..." << std::endl;
    //     unsigned long long index_construction_start_time = currentTime();
    //     OptimizedBaseline *Index = new OptimizedBaseline(Graph);
    //     unsigned long long index_construction_end_time = currentTime();
    //     std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
    //     delete Graph;
    //     std::cout << "Solving queries..." << std::endl;
    //     unsigned long long query_start_time = currentTime();
    //     optimized_baseline(Index, vertex_num, argv[2], argv[3]);
    //     unsigned long long query_end_time = currentTime();
    //     std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
    //     std::cout << "Optimized Baseline completed!" << std::endl;
    // }

    // if (std::strcmp(argv[4], "Kruskal") == 0) {
    //     std::cout << "Running kruskal reconstruction tree..." << std::endl;
    //     std::cout << "Constructing the index structure..." << std::endl;
    //     unsigned long long index_construction_start_time = currentTime();
    //     KruskalReconstructionTree *Index = new KruskalReconstructionTree(Graph);
    //     unsigned long long index_construction_end_time = currentTime();
    //     std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
    //     delete Graph;
    //     std::cout << "Solving queries..." << std::endl;
    //     unsigned long long query_start_time = currentTime();
    //     kruskal(Index, vertex_num, argv[2], argv[3]);
    //     unsigned long long query_end_time = currentTime();
    //     std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
    //     std::cout << "Kruskal reconstruction tree completed!" << std::endl;
    // }

    if (std::strcmp(argv[argc - 1], "TSF") == 0) {
        std::cout << "Running TSF-index..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        DifferentialKruskal *Index = new DifferentialKruskal(Graph);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        for (int i = 2; i < argc - 2; i++) {
            std::cout << "Solving queries..." << std::endl;
            unsigned long long query_start_time = currentTime();
            differential_kruskal(Index, vertex_num, argv[i], argv[argc - 2]);
            unsigned long long query_end_time = currentTime();
            std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        }
        std::cout << "TSF-index completed!" << std::endl;
    }

    unsigned long long end_time = currentTime();
    std::cout << "Program finished in " << timeFormatting(difftime(end_time, start_time)).str() << std::endl;

    return 0;

}