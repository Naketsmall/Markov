//
// Created by epsilon on 09.04.24.
//

#include <algorithm>
#include "MarkovChain.h"

MarkovChain::MarkovChain() {
    map = {};
    buf = {};
    size_map = 0;
    srandom(2);
}

int MarkovChain::backup_to_storage() {
    return 0;
}

int MarkovChain::get_size() const {
    return size_map;
}

void MarkovChain::insert(std::string s1, std::string s2) {
    if (map.find(s1) == map.end()) {
        buf[s2] = 1;
        map[s1] = buf;
        buf.erase(s2);
    }
    else {
        if (map[s1].find(s2) == map[s1].end())
            map[s1][s2] = 1;
        else
            map[s1][s2] = map[s1][s2] + 1;
    }
    size_map++;
}

void MarkovChain::insert(std::string s1, std::string s2, int count) {
    if (map.find(s1) == map.end()) {
        buf[s2] = count;
        map[s1] = buf;
        buf.erase(s2);
    }
    else {
        if (map[s1].find(s2) == map[s1].end())
            map[s1][s2] = count;
        else
            map[s1][s2] = map[s1][s2] + count;
    }
    size_map++;
}

auto print_key_value = [](const auto &key, const auto &value) {
    printf("\t\tKey:[%s] Value:[%d]\n", key.c_str(), value);
};

void MarkovChain::print() {
    for(std::unordered_map<std::string, std::map<std::string, int>>::iterator it = map.begin(); it != map.end(); ++it) {
        printf("Key: %s\n", (it->first).c_str());
        for (const auto &[key, value]: (it->second))
            print_key_value(key, value);
    }
}

const std::unordered_map<std::string, std::map<std::string, int>> &MarkovChain::get_map() {
    return map;
}

std::string get_probable_el(std::map<std::string, int> maap) {
    std::vector<std::string> words = {};
    for(std::map<std::string,int>::iterator it = maap.begin(); it != maap.end(); ++it) {
        for (int i = 0; i < it->second; i++) {
            words.push_back(it->first);
        }
    }
    int index = (random()) % words.size();

    //std::string vstr = "{";
    //for (int i = 0; i < words.size(); i++) {
      //  vstr += words[i] + " ";
    //}
    //vstr += "}";
    //printf("%s\n", vstr.c_str());

    return words[index];
}

std::string MarkovChain::get_next_word(std::string word) {
    if (map.count(word) == 0) {
        return "и"; // Мда
    } else {
        //std::string prob_word = get_max_from_the_map(map[word]);
        std::string prob_word = get_probable_el(map[word]);
        printf("found pair {%s:%s}\n", word.c_str(), prob_word.c_str());
        return prob_word;
    }
}

std::string MarkovChain::get_max_from_the_map(std::map<std::string, int> maap) {
    using pair_type = decltype(maap)::value_type;
    auto pr = std::max_element (
            std::begin(maap), std::end(maap),
            [] (const pair_type & p1, const pair_type & p2) {
                return p1.second < p2.second;
            }
    );
    return pr->first;
}


