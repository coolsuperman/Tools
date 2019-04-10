#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<io.h>

typedef struct _finddata_t Data;

void _GetName (const char*path,const char* search);
typedef struct NeedToUse {//封装隔绝，避免误用；
	char* check  ;//检查通配符
	char* suffix  ;//后缀；
	int count  ;//行数记录
	int count_file;//文件个数
}NeedToUse;

static NeedToUse NTU = { "*" , "\\" , 0 ,0};

static int IsSameSuffix (const char*FileInfo_name , const char*search) {//倒着判断后缀是否相等；
	int len_name = strlen (FileInfo_name);
	int len_search = strlen (search);
	for ( int i = 0; i < len_search; i++ ) {
		if ( FileInfo_name [len_name - i] != search [len_search - i] ) {
			return 0;
		}
	}
	return 1;
}

int CountLine (const char*PATH, const char* file_name) {
	NTU.count_file++;
	int _count = 0;
	int ch = 0;
	char file [260] = { 0 };
	strcat (file,PATH);
	strcat (file , file_name);
	FILE*fp = fopen (file , "r");
	if ( fp == NULL ) {
		printf ("open file faild!\n");
		return 0;
	}
	do {
		ch = fgetc (fp);
		if ( (char)ch == '\n' ) {
			_count++;
		}
	} while ( ch != EOF );
	fclose (fp);
	return _count;
}

void Filecheck (const char* path , Data*FileInfo,const char*search) {
	if ( FileInfo->attrib == _A_SUBDIR ) {//子文件
		if ( (strcmp (FileInfo->name , ".")) && (strcmp (FileInfo->name , "..")) ) {
			char grandchild_path [260] = { 0 };
			strcat (strcpy (grandchild_path , path) , FileInfo->name);
			strcat (grandchild_path , NTU.suffix);
			_GetName (grandchild_path,search);
		}
	}
	else {//普通文件
		if ( IsSameSuffix (FileInfo->name,search)) {
			int add = CountLine (path,FileInfo->name);
			printf ("%s:%d lines---PATH:%s\n" , FileInfo->name , add,path);
			NTU.count += add;
		}
	}
}

void _GetName (const char*path,const char* search) {
	long handle;
	Data  FileInfo;
	char new_path [260] = { 0 };
	handle = _findfirst (strcat (strcpy (new_path , path) , NTU.check) , &FileInfo);
	if ( handle == -1 ) {
		printf ("No any files exist in File:%s\n" , path);
		return;
	}
	else if ( FileInfo.attrib == _A_SUBDIR ) {//子文件夹
		if ( (!strcmp (FileInfo.name , ".")) && (!strcmp (FileInfo.name , "..")) ) {
			char child_path [260] = { 0 };
			strcat (strcpy (child_path , path) , FileInfo.name);
			strcat (child_path , NTU.suffix);
			_GetName (child_path,search);
		}
	}
	else {//普通文件
		if ( IsSameSuffix (FileInfo.name , search) ) {//找与目标后缀相同的文件；
			NTU.count += CountLine (path,FileInfo.name);
			printf ("%s:%d lines---PATH:%s\n" , FileInfo.name , NTU.count,path);
		}
	}
	while ( !_findnext (handle , &FileInfo) ) {
		Filecheck (path , &FileInfo,search);
	}
	_findclose (handle);
}

int main () {
	char search [260] = {0};
	char root_path [260] = { 0 };
	printf ("请输入你要查找的路径(C:\\\\example\\\\example\\\\):\n");
	scanf ("%s" , &root_path);
	printf ("请输入你要统计的文件后缀(.c):\n");
	scanf ("%s" , &search);
	_GetName (root_path,search);
	printf ("你的共%d个%s文件里%d行代码，还不错，但继续加油！！！\n",NTU.count_file,search,NTU.count);
	system ("pause");
	return 0;
}
