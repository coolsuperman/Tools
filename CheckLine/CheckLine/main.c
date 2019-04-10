#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<io.h>

void _GetName (char*path);

char* root_path = "D:\\Code\\";
char* search = ".h";
char* check = "*";
char* suffix = "\\";//后缀；
int count = 0;
typedef struct _finddata_t Data;

int CountLine (char*PATH,const char* file_name) {
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

void filecheck (char* path , Data*FileInfo) {
	if ( FileInfo->attrib == _A_SUBDIR ) {//子文件
		char grandchild_path [260] = { 0 };
		if ( (strcmp (FileInfo->name , ".")) && (strcmp (FileInfo->name , "..")) ) {
			strcat (strcpy (grandchild_path , path) , FileInfo->name);
			strcat (grandchild_path , suffix);
			_GetName (grandchild_path);
		}
	}
	else {//普通文件
		if (IsSame(FileInfo->name,search)) {
			int add = CountLine (path,FileInfo->name);
			printf ("%s:%d lines---PATH:%s\n" , FileInfo->name , add,path);
			count += add;
		}
	}
}

static int IsSame (const char*FileInfo_name , const char*search) {
	int len_name = strlen (FileInfo_name);
	int len_search = strlen (search);
	for ( int i = 0; i < len_search; i++ ) {
		if ( FileInfo_name [len_name - i] != search [len_search - i] ) {
			return 0;
		}
	}
	return 1;
}

void _GetName (char*path) {
	long handle;
	Data  FileInfo;
	char new_path [260] = { 0 };
	handle = _findfirst (strcat (strcpy (new_path , path) , check) , &FileInfo);
	if ( handle == -1 ) {
		printf ("No any files exist in File:%s\n" , path);
		return;
	}
	else if ( FileInfo.attrib == _A_SUBDIR ) {//子文件
		if ( (!strcmp (FileInfo.name , ".")) && (!strcmp (FileInfo.name , "..")) ) {
			char child_path [260] = { 0 };
			strcat (strcpy (child_path , path) , FileInfo.name);
			strcat (child_path , suffix);
			_GetName (child_path);
		}
	}
	else {//普通文件
		if ( IsSame (FileInfo.name , search) ) {
			count += CountLine (path,FileInfo.name);
			printf ("%s:%d lines---PATH:%s\n" , FileInfo.name , count,path);
		}
	}
	while ( !_findnext (handle , &FileInfo) ) {
		filecheck (path , &FileInfo);
	}
	_findclose (handle);
}


int main () {
	_GetName (root_path);
	printf ("总计书写了%d行代码，你已经超越了全国%%99.99的Coder！！！\n",count);
	system ("pause");
	return 0;
}
