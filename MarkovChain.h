//
// Created by epsilon on 09.04.24.
//

#ifndef CNN_MARKOVCHAIN_H
#define CNN_MARKOVCHAIN_H


#include <unordered_map>
#include <string>
#include <map>

class MarkovChain {
public:
    MarkovChain();
    int backup_to_storage();
    int get_size();
    void insert(std::string s1, std::string s2);
    void insert(std::string s1, std::string s2, int count);
    const std::unordered_map<std::string, std::map<std::string, int>> &get_map();
    void print();
private:
    int size_map;
    std::map<std::string, int> buf;
    std::unordered_map<std::string, std::map<std::string, int>> map;

};


#endif //CNN_MARKOVCHAIN_H
