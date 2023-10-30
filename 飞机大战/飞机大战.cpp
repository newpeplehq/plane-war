#include<stdio.h>
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<mmsystem.h>//音乐
#include<Windows.h>
#include<iostream>
#include<stdlib.h>
#include<algorithm>
#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996)
using namespace std;
enum My
{
	WIDTH = 404,
	HEIGHT = 720,
	BULLET_NUM=15,//子弹数量
	ENEMY_NUM=10,
	ENEMYBULLET_NUM=15,//敌机子弹数目
	BIG,
	SMALL
};
IMAGE bk;
int bkx, bky;
//保存玩家图片
IMAGE img_gamer[2];
IMAGE img_gamerExplode[2];
//保存子弹
IMAGE img_bull[2];
IMAGE img_enemy[2][2];
IMAGE img_enemybull[2];
int canvas[HEIGHT][WIDTH] = { 0 };
int score = 0;
int isExplode = 0;//飞机状态
int gamer_x, gamer_y;//飞机坐标
int tmp_numbers[100] = { 0 };
int numbers[3] = { 0 };
int cnt = 0;
char tmp[5];
int tmp_cnt = 0;
int color(int c)//颜色绘制
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);//根据输入c的值不同输出不同颜色，c的值为0到15
	return 0;
}

bool cmp(int a, int b)//从小到大排序与sort一起用
{
	return a > b;
}

struct Plance
{
	int x;//x坐标
	int y;//y坐标
	bool live;//是否存活
	int width;//宽度
	int height;//高度
	int hp;//血量
	int type;//敌机类型
}gamer,gamerExplode,bull[BULLET_NUM],enemy[ENEMY_NUM],enemybull[ENEMYBULLET_NUM];

void loadImg()//加载图片
{
	loadimage(&bk, "./images/background.jpg");//背景图
	loadimage(&img_gamer[0], "./images/gamer1.jpg");//玩家图片
	loadimage(&img_gamer[1], "./images/gamer2.jpg");//掩码图
	loadimage(&img_gamerExplode[0], "./images/gamerExplode1.jpg");//玩家爆炸图片
	loadimage(&img_gamerExplode[1], "./images/gamerExplode2.jpg");//掩码图
	loadimage(&img_bull[0], "./images/bullet1.jpg");//玩家子弹图片
	loadimage(&img_bull[1], "./images/bullet2.jpg");//掩码图
	loadimage(&img_enemybull[0], "./images/enemybullet1.jpg");//敌机子弹
	loadimage(&img_enemybull[1], "./images/enemybullet2.jpg");//掩码图
	loadimage(&img_enemy[0][0], "./images/enemy_1.jpg");//敌机图
	loadimage(&img_enemy[0][1], "./images/enemy_2.jpg");//掩码图
	loadimage(&img_enemy[1][0], "./images/enemyPlane1.jpg");//BOSS
	loadimage(&img_enemy[1][1], "./images/enemyPlane2.jpg");//掩码图
}

void enemyHp(int i)//敌机类型不一血量不同
{
	if (rand() % 10 == 0)//随机产生BOSS
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 104;
		enemy[i].height = 148;
	}
	else {
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 52;
		enemy[i].height = 39;
	}
};

bool Timer(int ms, int id)//定时器
{
	static DWORD t[10];
	if (clock() - t[id] > ms)//大于时间间隔
	{
		t[id] = clock();
		return true;
	}
	return false;
}

void gameStart()//游戏开始
{
	mciSendString("open ./images/game_music.mp3 alias BGM",0,0,0);//播放音乐
	mciSendString("play BGM repeat", 0, 0, 0);//重复播放
	loadImg();//加载图片
	bkx = 0;//加载背景坐标
	bky = -HEIGHT;
	gamer.x = WIDTH / 2;//加载玩家x坐标
	gamer.y = HEIGHT - 120;//加载玩家y坐标
	gamer.live = true;//玩家状态
	for (int i = 0; i < BULLET_NUM; i++)//初始化子弹
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	for (int i = 0; i < ENEMYBULLET_NUM; i++)//初始化敌机子弹
	{
		enemybull[i].x = 0;
		enemybull[i].y = 0;
		enemybull[i].live = true;
	}
	for (int i = 0; i < ENEMY_NUM; i++)//初始化敌机
	{
		enemy[i].live = false;
		enemyHp(i);
	}
}

