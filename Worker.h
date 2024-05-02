//
// Created by epsilon on 08.04.24.
//

#ifndef CNN_WORKER_H
#define CNN_WORKER_H


#include <string>
#include <map>
#include "MarkovChain.h"


class Worker {
public:
    Worker(int rank, int n);
    void work();
    int make_map(std::string filename);
    int listen_merge();
    const MarkovChain &get_map();
    void print_map();
    int listen_generate();
    int save_map_json(std::string filename);

private:
    int merge(int rank2, bool share);
    int rank, num_processes;
    MarkovChain map;
};


#endif //CNN_WORKER_H
