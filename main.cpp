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

    printf("%d\n", rank);


    std::string filename = "/home/epsilon/CLionProjects/CNN/text.txt";
    if (rank == 1) {
        Worker w1(0, 1);
        w1.make_map(filename);
    }
    MPI_Finalize();
    return 0;
}









