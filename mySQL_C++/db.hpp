#include<cstdio>
#include<cstdlib>
#include<mysql/mysql.h>
#include<jsoncpp/json/json.h>

//jsoncpp 一个核心类，两个重要方法
//json::Value 表示一个JSON对象，用法和std::map非常相似
//writer::write:把json对象序列化成字符串(std::string)
//Reader::parse：反序列化

namespace order_system{
  //创建MySQL句柄并建立连接
  static MYSQL* MySQLInit(){
    MYSQL*mysql = mysql_init(NULL);
    //2.建立句柄和数据库服务器之间的联系(连接服务器)
    if(mysql_real_connect(mysql,"127.0.0.1","root","199805","dish_system",3306,NULL,0)==NULL){
      printf("Connect failed! %s\n",mysql_error(mysql));
      return NULL;
    }
    //3.设置编码格式(让mysql服务器编码方式和客户端的方式匹配)//utf-8
    mysql_set_character_set(mysql,"utf8");
    return mysql;
  }
  static void MYSQLRelease(MYSQL* mysql){
    mysql_close(mysql);
  }
  class DishTable{//菜
    public:
      DishTable(MYSQL* mysql)
        :_mysql(mysql)
      { }
      //使用JSON作为函数参数
      //创建dish类也可以
      bool Insert(const Json::Value& dish){
        char sql[1024*4] = {0};
        //拼装SQL
        sprintf(sql,"Insert into dish_table values(null,'%s','%d')",
            dish["name"].asCString(),
            dish["price"].asInt());
        //执行SQL
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("DishTable Insert Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("DishTable Insert OK!\n");
        return true;
      }
      //使用const & 表示输入型参数
      //使用*表示输出型参数
      //& 表示输入输出型参数
      bool SelectAll(Json::Value* dishes){
        char sql[1024*4]={0};
        sprintf(sql,"select dish_id, name, price from dish_table");
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("DishTable SelectAll Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("DishTable SelectAll ok!\n");
        //构造结果
        MYSQL_RES* result=mysql_store_result(_mysql);
        int rows = mysql_num_rows(result);
        for(int i = 0;i<rows;++i){
          MYSQL_ROW l_row = mysql_fetch_row(result);
          Json::Value dish;
          //row[]取出的结果都是C风格字符串；
          dish["dish_id"] = atoi(l_row[0]);
          dish["name"] = l_row[1];
          dish["price"] = atoi(l_row[2]);
          dishes->append(dish);//添加到dishes里；
        }
        mysql_free_result(result);
        return true;
      }
      bool SelectOne(int32_t dish_id ,Json::Value* dish){//打印单个
        char sql[1024*4] = {0};
        sprintf(sql,"select dish_id, name, price from dish_table where dish_id = %d",dish_id);
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("DishTable SelectOne Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        //遍历结果集合;只有一条
        MYSQL_RES* result = mysql_store_result(_mysql);
        int rows = mysql_num_rows(result);
        if(rows!=1){
          printf("DishTable SelectOne result rows not 1!");
          mysql_free_result(result);
          return false;
        }
        MYSQL_ROW l_row = mysql_fetch_row(result);
        (*dish)["dish_id"]=dish_id;
        (*dish)["name"] = l_row[1];
        (*dish) ["price"] = atoi(l_row[2]);
        mysql_free_result(result);
        return true;
      }

      bool Update(const Json::Value& dish){
        char sql[1024*4]={0};
        sprintf(sql,"update dish_table set name = '%s',price = %d where dish_id = %d",
            dish["name"].asCString(),
            dish["price"].asInt(),
            dish["dish_id"].asInt());
        printf("sql: %s\n",sql);
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("DishTable Update Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("DishTable Update OK!\n");
        return true;
      }
      bool Delete(int32_t dish_id){
        char sql[1024*4]={0};
        sprintf(sql,"delete from dish_table where dish_id = %d",dish_id); 
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("DishTable Delete Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("DishTable Delete OK!\n");
        return true;
      }
    private:
      MYSQL* _mysql;
  };
  class OrderTable{//订单
    public:
      OrderTable(MYSQL* mysql)
        :_mysql(mysql)
      { }
      //使用JSON作为函数参数
      //创建dish类也可以
      bool Insert(const Json::Value& order){
        char sql[1024*4] = {0};
        //拼装SQL
        sprintf(sql,"Insert into order_table values(null,'%s','%s','%s','%d')",
            order["table_id"].asCString(),
            order["time"].asCString(),
            order["dishes"].asCString(),
            order["status"].asInt());
        //执行SQL
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("Order Insert Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("Order Insert OK!\n");
        return true;
      }
      //使用const & 表示输入型参数
      //使用*表示输出型参数
      //& 表示输入输出型参数
      bool SelectAll( Json::Value* orders){
        char sql[1024*4]={0};
        sprintf(sql,"select order_id, table_id, time, dishes, status from order_table");
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("OrderTable SelectAll Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("orderTable SelectAll ok!\n");
        //构造结果
        MYSQL_RES* result=mysql_store_result(_mysql);
        int rows = mysql_num_rows(result);
        for(int i = 0;i<rows;++i){
          MYSQL_ROW l_row = mysql_fetch_row(result);
          Json::Value order;
          //row[]取出的结果都是C风格字符串；
          order["order_id"] = l_row[0];
          order["table_id"] = l_row[1];
          order["time"] = l_row[2];
          order["dishes"] = l_row[3];
          order["status"] = l_row[4];
          orders->append(order);//添加到dishes里；
        }
        mysql_free_result(result);
        return true;
      }

      bool ChangeState(const Json::Value& order){
        char sql[1024*4]={0};
        sprintf(sql,"update order_table set status = %d where order_id = %d",
            order["status"].asInt(),
            order["order_id"].asInt());
        printf("sql: %s\n",sql);
        int ret = mysql_query(_mysql,sql);
        if(ret!=0){
          printf("OrderTable ChangeState Failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("OrderTable ChangeState OK!\n");
        return true;
      }
    private:
      MYSQL* _mysql;
  };
}//end order_system
