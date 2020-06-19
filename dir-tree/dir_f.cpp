#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>

#include "dir_f.h"


//////////////////////////////////////////////////////////////////////////////


// �������� ������ ����� � ���������� ��� ����� ������
node* Create_List(char* str,int size) {
	// ������� ��������� �� ����� ����
	node* list = (node*)malloc(sizeof(node));

	// � ���� �������� ���������� ��� ����� ��� ����� ��� ��� data
	list->data = (char*)malloc(strlen(str) + 1);
	strcpy(list->data, str);

	list->size = size;

	// ���������� �� ����� � ������� ��� ����
	list->brother = 0;
	list->child = 0;

	return list;
}

//���������� �����
node* Add_Brother(node* old_brother, char* str, int size) {
	node* temp_old_brother = old_brother;

	// ������� �������� ����� ������� ���� ����� � �������� �� ����� �������� ����
	while (temp_old_brother && temp_old_brother->brother) {
		temp_old_brother = temp_old_brother->brother;
	}

	// ������� ����
	node* list = Create_List(str,size);

	// ���� �������� ����� �� ���������� �� �� ���������� ����
	if (temp_old_brother) {
		temp_old_brother->brother = list;
	}
	else {
		// ���� ���� ���� ���� ���� �� ���� ���������� ����� �������
		temp_old_brother = list;
	}

	return old_brother;
}

//���������� �������
node* Add_Child(node* parent, char* str, int size) {

	// ���� � �������� ��� ���� ���� �� ��������� ���� ��� �� �������� �����
	if (parent->child) {
		parent->child = Add_Brother(parent->child, str,size);
	}
	else {
		// ���� ��� - ������� ����� ���� � ��������� ��� ��� ������� �������
		parent->child = Create_List(str,size);
	}

	return parent;
}


//////////////////////////////////////////////////////////////////////////////


// ��� ������� ������������
node* Scan(node* root, char* path) {

	struct _finddata_t uzel;
	intptr_t  hFile = _findfirst(path, &uzel);
	if (!hFile) {
		return root;
	}

	_findnext(hFile, &uzel);

	// ���� �� ��������� ������ ��� ������
	while (_findnext(hFile, &uzel) == 0) {
		// ���� ���� ������� ���������
		if (uzel.attrib & _A_SUBDIR) {
			//��������� ��� � ������ � ������ ������ �����1 -1
			root = Add_Child(root, uzel.name, -1);

			// ������ ����
			char new_path[2048];
			strcpy(new_path, path);
			strcpy(new_path + strlen(new_path) - 1, uzel.name);
			strcat(new_path, "\\*");

			//���������� ���������� ��� ����� ������ � ��������� ��� 
			node* next_root = root->child;
			while (next_root->brother) {
				next_root = next_root->brother;
			}
			next_root = Scan(next_root, new_path);
			// ���� ���� �������� ������
		}
		else {
			// ��������� � ��� �������� ���� ���� �������

			root = Add_Child(root, uzel.name, uzel.size);
		}
	}
	return root;
}

// ���� ������� ������
node* Free_Node(node* root) {

	//������� ��������� ���� ����� ����� 
	if (root->brother) {
		root->brother = Free_Node(root->brother);
	}
	// � �� �������
	if (root->child) {
		root->child = Free_Node(root->child);
	}
	// �������� �������
	free(root->data);
	free(root);

	return root;
}


//////////////////////////////////////////////////////////////////////////////


// ������� 1
void Check_Task_1(node* root, char* searched, FILE * file) {


	if (root->child) {

		//���������� ���� ����� ����������� � ������� ��� ���� ��� �������� ��� ��� ��� �����
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

// ������� 2
void Check_Task_2(node* root, FILE* file) {

	if (root->child) {

		int files_num = 0;
		if (root->child->size != -1) {
			files_num++;
		}
		
		//���������� ���� ����� ����������� � ������� ��� ���� ��� �������� ��� ��� ��� �����
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

// ������� 3
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



