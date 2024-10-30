#include"cppgame.h"

picture GameStartTitle; //开始游戏画面
picture GameScoreTitle; //得分
picture ShieldTitle; //护盾 
picture my_plane; //我方飞机 
picture GameOverTitle; //游戏结束画面 
char KeyBoardInPut; //键盘输入 
int shoot_down_number; //击落敌方飞船的数量 
int shield_number; //护盾数值 
clock_t StartTime, EndTime, NowTime; //时间戳 
double time_number; //花费时间 
int score_number; //最终得分 
int highest_score; //历史最高分
std::ifstream fin; //读文件 
std::ofstream fout; //写文件  

//敌方飞机类 
class other_plane :public picture
{
public:
	other_plane(int OtherPlaneX, int OtherPlaneY); //新构造函数 

private:
};
std::vector<other_plane>all_other_plane; //敌方飞机 

//敌方飞机的构造函数 
other_plane::other_plane(int OtherPlaneX, int OtherPlaneY)
{
	positionX = OtherPlaneX;
	positionY = OtherPlaneY;
	shape = { "###",
		   " ~ " };
	SizePictureX = 3;
	SizePictureY = 2;
	ColorNumber = 1;
	direction = 2;
	speed = 0.5;
}

//炮弹类 
class bullet :public picture
{
public:
	bullet(int MyPlaneX, int MyPlaneY); //新构造函数 

private:
};
std::vector<bullet>all_bullet; //炮弹 

//炮弹的构造函数 
bullet::bullet(int MyPlaneX, int MyPlaneY)
{
	positionX = MyPlaneX + 2;
	positionY = MyPlaneY - 1;
	shape = { "^" };
	SizePictureX = 1;
	SizePictureY = 1;
	ColorNumber = 10;
	direction = 1;
	speed = 1;
}

//生成随机数 
int CreateRandomNumber(int how_long, int start) //从start开始，区间大小位how_long 
{
	return rand() % how_long + start;//生成[3, 7]内的随机数
}

//初始化 
inline void init()
{
	//去除光标 
	flash_init();
	//设置控制台大小 
	SetConsoleWindowSize(51, 20);
	//设置控制台标题 
	SetConsoleTitleA("spaceship 1.0");
	//生成随机数种子
	srand((unsigned)time(NULL));
	//设置读写取文件
	fin.open("./data.txt", std::ios::in);
	std::string buf; //按照字符串输入 
	fin >> buf;
	highest_score = atoi(buf.c_str()); //转换最高分 

	//开始游戏画面
	GameStartTitle.shape = { "    SSSSSS    PPPPPPP          ",
						  "   SS         PP    PP         ",
						  "   SSS        PP     PP        ",
						  "     SSS      PP    PP         ",
						  "       SSS    PP  PPP          ",
						  "        SSS   PP PP            ",
						  "    SSSSSS    PPP              ",
						  "              PP               ",
						  "              PP   spaceship   ",
						  "              PP          1.0  ",
						  "                               ",
						  "Press any key to continue...   " };
	GameStartTitle.SizePictureX = 31;
	GameStartTitle.SizePictureY = 12;
	GameStartTitle.ColorNumber = 5;
	GameStartTitle.positionX = 10;
	GameStartTitle.positionY = 4;
	//得分 
	GameScoreTitle.shape = { "shoot down :" };
	GameScoreTitle.SizePictureX = 12;
	GameScoreTitle.SizePictureY = 1;
	GameScoreTitle.ColorNumber = 6;
	//护盾 
	ShieldTitle.shape = { "shield :" };
	ShieldTitle.SizePictureX = 8;
	ShieldTitle.SizePictureY = 1;
	ShieldTitle.ColorNumber = 4;
	ShieldTitle.positionX = 0;
	ShieldTitle.positionY = 1;
	//我方飞船
	my_plane.shape = { "_[|]_",
					"+++++", };
	my_plane.SizePictureX = 5;
	my_plane.SizePictureY = 2;
	my_plane.ColorNumber = 10;
	my_plane.positionX = 23;
	my_plane.positionY = 17;
	//游戏结束画面 
	GameOverTitle.shape = { "          Game Over!           ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "                               ",
						 "Press any key to continue...   " };
	GameOverTitle.SizePictureX = 31;
	GameOverTitle.SizePictureY = 12;
	GameOverTitle.ColorNumber = 5;
	GameOverTitle.positionX = 10;
	GameOverTitle.positionY = 4;
}

//开始界面
inline void StartGame()
{
	system("cls");

	while (1)
	{
		//图片显示 
		GameStartTitle.ShowPicture(51, 20);
		Sleep(200);
		//按任意键进入游戏 
		if (_kbhit())
		{
			_getch();
			Sleep(50);
			break;
		}
	}
}

//键盘输入函数 
inline void _KeyBoardInPut()
{
	if (_kbhit())
	{
		KeyBoardInPut = _getch();

		switch (KeyBoardInPut)
		{
		case 'w': my_plane.direction = 1; my_plane.speed = 1; break;
		case 's': my_plane.direction = 2; my_plane.speed = 1; break;
		case 'a': my_plane.direction = 3; my_plane.speed = 1; break;
		case 'd': my_plane.direction = 4; my_plane.speed = 1; break;
		case 'k':
			bullet one_bullet(my_plane.positionX, my_plane.positionY);
			all_bullet.push_back(one_bullet); break;
		}
	}
	else
		my_plane.speed = 0;
}

