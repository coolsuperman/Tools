#include"MySQL.hpp"


int main(){
  mySQL test;
  test.InitSQL("localhost","root","199805","HttpSever");
  test.ReCreateTable("root");
  test.CreateTable("root");
  test.Insert("'hehe','.cc','10','2019-7-14','2019-7-14'","root");
  test.Insert("'haha','.cc','10','2019-7-14','2019-7-14'","root");
  test.Insert("'heng','.cc','10','2019-7-14','2019-7-14'","root");
  test.Select("root");
  test.Select("root");
  test.Delete_byName("haha","root");
  test.Select("root");
  test.Update_byName("Mime",".txt","heng","root");
  test.Select("root");
  return 0;
}
