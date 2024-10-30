#include"cppgame.h"

picture GameStartTitle; //��ʼ��Ϸ����
picture GameScoreTitle; //�÷�
picture ShieldTitle; //���� 
picture my_plane; //�ҷ��ɻ� 
picture GameOverTitle; //��Ϸ�������� 
char KeyBoardInPut; //�������� 
int shoot_down_number; //����з��ɴ������� 
int shield_number; //������ֵ 
clock_t StartTime, EndTime, NowTime; //ʱ��� 
double time_number; //����ʱ�� 
int score_number; //���յ÷� 
int highest_score; //��ʷ��߷�
std::ifstream fin; //���ļ� 
std::ofstream fout; //д�ļ�  

//�з��ɻ��� 
class other_plane :public picture
{
public:
	other_plane(int OtherPlaneX, int OtherPlaneY); //�¹��캯�� 

private:
};
std::vector<other_plane>all_other_plane; //�з��ɻ� 

//�з��ɻ��Ĺ��캯�� 
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

//�ڵ��� 
class bullet :public picture
{
public:
	bullet(int MyPlaneX, int MyPlaneY); //�¹��캯�� 

private:
};
std::vector<bullet>all_bullet; //�ڵ� 

//�ڵ��Ĺ��캯�� 
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

//��������� 
int CreateRandomNumber(int how_long, int start) //��start��ʼ�������Сλhow_long 
{
	return rand() % how_long + start;//����[3, 7]�ڵ������
}

//��ʼ�� 
inline void init()
{
	//ȥ����� 
	flash_init();
	//���ÿ���̨��С 
	SetConsoleWindowSize(51, 20);
	//���ÿ���̨���� 
	SetConsoleTitleA("spaceship 1.0");
	//�������������
	srand((unsigned)time(NULL));
	//���ö�дȡ�ļ�
	fin.open("./data.txt", std::ios::in);
	std::string buf; //�����ַ������� 
	fin >> buf;
	highest_score = atoi(buf.c_str()); //ת����߷� 

	//��ʼ��Ϸ����
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
	//�÷� 
	GameScoreTitle.shape = { "shoot down :" };
	GameScoreTitle.SizePictureX = 12;
	GameScoreTitle.SizePictureY = 1;
	GameScoreTitle.ColorNumber = 6;
	//���� 
	ShieldTitle.shape = { "shield :" };
	ShieldTitle.SizePictureX = 8;
	ShieldTitle.SizePictureY = 1;
	ShieldTitle.ColorNumber = 4;
	ShieldTitle.positionX = 0;
	ShieldTitle.positionY = 1;
	//�ҷ��ɴ�
	my_plane.shape = { "_[|]_",
					"+++++", };
	my_plane.SizePictureX = 5;
	my_plane.SizePictureY = 2;
	my_plane.ColorNumber = 10;
	my_plane.positionX = 23;
	my_plane.positionY = 17;
	//��Ϸ�������� 
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

//��ʼ����
inline void StartGame()
{
	system("cls");

	while (1)
	{
		//ͼƬ��ʾ 
		GameStartTitle.ShowPicture(51, 20);
		Sleep(200);
		//�������������Ϸ 
		if (_kbhit())
		{
			_getch();
			Sleep(50);
			break;
		}
	}
}

//�������뺯�� 
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

//�з��ɴ������� 
inline void CreateOtherPlane()
{
	//ÿʮ���һ���з��ɴ� 
	NowTime = clock();
	int now_time_step = (NowTime - StartTime) / CLOCKS_PER_SEC % 10;

	//ά�ֵз��ɴ����� 
	if (all_other_plane.size() <= 4 + now_time_step)
	{
		other_plane one_other_plane(CreateRandomNumber(49, 0), 0);
		all_other_plane.push_back(one_other_plane);
	}
}

//�ӵ��͵з��ɴ�����ײ���
inline void bullet_plane_delete()
{
	//�з��ɴ��ı�Ե�͹�����ײ 
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

	//�ӵ�����ײ 
	for (int i = 0; i < all_bullet.size(); i++)
	{
		int bullet_x = all_bullet[i].positionX;
		int bullet_y = all_bullet[i].positionY;

		//��Ե��ײ��� 
		if (bullet_y == 0)
		{
			all_bullet.erase(all_bullet.begin() + i);
			continue;
		}

		for (int j = 0; j < all_other_plane.size(); j++)
		{
			int plane_x = all_other_plane[j].positionX;
			int plane_y = all_other_plane[j].positionY;

			//��ײ��� 
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

//��Ϸ����
inline void PlayGame()
{
	Sleep(100);

	//������ֵ��ʼ�� 
	shoot_down_number = 0; //����з��ɴ������� 
	shield_number = 10; //������ֵ 
	score_number = 0; //���յ÷� 

	//��ʼ��ʱ 
	StartTime = clock();

	while (1)
	{
		//�������� 
		_KeyBoardInPut();
		//�з��ɴ�������
		CreateOtherPlane();
		//�ӵ��͵з��ɴ�����ײ��� 
		bullet_plane_delete();
		//ͼƬ��ʾ 
		my_plane.ShowPicture(51, 20); //�ҷ��ɴ� 
		for (int i = 0; i < all_bullet.size(); i++) //�ӵ� 
			all_bullet[i].ShowPicture(51, 20);
		for (int i = 0; i < all_other_plane.size(); i++) //�з��ɴ� 
			all_other_plane[i].ShowPicture(51, 20);
		GameScoreTitle.ShowPicture(51, 20); //����з��ɴ����� 
		GoToPos(GameScoreTitle.positionX + GameScoreTitle.SizePictureX, GameScoreTitle.positionY);
		printf("%d", shoot_down_number);
		ShieldTitle.ShowPicture(51, 20); //�������� 
		GoToPos(ShieldTitle.positionX + ShieldTitle.SizePictureX, ShieldTitle.positionY);
		printf("%d", shield_number);

		if (shield_number == -1)
		{
			//��ʱ����
			EndTime = clock();//���������ʱ
			time_number = (double)(EndTime - StartTime) / CLOCKS_PER_SEC;

			//�������յ÷� 
			score_number += time_number;
			//������߷�,��д���ļ� 
			if (score_number > highest_score)
			{
				highest_score = score_number;
				fout.open("./data.txt", std::ios::trunc);
				fout << highest_score;
				fout.close();
			}

			break; //�޻��ܱ����䣬������Ϸ
		}

		Sleep(10);
		system("cls");
	}
}

//��������
inline void GameOver()
{
	system("cls");

	while (1)
	{
		//��ʾ����
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

		//��������ص���Ϸ��ʼ���� 
		if (_kbhit())
		{
			_getch();
			Sleep(50);
			break;
		}
	}
}

//������ 
int main()
{
	//��ʼ�� 
	init();
	while (1)
	{
		//��ʼ����
		StartGame();
		//��Ϸ����
		PlayGame();
		//��������
		GameOver();
	}

	return 0;
}