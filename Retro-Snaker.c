#include<stdio.h>
#include<string.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
 
 
#define up 'w'
#define down 's'
#define left 'a'
#define right 'd'
#define stop 'p'
 
void welcome();               // 60   开始界面
void Finish();                // 313  结束界面
void creatgraph();            // 88   围墙打印
void gotoxy(int x, int y);    // 133  光标跳转，横为X 0,1,2..
void gotoprint(int x, int y); // 143  跳转打印
void gotodelete(int x, int y);// 149  跳转删除
void creatfood();             // 155  食物产生
int ClickControl();           // 179  获取键盘信号
int Judge();                  // 292  游戏结束判断
void MovingBody();            // 195  蛇的移动 
void Eating();                // 246  蛇吃到东西后的操作（伸长）
void ChangeBody(int a,int b); // 267  蛇的坐标变换,后一个复制前一个STRUCT,a,b为head之前坐标 
 
/*全局变量 + 预处理：*/
typedef struct Snakes
{
	int x;
	int y;
	struct Snakes *next;
}snake;
 
snake *head,*tail;
 
struct Food
{
	int x;
	int y;
}food;
char name[10];
int score = 0;
char click = 1;
int speed;
 
/************************************************************/
 
int main()
{
	//system("color 0B");
	welcome();
	creatgraph();
	creatfood();
	if (ClickControl() == 0) return 0;
	return 0;
}
 
