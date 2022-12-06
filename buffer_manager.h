#include "parameters.h"
#include "filemanager.h"

#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <time.h>

using namespace std;







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    buffer poolmanager struct  ///////////////////////////////////////////////////////////////////////////
// 
struct bufferPoolManager{
    int pageFrameCounter = 0;
    int replacementPolicy = 0;
    int num_read = 0;
    int num_write = 0;
    int num_scan = 0;
    int num_scan_read = 0;
    int hit =0;
    int fail_hit = 0;

    void change_replacement_policy(int rep){
      replacementPolicy = rep;
    }

    void print_statistic(){
      cout<< "Number of hit: "<< hit <<
      ", NUmber of miss: "<< fail_hit << endl;
    }

    void reset_statistic(){
      hit =0;
      fail_hit = 0;
    }

};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    pageFrame struct and functions ///////////////////////////////////////////////////////////////////////////
// struct of page which include array of char with size of 
struct page {
    char content[Page_Size];
    int pageId; // real page_ID in the pageFrame
    int pin_counter = 0; //number of access to page
    bool dirty_flag; //show it has been changed
    bool ref_bit; // intial value is false
};

void print_page(char* page){
    for(int i=0; i< Page_Size; i++){
        cout << page[i] << ", ";
    }
    cout<< endl;
}



//init the pageFram
void init_pageFrame(page*& pageFrame){
    pageFrame = (page *) calloc(pageFrame_size, sizeof(page));
    if (pageFrame == NULL) {
        cout << "There is error in allocation memory for pageFrame (init_pageFrame function)!\n";
    }
}

// free the memory after all jobs are done
// void free_pageFrame(page*& pageFrame){
//     free(pageFrame);
// }

//verifiing pageframe

