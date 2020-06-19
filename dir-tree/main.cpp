#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "dir_f.h"

// Порядок ввода такой:   dir-tree.exe   1.txt   *path*   *номер задания*   *входные данные, если требуются*
int main(int  argc, char* argv[]){

	// открываем файл для записи
	FILE * file;
	file = fopen(argv[1], "w");

	// для корня определяем data
	char path[2048];
	strcpy(path, argv[2]);

	// создаем корень
	node* root = 0;
	root = Create_List(path,0);

	// сканируем дерево
	strcat(path, "\\*");
	root = Scan(root,path);

	// Выполняем данное задание
	int task = atoi(argv[3]);
	if (task == 1) {
		Check_Task_1(root, argv[4], file);
	} else if (task == 2){
		Check_Task_2(root, file);
	} else if  (task == 3){
		Check_Task_3(root, atoi(argv[4]), file);
	}

	// закрываем файл и чистим память
	fclose(file);
	root = Free_Node(root);
	return 0;
}