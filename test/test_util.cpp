#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../util/string_util.h"

//using namespace std;

using std::cout;
using std::endl;


int main(int argc, char *argv[])
{

  std::cout<<"========测试string split================"<<std::endl;
  std::vector<std::string> x = split("-1.0 f0:-1.8067193 f1:0.69105651 f2:-0.28146544 f3:0.33964814 f4:-0.36284297 f5:0.1967105 f6:-1.2954179 f7:0.17239107 f8:0.11193174 f9:-0.14544685 f10:-0.54972432 f11:1.3550106 f12:-0.033612579 f13:-0.18285816 f14:-0.1528969 f15:-0.35103533 f16:0.19409245 f17:-0.44514512 f18:-0.3461394 f19:-0.12465687", ' ');

  for(int i = 0;i<x.size();i++){
    //std::cout<<i<<"  "<<x[i]<<std::endl;
  }

  std::cout<<"=========测试读文件=================="<<std::endl;
  std::string fname = "../data/ida/train.csv";
  std::ifstream file(fname.c_str());
  int indx = 0;
  while(!file.eof()){
    std::string fuck;
    bool suck = getline(file,fuck);
    //std::cout<<indx<<"  "<<fuck<<std::endl;
    
    indx ++ ;
  }
  
  file.close();

  cout<<"==============测试file control============="<<endl;

  std::string a = "121313";
  long i = stol(a);
  cout<<i<<endl;
  return 0;
}
