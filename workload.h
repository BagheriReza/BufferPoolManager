#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "parameters.h"

using namespace std;



const int read_percentage = 65;
const int write_percentage = 25;
const int scan_percentage = 10;

struct transaction {
    int id = -1;
    int type = -1; // read ==0; write == 1; scan == 2;
    int pageId = -1;
    int end_pageId =-1; // it is use for scan transaction
};

void gen_workload(transaction* workload){
    srand (time(NULL));
    

    int num_read = int((total_request*read_percentage)/100);
    int num_write = int((total_request*write_percentage)/100);
    int num_scan = int((total_request*scan_percentage)/100);

    int read_counter = 0;
    int write_counter = 0;
    int scan_counter = 0;

    cout << "Number of read: " << num_read <<
    ", Number of write: " << num_write <<
    ", Number Scan: " << num_scan << endl;

    int rand_page;
    int rand_page_1;
    int i = 0;
    while(i < total_request){
        
        int type = rand() % 3 ;
        //cout << "i: "<< i<< ", type: "<< type << endl;
        if (type == Read_Transaction){
            cout << "Read_Transaction: "<< read_counter<< endl;
            if(read_counter >= num_read){
                continue;
            }
            read_counter++;
            rand_page = rand() % Num_Page;
            workload[i].id = i;
            workload[i].type = Read_Transaction;
            workload[i].pageId = rand_page;
            i++;

        } else if (type == Write_Transaction){
            cout << "Write_Transaction: "<< write_counter<< endl;
            if(write_counter >= num_write){
                continue;
            }
            write_counter++;
            rand_page = rand() % Num_Page;
            workload[i].id = i;
            workload[i].type = Write_Transaction;
            workload[i].pageId = rand_page;
            i++;

        } else {
            cout << "Scan_Transaction: "<< scan_counter<< endl;
            if(scan_counter >= num_scan){
                continue;
            }
            scan_counter++;
            rand_page = rand() % Num_Page;
            rand_page_1 = rand() % Num_Page;
            if (rand_page < rand_page_1){
                workload[i].id = i;
                workload[i].type = Scan_Transaction;
                workload[i].pageId = rand_page;
                workload[i].end_pageId = rand_page_1;

            } else if (rand_page > rand_page_1){
                workload[i].id = i;
                workload[i].type = Scan_Transaction;
                workload[i].pageId = rand_page_1;
                workload[i].end_pageId = rand_page;

            } else {
                workload[i].id = i;
                workload[i].type = Scan_Transaction;
                workload[i].pageId = rand_page;
                workload[i].end_pageId = rand_page;
            }
            i++;
        }
    }
    cout<< "finish \n";
}

void print_workload(transaction* workload){
    for (int i=0; i<total_request;i++){
        cout<< "transaction ID: "<< workload[i].id<<
        ", Transaction type: "<< workload[i].type <<
        ", start page Id: "<< workload[i].pageId<<
        ", end page Id: "<< workload[i].end_pageId << endl;
    }
}
// total_request is 2
void gen_workload_fifo(transaction* workload){
    int i = 1;
    //id=0, scan
    workload[0].id = 0;
    workload[0].type = Scan_Transaction;
    workload[0].pageId = 10;
    workload[0].end_pageId = 10+pageFrame_size+i;

    workload[1].id = 1;
    workload[1].type = Scan_Transaction;
    workload[1].pageId = 10;
    workload[1].end_pageId = 10+pageFrame_size+i;

    //id=1, 
}

// total_request is 4
void gen_workload_lru(transaction* workload){
    int i = 1;
    //id=0, scan
    workload[0].id = 0;
    workload[0].type = Scan_Transaction;
    workload[0].pageId = 10;
    workload[0].end_pageId = 10+pageFrame_size;

    workload[1].id = 1;
    workload[1].type = Scan_Transaction;
    workload[1].pageId = 10;
    workload[1].end_pageId = 10+pageFrame_size-1;
    
    workload[2].id = 2;
    workload[2].type = Read_Transaction;
    workload[2].pageId = 18;
    workload[2].end_pageId = -1;

    workload[3].id = 2;
    workload[3].type = Read_Transaction;
    workload[3].pageId = 10;
    workload[3].end_pageId = -1;

    workload[4].id = 3;
    workload[4].type = Scan_Transaction;
    workload[4].pageId = 10;
    workload[4].end_pageId = 10+pageFrame_size;

    //id=1, 
}