/**********************************************************/
void welcome()
{
	int i;
	gotoxy(15, 5);
	printf("/**********************************************/");
	gotoxy(15, 20);
	printf("/**********************************************/");
	gotoxy(35, 7);
	printf("贪吃蛇游戏");
	gotoxy(15, 16);
	printf("注意：请在英文输入法中操作!");
	gotoxy(15, 18);
	printf("操作说明：反向移动等同于吃到自己，wasd控制p暂停。"); 
	gotoxy(35, 22);
	printf("***Powered By Hollow Man***");
	gotoxy(20, 11);
	printf("请输入你的姓名（少于10字符）:");
	for (i=0;i<10;i++)
	{
		scanf("%c",&name[i]);
		if(name[i]=='\n')
			break;
		else if(i==9)
			break;
	}
	system("cls");
}
/**********************************************************/
void creatgraph()             
{
	int i;
	for (i = 0; i<58; i += 2)//打印上下边框
	{
		gotoprint(i, 0);
		gotoprint(i, 26);
	}
	for (i = 1; i < 26; i++)
	{
		gotoprint(0, i);
		gotoprint(56, i);
	}
	gotoxy(61, 1);
	printf("你好！%s",name);
	gotoxy(59, 3);
	printf("欢迎进入贪吃蛇游戏");
	gotoxy(61, 15);
	printf("你现在得分:     %d",score);
	gotoxy(65, 11);
	printf("=￣ω￣=");
	gotoxy(15, 27);
	printf("***Powered By Hollow Man***");
	head = (snake*)malloc(sizeof(snake));
	head->x = 16;
	head->y = 15;
	//gotoprint(head->x, head->y);
	tail = (snake*)malloc(sizeof(snake));
	snake *p = (snake*)malloc(sizeof(snake));
	snake *q = (snake*)malloc(sizeof(snake));
	p->x = 16;
	p->y = 16;
	q->x = 16;
	q->y = 17;
	head->next = p;
	p->next = q;
	q->next = tail;
	//gotoprint(p->x, p->y);
	//gotoprint(q->x, q->y);
	tail->next = NULL;
	tail->x = 4;
	tail->y = 2;
	
}
/**********************************************************/
void gotoxy(int x, int y)
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}
/**********************************************************/
void gotoprint(int x, int y)
{
	gotoxy(x, y);
	printf("■");
}
/**********************************************************/
void gotodelete(int x, int y)
{
	gotoxy(x, y);
	printf("  ");
}
/**********************************************************/
void creatfood()
{
	srand((int)time(NULL));
	lable:
	food.y = rand() % (25 - 1 + 1) + 1;
	food.x = rand() % (54 - 2 + 1) + 2;
	if (food.x % 2 != 0)
	{
		food.x = food.x+1;
	}
	snake *judge = head;
	while (1)  //遍历排除蛇身重复
	{
		if (judge->next == NULL) break;
		if (food.x == judge->x&&food.y == judge->y)
		{
			goto lable;
		}
		judge = judge->next;
	}
	gotoxy(food.x, food.y);
	printf("⊙");
}
/**********************************************************/
int ClickControl()
{
	char c;
	while (1)
	{
		if (Judge()==0) return 0;
		if (_kbhit())
		{
			click = _getch();
		}
		MovingBody();
		Eating();
	}
	return 1;
}
/**********************************************************/
void MovingBody()
{
	int count = 0;
	int a = head->x, b = head->y;
	snake *p = head;
	//通过先清空后打印实现动画效果
	while (1)
	{
		if (p->next == NULL) break;
		gotodelete(p->x, p->y);
		count++;
		p = p->next;
	}
	switch (click)
	{
		case up:
			head->y -= 1;
			ChangeBody(a,b);
			break;
		case down:
			head->y += 1;
			ChangeBody(a,b);
			break;
		case left:
			head->x -= 2;
			ChangeBody(a,b);
			break;
		case right:
			head->x += 2;
			ChangeBody(a,b);
			break;
		case stop:
			_getch();
			break;
	}
	p = head;
	while (1)
	{
		if (p->next == NULL) break;
		gotoprint(p->x, p->y);
		p = p->next;
	}
	p = head;
	gotoxy(0, 28);
	if (count <= 10) speed = 150;
	else if (count > 10 && count <= 20) speed = 100;
	else if (count > 20 && count <= 40) speed = 50;
	else speed = 10;
	Sleep(speed);
}
/**********************************************************/
void Eating()
{
	if (head->x == food.x&&head->y == food.y)
	{
		creatfood();
		snake *_new = (snake*)malloc(sizeof(snake));
		snake *p;
		p = head;
		while (1)
		{
			if (p->next->next == NULL) break;
			p = p->next;
		}
		p->next = _new;
		_new->next = tail;
		score += 10;
		gotoxy(77, 15);
		printf("%d", score);
	}
}
/**********************************************************/
void ChangeBody(int a,int b)
{
	snake *p = head->next;
	int mid1, mid2,_mid1,_mid2;
	mid1 = p->x;
	mid2 = p->y;
	while (1)
	{
		if (p->next->next == NULL) break;
		_mid1 = p->next->x;
		_mid2 = p->next->y;
		p->next->x = mid1;
		p->next->y = mid2;
		mid1 = _mid1;
		mid2 = _mid2; 
		p = p->next;
	}
	p = head->next;
	//if (p->next!= NULL)
	{
		p->x = a;
		p->y = b;
	}
}
/**********************************************************/
int Judge()
{
	if (head->x == 0|| head->x == 56 || head->y == 0 || head->y == 26)
	{
		Finish();
		return 0;
	}
	snake *p = head->next;
	while (1)
	{
		if (p->next == NULL) break;
		if (head->x == p->x&&head->y == p->y)
		{
			Finish();
			return 0;
		}
		p = p->next;
	}
	return 1;
}
/**********************************************************/
void Finish()
{
	system("cls");
	gotoxy(15, 10);
	printf("/**********************************************/");
	gotoxy(15, 20);
	printf("/**********************************************/");
	gotoxy(21, 12);
	printf("游 戏 结 束 ···      o(*￣▽￣*)o");
	gotoxy(33, 14);
	printf("%s", name);
	gotoxy(26, 16);
	printf("你的得分是： %d    :) :) :)",score);
	gotoxy(23, 18);
	printf("还不错哦!      继续努力O(∩_∩)O");
	gotoxy(0, 30);
	system("pause");
}