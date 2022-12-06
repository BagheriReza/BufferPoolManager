#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <atomic>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <mutex>

#include "buffer_manager.h"
#include "parameters.h"
#include "workload.h"
#include "get_time.h"

//#include "filemanager.h"
using namespace std;

struct arguments {
  transaction *warmup;
  transaction *workload;
  int RP;
  int workload_type;

};

struct thread_data
{
    int hit;
    int fail_hit;
};


int worker(void * _args)
{
  arguments *arg = (arguments *)_args;
  transaction *workload = arg->workload;
  transaction *warmup = arg->warmup;
  int replacement_policy = arg->RP;
  int end = arg->workload_type;
  cout << "replacemnet_policy: "<< replacement_policy<< ", type: "<< end<< endl;
  //transaction workload[total_request];

  cout << "Initial the pageFrame  ...!\n";
  //intial disk file
  disk_initial(); 

  // Initial the pageFrame 
  page *pageFrame;
  init_pageFrame(pageFrame);
  // this part is used to verfiy the pageFrame
  //verification_pageFrame(pageFrame);

  pageTable_hashMap pageTable;
  pageTable.init_empty();
  //verification_pageTable(&pageTable);

  bufferPoolManager bpm;
 
  // define the policy
  bpm.replacementPolicy = replacement_policy;

  char temp1[Page_Size]; // for reading and writing
  //read_disk_page(temp1, 9); // dummy for writing
  //bpm.print_statistic();
  int local_couter = 0;

  int warmup_end = Num_Page*10;

  // // warm up
  // while (local_couter < warmup_end){
 
  //   if (warmup[local_couter].type == Read_Transaction){

  //     read(temp1,warmup[local_couter].pageId,&pageTable,&bpm,pageFrame );

  //   }else if (warmup[local_couter].type == Write_Transaction){
  //     write(temp1,warmup[local_couter].pageId,&pageTable,&bpm,pageFrame );

  //   } else {
  //     scan(temp1,warmup[local_couter].pageId, warmup[local_couter].end_pageId,&pageTable,&bpm,pageFrame );

  //   }
  //   local_couter++;
  // } 

  //resting hit counter and missing counter
      
  bpm.reset_statistic();     
  // std::cout<< "replacement policy: "<<replacement_policy 
  //   << " ,Number of hit: "<< bpm.hit <<
  //     ", NUmber of miss: "<< bpm.fail_hit << endl;

  //real work_load

  // while (transaction_counter < total_request){
 
  //   if (workload[transaction_counter].type == Read_Transaction){

  //     read(temp1,workload[transaction_counter].pageId,&pageTable,&bpm,pageFrame );

  //   }else if (workload[transaction_counter].type == Write_Transaction){
  //     write(temp1,workload[transaction_counter].pageId,&pageTable,&bpm,pageFrame );

  //   } else {
  //     scan(temp1,workload[transaction_counter].pageId, workload[transaction_counter].end_pageId,&pageTable,&bpm,pageFrame );

  //   }
  //   transaction_counter++;
     
  // } 

  scan(temp1,0, end,&pageTable,&bpm,pageFrame ); 

  std::cout<< "replacement policy: "<<replacement_policy 
    << " ,Number of hit: "<< bpm.hit <<
      ", NUmber of miss: "<< bpm.fail_hit << endl;
  //bpm.print_statistic(); 
  float total_hit_miss = ((float)bpm.fail_hit+(float)bpm.hit);
  float missRate = ((float)bpm.fail_hit) / total_hit_miss;
  float hitrate = ((float)bpm.hit) / total_hit_miss;
  cout << " miss rate: "<< missRate << ", hit rate: "<< hitrate << endl; 


  free(pageFrame);

  return 0;
} 

int main(int argc, char *argv[]){
  

  for (int i = 0; i < argc; ++i)
        cout << argv[i] << "\n";
  int n_workers = atoi(argv[1]);
  int work_load = atoi(argv[2]);
  std::cout << "Number of workers : " << n_workers <<", Workload type: " <<work_load <<"\n";


  std::vector<std::thread> th;
  //int n_workers = 2;
  timer t1;
  double execution_time;
  transaction workload[total_request];
  int num_page_10 = 10;
  transaction warmup[num_page_10];
  
  // gen_warmup(warmup,num_page_10 ); 

  // gen_workload(workload); 
  //print_workload(workload);
  t1.start();
  //std::thread testTh;
  for (int i = 0; i < n_workers; i++){
    arguments *args = new arguments;
    args->workload = workload;   
    args->warmup = warmup; 
    args->RP = RP_FIFO;
    args->workload_type = work_load; 
    
    th.push_back(std::thread(worker , args));  
  }  
 
  std::for_each(th.begin(), th.end(), [](std::thread &t) 
    { 
        t.join(); 
    });   
  execution_time = t1.stop();
  cout << "execution_time: "<<execution_time << endl;;              
}  