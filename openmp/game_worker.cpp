#include "game_worker.h"

GameWorker::GameWorker() {
}

GameWorker::GameWorker(Table table, size_t threads_number) {
    table_ = table;
    next_table_ = table;

    running_ = false;
    stopped_ = false;
    threads_count_ = threads_number;
}

void GameWorker::Status() {
    table_.out();
}

void GameWorker::Run(size_t steps) {
    running_ = true;
    stopped_ = false;

    size_t step = 0;
    std::cout << threads_count_ << std::endl;
    #pragma omp parallel num_threads(threads_count_) shared(step)
    for (; step < steps && !stopped_; step++) {
        #pragma omp barrier
        #pragma omp single
            step++;
        #pragma omp barrier
        #pragma omp for nowait
        for (size_t row = 0; row < next_table_.height(); row++) {
            for (size_t column = 0; column < next_table_.width(); column++) {
                next_table_.setCell(row, column, 
                                    table_.getNextCellValue(row, column));
            }
        }
 

        #pragma omp single
        swap(table_, next_table_);
    }
    running_ = false;
}

void GameWorker::Stop() {
    stopped_ = true;
}
