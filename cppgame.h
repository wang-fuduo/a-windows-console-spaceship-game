//=============头文件=============// 
#include<windows.h>
#include<iostream>
#include<cstring>
#include<vector>
#include<conio.h>
#include<cstdlib>
#include<ctime>
#include<fstream>
//=============头文件=============// 

//=============小型辅助模块=============// 
//指定光标位置 
void GoToPos(int x, int y)
{
	HANDLE hout; //定义句柄
	COORD cor; //定义坐标
	hout = GetStdHandle(STD_OUTPUT_HANDLE); //获取标准输出句柄
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor); //设置光标位置
}

//设置颜色 
void color(int ColorNumber)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorNumber);
}
//=============小型辅助模块=============//

//===============有关图形的模块=============// 
//图片的类 
class picture
{
public:
	std::vector<std::string>shape; //图片样式
	double positionX; //图片X坐标 
	double positionY; //图片Y坐标 
	int SizePictureX; //横向大小 
	int SizePictureY; //纵向大小 
	int ColorNumber; //颜色数值 
	int direction; //移动方向
	double speed; //移动速度(一帧几格) 

	picture(); //构造函数 
	void ShowPicture(int WindowSizeX, int WindowSizeY); //显示图片 

private:
};

//图片的构造函数 
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

//图片的显示函数 
void picture::ShowPicture(int WindowSizeX, int WindowSizeY)
{
	//(1:上 2:下 3:左 4:右) 
	//(5:左上 6:右上 7:左下 8:右下)
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

	//防止超出画面
	if (positionX < 0)
		positionX = 0;
	else if (positionX + SizePictureX > WindowSizeX)
		positionX = WindowSizeX - SizePictureX;
	if (positionY < 0)
		positionY = 0;
	else if (positionY + SizePictureY > WindowSizeY)
		positionY = WindowSizeY - SizePictureY;

	//设置颜色 
	color(ColorNumber);

	//对每一行进行打印 
	for (int i = 0; i < SizePictureY; i++)
	{
		//设定光标换行 
		GoToPos(positionX, positionY + i);
		std::cout << shape[i];
	}
}
//===============有关图形的模块=============// 

//===============有关屏幕控制的================// 
//去除光标 
inline void flash_init()
{
	//去除光标 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}

//设置控制台大小 
void SetConsoleWindowSize(SHORT width, SHORT height)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT wrt = { 0,0,width - 1,height - 1 };
	SetConsoleWindowInfo(hStdOutput, TRUE, &wrt); // 设置窗体尺寸
	COORD coord = { width,height };
	SetConsoleScreenBufferSize(hStdOutput, coord); // 设置缓冲尺寸
}
//===============有关屏幕控制的================// 