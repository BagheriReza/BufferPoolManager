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
  transaction *workload;
  int RP;

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
  int replacement_policy = arg->RP;

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
  double execution_time ;
  timer t1;

  t1.start();
  while (local_couter < total_request){
 


    if (workload[local_couter].type == Read_Transaction){
      
      
      read(temp1,workload[local_couter].pageId,&pageTable,&bpm,pageFrame );



    }else if (workload[local_couter].type == Write_Transaction){
      write(temp1,workload[local_couter].pageId,&pageTable,&bpm,pageFrame );

    } else {
      scan(temp1,workload[local_couter].pageId, workload[local_couter].end_pageId,&pageTable,&bpm,pageFrame );

    } 
    local_couter++;
    
  }  

  execution_time = t1.stop(); 
 
  std::cout<< "replacement policy: "<<replacement_policy 
    << " ,Number of hit: "<< bpm.hit <<
      ", NUmber of miss: "<< bpm.fail_hit << endl;
  
  cout << "Execution_time" <<execution_time << endl;
  //bpm.print_statistic(); 

////////////////////////////// Tetsing CLOCK ///////////////////////////////
  // char temp1[Page_Size];
  // read_disk_page(temp1, 9);
 
  // read(temp1,0,&pageTable,&bpm,pageFrame ); 
  // print_page_status(Num_Page,&pageTable,pageFrame);
  // print_page(temp1);
  // cout <<"CLOCK:" <<(pageTable.clock) << endl;


  // read(temp1,1,&pageTable,&bpm,pageFrame ); 
  // print_page(temp1); 
  // print_page_status(Num_Page,&pageTable,pageFrame);
  // cout <<"CLOCK:" <<(pageTable.clock) << endl;

  // read(temp1,2,&pageTable,&bpm,pageFrame );
  // print_page(temp1); 
  // print_page_status(Num_Page,&pageTable,pageFrame);
  // cout <<"CLOCK:" <<(pageTable.clock) << endl;

  // scan(temp,10, 20,&pageTable,&bpm,pageFrame );
  // cout <<"CLOCK:" <<(pageTable.clock) << endl;

  // read(temp1,11,&pageTable,&bpm,pageFrame );
  // print_page_status(Num_Page,&pageTable,pageFrame);
  // print_page(temp1);
  // cout <<"CLOCK:" <<(pageTable.clock) << endl;

  // read(temp1,1,&pageTable,&bpm,pageFrame );
  // print_page_status(Num_Page,&pageTable,pageFrame);
  // print_page(temp1);
  // cout <<"CLOCK:" <<(pageTable.clock) << endl; 

  // read(temp1,12,&pageTable,&bpm,pageFrame );
  // print_page_status(Num_Page,&pageTable,pageFrame);
  // print_page(temp1);
  // cout <<"CLOCK:" <<(pageTable.clock) << endl; 

  // bpm.print_statistic();  
  ////////////////////////////// End of Tetsing CLOCK ///////////////////////////////


  free(pageFrame);

  return 0;
} 

int main(){
  std::vector<std::thread> th;
  int n_workers = 1;
 
  transaction workload[total_request];
  gen_workload_lru(workload);   
  print_workload(workload); 
   
  std::thread testTh;
  for (int i = 0; i < n_workers; i++){
    arguments *args = new arguments;
    args->workload = workload;   
    args->RP = i;
    
    th.push_back(std::thread(worker , args)); 
  }  
  
  std::for_each(th.begin(), th.end(), [](std::thread &t) 
    { 
        t.join(); 
    });      

}         