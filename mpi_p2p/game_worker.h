#ifndef GAME_WORKER_H
#define GAME_WORKER_H

#include "table.h"

class GameWorker {
    public:
    GameWorker();
    GameWorker(Table table, size_t threads_number);

    void Status();
    void Run(size_t steps);
    void Stop();
    void initializeThreads(size_t treads_number);

    bool isRunning() {
        return running_;
    }
    bool isStopped() {
        return stopped_;
    }

    size_t firstRow(size_t node_id = node_id_) {
        return rowSize() * node_id;
    }
    size_t lastRow(size_t node_id = node_id_) {
        return std::min(firstRow(node_id) + rowSize(), table.height());
    }

    Table table_;
    Table next_table_;
    private:

    size_t rowSize() {
        return (table.height() + threads_count_ - 1) / treads_count_;
    }
    size_t threads_count_;
    size_t node_id_;
    size_t prev_node_id_;
    size_t next_node_id_;
};

#endif