void verification_pageFrame (page*& pageFrame){
    cout << "starting pageFrame verfication...!\n";

    // page* pageFrame;
    // init_pageFrame(pageFrame);

    for(int i =0 ; i < pageFrame_size; i++){
        page page_;
        for (int j=0; j<Page_Size;j++){
            page_.content[j] = 65+(int)(j%26);
        }
        pageFrame[i] = page_;
    }
    int k = 3;
    int z = 6;
    cout << pageFrame[k].content[z] << "\n";

    pageFrame[k].content[z] = '*';

    cout << pageFrame[k].content[z] << "\n";


    for(int i =0 ; i < pageFrame_size; i++){
        page page_;
        page_ = pageFrame[i];

        for (int j=0; j<Page_Size;j++){
            if (page_.content[j] != 65+(int)(j%26)){
                cout << " there is failer in the pageFram read and write verification!"<< ", i: "<< i<< ", j: "<<j<< endl ;

            };
        }
        
    }

    //free(pageFrame);

    cout<< "The pageFrame verfication is done!\n";

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    pageTable struct and functions ///////////////////////////////////////////////////////////////////////////
//init hash function
struct pageTable_hashMap{
    std::map<int, int> m;
    std::map<int, bool> empty;
    std::deque<int> fifo;
    std::deque<int> lru;
    std::deque<int> mru;
    std::deque<int> clock;

    int get_keyByValue(int key){
      for (auto &i : m) {
        if (i.second == key) {
          key = i.first;
          return key;
        }
      }
    }
    int print_HashTable(){
      cout<< " HashTable: ";
      for (auto &i : m) {
        cout << "(" << i.first << ", " << i.second << "), ";
      }
      cout << "\n";
    }

    int get_random(page*& pageFrame){
      //srand (time(NULL));
      // randomly retun one pageframe then calculate the ppageId to return
      int rand_num =  (rand() % (pageFrame_size) ); 
      //cout << rand_num << endl;
      for (auto &i : m) {
        //cout << "(" << i.first << ", " << i.second << "), ";
        if (i.second == rand_num){
          return i.first;
        }
      }
      
    }

    int get_clock(page*& pageFrame){
      while (true){
        // cout<< "first: ";
        // clock_print();
        int pageId = clock.front();
        int pageFramId = get_pageFrameId(pageId);
        clock.pop_front();
        //cout << "********** pageId: "<< pageId << ", pin_counter: "<< pageFrame[pageFramId].pin_counter << "\n";
        if (pageFrame[pageFramId].pin_counter ==0){
          if (pageFrame[pageFramId].ref_bit == true){
            clock.push_front(pageId);
            // cout<< "before return: ";
            // clock_print();
            // print_HashTable();
            // cout << "pageId: "<< pageId << "======================================================";
            return pageId;
          }else {
            pageFrame[pageFramId].ref_bit = true;
            pageFrame[pageFramId].pin_counter =0;
          }
        }
        // cout<< "end: ";
        // clock_print();
        clock.push_back(pageId);
      }
    }

    bool init_empty(){
        for (int i=0; i<pageFrame_size;i++){
            empty[i] = true;
        }
    }

    void clock_print(){
      std::deque<int>::iterator it = clock.begin();

      while (it != clock.end())
        std::cout << ' ' << *it++;

      std::cout << '\n';
    }

    void fifo_print(){
      std::deque<int>::iterator it = fifo.begin();

      while (it != fifo.end())
        std::cout << ' ' << *it++;

      std::cout << '\n';
    }

    void lru_print(){
      std::deque<int>::iterator it = lru.begin();

      while (it != lru.end())
        std::cout << ' ' << *it++;

      std::cout << '\n';
    }

    void mru_print(){
      std::deque<int>::iterator it = mru.begin();

      while (it != mru.end())
        std::cout << ' ' << *it++;

      std::cout << '\n';
    }

    void lru_push_queue(int element){
      std::deque<int>::iterator it = lru.begin();
      while (it != lru.end()){
        if (*it == element){
          //cout << *it << endl;
          lru.erase(it);
          break;
        }
        *it++;
      }
      lru.push_back(element);
    }

    void mru_push_queue(int element){
      std::deque<int>::iterator it = mru.begin();
      while (it != mru.end()){
        if (*it == element){
          mru.erase(it);
          break;
        }
        *it++;
      }
      mru.push_back(element);
    }

    void fifo_push_queue(int element){
      std::deque<int>::iterator it = fifo.begin();
      while (it != fifo.end()){
        if (*it == element){
          return ;
        }
        *it++;
      }
      fifo.push_back(element);
      //cout << "fifo.at(0): "<< fifo.at(0) <<endl;
    }

    void clock_push_queue(int element){
      std::deque<int>::iterator it = clock.begin();
      while (it != clock.end()){
        if (*it == element){
          return ;
        }
        *it++;
      }
      clock.push_back(element);
    }

    bool insert_key_value(int pageID, int pageFrameId ){

        fifo.push_back(pageID); //fifo_push_queue(pageID);
        clock.push_back(pageID);
        empty.erase(pageFrameId);
        m[pageID] = (pageFrameId+1);
        return true;
    }
    bool delete_key_value(int pageID ){
        fifo.pop_front();
        lru.pop_front();
        mru.pop_back();
        clock.pop_front();
        int pageFrameId = m[pageID];
        m.erase(pageID);
        empty[pageFrameId] = true;
        return true;
    }

    int get_pageFrameId(int pageId){
        int pageFrameId;
        pageFrameId = (m[pageId]);
        if (pageFrameId == 0) return -1;// it means th pageId doesn't exist in the pageFrame
        return (pageFrameId-1);
    }

    int get_empty_spot(){
        return empty.begin()->first;

    }

    int get_length(){
        return m.size();
    }

};

void verification_pageTable(pageTable_hashMap* pageTable){
    int key = 0;
    int value = 0;
    (*pageTable).insert_key_value(key, value);

    key = 2;
    value = 20;
    (*pageTable).insert_key_value(key, value);

    key = 3;
    value = 30;
    (*pageTable).insert_key_value(key, value);

    std::cout<< "Length of map list: " << (*pageTable).get_length() << "\n";

    // check if key is present
    if ((*pageTable).m.find(3) != (*pageTable).m.end())
      std::cout << "map contains key 3!\n";
    // retrieve
    key = 0;
    std::cout << "pageTable key:" << key << ", value: "<<(*pageTable).get_pageFrameId(key) << '\n';
    key = 2;
    std::cout << "pageTable key:" << key << ", value: "<<(*pageTable).get_pageFrameId(key) << '\n';
    key = 3;
    std::cout << "pageTable key:" << key << ", value: "<<(*pageTable).get_pageFrameId(key) << '\n';


    if ((*pageTable).delete_key_value(key)){
      std::cout<< "The key "<< key << " is successfully deleted!\n" ;
    }

    std::cout<< "Length of map list: " << (*pageTable).get_length() << "\n";


    key = -1;
    value = (*pageTable).get_pageFrameId(key);
    if (value == -1){
        std::cout<< "The pageId: " << key << " doesn't exist in the pageFrame!" << std::endl;

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    funnctions for test ///////////////////////////////////////////////////////////////////////////
//dummy function for fill the array of char 
void read_disk_page(char* content, int pageId){
  for (int i=0; i< Page_Size;i++){
    content[i] = 65+pageId;
  }
}

void print_page_status(int pageId,pageTable_hashMap* pageTable,page*& pageFrame){
  int pageFrameId =(*pageTable).get_pageFrameId(pageId);
  cout<< "PageId: "<< pageFrame[pageFrameId].pageId<< 
    ", pin_counter: "<< pageFrame[pageFrameId].pin_counter <<
    ", dirty_flag: "<< pageFrame[pageFrameId].dirty_flag <<
    ", ref_bit: "<< pageFrame[pageFrameId].ref_bit << endl;
}

int rp_pageFrameID(pageTable_hashMap* pageTable,bufferPoolManager* bpm,page*& pageFrame){
  int pageId =-1;
  switch ((*bpm).replacementPolicy)
    {
      case RP_FIFO:
        //cout<< "FIFO\n";
        //cout<< "FIFO policy select pageId: " << pageTable->fifo.front() << endl;
        return pageTable->fifo.front();
      case RP_RANDOM:
        //cout<< "RANDOM\n";
        pageId = pageTable->get_random(pageFrame);
        //cout<< "RANDOM policy select pageId: " << pageId << endl;
        return pageId;
      case RP_LRU:
        //cout<< "LRU\n";
        //cout<< "LRU policy select pageId: " <<pageTable->lru.front() << endl;
        return pageTable->lru.front();
      case RP_MRU:
        //cout<< "MRU\n";
        //cout<< "MRU policy select pageId: " <<pageTable->mru.back() << endl;
        return pageTable->mru.back();
      case RP_CLOCK:
        //cout<< "CLOCK\n";
        pageId = pageTable->get_clock(pageFrame);
        //cout<< "CLOCK policy select pageId: " << pageId << endl;
        return pageId;

    }

}

void fetch_page(int pageId,pageTable_hashMap* pageTable,bufferPoolManager* bpm,page*& pageFrame){
  //checking is there any free pagee in pageframe
  if ((*pageTable).get_length() <= pageFrame_size){
    //there is empty spot in pageFrame
    //cout << "number of page in pageFrame: "<< (*pageTable).get_length() << endl;
    // finding empty spot
    int pageFrame_pageId = (*pageTable).get_empty_spot();
    
    // read from disk
    char temp[Page_Size];
    read_disk(temp, pageId);
    // cout<< "the test of read disk\n";
    // print_page(temp);

    // move it to pageframe
    strncpy(pageFrame[pageFrame_pageId].content,temp,Page_Size);
    (*pageTable).insert_key_value(pageId, pageFrame_pageId);
    pageFrame[pageFrame_pageId].dirty_flag = false;
    pageFrame[pageFrame_pageId].pin_counter = 0;
    pageFrame[pageFrame_pageId].ref_bit = false;
    pageFrame[pageFrame_pageId].pageId = pageId;
    

  }else {
    // there is no empty slot
    //cout << "There is no empty spot in the pageFrame!\n";
    // select the replacement policy 
    int key_pageId = rp_pageFrameID(pageTable,bpm,pageFrame);

    // delete from pageTable
    int pageFrameID = (*pageTable).get_pageFrameId(key_pageId);

    (*pageTable).delete_key_value(key_pageId);

    // move to new memory
    // read from disk
    char temp[Page_Size];
    read_disk(temp, pageId);
    // cout<< "the test of read disk\n";
    // print_page(temp);

    //check if the page is dirty need to write_back to disk
    if (pageFrame[pageFrameID].dirty_flag== true){
      //cout<<"writing on the disk ----------------------------------------------\n";
      write_disk(key_pageId,pageFrame[pageFrameID].content);
    }



    // move it to pageframe
    strncpy(pageFrame[pageFrameID].content,temp,Page_Size);
    //add to pageTable
    (*pageTable).insert_key_value(pageId,pageFrameID);

    pageFrame[pageFrameID].dirty_flag = false;
    pageFrame[pageFrameID].pin_counter = 0;
    pageFrame[pageFrameID].ref_bit = false;
    pageFrame[pageFrameID].pageId = pageId;

  }

}

void read(char* content,int pageId,pageTable_hashMap* pageTable,bufferPoolManager* bpm,page*& pageFrame ){
  //cout<< "start reading...\n";
  
  // Step0: need to check, does page exist in the pageFrame or no?
  int pageFrameId = -1;
  pageFrameId =(*pageTable).get_pageFrameId(pageId);
  //cout<< pageFrameId<< endl;
  if (pageFrameId == -1) {
    // It means that page doesn't exist in the memory
    // so we need to fetch page to memory
    //cout<< "pageId: "<< pageId<< " doesn't exist in the pageFrame!\n";
    (*bpm).fail_hit++;

    fetch_page(pageId,pageTable,bpm,pageFrame);
    pageFrameId =(*pageTable).get_pageFrameId(pageId);

    
  } else {
    (*bpm).hit++;
    //cout<< "pageId: "<< pageId<< " exist in the pageFrame!\n";

  }
  // I consider it will successfully load to pageframe inside if condition
  (*pageTable).lru_push_queue(pageId);
  (*pageTable).mru_push_queue(pageId);
  pageFrame[pageFrameId].pin_counter++;
  pageFrame[pageFrameId].ref_bit = false;

  strncpy(content,pageFrame[pageFrameId].content,Page_Size);

  pageFrame[pageFrameId].pin_counter--;
  //print_page_status(pageId,pageTable,pageFrame);

}



void write(char* content,int pageId,pageTable_hashMap* pageTable,bufferPoolManager* bpm,page*& pageFrame ){
  //cout<< "start writing...\n";
  
  // Step0: need to check, does page exist in the pageFrame or no?
  int pageFrameId = -1;
  pageFrameId =(*pageTable).get_pageFrameId(pageId);
  //cout<< pageFrameId<< endl;
  if (pageFrameId == -1) {
    // It means that page doesn't exist in the memory
    // so we need to fetch page to memory
    //cout<< "pageId: "<< pageId<< " doesn't exist in the pageFrame!\n";
    (*bpm).fail_hit++;

    fetch_page(pageId,pageTable,bpm,pageFrame);
    pageFrameId =(*pageTable).get_pageFrameId(pageId);

    
  } else {
    (*bpm).hit++;
    //cout<< "pageId: "<< pageId<< " exist in the pageFrame!\n";

  }
  // I consider it will successfully load to pageframe inside if condition
  (*pageTable).lru_push_queue(pageId);
  (*pageTable).mru_push_queue(pageId);
  pageFrame[pageFrameId].pin_counter++;
  pageFrame[pageFrameId].ref_bit = false;
  pageFrame[pageFrameId].dirty_flag = true;

  strncpy(pageFrame[pageFrameId].content,content,Page_Size);

  pageFrame[pageFrameId].pin_counter--;
  //print_page_status(pageId,pageTable,pageFrame);

}

void scan(char* content,int pageId_start, int pageId_end,pageTable_hashMap* pageTable,bufferPoolManager* bpm,page*& pageFrame ){
  for (int i=pageId_start; i < pageId_end;i++){
    char temp[Page_Size];
    read(temp,i,pageTable,bpm,pageFrame );
  }
}