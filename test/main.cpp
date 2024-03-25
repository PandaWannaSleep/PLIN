#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
#include <unistd.h>
#include <omp.h>

#include "common.h"
#include "../include/plin_index.h"

using TestIndex = PlinIndex;

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

template<typename IndexType>
double run_test(std::vector<QueryType> querys, int thread_cnt) {
    // recover the index from preload
    IndexType test_index("/mnt/pmem/poolfile", "plin", true);

    // define a atomic var for current position of queries in MP environment
    std::atomic_int cur_pos(0);     //initial as 0
    // ensure the insert key be different from the keys exists in the index
    int small_noise = getRandom() & 0xff;   

    // set the timer
    #pragma omp barrier
    auto start = seconds();

    // start the parallel
    #pragma omp parallel num_threads(thread_cnt)
    {
        #pragma omp for schedule(static)
        for(size_t i = 0; i < querys.size(); ++i) {
            int pos = cur_pos.fetch_add(1, std::memory_order_relaxed);
            OperationType op = querys[pos].op;
            _key_t key = querys[pos].key;

            switch (op)
            {
            case OperationType::READ: {
                _payload_t answer;
                test_index.find(key, answer);
                if(_key_t(answer) != key){
                    std::cout<<"#Query: "<<i<<std::endl;
                    std::cout<<"#Wrong answer: "<<answer<<std::endl;
                    std::cout<<"#Correct answer: "<<key<<std::endl;
                    throw std::logic_error("READ Operation Answer wrong!");
                }
                break;
            }
            case OperationType::INSERT: {
                test_index.upsert(key, key);
                break;
            }
            case OperationType::UPDATE: {
                test_index.upsert(key, key);
                break;
            }
            case OperationType::DELETE: {
                test_index.remove(key);
                break;
            }
            
            default:
                std::cout << "Error: unknown operation!" << std::endl;
                exit(0);
                break;
            }
        }
    }
    #pragma omp barrier
    auto end = seconds();

    return end - start;
}

int main(int argc, char ** argv){
    string wl_fname = "workload.txt";
    int num_thread = 4;

    static const char * optstr = "f:t:h";
    opterr = 0;
    char opt;
    while((opt = getopt(argc, argv, optstr)) != -1){
        switch(opt) {
        case 'f':
            wl_fname = string(optarg);
            break;
        case 't':
            if(atoi(optarg) > 0)
                num_thread = atoi(optarg);
            break;
        case '?':
        case 'h':
        default:
            cout << "USAGE: "<< argv[0] << "[option]" << endl;
            cout << "\t -h: " << "Print the USAGE" << endl;
            cout << "\t -f: " << "Filename of the workload" << endl;
            cout << "\t -t: " << "Number of Threads to excute the workload" << endl;
            cout << "\t -i: " << "The index tree type" << endl;
            exit(-1);
            break;
        }
        
    }

    ifstream fin(wl_fname.c_str());
    if(!fin) {
        cout << "workload file not openned" << endl;
        exit(-1);
    }
    std::vector<QueryType> querys;
    int op;
    _key_t key;
    while(fin >> op >> key) {
        querys.push_back({(OperationType)op, key});
    }

    double time = run_test<TestIndex>(querys, num_thread);

    // cout<<"Thread "<<num_thread<<" execution time:"<<time<<endl;
    cout<<time<<endl;

    return 0;

}