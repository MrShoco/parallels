#ifndef GAME_THREAD_H
#define GAME_THREAD_H

#include "game_worker.h"

class GameWorker;

class GameThread {
    public:

    void InitializeThread(GameWorker *worker, size_t first_row, size_t last_row);

    private:

    static void* ThreadActionPointer(void* arg);
    void ThreadAction(GameWorker *worker, size_t first_row, size_t last_row);

    pthread_t thread_id;
};

struct GameThreadArg {
    GameThread *thread;
    GameWorker *worker;
    size_t first_row;
    size_t last_row;
    
    GameThreadArg(GameThread *thread_, GameWorker *worker_,
                  size_t first_row_, size_t last_row_);
};

#endif
