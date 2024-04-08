#include <cstdio>
#include <string>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <mpi.h>
#include "Worker.h"


void check(int val) {
    if (val != MPI_SUCCESS) {
        printf("ERROR\n");
        MPI_Abort(MPI_COMM_WORLD, val);
    }
}

int main(int argc, char* argv[]) {

    int rank, size;
    MPI_Status status;
    check(MPI_Init(&argc, &argv));
    check(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    check(MPI_Comm_size(MPI_COMM_WORLD, &size));

    printf("process:%d/%d has started\n", rank, size);


    std::string f1 = "/home/epsilon/CLionProjects/CNN/t1.txt";
    std::string f2 = "/home/epsilon/CLionProjects/CNN/t2.txt";
    if (rank == 0) {
        Worker w(rank, size);
        w.make_map(f1);
    }
    if (rank == 1) {
        Worker w(rank, size);
        w.make_map(f2);
    }
    MPI_Finalize();
    return 0;
}









