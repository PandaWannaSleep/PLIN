#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <omp.h>

#include "common.h"
#include "../include/plin_index.h"


using TestIndex = PlinIndex;
// extern PMAllocator * galc;

using std::cout;
using std::endl;
using std::ifstream;

typedef double mytime_t;

_key_t *keys;

// Implement the preload function based on the specific situation of TestIndex.
template <typename IndexType>
void preload(IndexType &test_index, _key_t* keys, _payload_t* payloads, int64_t loadsize){
    std::sort(keys, keys + loadsize);
    test_index.bulk_load(keys, payloads, loadsize);
}

int main(int argc, char ** argv) {
    // int num_threads = 4;

    // if(argc > 1 && atoi(argv[1]) > 0) {
    //     num_threads = atoi(argv[1]);
    // }
    // open the data file
    std::string filename = "dataset.dat";
    std::ifstream fin(filename.c_str(), std::ios::binary);
    if(!fin) {
        cout << "File not exists or Open error\n";
        exit(-1);
    }

    // read all the key into vector keys
    keys = new _key_t[sizeof(_key_t) * LOADSCALE * MILLION];
    fin.read((char *)keys, sizeof(_key_t) * LOADSCALE * MILLION);

    cout << INDEXNAME << endl;
    TestIndex test_index("/mnt/pmem/poolfile");
    
    double start = seconds();
    preload(test_index, keys, keys, LOADSCALE * MILLION);
    double time = seconds() - start;
    std::cout<<"preload time:"<<time<<std::endl;

    delete keys;
    fin.close();
    return 0;
}


