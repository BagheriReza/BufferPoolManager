#ifndef DBERROR_H
#define DBERROR_H

#include <stdlib.h>
#include <atomic>


std::atomic<int> transaction_counter(0);

// const int pageFrame_size = 1024*500;
// const int Page_Size = 1024;
// const int Num_Page = 1024*1024;

const int pageFrame_size = 8;
const int Page_Size = 20;
const int Num_Page = 20;

//total request from workload
//const int total_request = Num_Page*10;
const int total_request = 10;


const int RP_FIFO = 0;
const int RP_LRU = 1;
const int RP_MRU = 2;
const int RP_CLOCK = 3;
const int RP_RANDOM = 4;

const int Read_Transaction = 0;
const int Write_Transaction = 1; 
const int Scan_Transaction = 2;

std::string dbfile ="/dev/shm/db.txt";
//std::string dbfile ="db.txt";

const int DEFAULT_NUMBER_OF_WORKERS = 1;
#endif 