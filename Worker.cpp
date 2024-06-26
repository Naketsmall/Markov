//
// Created by epsilon on 08.04.24.
//

#include <algorithm>
#include <fstream>
#include <codecvt>
#include <mpi.h>
#include <sstream>
#include "Worker.h"
#include "resources.h"


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
    str.erase(std::remove_if(str.begin(), str.end(), IsChars("()/\" —!-?,.:;1234567890")), str.end());
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
        printf("n%d: File %s is opened\n", rank, filename.c_str());
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
    printf("n%d: size of map: %d\n", rank, map.get_size());
    return 0;
}

std::string get_str_message(){
    MPI_Status status;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    int count;
    MPI_Get_count(&status,MPI_CHAR,&count);

    char s2[count];
    MPI_Recv(&s2, count, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    return std::string(s2);
}

void Worker::work() {
    int buf = 1;
    std::string filename;
    MPI_Status status;

    while (true) {
        MPI_Send(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        filename = get_str_message();
        printf("n%d: got msg: %s\n", rank, filename.c_str());
        if (filename == "0")
            break;
        this->make_map(filename);
    }
    printf("n%d has finished making map\n", rank);

}

std::vector<std::string> split_msg(std::string msg, std::string delimiter) {
    std::vector<std::string> vec = {};

    size_t pos = 0;
    std::string token;
    while ((pos = msg.find(delimiter)) != std::string::npos) {
        token = msg.substr(0, pos);
        vec.push_back(token);
        msg.erase(0, pos + delimiter.length());
    }
    vec.push_back(msg);
    return vec;
}

int Worker::listen_merge() {
    int buf, rank2;
    MPI_Status status;
    while (1) {
        printf("n%d: merge worker ready\n", rank);
        MPI_Send(&CODE_WORKER_READY, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&buf, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (buf == CODE_MERGE_SHARE) {
            printf("n%d: merge code_share\n", rank);
            MPI_Recv(&rank2, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            merge(rank2, 1);
            MPI_Send(&CODE_WORKER_EXITED, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            return 0;
        } else if (buf == CODE_MERGE_GET) {
            printf("n%d: merge code_get\n", rank);
            MPI_Recv(&rank2, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            merge(rank2, 0);
        } else if (buf == CODE_WORKER_LET_EXIT) {
            printf("n%d: merge code_exit\n", rank);
            return 1;
        }
    }

}

int Worker::merge(int rank2, bool share) {
    std::unordered_map<std::string, std::map<std::string, int>> mc = map.get_map();
    std::string msg;
    printf("n%d: sssize of map: %d\n", rank, map.get_size());
    if (share) {
        for(std::unordered_map<std::string, std::map<std::string, int>>::iterator it = mc.begin(); it != mc.end(); ++it) {
            for (const auto &[key, value]: (it->second)) {
                msg = it->first + "/" + key + "/" + std::to_string(value);
                //printf("n%d: sending msg=%s\n", rank,  msg.c_str());
                MPI_Send(&msg[0], msg.size()+1, MPI_CHAR, rank2, 0, MPI_COMM_WORLD);
            }
        }
        msg = "0";
        MPI_Send(&msg[0], msg.size()+1, MPI_CHAR, rank2, 0, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        int count;
        std::vector<std::string> splitted_msg = {};
        char buff[1] = {'1'};
        while (true) {
            MPI_Probe(rank2, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
            MPI_Get_count(&status,MPI_CHAR, &count);
            char buff[count];
            MPI_Recv(&buff, count, MPI_CHAR, rank2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("n%d: recieving msg=%s\n", rank, buff);
            if (buff[0] == '0')
                break;
            splitted_msg = split_msg(std::string(buff), "/");
            map.insert(splitted_msg[0], splitted_msg[1], stoi(splitted_msg[2]));
            //map.print();
        }

    }
    return 1;
}

const MarkovChain &Worker::get_map() {
    return map;
}

void Worker::print_map() {
    map.print();
}

int Worker::listen_generate() {
    std::string n_word;
    std::string inp;
    while (true) {
        inp = get_str_message();
        n_word = map.get_next_word(inp);
        if (inp == "0") {
            printf("worker: finished listen_generate\n");
            return 0;
        }
        MPI_Send(&n_word[0], n_word.size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
}

int Worker::save_map_json(std::string filename) {
    std::stringstream ss = map.serialize_json();
    //std::cout << ss.str() << std::endl;
    std::ofstream f(filename);
    f << ss.rdbuf();
    f.close();
    return 0;
}

// /home/epsilon/CLionProjects/CNN/t1.txt
// /home/epsilon/CLionProjects/CNN/t2.txt