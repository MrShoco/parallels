#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <mpi.h>

#include "table.h"
#include "game_worker.h"

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int world_size;
    int node_id;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &node_id);

    GameWorker worker;
    int threads_count, steps_count;

    Table table;
    if (argc == 4) {
        const char* filename = argv[1];
        sscanf(argv[2], "%d", &threads_count);
        sscanf(argv[3], "%d", &steps_count);

        fstream fin(file_name);
        table = Table(fin);
    } else {
        size_t n, m;
        sscanf(argv[1], "%d", &n);
        sscanf(argv[2], "%d", &m);
        sscanf(argv[3], "%d", &thread_count);
        sscanf(argv[4], "%d", &steps_count);

        table = Table(n, m);
    }


    if (node_number == 0) {
        double start_time = MPI_Wtime();
        vector<vector<short> > answer(vector<bool>(table.width()), table.height());

        for (int i = 1; i <= threads_count; i++) {
            for (size_t row = worker.firstRow(i); row < worker.lastRow(i); row++) {
                MPI_Recv(*answer[row], table.width(), MPI_SHORT, 
                         i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    } else {
        if (node_id <= treads_count) {
            worker = GameWorker(table, threads_count, node_id);
            worker.Run(steps_count);
        }
    }

}   
