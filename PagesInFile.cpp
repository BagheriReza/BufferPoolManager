#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <bits/stdc++.h> 
using namespace std;

int PAGE_SIZE = 16;
int N_O_PAGES = 100;

int initial(string filepath, int numberOfPages,int pageSize)
{
  std::ofstream file;
  file.open(filepath); 
  for(int i=0;i<=numberOfPages;i++)
  {
    char init = '0';
    if(i%2==1)
    {
      init = '1';
    }
    string str;
    str.append(pageSize,init);
   
    file << str;
  }
  file.seekp(ios::end); 
  file.close();
  return 0;
}

int myWrite(string filepath, int pageNumber,int pageSize, char * data)
{
  std::fstream fs;  
  fs.open(filepath);
  // cout << "the current position of pointer is :"
  //       << fs.tellp() << endl;
   fs.seekp((pageNumber - 1) * pageSize);
  //  cout << "the current position of pointer is :"
  //        << fs.tellp() << endl;

  fs.write(data, pageSize);

  // cout << "size of data is :"
  //        << sizeof(data) << endl;
  fs.close();
  return 0;
}

void myRead(string filepath, int pageNumber, int pageSize)
{
    fstream fs;
    fs.open(filepath, ios::in | ios::binary);
    fs.seekg((pageNumber-1) * pageSize);
    char *page = new char [pageSize];
    fs.read(page ,pageSize);
    cout << page << endl;
    fs.seekg(0, ios::end);
    fs.close();
}

int main() {

    string filepath ="bim3";

    initial(filepath,N_O_PAGES,PAGE_SIZE);

    string str ="abcdefghijklmnso" ;
char arr[str.length() + 1]; 

	strcpy(arr, str.c_str()); 
   // char test[16] = ;

    myWrite(filepath,20,PAGE_SIZE, arr);

    myRead(filepath,20,PAGE_SIZE);
    return 0;
} 