//敌方飞船的生成 
inline void CreateOtherPlane()
{
	//每十秒加一个敌方飞船 
	NowTime = clock();
	int now_time_step = (NowTime - StartTime) / CLOCKS_PER_SEC % 10;

	//维持敌方飞船数量 
	if (all_other_plane.size() <= 4 + now_time_step)
	{
		other_plane one_other_plane(CreateRandomNumber(49, 0), 0);
		all_other_plane.push_back(one_other_plane);
	}
}

//子弹和敌方飞船的碰撞检测
inline void bullet_plane_delete()
{
	//敌方飞船的边缘和攻击碰撞 
	for (int j = 0; j < all_other_plane.size(); j++)
	{
		int plane_x = all_other_plane[j].positionX;
		int plane_y = all_other_plane[j].positionY;

		if (plane_y == 18)
			all_other_plane.erase(all_other_plane.begin() + j);
		if (plane_y + 1 >= my_plane.positionY &&
			plane_y <= my_plane.positionY + 1 &&
			plane_x + 2 >= my_plane.positionX &&
			plane_x <= my_plane.positionX + 4)
		{
			all_other_plane.erase(all_other_plane.begin() + j);
			shield_number--;
		}
	}

	//子弹的碰撞 
	for (int i = 0; i < all_bullet.size(); i++)
	{
		int bullet_x = all_bullet[i].positionX;
		int bullet_y = all_bullet[i].positionY;

		//边缘碰撞检测 
		if (bullet_y == 0)
		{
			all_bullet.erase(all_bullet.begin() + i);
			continue;
		}

		for (int j = 0; j < all_other_plane.size(); j++)
		{
			int plane_x = all_other_plane[j].positionX;
			int plane_y = all_other_plane[j].positionY;

			//相撞检测 
			if (bullet_x >= plane_x && bullet_x < plane_x + all_other_plane[j].SizePictureX &&
				bullet_y <= plane_y + all_other_plane[j].SizePictureY && bullet_y >= plane_y)
			{
				all_other_plane.erase(all_other_plane.begin() + j);
				all_bullet.erase(all_bullet.begin() + i);
				shoot_down_number++;
				score_number += 10;
			}
		}
	}
}

//游戏界面
inline void PlayGame()
{
	Sleep(100);

	//各种数值初始化 
	shoot_down_number = 0; //击落敌方飞船的数量 
	shield_number = 10; //护盾数值 
	score_number = 0; //最终得分 

	//开始计时 
	StartTime = clock();

	while (1)
	{
		//键盘输入 
		_KeyBoardInPut();
		//敌方飞船的生成
		CreateOtherPlane();
		//子弹和敌方飞船的碰撞检测 
		bullet_plane_delete();
		//图片显示 
		my_plane.ShowPicture(51, 20); //我方飞船 
		for (int i = 0; i < all_bullet.size(); i++) //子弹 
			all_bullet[i].ShowPicture(51, 20);
		for (int i = 0; i < all_other_plane.size(); i++) //敌方飞船 
			all_other_plane[i].ShowPicture(51, 20);
		GameScoreTitle.ShowPicture(51, 20); //击落敌方飞船数量 
		GoToPos(GameScoreTitle.positionX + GameScoreTitle.SizePictureX, GameScoreTitle.positionY);
		printf("%d", shoot_down_number);
		ShieldTitle.ShowPicture(51, 20); //护盾数量 
		GoToPos(ShieldTitle.positionX + ShieldTitle.SizePictureX, ShieldTitle.positionY);
		printf("%d", shield_number);

		if (shield_number == -1)
		{
			//计时结束
			EndTime = clock();//程序结束用时
			time_number = (double)(EndTime - StartTime) / CLOCKS_PER_SEC;

			//计算最终得分 
			score_number += time_number;
			//更新最高分,并写入文件 
			if (score_number > highest_score)
			{
				highest_score = score_number;
				fout.open("./data.txt", std::ios::trunc);
				fout << highest_score;
				fout.close();
			}

			break; //无护盾被击落，结束游戏
		}

		Sleep(10);
		system("cls");
	}
}

//结束界面
inline void GameOver()
{
	system("cls");

	while (1)
	{
		//显示数据
		GameOverTitle.ShowPicture(51, 20);
		GoToPos(14, 6);
		std::cout << "time:          " << time_number << "s";
		GoToPos(14, 8);
		std::cout << "shoot down:    " << shoot_down_number;
		GoToPos(14, 10);
		std::cout << "score:         " << score_number;
		GoToPos(14, 12);
		std::cout << "highest score: " << highest_score;
		Sleep(4000);

		//按任意键回到游戏开始画面 
		if (_kbhit())
		{
			_getch();
			Sleep(50);
			break;
		}
	}
}

//主函数 
int main()
{
	//初始化 
	init();
	while (1)
	{
		//开始界面
		StartGame();
		//游戏界面
		PlayGame();
		//结束界面
		GameOver();
	}

	return 0;
}