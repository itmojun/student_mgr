#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stu_mgr.h"
#include "user_mgr.h"


void input_stu(void);
void delete_stu(void);
void update_stu(void);


int main()
{
	int op;

	stu_list = create();
	load_data();

	printf("欢迎使用学生信息管理系统！\n");

	user_login();

	time_t t;
	time(&t);
	struct tm* now_time = localtime(&t);
	const char* weekday[] = {"日", "一", "二", "三", "四", "五", "六"};
	printf("\n系统时间：%d年%d月%d日 %d:%02d:%02d 星期%s\n", now_time->tm_year + 1900, now_time->tm_mon + 1, now_time->tm_mday, now_time->tm_hour, now_time->tm_min, now_time->tm_sec, weekday[now_time->tm_wday]);

	printf("\n操作说明：\n");
	printf("录入学生信息请输入1\n");
	printf("删除学生信息请输入2\n");
	printf("修改学生信息请输入3\n");
	printf("查询学生信息请输入4\n");
	printf("添加普通用户请输入5\n");
	printf("删除普通用户请输入6\n");
	printf("退出系统请输入0\n");

	while(1)
	{
		printf("\n请输入操作代号：");
		
		if(1 != scanf("%d", &op))
		{
			// 如果输入失败，就清空标准输入缓冲区
			while(getchar() != '\n');

			op = -1;
		}

		if(user.user_role == 2 && (op == 2 || op == 3 || op == 5 || op == 6))
		{
			printf("\n很抱歉，您没有权限执行此功能！\n");
			continue;
		}

		switch(op)
		{
			case 0:
			{
				// 退出本系统
				printf("\n感谢使用，下次再见！\n");
				//destroy(stu_list);
				exit(0);
			}
			break;

			case 1:
			{
				// 录入学生信息
				

				input_stu();
				save_data();
			}
			break;

			case 2:
			{
				// 删除学生信息
				delete_stu();
				save_data();
			}
			break;

			case 3:
			{
				// 修改学生信息
				update_stu();		
				save_data();	
			}
			break;

			case 4:
			{
				// 查询学生信息
				printf("\n所有学生信息如下：\n");
				show(stu_list);
				printf("\n共有 %d 名学生。\n", size(stu_list));
			}
			break;

			case 5:
			{
				// 添加普通用户
				user_add();
			}
			break;

			case 6:
			{
				// 删除普通用户
				user_del();
			}
			break;

			default:
			{
				// 输入错误
				printf("\n输入错误，请重新输入！\n");
			}
		}
	}

	return 0;
}


void input_stu(void)
{
	student s;

	printf("\n请按照如下提示录入学生信息。\n");
	printf("学号：");
	scanf("%d", &s.sno);
	printf("姓名：");
	scanf("%s", s.name);
	getchar();
	printf("性别(0-女,1-男)：");
	scanf("%c", &s.sex);
	printf("视力：");
	scanf("%f", &s.sight);

	push_back(stu_list, s);

	printf("\n录入学生信息成功！\n");
}


void delete_stu(void)
{
	int sno;

	printf("\n请输入要删除学生的学号：");
	scanf("%d", &sno);

	if(remove_junge(stu_list, sno))
	{
		printf("\n删除学生信息成功！\n");
	}
	else
	{
		printf("\n学号不存在，删除失败！\n");
	}	
}


void update_stu(void)
{
	int sno;
	student* p = NULL;

	printf("\n请输入要修改学生的学号：");
	scanf("%d", &sno);

	p = find(stu_list, sno);

	if(p == NULL)
	{
		printf("\n学号不存在，修改失败！\n");
		return;
	}

	printf("\n请按照如下提示录入学生的更新信息。\n");
	printf("学号(New)：");
	scanf("%d", &(p->sno));
	printf("姓名(New)：");
	scanf("%s", p->name);
	getchar();
	printf("性别(0-女,1-男)(New)：");
	scanf("%c", &(p->sex));
	printf("视力(New)：");
	scanf("%f", &(p->sight));

	printf("\n修改学生信息成功！\n");
}


