#include <random>
#include <iostream>
#include <unistd.h>
#include "../include/plin_index.h"

using TestIndex = PlinIndex;



void test(uint64_t thread_cnt){

    // NVM pool path
    TestIndex test_index("/mnt/pmem/poolfile", "plin", true);

    _payload_t answer;
    test_index.find(6468584279992246272, answer);
    std::cout<<answer<<std::endl;
    
    test_index.upsert(6468584279992246272, 2);

    test_index.find(6468584279992246272, answer);
    std::cout<<answer<<std::endl;


}

int main() {
    test(4);

}
