#ifndef GAME_WORKER_H
#define GAME_WORKER_H

#include <mpi.h>

#include "table.h"

class GameWorker {
    public:
    GameWorker();
    GameWorker(Table table, size_t threads_count, size_t node_id);

    void Status();
    void Run(size_t steps);
    void Stop();
    void initializeThreads(size_t treads_number);

    size_t firstRow(size_t node_id) {
        return rowSize() * (node_id - 1);
    }
    size_t lastRow(size_t node_id) {
        return std::min(firstRow(node_id) + rowSize(), table_.height()) - 1;
    }

    Table table_;
    Table next_table_;
    private:

    size_t firstRow() {
        return firstRow(node_id_);
    }
    size_t lastRow() {
        return lastRow(node_id_);
    }
    size_t rowSize() {
        return (table_.height() + threads_count_ - 1) / threads_count_;
    }

    size_t threads_count_;
    size_t node_id_;
    size_t prev_node_id_;
    size_t next_node_id_;
};

#endif
