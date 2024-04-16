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
        printf("master: making map finished\n");
        MPI_Barrier(MPI_COMM_WORLD);
        int last_worker = master.merge();
        printf("master: last_worker=%d\n", last_worker);
        MPI_Finalize();
    }
    else {
        Worker w(rank, size);
        MPI_Barrier(MPI_COMM_WORLD);
        w.work();
        printf("n%d: finished parsing\n", rank);
        MPI_Barrier(MPI_COMM_WORLD);
        printf("n%d: map_size before merge: %zu\n", rank, w.get_map().size());
        w.listen_merge();
        printf("n%d: map_size after merge: %zu\n", rank, w.get_map().size());

    }

    return 0;
}
