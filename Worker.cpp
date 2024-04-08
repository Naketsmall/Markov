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

std::string Worker::clean_word(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), IsChars("() !,.:;1234567890")), str.end());
    for (int i = 0; i < str.size(); i++)
        str[i] = tolower(str[i]);
        return str;
}

Worker::Worker(int rank, int size) {
    this->rank = rank;
    this->size = size;
    map = {};
}

int Worker::make_map(std::string filename) {
    std::ifstream file(filename);

    if(file.is_open())
        printf("n%d: File is opened\n", rank);
    else {
        printf("n%d: ERROR - FILE CAN'T BE OPENED\n", rank);
        return 1;
    }

    std::string s1, s2;
    std::map<std::string, int> buf{{s1, 1}};
    bool even = 0;
    file >> s1; //TODO: checking for zero string
    s1 = clean_word(s1);
    file >> s2;
    s2 = clean_word(s2);

    if (map.find(s1) == map.end()){
        map[s1] = buf;
    }

    while(!file.eof()) {
        if (even) {
            if (map.find(s2) == map.end()){
                map[s2] = buf;
            } else {
                map[s2][s1] = map[s2][s1] + 1;
            }
            file >> s1;
            s1 = clean_word(s1);
        } else {
            if (map.find(s1) == map.end()){
                map[s1] = buf;
            } else {
                map[s1][s2] = map[s1][s2] + 1;
            }
            file >> s2;
            s2 = clean_word(s2);
        }
        even = !even;
    }
    file.close();

    auto print_key_value = [](const auto& key, const auto& value) {
        printf("Key:[%s] Value:[%d]\n", key.c_str(), value);
    };

    for (const auto& [key, value] : map["my"]) // TODO: Чет фигня какая-то
        print_key_value(key, value);


    return 0;
}

int Worker::backup_to_storage() {
    return 0;
}

int Worker::merge() {
    return 0;
}


