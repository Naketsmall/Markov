//
// Created by epsilon on 10.04.24.
//

#include <mpi.h>
#include "Master.h"

Master::Master(int n) {
    num_processes = n;
    filenames = {};
}

void Master::add_filename(std::string filename) {
    filenames.push_back(filename);
}

void Master::listen() {
    int free_rank, buf;
    MPI_Status status;
    std::string filename;
    printf("master: started orchestring\n");
    while (!filenames.empty()) {
        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //TODO: tags, constants
        filename = filenames[filenames.size() - 1];
        buf = filename.size();
        //filenames.pop_back();

        printf("master: gonna send %s\n", filename.c_str());
        MPI_Send(&buf, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
        MPI_Send(&filename, filename.size(), MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
    printf("master: all filenames were sent\n");
    filename = "0";
    for (int i = 1; i < num_processes; i++) {
        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Send(filename.c_str(), filename.size(), MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
    printf("master: all workers were informed\n");

}

std::vector<std::string> Master::get_filenames() {
    return filenames;
}
