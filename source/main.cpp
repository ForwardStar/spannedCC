#include "commonfunctions.h"
#include "temporal_graph.h"
#include "online_search.h"
#include "baseline.h"
#include "kruskal.h"
#include "differential_kruskal.h"
#include "C_TSF.h"
#include "LCT.h"
#include "LCT_chunk.h"
#include "LCT_prt.h"
#include "optimized_baseline.h"

bool debug = false;

TemporalGraph * build(char * argv[], double subgraph_fraction) {

    std::cout << "Building graph..." << std::endl;
    unsigned long long build_graph_start_time = currentTime();
    TemporalGraph * Graph = new TemporalGraph(argv[1], (char *)"Undirected", subgraph_fraction);
    unsigned long long build_graph_end_time = currentTime();
    std::cout << "Build graph success in " << timeFormatting(difftime(build_graph_end_time, build_graph_start_time)).str() << std::endl;
    std::cout << "n = " << Graph->numOfVertices() << ", m = " << Graph->numOfEdges() << ", tmax = " << Graph->tmax << ", size = " << Graph->size() << " bytes" << std::endl;
    return Graph;
    
}

int main(int argc, char * argv[]) {

    std::ios::sync_with_stdio(false);

    unsigned long long start_time = currentTime();

    double update_fraction = 0.0;
    double subgraph_fraction = 1.0;

    if (std::strcmp(argv[argc - 1], "Debug") == 0) {
        debug = true;
        argc--;
    }

    if (argc == 6 && std::strcmp(argv[argc - 1], "update") == 0) {
        std::cout << "Index update mode enabled. Please input the fraction of timestamps to update (0 < x < 1): ";
        std::cin >> update_fraction;
        argc--;
    }

    if (argc == 6 && std::strcmp(argv[argc - 1], "subgraph") == 0) {
        std::cout << "Subgraph mode enabled. Please input the fraction of timestamps in the subgraph (0 < x < 1): ";
        std::cin >> subgraph_fraction;
        argc--;
    }

    if (argc != 5) {
        std::cout << "Parameters are non-standard. Please check the readme file." << std::endl;
    }

    TemporalGraph * Graph = build(argv, subgraph_fraction);
    //Graph->shrink_to_fit();
    int vertex_num = Graph->numOfVertices();
    //std::cout<<argv[argc - 1]<<std::endl;
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
        BaselineIndex *Index = new BaselineIndex(Graph, 1 - update_fraction);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        if (update_fraction > 0) {
            std::cout << "Updating the index structure..." << std::endl;
            int t1 = int(Graph->tmax * (1 - update_fraction));
            int num_of_edges = 0;
            for (int t = t1 + 1; t <= Graph->tmax; t++) {
                num_of_edges += Graph->temporal_edge[t].size();
            }
            std::cout << "Number of edges to be updated: " << num_of_edges << std::endl;
            unsigned long long index_update_start_time = currentTime();
            Index->update(Graph);
            unsigned long long index_update_end_time = currentTime();
            std::cout << "Index update completed in " << timeFormatting(difftime(index_update_end_time, index_update_start_time)).str() << std::endl;
        }
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

    if (std::strcmp(argv[argc - 1], "TSF") == 0) {
        std::cout << "Running TSF-index..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        DifferentialKruskal *Index = new DifferentialKruskal(Graph, 1 - update_fraction);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        if (update_fraction > 0) {
            std::cout << "Updating the index structure..." << std::endl;
            int t1 = int(Graph->tmax * (1 - update_fraction));
            int num_of_edges = 0;
            for (int t = t1 + 1; t <= Graph->tmax; t++) {
                num_of_edges += Graph->temporal_edge[t].size();
            }
            std::cout << "Number of edges to be updated: " << num_of_edges << std::endl;
            unsigned long long index_update_start_time = currentTime();
            Index->update(Graph);
            unsigned long long index_update_end_time = currentTime();
            std::cout << "Index update completed in " << timeFormatting(difftime(index_update_end_time, index_update_start_time)).str() << std::endl;
        }
        //std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
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

    if (std::strcmp(argv[argc - 1], "TSF-basic") == 0) {
        std::cout << "Running Compressed TSF-index..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        ctsf *Index = new ctsf(Graph, 1 - update_fraction);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        // if (update_fraction > 0) {
        //     std::cout << "Updating the index structure..." << std::endl;
        //     int t1 = int(Graph->tmax * (1 - update_fraction));
        //     int num_of_edges = 0;
        //     for (int t = t1 + 1; t <= Graph->tmax; t++) {
        //         num_of_edges += Graph->temporal_edge[t].size();
        //     }
        //     std::cout << "Number of edges to be updated: " << num_of_edges << std::endl;
        //     unsigned long long index_update_start_time = currentTime();
        //     Index->update(Graph);
        //     unsigned long long index_update_end_time = currentTime();
        //     std::cout << "Index update completed in " << timeFormatting(difftime(index_update_end_time, index_update_start_time)).str() << std::endl;
        // }
        //std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        for (int i = 2; i < argc - 2; i++) {
            std::cout << "Solving queries..." << std::endl;
            unsigned long long query_start_time = currentTime();
            Ctsf(Index, vertex_num, argv[i], argv[argc - 2]);
            unsigned long long query_end_time = currentTime();
            std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        }
        std::cout << "Compressed TSF-index completed!" << std::endl;
    }
    
    if (std::strcmp(argv[argc - 1], "LCT") == 0) {
        std::cout << "Running LCT-index..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        LCTindex *Index = new LCTindex(Graph, 1 - update_fraction);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        if (update_fraction > 0) {
            std::cout << "Updating the index structure..." << std::endl;
            int t1 = int(Graph->tmax * (1 - update_fraction));
            int num_of_edges = 0;
            for (int t = t1 + 1; t <= Graph->tmax; t++) {
                num_of_edges += Graph->temporal_edge[t].size();
            }
            std::cout << "Number of edges to be updated: " << num_of_edges << std::endl;
            unsigned long long index_update_start_time = currentTime();
            Index->update(Graph);
            unsigned long long index_update_end_time = currentTime();
            std::cout << "Index update completed in " << timeFormatting(difftime(index_update_end_time, index_update_start_time)).str() << std::endl;
        }
        std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        for (int i = 2; i < argc - 2; i++) {
            std::cout << "Solving queries..." << std::endl;
            unsigned long long query_start_time = currentTime();
            LCT(Index, vertex_num, argv[i], argv[argc - 2]);
            unsigned long long query_end_time = currentTime();
            std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        }
        std::cout << "Compressed TSF-index completed!" << std::endl;
    }

    if (std::strcmp(argv[argc - 1], "TSF-MSF") == 0) {
        std::cout << "Running TSF-index with TSF-construct-MSF algorithm..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        LCTchunk *Index = new LCTchunk(Graph, 1 - update_fraction);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        if (update_fraction > 0) {
            std::cout << "Updating the index structure..." << std::endl;
            int t1 = int(Graph->tmax * (1 - update_fraction));
            int num_of_edges = 0;
            for (int t = t1 + 1; t <= Graph->tmax; t++) {
                num_of_edges += Graph->temporal_edge[t].size();
            }
            std::cout << "Number of edges to be updated: " << num_of_edges << std::endl;
            unsigned long long index_update_start_time = currentTime();
            Index->update(Graph);
            unsigned long long index_update_end_time = currentTime();
            std::cout << "Index update completed in " << timeFormatting(difftime(index_update_end_time, index_update_start_time)).str() << std::endl;
        }
        std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        for (int i = 2; i < argc - 2; i++) {
            std::cout << "Solving queries..." << std::endl;
            unsigned long long query_start_time = currentTime();
            LCTc(Index, vertex_num, argv[i], argv[argc - 2]);
            unsigned long long query_end_time = currentTime();
            std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        }
        std::cout << "Chunk TSF-index completed!" << std::endl;
    }

    if (std::strcmp(argv[argc - 1], "TSF-PST") == 0) {
        std::cout << "Running TSF-prt-index..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        LCTprt *Index = new LCTprt(Graph, 1 - update_fraction);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
        if (update_fraction > 0) {
            std::cout << "Updating the index structure..." << std::endl;
            int t1 = int(Graph->tmax * (1 - update_fraction));
            int num_of_edges = 0;
            for (int t = t1 + 1; t <= Graph->tmax; t++) {
                num_of_edges += Graph->temporal_edge[t].size();
            }
            std::cout << "Number of edges to be updated: " << num_of_edges << std::endl;
            unsigned long long index_update_start_time = currentTime();
            Index->update(Graph);
            unsigned long long index_update_end_time = currentTime();
            std::cout << "Index update completed in " << timeFormatting(difftime(index_update_end_time, index_update_start_time)).str() << std::endl;
        }
        std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        for (int i = 2; i < argc - 2; i++) {
            std::cout << "Solving queries..." << std::endl;
            unsigned long long query_start_time = currentTime();
            LCTp(Index, vertex_num, argv[i], argv[argc - 2]);
            unsigned long long query_end_time = currentTime();
            std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        }
        std::cout << "Priority search tree TSF-index completed!" << std::endl;
    }

    unsigned long long end_time = currentTime();
    std::cout << "Program finished in " << timeFormatting(difftime(end_time, start_time)).str() << std::endl;

    return 0;

}