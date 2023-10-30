#include<stdio.h>
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<mmsystem.h>//����
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
	BULLET_NUM=15,//�ӵ�����
	ENEMY_NUM=10,
	ENEMYBULLET_NUM=15,//�л��ӵ���Ŀ
	BIG,
	SMALL
};
IMAGE bk;
int bkx, bky;
//�������ͼƬ
IMAGE img_gamer[2];
IMAGE img_gamerExplode[2];
//�����ӵ�
IMAGE img_bull[2];
IMAGE img_enemy[2][2];
IMAGE img_enemybull[2];
int canvas[HEIGHT][WIDTH] = { 0 };
int score = 0;
int isExplode = 0;//�ɻ�״̬
int gamer_x, gamer_y;//�ɻ�����
int tmp_numbers[100] = { 0 };
int numbers[3] = { 0 };
int cnt = 0;
char tmp[5];
int tmp_cnt = 0;
int color(int c)//��ɫ����
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);//��������c��ֵ��ͬ�����ͬ��ɫ��c��ֵΪ0��15
	return 0;
}

bool cmp(int a, int b)//��С����������sortһ����
{
	return a > b;
}

struct Plance
{
	int x;//x����
	int y;//y����
	bool live;//�Ƿ���
	int width;//���
	int height;//�߶�
	int hp;//Ѫ��
	int type;//�л�����
}gamer,gamerExplode,bull[BULLET_NUM],enemy[ENEMY_NUM],enemybull[ENEMYBULLET_NUM];

void loadImg()//����ͼƬ
{
	loadimage(&bk, "./images/background.jpg");//����ͼ
	loadimage(&img_gamer[0], "./images/gamer1.jpg");//���ͼƬ
	loadimage(&img_gamer[1], "./images/gamer2.jpg");//����ͼ
	loadimage(&img_gamerExplode[0], "./images/gamerExplode1.jpg");//��ұ�ըͼƬ
	loadimage(&img_gamerExplode[1], "./images/gamerExplode2.jpg");//����ͼ
	loadimage(&img_bull[0], "./images/bullet1.jpg");//����ӵ�ͼƬ
	loadimage(&img_bull[1], "./images/bullet2.jpg");//����ͼ
	loadimage(&img_enemybull[0], "./images/enemybullet1.jpg");//�л��ӵ�
	loadimage(&img_enemybull[1], "./images/enemybullet2.jpg");//����ͼ
	loadimage(&img_enemy[0][0], "./images/enemy_1.jpg");//�л�ͼ
	loadimage(&img_enemy[0][1], "./images/enemy_2.jpg");//����ͼ
	loadimage(&img_enemy[1][0], "./images/enemyPlane1.jpg");//BOSS
	loadimage(&img_enemy[1][1], "./images/enemyPlane2.jpg");//����ͼ
}

void enemyHp(int i)//�л����Ͳ�һѪ����ͬ
{
	if (rand() % 10 == 0)//�������BOSS
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

bool Timer(int ms, int id)//��ʱ��
{
	static DWORD t[10];
	if (clock() - t[id] > ms)//����ʱ����
	{
		t[id] = clock();
		return true;
	}
	return false;
}

void gameStart()//��Ϸ��ʼ
{
	mciSendString("open ./images/game_music.mp3 alias BGM",0,0,0);//��������
	mciSendString("play BGM repeat", 0, 0, 0);//�ظ�����
	loadImg();//����ͼƬ
	bkx = 0;//���ر�������
	bky = -HEIGHT;
	gamer.x = WIDTH / 2;//�������x����
	gamer.y = HEIGHT - 120;//�������y����
	gamer.live = true;//���״̬
	for (int i = 0; i < BULLET_NUM; i++)//��ʼ���ӵ�
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	for (int i = 0; i < ENEMYBULLET_NUM; i++)//��ʼ���л��ӵ�
	{
		enemybull[i].x = 0;
		enemybull[i].y = 0;
		enemybull[i].live = true;
	}
	for (int i = 0; i < ENEMY_NUM; i++)//��ʼ���л�
	{
		enemy[i].live = false;
		enemyHp(i);
	}
}

void gameDraw()//������ʼ��
{
	putimage(0, 0, &bk);//���뱳��ͼ
	outtextxy(WIDTH * 85 / 100, HEIGHT * 5 / 100,_T("���а�"));//�������а�
		char tmp_s[5];//�����������ݲ�������������а�
	for (int i = 0; i <= 8; i++){
			sprintf_s(tmp_s, "%d", tmp_numbers[i]);
			outtextxy(WIDTH * 85 / 100, HEIGHT * (10 + 5 * i) / 100, tmp_s);
	}
	if (isExplode == 0)//���״̬
	{
		putimage(gamer.x, gamer.y, &img_gamer[0], NOTSRCERASE);//�������ͼ
		putimage(gamer.x, gamer.y, &img_gamer[1], SRCINVERT);
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bull[i].live)
			{
				putimage(bull[i].x, bull[i].y, &img_bull[0], NOTSRCERASE);//�����ӵ�ͼ
				putimage(bull[i].x, bull[i].y, &img_bull[1], SRCINVERT);

			}
		}for (int i = 0; i < ENEMYBULLET_NUM; i++)
		{
			if (enemybull[i].live)
			{
				putimage(enemybull[i].x, enemybull[i].y, &img_enemybull[0], NOTSRCERASE);//����л��ӵ�ͼ
				putimage(enemybull[i].x, enemybull[i].y, &img_enemybull[1], SRCINVERT);
			}
		}
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i].live)
			{
				if (enemy[i].type == BIG)
				{
					putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);//����л�ͼ
					putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1], SRCINVERT);
				}
				else
				{
					putimage(enemy[i].x, enemy[i].y, &img_enemy[0][0], NOTSRCERASE);//����BOSSͼ
					putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], SRCINVERT);
				}
			}
		}
	}
	else//��ұ�ը
	{
		putimage(gamerExplode.x, gamerExplode.y, &img_gamerExplode[0], NOTSRCERASE);
		putimage(gamerExplode.x, gamerExplode.y, &img_gamerExplode[1], SRCINVERT);
	}
	outtextxy(WIDTH /2, HEIGHT * 9/10, _T("�÷�:"));//����Ϸ������ʾ�÷�
	TCHAR  s[5];
	_stprintf_s(s, _T("%d"), score);		
	outtextxy(WIDTH * 55 / 100, HEIGHT * 9 / 10, s);
	Sleep(10);
}

