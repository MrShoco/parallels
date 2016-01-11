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
        const char* file_name = argv[1];
        sscanf(argv[2], "%d", &threads_count);
        sscanf(argv[3], "%d", &steps_count);

        fstream fin(file_name);
        table = Table(fin);
    } else {
        size_t n, m;
        sscanf(argv[1], "%d", &n);
        sscanf(argv[2], "%d", &m);
        sscanf(argv[3], "%d", &threads_count);
        sscanf(argv[4], "%d", &steps_count);

        table = Table(n, m);
    }

    if (node_id == 0) {
        worker = GameWorker(table, threads_count, node_id);
        double start_time = MPI_Wtime();
        vector<vector<short> > answer(table.height(), vector<short>(table.width()));

        for (int i = 1; i <= threads_count; i++) {
            for (size_t row = worker.firstRow(i); row <= worker.lastRow(i); row++) {
                MPI_Recv(&answer[row][0], table.width(), MPI_SHORT, 
                         i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        double end_time = MPI_Wtime();
        cout << "work time: " << end_time - start_time << endl;

      /*  for (int i = 0; i < answer.size(); i++) {
            for (int j = 0; j < answer[i].size(); j++) {
                if(answer[i][j])
                    cout << "*";
                else
                    cout << ".";
            }
            cout << endl;
	}*/
    } else {
        if (node_id <= threads_count) {
            worker = GameWorker(table, threads_count, node_id);
            worker.Run(steps_count);
        }
    }

    MPI_Finalize();
    return 0;
}   