void gameDraw()//画布初始化
{
	putimage(0, 0, &bk);//放入背景图
	outtextxy(WIDTH * 85 / 100, HEIGHT * 5 / 100,_T("排行榜"));//加载排行榜
		char tmp_s[5];//用数组存进数据并按行输出在排行榜
	for (int i = 0; i <= 8; i++){
			sprintf_s(tmp_s, "%d", tmp_numbers[i]);
			outtextxy(WIDTH * 85 / 100, HEIGHT * (10 + 5 * i) / 100, tmp_s);
	}
	if (isExplode == 0)//玩家状态
	{
		putimage(gamer.x, gamer.y, &img_gamer[0], NOTSRCERASE);//放入玩家图
		putimage(gamer.x, gamer.y, &img_gamer[1], SRCINVERT);
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bull[i].live)
			{
				putimage(bull[i].x, bull[i].y, &img_bull[0], NOTSRCERASE);//放入子弹图
				putimage(bull[i].x, bull[i].y, &img_bull[1], SRCINVERT);

			}
		}for (int i = 0; i < ENEMYBULLET_NUM; i++)
		{
			if (enemybull[i].live)
			{
				putimage(enemybull[i].x, enemybull[i].y, &img_enemybull[0], NOTSRCERASE);//放入敌机子弹图
				putimage(enemybull[i].x, enemybull[i].y, &img_enemybull[1], SRCINVERT);
			}
		}
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i].live)
			{
				if (enemy[i].type == BIG)
				{
					putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);//放入敌机图
					putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1], SRCINVERT);
				}
				else
				{
					putimage(enemy[i].x, enemy[i].y, &img_enemy[0][0], NOTSRCERASE);//放入BOSS图
					putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], SRCINVERT);
				}
			}
		}
	}
	else//玩家爆炸
	{
		putimage(gamerExplode.x, gamerExplode.y, &img_gamerExplode[0], NOTSRCERASE);
		putimage(gamerExplode.x, gamerExplode.y, &img_gamerExplode[1], SRCINVERT);
	}
	outtextxy(WIDTH /2, HEIGHT * 9/10, _T("得分:"));//在游戏中央显示得分
	TCHAR  s[5];
	_stprintf_s(s, _T("%d"), score);		
	outtextxy(WIDTH * 55 / 100, HEIGHT * 9 / 10, s);
	Sleep(10);
}

void createBullet() //创建子弹
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bull[i].live)//子弹状态
		{
			bull[i].x = gamer.x+60;
			bull[i].y = gamer.y;
			bull[i].live = true;
			break;
		}
	}
}

void BulletMove(int speed)//子弹移动
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bull[i].live)//子弹状态
		{
			bull[i].y -=speed;//子弹移动速度
			if (bull[i].y < 0)//越界判断
			{
				bull[i].x = 0;
				bull[i].y = 0;
				bull[i].live = false;
			}
		}
	}
}

void createenemyBullet()
{
	for (int i = 0; i < ENEMYBULLET_NUM; i++)//遍历敌机子弹
	{
		for (int j = 0; i < ENEMY_NUM; j++)//遍历敌机
		{
			if (!enemy[j].live)
				continue;
			else {
				if (!enemybull[i].live)//敌机子弹状态
				{
					enemybull[i].x = enemy[j].x + 60;//敌机子弹
					enemybull[i].y = enemy[j].y;
					enemybull[i].live = true;
					break;
				}
			}
		}
	}
}

void enemyBulletMove(int speed)
{
	for (int i = 0; i < ENEMYBULLET_NUM; i++)
	{
		if (enemybull[i].live)//敌机子弹状态
		{
			enemybull[i].y += speed;//敌机子弹移动速度
			if (enemybull[i].y > 0)
			{
				enemybull[i].x = 0;
				enemybull[i].y = 0;
				enemybull[i].live = false;
			}
		}
	}
}

void createEnemy()//产生敌机
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)//敌机状态
		{
			enemy[i].live = true;//敌机存在
			enemy[i].x = rand() % (WIDTH - 60);//x坐标
			enemy[i].y = 0;//y坐标
			enemyHp(i);//生成血量
			break;
		}
	}
}

void enemyMove(int speed) //敌机移动
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)//敌机状态
		{
			enemybull[i].y += speed;//敌机子弹移动
			enemy[i].y+=speed;//敌机移动速度
			if (enemy[i].y > HEIGHT)//越界判断
			{
				enemy[i].live = false;//敌机消失
			}
		}
	}
}

void playPlance()//飞机大战
{
	for (int i = 0; i < ENEMY_NUM;i++)
	{
		if (!enemy[i].live)//敌机状态
			continue;
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bull[i].live)//子弹状态
				continue;
			if (bull[k].x > enemy[i].x 
				&& bull[k].x<enemy[i].x + enemy[i].width 
				&&bull[k].y>enemy[i].y 
				&& bull[k].y < enemy[i].y + enemy[i].height)//判断是否击中
			{
				bull[k].live = false;//子弹消失
				mciSendString("open ./images/gotEnemy.mp3 alias BGM", 0, 0, 0);//播放子弹击中敌机的音乐
				enemy[i].hp--;//血量变化
			}
		}
		if (enemy[i].type ==BIG&&enemy[i].hp<=0)//大飞机加三分
		{
			enemy[i].live = false;//BOSS消亡
			score+=3;//得分变化
			createEnemy();//产生敌机
		}
		else if (enemy[i].type == SMALL && enemy[i].hp <= 0)//小飞机加两分
		{
			enemy[i].live = false;//敌机消亡
			score += 2;//得分变化
			createEnemy();//产生敌机
		}
	}
}

void FileWrite()//存档
{
	FILE* fp;
	fp = fopen("data.txt", "w");//打开txt文件
	fprintf(fp, "%d\n", score);//存入分数
	fprintf(fp, "%d\n", gamer_x);//存入玩家x坐标
	fprintf(fp, "%d\n", gamer_y);//存入玩家y坐标
	fclose(fp);  // 关闭文件
	printf_s("\n保存成功\n");
}