void createBullet() //�����ӵ�
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bull[i].live)//�ӵ�״̬
		{
			bull[i].x = gamer.x+60;
			bull[i].y = gamer.y;
			bull[i].live = true;
			break;
		}
	}
}

void BulletMove(int speed)//�ӵ��ƶ�
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bull[i].live)//�ӵ�״̬
		{
			bull[i].y -=speed;//�ӵ��ƶ��ٶ�
			if (bull[i].y < 0)//Խ���ж�
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
	for (int i = 0; i < ENEMYBULLET_NUM; i++)//�����л��ӵ�
	{
		for (int j = 0; i < ENEMY_NUM; j++)//�����л�
		{
			if (!enemy[j].live)
				continue;
			else {
				if (!enemybull[i].live)//�л��ӵ�״̬
				{
					enemybull[i].x = enemy[j].x + 60;//�л��ӵ�
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
		if (enemybull[i].live)//�л��ӵ�״̬
		{
			enemybull[i].y += speed;//�л��ӵ��ƶ��ٶ�
			if (enemybull[i].y > 0)
			{
				enemybull[i].x = 0;
				enemybull[i].y = 0;
				enemybull[i].live = false;
			}
		}
	}
}

void createEnemy()//�����л�
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)//�л�״̬
		{
			enemy[i].live = true;//�л�����
			enemy[i].x = rand() % (WIDTH - 60);//x����
			enemy[i].y = 0;//y����
			enemyHp(i);//����Ѫ��
			break;
		}
	}
}

void enemyMove(int speed) //�л��ƶ�
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)//�л�״̬
		{
			enemybull[i].y += speed;//�л��ӵ��ƶ�
			enemy[i].y+=speed;//�л��ƶ��ٶ�
			if (enemy[i].y > HEIGHT)//Խ���ж�
			{
				enemy[i].live = false;//�л���ʧ
			}
		}
	}
}

void playPlance()//�ɻ���ս
{
	for (int i = 0; i < ENEMY_NUM;i++)
	{
		if (!enemy[i].live)//�л�״̬
			continue;
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bull[i].live)//�ӵ�״̬
				continue;
			if (bull[k].x > enemy[i].x 
				&& bull[k].x<enemy[i].x + enemy[i].width 
				&&bull[k].y>enemy[i].y 
				&& bull[k].y < enemy[i].y + enemy[i].height)//�ж��Ƿ����
			{
				bull[k].live = false;//�ӵ���ʧ
				mciSendString("open ./images/gotEnemy.mp3 alias BGM", 0, 0, 0);//�����ӵ����ел�������
				enemy[i].hp--;//Ѫ���仯
			}
		}
		if (enemy[i].type ==BIG&&enemy[i].hp<=0)//��ɻ�������
		{
			enemy[i].live = false;//BOSS����
			score+=3;//�÷ֱ仯
			createEnemy();//�����л�
		}
		else if (enemy[i].type == SMALL && enemy[i].hp <= 0)//С�ɻ�������
		{
			enemy[i].live = false;//�л�����
			score += 2;//�÷ֱ仯
			createEnemy();//�����л�
		}
	}
}

