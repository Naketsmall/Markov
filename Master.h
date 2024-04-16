//
// Created by epsilon on 10.04.24.
//

#ifndef CNN_MASTER_H
#define CNN_MASTER_H


#include <vector>
#include <string>

class Master {
public:
    Master(int n);
    void add_filename(std::string filename);
    void analyze();
    int merge();
    std::vector<std::string> get_filenames();
private:
    std::vector<std::string> filenames;
    int num_processes;

};


#endif //CNN_MASTER_H
