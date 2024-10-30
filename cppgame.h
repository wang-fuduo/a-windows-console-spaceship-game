//=============ͷ�ļ�=============// 
#include<windows.h>
#include<iostream>
#include<cstring>
#include<vector>
#include<conio.h>
#include<cstdlib>
#include<ctime>
#include<fstream>
//=============ͷ�ļ�=============// 

//=============С�͸���ģ��=============// 
//ָ�����λ�� 
void GoToPos(int x, int y)
{
	HANDLE hout; //������
	COORD cor; //��������
	hout = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ��׼������
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor); //���ù��λ��
}

//������ɫ 
void color(int ColorNumber)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorNumber);
}
//=============С�͸���ģ��=============//

//===============�й�ͼ�ε�ģ��=============// 
//ͼƬ���� 
class picture
{
public:
	std::vector<std::string>shape; //ͼƬ��ʽ
	double positionX; //ͼƬX���� 
	double positionY; //ͼƬY���� 
	int SizePictureX; //�����С 
	int SizePictureY; //�����С 
	int ColorNumber; //��ɫ��ֵ 
	int direction; //�ƶ�����
	double speed; //�ƶ��ٶ�(һ֡����) 

	picture(); //���캯�� 
	void ShowPicture(int WindowSizeX, int WindowSizeY); //��ʾͼƬ 

private:
};

//ͼƬ�Ĺ��캯�� 
picture::picture()
{
	positionX = 0;
	positionY = 0;
	SizePictureY = 0;
	SizePictureX = 0;
	ColorNumber = 15;
	direction = 1;
	speed = 0;
}

//ͼƬ����ʾ���� 
void picture::ShowPicture(int WindowSizeX, int WindowSizeY)
{
	//(1:�� 2:�� 3:�� 4:��) 
	//(5:���� 6:���� 7:���� 8:����)
	switch (direction)
	{
	case 1: positionY -= speed; break;
	case 2: positionY += speed; break;
	case 3: positionX -= speed; break;
	case 4: positionX += speed; break;
	case 5: positionX -= speed; positionY -= speed; break;
	case 6: positionX += speed; positionY -= speed; break;
	case 7: positionX -= speed; positionY += speed; break;
	case 8: positionX += speed; positionY += speed; break;
	}

	//��ֹ��������
	if (positionX < 0)
		positionX = 0;
	else if (positionX + SizePictureX > WindowSizeX)
		positionX = WindowSizeX - SizePictureX;
	if (positionY < 0)
		positionY = 0;
	else if (positionY + SizePictureY > WindowSizeY)
		positionY = WindowSizeY - SizePictureY;

	//������ɫ 
	color(ColorNumber);

	//��ÿһ�н��д�ӡ 
	for (int i = 0; i < SizePictureY; i++)
	{
		//�趨��껻�� 
		GoToPos(positionX, positionY + i);
		std::cout << shape[i];
	}
}
//===============�й�ͼ�ε�ģ��=============// 

//===============�й���Ļ���Ƶ�================// 
//ȥ����� 
inline void flash_init()
{
	//ȥ����� 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}

//���ÿ���̨��С 
void SetConsoleWindowSize(SHORT width, SHORT height)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT wrt = { 0,0,width - 1,height - 1 };
	SetConsoleWindowInfo(hStdOutput, TRUE, &wrt); // ���ô���ߴ�
	COORD coord = { width,height };
	SetConsoleScreenBufferSize(hStdOutput, coord); // ���û���ߴ�
}
//===============�й���Ļ���Ƶ�================// 