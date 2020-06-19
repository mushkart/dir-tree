#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "dir_f.h"

// ������� ����� �����:   dir-tree.exe   1.txt   *path*   *����� �������*   *������� ������, ���� ���������*
int main(int  argc, char* argv[]){

	// ��������� ���� ��� ������
	FILE * file;
	file = fopen(argv[1], "w");

	// ��� ����� ���������� data
	char path[2048];
	strcpy(path, argv[2]);

	// ������� ������
	node* root = 0;
	root = Create_List(path,0);

	// ��������� ������
	strcat(path, "\\*");
	root = Scan(root,path);

	// ��������� ������ �������
	int task = atoi(argv[3]);
	if (task == 1) {
		Check_Task_1(root, argv[4], file);
	} else if (task == 2){
		Check_Task_2(root, file);
	} else if  (task == 3){
		Check_Task_3(root, atoi(argv[4]), file);
	}

	// ��������� ���� � ������ ������
	fclose(file);
	root = Free_Node(root);
	return 0;
}