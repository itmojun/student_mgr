#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "md5.h"
#include "user_mgr.h"


user_info user;


void user_login()
{
	char user_name[31], user_pass[31], user_pass_check[31];

	printf("\n请登录！\n");
	printf("用户名：");
	scanf("%s", user_name);
	// 输入密码时没有回显
	strcpy(user_pass, getpass("密码："));

	if(is_first_run())
	{
		if(strcmp(user_name, "admin") != 0 || strcmp(user_pass, "123456") != 0)
		{
			printf("用户名或密码错误，登录失败！\n");
			exit(1);
		}

		// 首次登录成功
		printf("\n为了系统安全，请重新设置管理员用户名和密码。\n");
		printf("管理员用户名：");
		scanf("%s", user_name);
		// 输入密码时没有回显

		while(1)
		{
			strcpy(user_pass, getpass("管理员密码："));
			strcpy(user_pass_check, getpass("再次输入管理员密码："));

			if(strcmp(user_pass, user_pass_check) == 0) break;
			
			printf("\n两次输入的密码不一致，请重新输入！\n");
		}

		strcpy(user.user_name, user_name);
		md5_encode(user_pass, strlen(user_pass), user.user_pass);
		user.user_role = 1;
		user.reg_time = user.last_login_time = time(NULL);

		FILE* fp = fopen("user_data", "wb");

		if(fp == NULL)
		{
			perror("open user_data fail");
		}

		fwrite(&user, sizeof(user), 1, fp);

		fclose(fp);

		printf("\n设置管理员用户名和密码成功！\n");
	}	
	else
	{
		// 非首次登录
		if(!user_login_check(user_name, user_pass))
		{
			printf("用户名或密码错误，登录失败！\n");
			exit(1);
		}

		// 更新用户的最近登录时间
		user.last_login_time = time(NULL);
		update_user_info();
	}

	printf("\n%s，您好，欢迎使用本系统！\n", user.user_name);
}


int is_first_run(void)
{
	FILE* fp = fopen("user_data", "rb");
	
	if(fp == NULL) return 1;
	
	fclose(fp);

	return 0;
}


int user_login_check(const char* user_name, const char* user_pass)
{
	FILE* fp = NULL;
	user_info ui;
	char user_pass_md5[33];
	int check_success = 0;

	fp = fopen("user_data", "rb");

	if(fp == NULL) return 0;

	md5_encode(user_pass, strlen(user_pass), user_pass_md5);

	while(fread(&ui, sizeof(ui), 1, fp) == 1)
	{
		if(strcmp(user_name, ui.user_name) == 0 && strcmp(user_pass_md5, ui.user_pass) == 0)
		{
			// 登录验证成功
			user = ui;
			check_success = 1;
			break;
		}
	}

	fclose(fp);

	return check_success;
}


// 校验用户名是否已存在
// 存在返回 0， 不存在返回 1
int check_user_name(const char* user_name)
{
	FILE* fp = NULL;
	user_info ui;
	int check_success = 1;

	fp = fopen("user_data", "rb");

	if(fp == NULL) return 1;

	while(fread(&ui, sizeof(ui), 1, fp) == 1)
	{
		if(strcmp(user_name, ui.user_name) == 0)
		{
			// 用户名已经存在
			check_success = 0;
			break;
		}
	}

	fclose(fp);

	return check_success;
}


// 添加普通用户
void user_add(void)
{
	user_info ui;
	char user_pass[31], user_pass_check[31];

	printf("\n请按照下面提示输入普通用户的信息。\n");
	
	while(1)
	{
		printf("用户名：");
		scanf("%s", ui.user_name);
		
		if(check_user_name(ui.user_name)) break;

		printf("\n用户名已存在，请重新输入！\n");
	}

	while(1)
	{
		strcpy(user_pass, getpass("密码："));
		strcpy(user_pass_check, getpass("再次输入密码："));

		if(strcmp(user_pass, user_pass_check) == 0) break;

		printf("\n两次输入的密码不一致，请重新输入！\n");
	}

	md5_encode(user_pass, strlen(user_pass), ui.user_pass);
	ui.user_role = 2;
	ui.reg_time = time(NULL);
	ui.last_login_time = 0;

	FILE* fp = fopen("user_data", "ab");

	if(fp == NULL)
	{
		perror("open user_data fail");
	}

	fwrite(&ui, sizeof(ui), 1, fp);

	fclose(fp);

	printf("\n添加普通用户成功！\n");
}


// 删除普通用户
void user_del(void)
{
	char user_name[31];

	printf("\n请输入要删除的普通用户名：");
	scanf("%30s", user_name);

	FILE* fp = NULL;
	user_info ui;
	int del_success = 0;

	fp = fopen("user_data", "r+b");

	if(fp == NULL)
	{
		perror("open user_data fail");
	}
	
	while(fread(&ui, sizeof(ui), 1, fp) == 1)
	{
		if(ui.user_role == 2 && strcmp(user_name, ui.user_name) == 0)
		{
			fseek(fp, -sizeof(ui), SEEK_CUR);
			memset(&ui, 0, sizeof(ui));
			fwrite(&ui, sizeof(ui), 1, fp);
			del_success = 1;
			break;
		}
	}

	fclose(fp);

	if(del_success)
		printf("\n删除普通用户 %s 成功！\n", user_name);
	else
		printf("\n普通用户 %s 不存在，删除失败！\n", user_name);
}


// 更新 user_data 文件中的当前用户信息
void update_user_info(void)
{
	FILE* fp = NULL;
	user_info ui;

	fp = fopen("user_data", "r+b");

	if(fp == NULL)
	{
		perror("open user_data fail");
	}
	
	while(fread(&ui, sizeof(ui), 1, fp) == 1)
	{
		if(strcmp(user.user_name, ui.user_name) == 0)
		{
			fseek(fp, -sizeof(ui), SEEK_CUR);
			fwrite(&user, sizeof(user), 1, fp);
			break;
		}
	}

	fclose(fp);
}
