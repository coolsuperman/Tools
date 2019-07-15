#include"MySQL.hpp"


int main(){
  mySQL demo;
  demo.InitSQL("localhost","root","199805","HttpSever");
  demo.ReCreateTable("root");
  demo.CreateTable("root");
  demo.Insert("'hehe','.cc','10','2019-7-14','./Web'","root");
  demo.Insert("'haha','.cc','10','2019-7-14','./Web'","root");
  demo.Insert("'heng','.cc','10','2019-7-14','./Web'","root");
  demo.Select("root");
  for(size_t i =0; i<demo.Data.size();i++){//测试Org_Data函数；
    for(auto e:demo.Data[i]){
      std::cout<<e.first<<"-"<<e.second<<" ";
    }
    std::cout<<std::endl;
  }
  demo.Delete_byName("haha","root");
  demo.Delete_byName("heng","root");
  demo.Delete_byName("hehe","root");
  demo.Select("root");
  demo.Update_byName("Mime",".txt","heng","root");
  demo.Select("root");
  return 0;
}
