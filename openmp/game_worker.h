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

    Table table_;
    Table next_table_;
    private:


    bool running_;
    bool stopped_;
    size_t threads_count_;
};

#endif
