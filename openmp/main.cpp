#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <omp.h>

#include "table.h"
#include "game_worker.h"

using namespace std;

struct GameArg {
    GameWorker *worker;
    size_t steps_count;
    GameArg(GameWorker *worker_, size_t steps_count_): 
        worker(worker_), steps_count(steps_count_) {}
};

void helpOutput(string cmd = "");

void* runGame(void* arg) {
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

    GameArg *game_arg = (GameArg*)arg;

    game_arg->worker->Run(game_arg->steps_count);

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    std::cout << duration << std::endl;

    delete game_arg;
    return NULL;
}

int main() {
    omp_set_nested(1);
    #pragma omp parallel num_threads(2)
    {
        #pragma omp single
        {
            string cmd;
            GameWorker worker;
            helpOutput();
            
            while (true) {
                cin >> cmd;
                transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
                if (cmd == "start") {
                    string file_name;
                    cout << "Read from (f)ile or generate (r)andom" << endl;
                    cin >> cmd;

                    if (cmd == "f" || cmd == "r") {
                        Table table;
                        if (cmd == "f") {
                            cout << "Enter file name:" << endl;
                            cin >> file_name;

                            fstream fin(file_name);
                            table = Table(fin);
                        } else {
                            cout << "Enter n and m:" << endl;
                            size_t n, m;
                            cin >> n >> m;
                            table = Table(n, m);
                        }

                        size_t threads_number;
                        cout << "Enter number of threads:" << endl;
                        cin >> threads_number;

                        worker = GameWorker(table, threads_number);
                    } else {
                        cout << "Wrong command" << endl;
                    }
                } else if (cmd == "status") {
                    if (worker.isRunning())
                        cout << "Is running now" << endl;
                    else
                        worker.Status();
                } else if (cmd == "run") {
                    if (worker.isRunning())
                        cout << "Already run" << endl;
                    else {
                        cout << "Enter number of steps" << endl; 
                        size_t steps_count;
                        cin >> steps_count;

                        #pragma omp task
                        {
                            runGame((void*)(new GameArg(&worker, steps_count)));
                        }
                    }
                } else if (cmd == "stop") {
                    if (worker.isStopped())
                        cout << "Already stopped" << endl;
                    else
                        worker.Stop();
                } else if (cmd == "quit") {
                    cout << "Good bye..." << endl;
                    break;
                } else {
                    helpOutput(cmd);
                }
            }
        }
    }
}   

void helpOutput(string cmd) {
    if(cmd.size() > 0)
        cout << "Unknown command: " << cmd << endl;
    cout << "Commands:" << endl;
    cout << "START" << endl;
    cout << "STATUS" << endl;
    cout << "RUN" << endl;
    cout << "STOP" << endl;
    cout << "QUIT" << endl;
    if(cmd.size() > 0)
        cout << "Try again..." << endl;

}