void FileWrite()//�浵
{
	FILE* fp;
	fp = fopen("data.txt", "w");//��txt�ļ�
	fprintf(fp, "%d\n", score);//�������
	fprintf(fp, "%d\n", gamer_x);//�������x����
	fprintf(fp, "%d\n", gamer_y);//�������y����
	fclose(fp);  // �ر��ļ�
	printf_s("\n����ɹ�\n");
}

void FileRead()//����
{
	FILE* fp;
	fp = fopen("data.txt", "r");//��txt�ļ�
	mciSendString("open ./images/game_music.mp3 alias BGM", 0, 0, 0);//������
	mciSendString("play BGM repeat", 0, 0, 0);//�ظ���������
	char ttmmpp[50] = { 0 };
	while (fgets(ttmmpp, sizeof(ttmmpp), fp)) {//���������ȡ�ļ�����������
		numbers[cnt++] = atoi(ttmmpp);//��ȡ�ļ�������
	}
	score = numbers[0];//����
	gamer_x = numbers[1];//x����
	gamer_y = numbers[2];//y����
	fclose(fp);//�ر��ļ�
	gameStart();//��Ϸ��ʼ
}

void rank_list()//���а�
{
	FILE* fp = fopen("rank.txt", "r");//��txt�ļ�
	char buffer[10] = { 0 };//��ȡ�ļ�������
	while (fgets(buffer, sizeof(buffer), fp)) {
		tmp_numbers[tmp_cnt++] = atoi(buffer);
	}
	fclose(fp);//�ر��ļ�
	sort(tmp_numbers, tmp_numbers + tmp_cnt, cmp);//����ʷ���ݽ�������
	FILE* fp2 = fopen("rank.txt", "w");
	for (int i = 0; i <=9; i++)
	{
		fprintf(fp2, "%d\n", tmp_numbers[i]);//�����С���������
	}
	fclose(fp2);//�ر��ļ�
}

void game_over() {
	FILE* fp = fopen("rank.txt", "a");//����Ϸ����ʱ׷�ӵ÷ֽ���txt�ļ���
	fprintf(fp, "%d", score);//׷�ӵ÷�
	fclose(fp);//�ر��ļ�
}

void be_touched()
{
	for (int i = 0; i < ENEMY_NUM; i++)//�������еĵл�
	{
		if (!enemy[i].live)//���л�������ʱ
		{
			continue;
		}
		if (gamer.x +10 >= enemy[i].x 
			&&gamer.y  >= enemy[i].y
			&& gamer.x <= enemy[i].x + enemy[i].width
			&& gamer.y <= enemy[i].y + enemy[i].height)//����ҷɻ������л�ʱ���ɻ�����
		{
			game_over();//��Ϸ����
			gamer.live = false;//���״̬
			isExplode = 1;//���״̬
			color(11);//������ɫ
			FileWrite();//�浵
			printf_s("\n��Ϸ������\n");//��������
			printf_s("�����յ÷�Ϊ%d\n", score);//���յ÷�
			Sleep(300);
			system("pause");
			exit(0);
		}
	}
}

void draw_start_page()//����̨��ʼ����
{

		color(11);//������ɫ
		system("cls");//����
		printf_s("�밴R������Enter�����¿�ʼ��Ϸ\n");//��ʼ����
		printf_s("����Z������Enter�����ϴ���Ϸ��ʼ\n");//�������
		printf_s("����Ϸ�з�������Ʒɻ��ƶ�,�ո�������ӵ�\n");
		printf_s("����Esc�浵\n");//�浵����
		char input;//��������
		cin >> input;
		if (input == 'Z' || input == 'z')//��ȡ��������Ϊz
		{
			FileRead();//����
		}
		else if (input =='R' || input == 'r')//��ȡ��������Ϊr
		{
			gameStart();//��Ϸ��ʼ
		}
	char ch=getchar();//��ֹvs����
}

void gamerMove(int speed) {
	bkx++;//��������
	bky++;
	if (bkx >= HEIGHT)//��������
		bkx = -HEIGHT;
	if (bky >= HEIGHT)
		bky = -HEIGHT;
	//����Ƿ��м��̰���
#if 0
#elif 1
	//ʹ��windows������ȡ��������
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
		createBullet();//�����ӵ�
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		FileWrite();//�浵
		system("pause");
		exit(0);
	}
}

int main()
{
	rank_list();//���а�
	//��������
	initgraph(WIDTH,HEIGHT,SHOWCONSOLE);
	BeginBatchDraw();//˫����
	draw_start_page();//��ʼҳ��
	while (1)
	{
		FlushBatchDraw();
		gameDraw();//��ʼ������
		gamerMove(3);//����ƶ�
		BulletMove(5);//�ӵ��ƶ�
		enemyBulletMove(5);//�л��ӵ��ƶ�
		if (Timer(500,0))
		{
			createEnemy();//�����л�
		}
		if (Timer(10, 2))
		{
			enemyMove(1);//�л��ƶ�
		}
		playPlance();//�ɻ���ս
		be_touched();//��ײ���
	}
	return 0;
}
