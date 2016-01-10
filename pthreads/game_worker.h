#ifndef GAME_WORKER_H
#define GAME_WORKER_H

#include "table.h"
#include "game_thread.h"

class GameThread;

class GameWorker {
    friend class GameThread;

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
    int active_threads_count_;
    std::vector<GameThread> threads_;

    pthread_mutex_t iteration_mutex;
    pthread_cond_t iteration_cond;
    pthread_mutex_t finish_mutex;
    pthread_cond_t finish_cond;
};

#endif
