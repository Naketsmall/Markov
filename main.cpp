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
        printf("ERROR\n");
        MPI_Abort(MPI_COMM_WORLD, val);
    }
}

int main(int argc, char* argv[]) {

    //system("chcp 1251");

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
        //for (std::string f: master.get_filenames())
          //  printf("%s\n", f.c_str());
        MPI_Barrier(MPI_COMM_WORLD);
        master.listen();
    }
    else {
        MPI_Barrier(MPI_COMM_WORLD);
        Worker w(rank, size);
        w.work();
        //w.make_map(f1);
    }
    MPI_Finalize();
    return 0;
}









