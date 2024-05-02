//
// Created by epsilon on 09.04.24.
//

#ifndef CNN_MARKOVCHAIN_H
#define CNN_MARKOVCHAIN_H


#include <unordered_map>
#include <string>
#include <map>

class MarkovChain { //TODO: const functions
public:
    MarkovChain();
    MarkovChain(std::stringstream &ss);
    int backup_to_storage();
    int get_size() const;
    void insert(std::string s1, std::string s2);
    void insert(std::string s1, std::string s2, int count);
    const std::unordered_map<std::string, std::map<std::string, int>> &get_map();
    void print();
    std::string get_next_word(std::string word);
    std::stringstream serialize_json();
private:
    int size_map;
    std::map<std::string, int> buf;
    std::unordered_map<std::string, std::map<std::string, int>> map;
    std::string get_max_from_the_map(std::map<std::string, int> maap); // Теряем абстрактность

};


#endif //CNN_MARKOVCHAIN_H
