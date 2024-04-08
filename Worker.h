//
// Created by epsilon on 08.04.24.
//

#ifndef CNN_WORKER_H
#define CNN_WORKER_H


#include <string>
#include <map>


class Worker {
public:
    Worker(int rank, int size);
    int make_map(std::string filename);
    int backup_to_storage();
    int merge();

private:
    int rank, size;
    std::unordered_map<std::string, std::map<std::string, int>> map;
    std::string clean_word(std::string str);

};


#endif //CNN_WORKER_H
