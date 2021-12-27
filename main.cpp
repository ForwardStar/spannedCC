#include <ctime>
#include <iostream>
#include <cstring>
#include "temporal_graph.h"
#include "online_search.h"
#include "baseline.h"

int main(int argc, char * argv[]) {

    int start_time = time(NULL);

    if (argc != 6) {
        std::cout << "Parameter error. Please check the readme file." << std::endl;
    	return 0;
    }

    std::cout << "Building graph..." << std::endl;
    TemporalGraph * Graph = new TemporalGraph(argv[1], argv[4]);
    std::cout << "Build graph success!" << std::endl;

    if (std::strcmp(argv[5], "Online") == 0) {
        std::cout << "Running online search..." << std::endl;
        online(Graph, argv[2], argv[3]);
        std::cout << "Online search completed!" << std::endl;
    }

    if (std::strcmp(argv[5], "Baseline") == 0) {
        std::cout << "Running baseline..." << std::endl;
        baseline(Graph, argv[2], argv[3]);
        std::cout << "Baseline completed!" << std::endl;
    }

    delete Graph;

    int end_time = time(NULL);
    std::cout << "Program finished in " << difftime(end_time, start_time) << " seconds." << std::endl;

    return 0;

}
