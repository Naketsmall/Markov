//
// Created by epsilon on 10.04.24.
//

#include <mpi.h>
#include <sstream>
#include "Master.h"
#include "resources.h"


Master::Master(int n) {
    num_processes = n;
    filenames = {};
}

void Master::add_filename(std::string filename) {
    filenames.push_back(filename);
}

void Master::analyze() {
    int free_rank, buf;
    MPI_Status status;
    std::string filename;
    printf("master: started orchestring\n");
    while (!filenames.empty()) {
        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //TODO: tags, constants
        filename = filenames[filenames.size() - 1];
        buf = filename.size();
        filenames.pop_back();

        //printf("master: gonna send %s\n", filename.c_str());
        MPI_Send(&filename[0], filename.size()+1, MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
    printf("master: all filenames were sent\n");
    filename = "0";
    for (int i = 1; i < num_processes; i++) {
        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Send(&filename[0], filename.size()+1, MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
    printf("master: all workers were informed\n");
}

std::vector<std::string> Master::get_filenames() {
    return filenames;
}

int Master::merge() {
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    int buf, workers_active;
    int queue = -1 ;
    workers_active = num_processes - 1;
    MPI_Status status;
    while (workers_active > 1) {
        printf("master: workers_active=%d\n", workers_active);
        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (buf == CODE_WORKER_EXITED) {
            printf("master: n%d exited\n", status.MPI_SOURCE);
            workers_active -= 1;
        } else if (buf == CODE_WORKER_READY) {
            if (queue > 0) {
                printf("master: n%d was second\n", status.MPI_SOURCE);
                MPI_Send(&CODE_MERGE_SHARE, 1, MPI_INT, queue, 0, MPI_COMM_WORLD);
                MPI_Send(&status.MPI_SOURCE, 1, MPI_INT, queue, 0, MPI_COMM_WORLD);
                MPI_Send(&CODE_MERGE_GET, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                MPI_Send(&queue, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                queue = -1;
            } else {
                printf("master: n%d was added to queue\n", status.MPI_SOURCE);
                queue = status.MPI_SOURCE;
            }
        }
    }
    if (queue > 0) {
        MPI_Send(&CODE_WORKER_LET_EXIT, 1, MPI_INT, queue, 0, MPI_COMM_WORLD);
        return queue;
    }

    printf("master: finished cycle\n");
    MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("master: got recv\n");
    MPI_Send(&CODE_WORKER_LET_EXIT, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    return status.MPI_SOURCE;
}

std::string Master::make_sentence(std::string word, int length, int w_rank) {
    std::stringstream ss;
    std::string cur = std::string(word);
    ss << (word + " ");
    printf("master: make_sentence started\n");
    int count;
    MPI_Status status;
    for (int i = 1; i < length; i++) {
        printf("master: cur[%d]=%s\n", i, cur.c_str());
        MPI_Send(&cur[0], cur.size()+1, MPI_CHAR, w_rank, 0, MPI_COMM_WORLD);
        MPI_Probe(w_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status,MPI_CHAR,&count);
        char s2[count];
        MPI_Recv(&s2, count, MPI_CHAR, w_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Got s[%d]: %s\n", i, s2);
        ss << s2;
        ss << " ";
        cur = std::string(s2);
    }
    printf("master: make_sentence finished\n");
    cur = "0";
    MPI_Send(&cur[0], cur.size()+1, MPI_CHAR, w_rank, 0, MPI_COMM_WORLD);
    return ss.str();
}

