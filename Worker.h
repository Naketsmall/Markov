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
    int merge(int rank2, bool share);
    const std::unordered_map<std::string, std::map<std::string, int>> &get_map();

private:
    int rank, num_processes;
    MarkovChain map;

};


#endif //CNN_WORKER_H