void FileRead()//读档
{
	FILE* fp;
	fp = fopen("data.txt", "r");//打开txt文件
	mciSendString("open ./images/game_music.mp3 alias BGM", 0, 0, 0);//打开音乐
	mciSendString("play BGM repeat", 0, 0, 0);//重复播放音乐
	char ttmmpp[50] = { 0 };
	while (fgets(ttmmpp, sizeof(ttmmpp), fp)) {//定义数组读取文件分数，坐标
		numbers[cnt++] = atoi(ttmmpp);//读取文件中数据
	}
	score = numbers[0];//分数
	gamer_x = numbers[1];//x坐标
	gamer_y = numbers[2];//y坐标
	fclose(fp);//关闭文件
	gameStart();//游戏开始
}

void rank_list()//排行榜
{
	FILE* fp = fopen("rank.txt", "r");//打开txt文件
	char buffer[10] = { 0 };//读取文件中数据
	while (fgets(buffer, sizeof(buffer), fp)) {
		tmp_numbers[tmp_cnt++] = atoi(buffer);
	}
	fclose(fp);//关闭文件
	sort(tmp_numbers, tmp_numbers + tmp_cnt, cmp);//对历史数据进行排序
	FILE* fp2 = fopen("rank.txt", "w");
	for (int i = 0; i <=9; i++)
	{
		fprintf(fp2, "%d\n", tmp_numbers[i]);//输出按小到大的数据
	}
	fclose(fp2);//关闭文件
}

void game_over() {
	FILE* fp = fopen("rank.txt", "a");//在游戏结束时追加得分进入txt文件中
	fprintf(fp, "%d", score);//追加得分
	fclose(fp);//关闭文件
}

void be_touched()
{
	for (int i = 0; i < ENEMY_NUM; i++)//遍历所有的敌机
	{
		if (!enemy[i].live)//当敌机不存在时
		{
			continue;
		}
		if (gamer.x +10 >= enemy[i].x 
			&&gamer.y  >= enemy[i].y
			&& gamer.x <= enemy[i].x + enemy[i].width
			&& gamer.y <= enemy[i].y + enemy[i].height)//当玩家飞机碰到敌机时，飞机消亡
		{
			game_over();//游戏结束
			gamer.live = false;//玩家状态
			isExplode = 1;//玩家状态
			color(11);//字体颜色
			FileWrite();//存档
			printf_s("\n游戏结束！\n");//结束界面
			printf_s("你最终得分为%d\n", score);//最终得分
			Sleep(300);
			system("pause");
			exit(0);
		}
	}
}

void draw_start_page()//控制台开始界面
{

		color(11);//字体颜色
		system("cls");//清屏
		printf_s("请按R并按下Enter键重新开始游戏\n");//开始界面
		printf_s("按下Z并按下Enter键从上次游戏开始\n");//规则介绍
		printf_s("在游戏中方向键控制飞机移动,空格键发射子弹\n");
		printf_s("按下Esc存档\n");//存档按键
		char input;//键盘输入
		cin >> input;
		if (input == 'Z' || input == 'z')//获取键盘输入为z
		{
			FileRead();//读档
		}
		else if (input =='R' || input == 'r')//获取键盘输入为r
		{
			gameStart();//游戏开始
		}
	char ch=getchar();//防止vs警告
}

void gamerMove(int speed) {
	bkx++;//背景坐标
	bky++;
	if (bkx >= HEIGHT)//背景滚动
		bkx = -HEIGHT;
	if (bky >= HEIGHT)
		bky = -HEIGHT;
	//检测是否有键盘按下
#if 0
#elif 1
	//使用windows函数获取键盘输入
	if (GetAsyncKeyState(VK_UP))
	{
		if (gamer.y > 0) {
			gamer.y -= speed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (gamer.y + 120 < HEIGHT) {
			gamer.y += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		if (gamer.x + 60 > 0) {
			gamer.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (gamer.x + 60 < WIDTH) {
			gamer.x += speed;
		}
	}
#endif //0
	if (GetAsyncKeyState(VK_SPACE) && Timer(100, 1))
	{
		createBullet();//产生子弹
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		FileWrite();//存档
		system("pause");
		exit(0);
	}
}

int main()
{
	rank_list();//排行榜
	//创建窗口
	initgraph(WIDTH,HEIGHT,SHOWCONSOLE);
	BeginBatchDraw();//双缓冲
	draw_start_page();//开始页面
	while (1)
	{
		FlushBatchDraw();
		gameDraw();//初始化画面
		gamerMove(3);//玩家移动
		BulletMove(5);//子弹移动
		enemyBulletMove(5);//敌机子弹移动
		if (Timer(500,0))
		{
			createEnemy();//产生敌机
		}
		if (Timer(10, 2))
		{
			enemyMove(1);//敌机移动
		}
		playPlance();//飞机大战
		be_touched();//碰撞检测
	}
	return 0;
}
