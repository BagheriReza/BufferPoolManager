#include "parameters.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <bits/stdc++.h> 
using namespace std;




int disk_initial()
{
  std::ofstream file;
  file.open(dbfile); 
  for(int i=1;i<=Num_Page;i++)
  {
    char init = 64+(i%26) ;
    // if(i%2==1)
    // {
    //   init = '1';
    // }
    string str;
    str.append(Page_Size,init);
   
    file << str;
  }
  file.seekp(ios::end); 
  file.close();
  return 0;
}

int write_disk(int pageNumber, char * data)
{
  std::fstream fs;  
  fs.open(dbfile);
  // cout << "the current position of pointer is :"
  //       << fs.tellp() << endl;
   fs.seekp((pageNumber) * Page_Size);
  //  cout << "the current position of pointer is :"
  //        << fs.tellp() << endl;

  fs.write(data, Page_Size);

  // cout << "size of data is :"
  //        << sizeof(data) << endl;
  fs.close();
  return 0;
}

void read_disk(char* content, int pageNumber)
{
    fstream fs;
    fs.open(dbfile, ios::in | ios::binary);
    fs.seekg((pageNumber) * Page_Size);
    //char *page = new char [Page_Size];
    fs.read(content ,Page_Size);
    //cout << content << endl;
    fs.seekg(0, ios::end);
    fs.close();
}