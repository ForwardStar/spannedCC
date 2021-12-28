#include "commonfunctions.h"
#include "temporal_graph.h"
#include "online_search.h"
#include "baseline.h"

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

    int start_time = time(NULL);

    if (argc != 5) {
        std::cout << "Parameter error. Please check the readme file." << std::endl;
    	return 0;
    }

    TemporalGraph * Graph = build(argv);

    if (std::strcmp(argv[4], "Online") == 0) {
        std::cout << "Running online search..." << std::endl;
        int online_search_start_time = time(NULL);
        online(Graph, argv[2], argv[3]);
        int online_search_end_time = time(NULL);
        std::cout << "Online search completed in " << timeFormatting(difftime(online_search_end_time, online_search_start_time)).str() << std::endl;
    }

    if (std::strcmp(argv[4], "Baseline") == 0) {
        std::cout << "Running baseline..." << std::endl;
        BaselineIndex Index;
        std::ifstream is((char *)"model");
        if (is.good()) {
            Index.deserialize(is);
        }
        else {
            std::cout << "Constructing the index structure..." << std::endl;
            int index_construction_start_time = time(NULL);
            Index = BaselineIndex(Graph);
            int index_construction_end_time = time(NULL);
            std::cout << "Index construction completed in " << timeFormatting(difftime(index_construction_end_time, index_construction_start_time)).str() << std::endl;
            std::ofstream os((char *)"model");
            Index.serialize(os);
        }
        std::cout << "Solving queries..." << std::endl;
        int query_start_time = time(NULL);
        baseline(&Index, Graph, argv[2], argv[3]);
        int query_end_time = time(NULL);
        std::cout << "Query completed in " << timeFormatting(difftime(query_end_time, query_start_time)).str() << std::endl;
        std::cout << "Baseline completed!" << std::endl;
    }

    delete Graph;

    int end_time = time(NULL);
    std::cout << "Program finished in " << timeFormatting(difftime(end_time, start_time)).str() << std::endl;

    return 0;

}
