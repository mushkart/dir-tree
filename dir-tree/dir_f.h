
typedef struct NODE {
	char* data;
	int size;
	struct NODE* child;
	struct NODE* brother;
}node;



node* Create_List(char* str, int size);
node* Add_Brother(node* old_brother, char* str, int size);
node* Add_Child(node* parent, char* str, int* size);

node* Scan(node * root, char* path);
node* Free_Node(node* root);

void Check_Task_1(node* root, char* searched, FILE* file);
void Check_Task_2(node* root, FILE* file);
void Check_Task_3(node* root, int size, FILE* file);
