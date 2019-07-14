
#include<mysql/mysql.h>
#include<iostream>
#include<string>
#include<vector>

class mySQL{
  public:
    mySQL();
    ~mySQL();
    bool InitSQL(std::string host,std::string user,std::string password,std::string dbname);
    bool exeSQL(std::string query);
    bool CreateTable(std::string table_name);
    bool ReCreateTable(std::string table_name);
    bool Insert(const std::string& str,const std::string& table_name);
    bool Delete_byName(const std::string& str,const std::string& table_name);
    bool Update_byName(const std::string& item,const std::string& upadte,const std::string& name,const std::string& table_name);
    bool Select(const std::string& table_name);
  private:
    MYSQL* msql=NULL;
    MYSQL_RES* result;
    MYSQL_ROW row;
    int aff_rows=0;
    std::vector<std::string> Name;//存放项的名称；
    std::vector<std::vector<std::string>> all;//存放每行的每项数据；
};

mySQL::mySQL(){
  msql=mysql_init(NULL);
  if(msql==NULL){
    std::cerr<<"(Error): InitSQL Failed"<<mysql_error(msql)<<std::endl;
    exit(1);
  }
}

mySQL::~mySQL(){
  mysql_close(msql);
}

bool mySQL::InitSQL(std::string host,std::string user,std::string password,std::string dbname){
  MYSQL* back=mysql_real_connect(msql,host.c_str(),user.c_str(),password.c_str(),dbname.c_str(),0,NULL,0);
  if(back==NULL){
    std::cerr<<"(Error): ConnetSQL Failed"<<mysql_error(msql)<<std::endl;
    exit(-2);
  }
  msql=back;
  return true;
}

bool mySQL::exeSQL(std::string query){//sql操作模块；
  if(mysql_query(msql,query.c_str())){//返回非零表示失败；
    std::cerr<<"(Error)ExeSQL: "<<query<<mysql_error(msql)<<std::endl;
    return false;
  }
  result=mysql_store_result(msql);//返回处理的结果集；
  MYSQL_FIELD* filed;
  if(result){//说明是SELECT，结果集里有数据；
    for(int i=0;filed=mysql_fetch_field(result);i++){
      std::string in=filed->name;//将表中的每一项的项目名称放入Name中；
      Name.push_back(in);
      std::cout<<Name[i]<<"  ";
    }
    std::cout<<std::endl;
    size_t num=mysql_num_fields(result);
    while(row=mysql_fetch_row(result)){//获取结果集中每一行
      std::vector<std::string> in;
      //std::cerr<<"Row:"<<row<<" result:"<<result<<std::endl;
      for(size_t j=0;j<num;j++){
        std::string every=row[j];
        in.push_back(every);
        std::cout<<row[j]<<"   ";
      }
      all.push_back(in);
      std::cout<<std::endl;
    }
  }else{
    if(mysql_field_count(msql)==0){//最近的一条指令的列数
      aff_rows=mysql_affected_rows(msql);//返回update，insert，delete影响的行数；
      std::cout<<aff_rows<<"lines  Has been Change!"<<std::endl;
    }
    else{
      std::cerr<<"(Error): Get Result Failed!"<<std::endl;
      mysql_free_result(result);
      return false;
    }
  }
  mysql_free_result(result);
  return true;
}

bool mySQL::CreateTable(std::string table_name){
  std::string query="create table if not exists `"+table_name;
  query+="`(ID int primary key auto_increment comment '文件序号',";
  query+="Name varchar(20) not null unique key comment '文件名',";
  query+="Mime varchar(20) not null comment '文件类型',";
  query+="Bytes int not null comment '文件大小',";
  query+="add_time datetime not null comment '添加时间',";
  query+="Last_Modify timestamp comment '修改时间'";
  query+=")engine innodb;";
  std::cout<<query<<std::endl;
  return exeSQL(query);
}

bool mySQL::ReCreateTable(std::string table_name){
  std::string query="drop table if exists `"+table_name+"`;";
  std::cout<<query<<std::endl;
  return exeSQL(query);
}

bool mySQL::Insert(const std::string& src,const std::string& table_name){
  std::string query="insert into `"+table_name;
  query+="` (Name,Mime,Bytes,add_time,Last_Modify) values ("+src+");";
  std::cout<<query<<std::endl;
  return exeSQL(query);
}

bool mySQL::Select(const std::string& table_name){
  std::string query="SELECT * from "+table_name+";";
  std::cout<<query<<std::endl;
  return exeSQL(query);
}

bool mySQL::Delete_byName(const std::string& str,const std::string& table_name){
  std::string query="DELETE FROM "+table_name+" WHERE Name = '"+str+"';";
  std::cout<<query<<std::endl;
  return exeSQL(query);
}

bool mySQL::Update_byName(const std::string& item,const std::string& update,const std::string& name,const std::string& table_name){
  std::string query="UPDATE "+table_name+" SET "+item+" = \'"+update+"\' WHERE Name = '"+name+"';";
  std::cout<<query<<std::endl;
  return exeSQL(query);
}
