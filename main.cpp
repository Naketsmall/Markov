#include <cstdio>
#include <string>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <mpi.h>
#include "Worker.h"
#include "Master.h"


void check(int val) {
    if (val != MPI_SUCCESS) {
        printf("ERROR WITH MPI\n");
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


    if (rank == 0) {
        Master master(size);


        std::string inp;
        printf("Enter filenames\n");
        std::cin >> inp;
        while (inp != "0"){
            master.add_filename(inp);
            std::cin >> inp;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        master.analyze();
        printf("master: finished\n");
        MPI_Barrier(MPI_COMM_WORLD);
    }
    else {
        MPI_Barrier(MPI_COMM_WORLD);
        Worker w(rank, size);
        w.work();
        printf("n%d: finished parsing\n", rank);
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 1) {
            printf("n%d: map_size before merge: %zu\n", rank, w.get_map().size());
            w.merge(2, 0);
            printf("n%d: map_size after merge: %zu\n", rank, w.get_map().size());

        }
        if (rank == 2) {
            printf("n%d: map_size before merge: %zu\n", rank, w.get_map().size());
            w.merge(1, 1);

        }

    }
    MPI_Finalize();
    return 0;
}
