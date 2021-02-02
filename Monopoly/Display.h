#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include "Vector2.h"
#include "Manager.h"
#include "Grid.h"
#include "Bank.h"
#include "StockMarket.h"
#include "GameParameter.h"
using namespace std;

class Display
{
public:
	Display();
	static void Init();
	static Display* instance;
	static void drawAll();
	static void drawPlayerStatus(int player); // �e��player��(0~3)���a���U�����A��
	static void movePlayer(int player, int start, int end); // ���player�Ӫ��a�qstart���ʨ�end(0~27)
	static void drawHouse(int player, int pos, int level); // ��player�Ӫ��a�b��pos��(0~27)�\�ɯ�level��(0~3)���Фl�A�R�Ĥ@��level�N��0
	static void hint(string str); // �k�U��ܿ�J�r��
	static void hint2(string str); // �k�U��ܿ�J�r�� second line
	static void hint3(string str); // �k�U��ܿ�J�r�� third line
	static void drawWinner();
	//-------------main selections-----------
	static void drawActions();
	static void eraseActions();
	static void chooseAction(int option);
	static void eraseChosenAction(int option);
	//-------------free travel---------------
	static void chooseGrid(int pos);
	static void eraseChosenGrid(int pos);
	//-------------choose one of two---------
	static void drawOptionInquire();
	static void changeOption();
	//-------------money---------------------
	static void drawHorizotalSelector();
	static void drawMony(int amount);
	static void eraseMoney(int amount);
	//-------------stock----------------------
	static void drawStocks();
	static void drawStockPrice(int stock);
	//-------------menu-----------------------
	static void drawMenu();
	static void eraseMenu();
	static void chooseOption(int option);
	static void eraseOption(int option);
	//-------------title----------------------
	static void drawTitle();
	static void eraseTitle();
	static void chooseTitleOption(int option);
	static void eraseTitleOption(int option);
	static void drawChooseNumber();
	static void eraseChooseNumber();
	static void chooseNumbersOfPlayer(int n);
	static void eraseChosenNumber(int n);
	//----------------------------------------
	static void empty();
private:
	static COORD cursor;
	static Vector2 place[28];
	static string actions[10];
	static string options[4];
	static string startOptions[3];
	static bool confirm;
	static void gotoXY(int x, int y);
	static void changeColor(int n);
	static void drawLineO(int x);
	static void drawLineP();
	static void drawFrame(Vector2 origin);
	static void drawName(Vector2 origin, string name);
	static string getFullWidthNum(int num);
};