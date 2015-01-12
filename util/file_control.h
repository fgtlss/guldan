#ifndef FILE_CONTROL_H_
#define FILE_CONTROL_H_

#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::endl;

class File_Control{
 private:
  std::string fname;//文件的名字
  std::string current;//当前行的内容
  std::ifstream file;

 public:
 File_Control(std::string fn) :
  fname(fn),current(""),file(fn.c_str()) {
    if(!file)
      throw ;
  }
  
  void print_info(){
    cout<<"文件名"<<fname<<endl;
  }

  void print_file(){
    int indx = 0;
    while(!file.eof()){
      std::string temp;
      getline(file,temp);
      cout<<indx<<" "<<temp<<endl;
      indx ++ ;
    }
  }

  bool has_next(){
    return !file.eof();
  }

  std::string next(){
    getline(file,current);
    return current;
  }

  std::string current_line(){
    return current;
  }
  
  //返回开头
  void restart(){
    file.clear();
    file.seekg(0);
  }

  //析构
  ~File_Control(){
    file.close();
  }
  
};


#endif
