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

void clean_word(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), IsChars("() !?,.:;1234567890")), str.end());
    for (int i = 0; i < str.size(); i++)
        str[i] = tolower(str[i]);
}

Worker::Worker(int rank, int n) {
    this->rank = rank;
    this->num_processes = n;
    map = {};
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
    clean_word(s1);
    clean_word(s2);

    while (!file.eof()) {
        map.insert(s1, s2);
        file >> s1;
        clean_word(s1);
        s1.swap(s2);
    }

    file.close();
    map.print();
    return 0;
}


