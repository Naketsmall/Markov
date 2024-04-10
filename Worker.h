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
    int make_map(std::string filename);

private:
    int rank, num_processes;
    MarkovChain map;

};


#endif //CNN_WORKER_H