//total_request = 5
void gen_workload_clock(transaction* workload){
    int i = 1;
    //id=0, scan
    workload[0].id = 0;
    workload[0].type = Scan_Transaction;
    workload[0].pageId = 10;
    workload[0].end_pageId = 10+pageFrame_size+1;
    
    workload[1].id = 2;
    workload[1].type = Read_Transaction;
    workload[1].pageId = 11;
    workload[1].end_pageId = -1;

    workload[2].id = 2;
    workload[2].type = Read_Transaction;
    workload[2].pageId = 12;
    workload[2].end_pageId = -1;

    workload[3].id = 2;
    workload[3].type = Read_Transaction;
    workload[3].pageId = 13;
    workload[3].end_pageId = -1;

    workload[4].id = 2;
    workload[4].type = Read_Transaction;
    workload[4].pageId = 18;
    workload[4].end_pageId = -1;

    workload[5].id = 0;
    workload[5].type = Scan_Transaction;
    workload[5].pageId = 10;
    workload[5].end_pageId = 10+pageFrame_size+1;


    //id=1, 
}

void gen_workload_clock2(transaction* workload){
    int i = 1;
    //id=0, scan
    workload[0].id = 0;
    workload[0].type = Scan_Transaction;
    workload[0].pageId = 0;
    workload[0].end_pageId = pageFrame_size;

    workload[1].id = 0;
    workload[1].type = Scan_Transaction;
    workload[1].pageId = 0;
    workload[1].end_pageId = 4;

    workload[2].id = 0;
    workload[2].type = Scan_Transaction;
    workload[2].pageId = 11;
    workload[2].end_pageId = 14;

    workload[3].id = 0;
    workload[3].type = Scan_Transaction;
    workload[3].pageId = 0;
    workload[3].end_pageId = 4;

    workload[4].id = 0;
    workload[4].type = Scan_Transaction;
    workload[4].pageId = 11;
    workload[4].end_pageId = 14;
    //id=1, 
}


void gen_warmup(transaction* workload, int total_request){
    srand (time(NULL));
    

    int num_read = int((total_request*read_percentage)/100);
    int num_write = int((total_request*write_percentage)/100);
    int num_scan = int((total_request*scan_percentage)/100);

    int read_counter = 0;
    int write_counter = 0;
    int scan_counter = 0;

    cout << "Number of read: " << num_read <<
    ", Number of write: " << num_write <<
    ", Number Scan: " << num_scan << endl;

    int rand_page;
    int rand_page_1;
    int i = 0;
    while(i < total_request){
        int type = rand() % 3 ;
        if (type == Read_Transaction){
            if(read_counter >= num_read){
                continue;
            }
            read_counter++;
            rand_page = rand() % Num_Page;
            workload[i].id = i;
            workload[i].type = Read_Transaction;
            workload[i].pageId = rand_page;
            i++;

        } else if (type == Write_Transaction){
            if(write_counter >= num_write){
                continue;
            }
            write_counter++;
            rand_page = rand() % Num_Page;
            workload[i].id = i;
            workload[i].type = Write_Transaction;
            workload[i].pageId = rand_page;
            i++;

        } else {
            if(scan_counter >= num_scan){
                continue;
            }
            scan_counter++;
            rand_page = rand() % Num_Page;
            rand_page_1 = rand() % Num_Page;
            if (rand_page < rand_page_1){
                workload[i].id = i;
                workload[i].type = Scan_Transaction;
                workload[i].pageId = rand_page;
                workload[i].end_pageId = rand_page_1;

            } else if (rand_page > rand_page_1){
                workload[i].id = i;
                workload[i].type = Scan_Transaction;
                workload[i].pageId = rand_page_1;
                workload[i].end_pageId = rand_page;

            } else {
                workload[i].id = i;
                workload[i].type = Scan_Transaction;
                workload[i].pageId = rand_page;
                workload[i].end_pageId = rand_page;
            }
            i++;
        }
    }
}

void gen_workload_read(transaction* workload){

    workload[0].id = 0;
    workload[0].type = Read_Transaction;
    workload[0].pageId = 10;
    workload[0].end_pageId = -1;
}