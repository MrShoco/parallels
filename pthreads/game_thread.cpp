#include "game_thread.h"
#include <chrono>

GameThreadArg::GameThreadArg(GameThread *thread_, GameWorker *worker_,
                  size_t first_row_, size_t last_row_):
        thread(thread_), 
        worker(worker_), 
        first_row(first_row_), 
        last_row(last_row_)
    {
    }

void GameThread::InitializeThread(GameWorker *worker, size_t first_row, size_t last_row) {
    GameThreadArg* arg = new GameThreadArg(this, worker, first_row, last_row);

    pthread_create(&thread_id, NULL, ThreadActionPointer, (void*)arg);
}

void* GameThread::ThreadActionPointer(void* arg) {
    GameThreadArg *t_arg = (GameThreadArg*)arg;

    t_arg->thread->ThreadAction(t_arg->worker, t_arg->first_row, t_arg->last_row);
    delete t_arg;
    return NULL;
}

void GameThread::ThreadAction(GameWorker *worker, size_t first_row, size_t last_row) {
    while (true) {
        for (size_t row = first_row; row < last_row; row++) {
            for (size_t column = 0; column < worker->next_table_.width(); column++) {
                worker->next_table_.setCell(row, column, 
                                            worker->table_.getNextCellValue(row, column));
            }
        }
        pthread_mutex_lock(&(worker->iteration_mutex));
        worker->active_threads_count_--;
        if (worker->active_threads_count_ == 0) {
            pthread_cond_signal(&(worker->finish_cond));
        }
        pthread_cond_wait(&(worker->iteration_cond), &(worker->iteration_mutex));
        pthread_mutex_unlock(&(worker->iteration_mutex));
    }
}
