#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>

#include "dir_f.h"


//////////////////////////////////////////////////////////////////////////////


// Создание нового листа и заполнение его семьи нулями
node* Create_List(char* str,int size) {
	// Создаем указатель на новый лист
	node* list = (node*)malloc(sizeof(node));

	// В этой струкуре определяем имя папки или файла как его data
	list->data = (char*)malloc(strlen(str) + 1);
	strcpy(list->data, str);

	list->size = size;

	// Определяем ее детей и братьев как нули
	list->brother = 0;
	list->child = 0;

	return list;
}

//Добавление брата
node* Add_Brother(node* old_brother, char* str, int size) {
	node* temp_old_brother = old_brother;

	// Находим младшего среди братьев того брата к которому мы хотим добавить узел
	while (temp_old_brother && temp_old_brother->brother) {
		temp_old_brother = temp_old_brother->brother;
	}

	// Создаем лист
	node* list = Create_List(str,size);

	// Если старшего брата не существует то им становится лист
	if (temp_old_brother) {
		temp_old_brother->brother = list;
	}
	else {
		// Если хоть один брат есть то лист становится самым младшим
		temp_old_brother = list;
	}

	return old_brother;
}

//Добавление ребенка
node* Add_Child(node* parent, char* str, int size) {

	// Если у родителя уже есть дети то определям узел как их младшего брата
	if (parent->child) {
		parent->child = Add_Brother(parent->child, str,size);
	}
	else {
		// Если нет - создаем новый узел и вписываем его как первого ребенка
		parent->child = Create_List(str,size);
	}

	return parent;
}


//////////////////////////////////////////////////////////////////////////////


// Сам процесс сканирования
node* Scan(node* root, char* path) {

	struct _finddata_t uzel;
	intptr_t  hFile = _findfirst(path, &uzel);
	if (!hFile) {
		return root;
	}

	_findnext(hFile, &uzel);

	// пока не останется файлов для поиска
	while (_findnext(hFile, &uzel) == 0) {
		// если узел явяется каталогом
		if (uzel.attrib & _A_SUBDIR) {
			//добавляем его в дерево и размер ставим равны1 -1
			root = Add_Child(root, uzel.name, -1);

			// Меняем путь
			char new_path[2048];
			strcpy(new_path, path);
			strcpy(new_path + strlen(new_path) - 1, uzel.name);
			strcat(new_path, "\\*");

			//Определяем подкаталог как новый корень и сканируем его 
			node* next_root = root->child;
			while (next_root->brother) {
				next_root = next_root->brother;
			}
			next_root = Scan(next_root, new_path);
			// если узел является файлом
		}
		else {
			// добавляем к его родителю этот узел ребенка

			root = Add_Child(root, uzel.name, uzel.size);
		}
	}
	return root;
}

// Тупа очистка памяти
node* Free_Node(node* root) {

	//Удаляем рекурсией всех детей точки 
	if (root->brother) {
		root->brother = Free_Node(root->brother);
	}
	// и ее братьев
	if (root->child) {
		root->child = Free_Node(root->child);
	}
	// Конечная очистка
	free(root->data);
	free(root);

	return root;
}


//////////////////////////////////////////////////////////////////////////////


// Задание 1
void Check_Task_1(node* root, char* searched, FILE * file) {


	if (root->child) {

		//перебираем всех детей подкаталога и выводим его если там найдется тот что нам нужен
		node* temp = root->child;
		while (temp->brother) {
			temp = temp->brother;
			if (!strcmp(temp->data, searched)) {
				fprintf(file, "%s \n", root->data);
			}
		}
		
		Check_Task_1(root->child, searched,file);
	}

	if (root->brother) {

		Check_Task_1(root->brother, searched,file);
	}
	

}

// Задание 2
void Check_Task_2(node* root, FILE* file) {

	if (root->child) {

		int files_num = 0;
		if (root->child->size != -1) {
			files_num++;
		}
		
		//перебираем всех детей подкаталога и выводим его если там найдется тот что нам нужен
		node* temp = root->child;
		
		while (temp->brother) {
			if (temp->brother->size != -1) {
				files_num++;
			}
			temp = temp->brother;
		}

		fprintf(file, "%s:   %d files\n", root->data, files_num);

		Check_Task_2(root->child, file);
	}

	if (root->brother) {
		Check_Task_2(root->brother, file);
	}

}

// Задание 3
void Check_Task_3(node* root, int size, FILE* file) {

	if (root->child) {

		if (root->child->size > size && root->child->size != -1 ) {
			fprintf(file, "%s:   %d bytes\n", root->child->data, root->child->size);
		}

		Check_Task_3(root->child, size, file);
	}

	if (root->brother) {

		if (root->brother->size > size && root->brother->size != -1) {
			fprintf(file, "%s:   %d bytes\n", root->brother->data, root->brother->size);
		}

		Check_Task_3(root->brother, size, file);
	}

}



