//
// Created by epsilon on 08.04.24.
//

#include <algorithm>
#include <fstream>
#include <codecvt>
#include <mpi.h>
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
    str.erase(std::remove_if(str.begin(), str.end(), IsChars("() —!-?,.:;1234567890")), str.end());
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

    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    file.imbue(utf8_locale);

    if (file.is_open())
        printf("n%d: File is opened\n", rank);
    else {
        printf("n%d: ERROR - FILE CAN'T BE OPENED\n", rank);
        return 1; //TODO: Add resource file with constants and my errcodes
    }

    std::string s1, s2;
    file >> s1;
    file >> s2;
    clean_word(s1);
    clean_word(s2);

    while (!file.eof()) {
        if (!s1.empty() && !s2.empty())
            map.insert(s1, s2);
        file >> s1;
        clean_word(s1);
        s1.swap(s2);
    }

    file.close();
    //map.print();
    printf("size of map: %d", map.get_size());
    return 0;
}

void Worker::work() {
    int buf = 1;
    std::string filename;
    MPI_Status status;

    while (true) {
        MPI_Send(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&filename, buf, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("n%d: got msg: %s\n", rank, filename.c_str());
        if (filename == "0")
            break;
        this->make_map(filename);
    }
    printf("n%d has finished making map\n", rank);


}

// /home/epsilon/CLionProjects/CNN/t1.txt
//