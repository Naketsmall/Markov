//
// Created by epsilon on 09.04.24.
//

#include "MarkovChain.h"

MarkovChain::MarkovChain() {
    map = {};
    buf = {};
    size_map = 0;
}

int MarkovChain::backup_to_storage() {
    return 0;
}

int MarkovChain::get_size() {
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


