#ifndef _STU_MGR_H_
#define _STU_MGR_H_


typedef struct
{
	int sno;
	char name[30];
	char sex;
	float sight;
} student;


typedef student elem_type;


// 声明节点类型
typedef struct node_t
{
        elem_type data;       // 数据域，存放一个数据元素
        struct node_t* next;  // 指针域，存放后继节点的指针
} node;


extern node* stu_list;


node* create(void);
void destroy(node* l);
void save_data(void);
void load_data(void);
void push_back(node* l, elem_type data);
int remove_junge(node* l, int sno);
int update(node* l, int sno, elem_type new_val);
void clear(node* l);
int size(node* l);
int empty(node* l);
void show(node* l);
student* find(node* l, int sno);
void sort(node* l, int order);


#endif

