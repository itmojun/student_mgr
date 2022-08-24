#include <stdio.h>
#include <stdlib.h>
#include "stu_mgr.h"


node* stu_list = NULL;


// 创建一个空链表
node* create(void)
{
	node* n = (node*)malloc(sizeof(node));
	n->next = NULL;

	return n;
}


// 销毁某个链表
void destroy(node* l)
{
	node* p = NULL;

	while(l != NULL)
	{
		p = l->next;
		free(l);
		l = p;
	}
}


void save_data(void)
{
	FILE* fp = NULL;

	fp = fopen("stu_data", "wb");
	
	if(fp == NULL)
	{
		perror("fopen fail");
	}

	// 遍历链表所有节点
	node* l = stu_list->next;
	while(l != NULL)
	{
		fwrite(&(l->data), sizeof(student), 1, fp);
		l = l->next;
	}

	fclose(fp);
}


void load_data(void)
{
	FILE* fp = NULL;
	student s;

	fp = fopen("stu_data", "rb");

	if(fp == NULL) return;

	while(fread(&s, sizeof(s), 1, fp))
	{
		push_back(stu_list, s);
	}

	fclose(fp);
}


// 在链表尾部插入节点
void push_back(node* l, elem_type data)
{
	// 创建一个新节点
	node* n = (node*)malloc(sizeof(node));
	n->data = data;
	n->next = NULL;

	// 找到链表的尾节点
	while(l->next != NULL)
		l = l->next;

	// 在尾部插入新节点
	l->next = n;
}


/*
// 在链表头部插入节点
void push_front(node* l, elem_type data)
{
	// 创建一个新节点
	node* n = (node*)malloc(sizeof(node));
	n->data = data;
	n->next = l->next;

	// 将新节点插入头节点之后
	l->next = n;
}


// 在链表中任意位置插入节点
void insert(node* l, unsigned int pos, elem_type data)
{
	// 创建一个新节点
	node* n = (node*)malloc(sizeof(node));
	n->data = data;

	// 找到要插入新节点的位置
	while(pos-- && l->next != NULL) l = l->next;

	// 在上面找到的节点之后插入新节点
	n->next = l->next;
	l->next = n;
}
*/

// 删除某个节点
int remove_junge(node* l, int sno)
{
	node* p = NULL;

	// 找到待删除节点的前驱节点
	// 如果待删除的节点存在，下面循环结束后 l 就是待删除节点的前驱节点的指针
	while(l->next != NULL && l->next->data.sno != sno)
		l = l->next;

	// 目标节点不存在，删除失败返回 0
	if(l->next == NULL) return 0;

	// 将待删除节点的前驱和后继连接起来，否则链表会断为两截
	// 释放待删除的节点
	p = l->next;
	l->next = p->next;
	free(p);

	// 删除成功就返回 1
	return 1;
}


// 修改（更新）链表节点数据
int update(node* l, int sno, elem_type new_val)
{
	// 遍历链表找到目标节点
	l = l->next;
	while(l != NULL && l->data.sno != sno)
		l = l->next;

	// 如果找不到要修改的数据，就返回 0，表示修改失败
	if(l == NULL) return 0;

	// 如果找到了要修改的数据，就修改它，然后返回 1，表示修改成功
	l->data = new_val;
	return 1;
}


// 清空，即删除所有存放有数据元素的节点，就是将所有数据元素删除
void clear(node* l)
{
	node* p = l->next, *q = NULL;

	while(p != NULL)
	{
		q = p->next;
		free(p);
		p = q;
	}

	l->next = NULL;
}


// 求长度
int size(node* l)
{
	int cnt = 0;
	
	l = l->next;

	while(l != NULL)
	{
		cnt++;
		l = l->next;
	}

	return cnt;
}


// 判空
int empty(node* l)
{
	return !(l->next);
}


// 打印链表中所有节点的数据
void show(node* l)
{
	// 遍历链表所有节点
	l = l->next;
	while(l != NULL)
	{
		printf("%d %s %s %g\n", l->data.sno, l->data.name, l->data.sex ? "男" : "女", l->data.sight);
		l = l->next;
	}

	printf("\n");
}


student* find(node* l, int sno)
{
	// 遍历链表所有节点
	l = l->next;
	while(l != NULL)
	{
		if(l->data.sno == sno) return &(l->data);
		l = l->next;
	}

	return NULL;	
}


// 逆序
void reverse(node* l)
{
	node* p = NULL, *q = NULL;

	// 如果链表为空（没有任何数据元素），直接返回
	if(l->next == NULL) return;

	p = l->next->next;
	l->next->next = NULL;  // 最初的第一个节点逆序后将变为尾节点

	// 从第二个节点开始，依次插入到最前面（头节点之后）
	while(p != NULL)
	{
		q = p->next;
		p->next = l->next;
		l->next = p;
		p = q;
	}
}


// 排序
void sort(node* l, int order)
{
	node* p = NULL, *q = NULL, *k = NULL;
	elem_type tmp;

	// 如果链表为空，直接返回
	if(l->next == NULL) return;

	for(p = l->next; p->next != NULL; p = p->next)
	{
		k = p;

		for(q = p->next; q != NULL; q = q->next)
		{
			if(order ? q->data.sight > k->data.sight : q->data.sight < k->data.sight) k = q;
		}

		if(k != p)
		{
			tmp = k->data;
			k->data = p->data;
			p->data = tmp;	
		}
	}
}

