//
// Created by epsilon on 08.04.24.
//

#include <algorithm>
#include <fstream>
#include "Worker.h"


class IsChars {
public:
    IsChars(const char *charsToRemove) : chars(charsToRemove) {};

    bool operator()(char c) {
        for (const char *testChar = chars; *testChar != 0; ++testChar)
            if (*testChar == c) { return true; }
        return false;
    }

private:
    const char *chars;
};

void Worker::clean_word(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), IsChars("() !?,.:;1234567890")), str.end());
    for (int i = 0; i < str.size(); i++)
        str[i] = tolower(str[i]);
}

Worker::Worker(int rank, int size) {
    this->rank = rank;
    this->size = size;
    map = {};
}

auto print_key_value = [](const auto &key, const auto &value) {
    printf("\t\tKey:[%s] Value:[%d]\n", key.c_str(), value);
};

auto print_map(std::unordered_map<std::string, std::map<std::string, int>> &map){
    for(std::unordered_map<std::string, std::map<std::string, int>>::iterator it = map.begin(); it != map.end(); ++it) {
        printf("Key: %s\n", (it->first).c_str());
        for (const auto &[key, value]: (it->second))
            print_key_value(key, value);
    }
}

void map_insert(std::unordered_map<std::string, std::map<std::string, int>> &map, std::map<std::string, int> buf,
                std::string &s1, std::string &s2) {
    if (map.find(s1) == map.end())
        map[s1] = buf;
    else {
        if (map[s1].find(s2) == map[s1].end())
            map[s1][s2] = 1;
        else
            map[s1][s2] = map[s1][s2] + 1;
    }
}

int Worker::make_map(std::string filename) {
    std::ifstream file(filename);

    if (file.is_open())
        printf("n%d: File is opened\n", rank);
    else {
        printf("n%d: ERROR - FILE CAN'T BE OPENED\n", rank);
        return 1; //TODO: Add resource file with constants and my errcodes
    }

    std::string s1, s2;
    file >> s1; //TODO: checking for zero string
    file >> s2;
    s1 = clean_word(s1);
    s2 = clean_word(s2);
    std::map<std::string, int> buf{{s2, 1}};

    while (!file.eof()) {
        buf[s2] = 1;
        map_insert(map, buf, s1, s2);
        buf.erase(s2);
        file >> s1;
        s1 = clean_word(s1);
        s1.swap(s2);
    }

    file.close();
    print_map(map);
    return 0;
}

int Worker::backup_to_storage() {
    return 0;
}

int Worker::merge() {
    return 0;
}


