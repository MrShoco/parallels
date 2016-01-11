#include "game_worker.h"

GameWorker::GameWorker() {
}

GameWorker::GameWorker(Table table, size_t threads_count, size_t node_id) {
    table_ = table;
    next_table_ = table;

    threads_count_ = threads_count;
    node_id_ = node_id;
    prev_node_id_ = (node_id - 2 + threads_count) % threads_count + 1;
    next_node_id_ = (node_id) % threads_count + 1;
}

void GameWorker::Status() {
    table_.out();
}

void GameWorker::Run(size_t steps) {
    for (int step = 0; step < steps; step++) {
        MPI_Send((void*)table_.getRow(firstRow()), table_.width(), MPI_SHORT,
                  prev_node_id_, 0, MPI_COMM_WORLD);
        MPI_Send((void*)table_.getRow(lastRow()), table_.width(), MPI_SHORT,
                  next_node_id_, 0, MPI_COMM_WORLD);

        std::vector<short> prev_row(table_.width());
        MPI_Recv(&prev_row[0], prev_row.size(), MPI_SHORT,
                 next_node_id_, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::vector<short> next_row(table_.width());
        MPI_Recv(&next_row[0], next_row.size(), MPI_SHORT,
                 prev_node_id_, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        table_.setRow(table_.neighbourRow(firstRow(), -1), next_row);
        table_.setRow(table_.neighbourRow(lastRow(), 1), prev_row);

        for (size_t row = firstRow(); row <= lastRow(); row++) {
            for (size_t column = 0; column < next_table_.width(); column++) {
                next_table_.setCell(row, column, 
                                    table_.getNextCellValue(row, column));
            }
        }
 

        swap(table_, next_table_);
    }

    for (int row = firstRow(); row <= lastRow(); row++) {
        MPI_Send((void*)table_.getRow(row), table_.width(), MPI_SHORT,
                 0, 0, MPI_COMM_WORLD);
    }
}
