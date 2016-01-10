#include "game_worker.h"

GameWorker::GameWorker() {
}

GameWorker::GameWorker(Table table, size_t threads_number) {
    table_ = table;
    next_table_ = table;

    active_threads_count_ = 0; 
    running_ = false;
    stopped_ = false;
    pthread_mutex_init(&iteration_mutex, NULL);
    pthread_cond_init(&iteration_cond, NULL);
    pthread_mutex_init(&finish_mutex, NULL);
    pthread_cond_init(&finish_cond, NULL);

}

void GameWorker::Status() {
    table_.out();
}

void GameWorker::Run(size_t steps) {
    running_ = true;
    stopped_ = false;

    for (size_t step = 0; step < steps && !stopped_; step++) {
        active_threads_count_ =  threads_.size();
        
        pthread_cond_broadcast(&iteration_cond);
        pthread_mutex_lock(&finish_mutex);
        pthread_cond_wait(&finish_cond, &finish_mutex);
        pthread_mutex_lock(&iteration_mutex);
        pthread_mutex_unlock(&iteration_mutex);
        pthread_mutex_unlock(&finish_mutex);

        swap(table_, next_table_);
    }
    running_ = false;
}

void GameWorker::Stop() {
    stopped_ = true;
}

void GameWorker::initializeThreads(size_t threads_number) {
    threads_.resize(threads_number);
    size_t rows_count = (table_.height() + threads_.size() - 1) / threads_.size();
    for (size_t i = 0; i < threads_.size(); i++) {
        int first_row = i * rows_count;
        int last_row = std::min(first_row + rows_count, table_.height());
        threads_[i].InitializeThread(this, first_row, last_row);
    }
}
