#include "Display.h"
#pragma comment(lib, "winmm.lib")

HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
Display* Display::instance;
COORD Display::cursor;
Vector2 Display::place[28];
string Display::actions[10];
string Display::options[4];
string Display::startOptions[3];
bool Display::confirm;

#define GAP_X 2
#define GAP_Y 1
#define SPACE_X 8
#define SPACE_Y 8
#define STATUS_X 0
#define	STATUS_Y 45
#define DICE_X 70
#define DICE_Y 47
#define ACTION_X 90
#define ACTION_Y 27
#define WHITE 4
#define BLACK 5
#define BACKGROUND 6
#define BG_GRAY 7
#define GREEN 8
#define RED 9


Display::Display()
{
	Init();
}

void Display::Init()
{	// O
	for(int i = 0; i <= 2; i++)
		place[i] = Vector2((SPACE_X + GAP_X) * i, 0);
	place[3] = Vector2((SPACE_X + GAP_X) * 2, (SPACE_Y + GAP_Y) * 2);
	for (int i = 0; i <= 2; i++)
		place[i + 4] = Vector2((SPACE_X + GAP_X) * (2 - i), (SPACE_Y + GAP_Y) * 4);
	place[7] = Vector2(0, (SPACE_Y + GAP_Y) * 2);
	place[8] = Vector2((SPACE_X + GAP_X) * 3, (SPACE_Y + GAP_Y) * 4); // dot
	// O
	for (int i = 0; i <= 2; i++)
		place[i + 9] = Vector2((SPACE_X + GAP_X) * (i + 4), 0);
	place[12] = Vector2((SPACE_X + GAP_X) * 6, (SPACE_Y + GAP_Y) * 2);
	for (int i = 0; i <= 2; i++)
		place[i + 13] = Vector2((SPACE_X + GAP_X) * (6 - i), (SPACE_Y + GAP_Y) * 4);
	place[16] = Vector2((SPACE_X + GAP_X) * 4, (SPACE_Y + GAP_Y) * 2);
	place[17] = Vector2((SPACE_X + GAP_X) * 7, (SPACE_Y + GAP_Y) * 4); // dot
	// P
	for(int i = 0; i < 5; i++)
		place[i + 18] = Vector2((SPACE_X + GAP_X) * 8, (SPACE_Y + GAP_Y) * i);
	place[23] = Vector2((SPACE_X + GAP_X) * 9, 0);
	for (int i = 0; i < 3; i++)
		place[i + 24] = Vector2((SPACE_X + GAP_X) * 10, (SPACE_Y + GAP_Y) * i);
	place[27] = Vector2((SPACE_X + GAP_X) * 9, (SPACE_Y + GAP_Y) * 2);

	actions[0] = "擲骰子";
	actions[1] = "看股市";
	actions[2] = "買股票";
	actions[3] = "賣股票";
	actions[4] = "借款";
	actions[5] = "存款";
	actions[6] = "提款";
	actions[7] = "選單";
	actions[8] = "超級骰子";
	actions[9] = "路障";

	options[0] = "回主選單";
	options[1] = "儲存遊戲";
	options[2] = "載入存檔";
	options[3] = "離開遊戲";

	startOptions[0] = "開  始  遊  戲";
	startOptions[1] = "繼  續  遊  戲";
	startOptions[2] = "離  開  遊  戲";
}

void Display::drawAll()
{
	changeColor(BACKGROUND);
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 119; j++)
			cout << ' ';
		cout << '\n';
	}
	changeColor(WHITE);
	drawLineO(0);
	drawLineO((SPACE_X + GAP_X) * 4);
	drawLineP();
	for (int i = 0; i < 28; i++)
	{		
		drawFrame(place[i]);
		drawName(place[i], Grid::instance->grids[i]->name);
		gotoXY(place[i].x + 1, place[i].y);
		cout << i;
	}
	drawPlayerStatus(Manager::instance->currentPlayerId);
	for (int i = 0; i < Manager::instance->numbersOfPlayer; i++)
	{
		changeColor(i);
		gotoXY(place[Manager::instance->players[i].position].x + i * 2, place[Manager::instance->players[i].position].y + 6);
		cout << getFullWidthNum(i + 1);
	}
	for (int i = 0; i < 28; i++)
		if (Grid::instance->grids[i]->ownerID >= 0)
		{
			drawHouse(Grid::instance->grids[i]->ownerID, i, 0);
			drawHouse(Grid::instance->grids[i]->ownerID, i, Grid::instance->grids[i]->estateLevel);
		}
}

void Display::drawPlayerStatus(int player)
{
	changeColor(player);
	for (int i = 0; i < 5; i++)
	{
		gotoXY(STATUS_X, STATUS_Y + i);
		cout << "                                                                     ";
	}
	gotoXY(STATUS_X, STATUS_Y);
	printf("第 %d 回合", Manager::instance->totalRound - Manager::instance->remain);
	gotoXY(STATUS_X + 10, STATUS_Y + 2);
	cout << "Player" << player + 1;
	gotoXY(STATUS_X + 27, STATUS_Y + 1);
	if (Manager::instance->players[player].budget >= 0)	
		cout << "現金: " << Manager::instance->players[player].budget;
	else
		cout << "欠款: " << Manager::instance->players[player].budget;

	gotoXY(STATUS_X + 42, STATUS_Y + 1);
	cout << "存款: " << Bank::instance->deposit[player];
	gotoXY(STATUS_X + 55, STATUS_Y + 1);
	cout << "總資產: " << Manager::instance->assetEval(player);
	gotoXY(STATUS_X + 27, STATUS_Y + 2);
	cout << "道具: ";
	gotoXY(STATUS_X + 27, STATUS_Y + 3);
	cout << "股票: " + StockMarket::instance->IncName[0] + " " + to_string(StockMarket::instance->hold[player][0]);
	gotoXY(STATUS_X + 42, STATUS_Y + 3);
	cout << StockMarket::instance->IncName[1] + " " + to_string(StockMarket::instance->hold[player][1]);
	gotoXY(STATUS_X + 33, STATUS_Y + 4);
	cout << StockMarket::instance->IncName[2] + " " + to_string(StockMarket::instance->hold[player][2]);
	gotoXY(STATUS_X + 42, STATUS_Y + 4);
	cout << StockMarket::instance->IncName[3] + " " + to_string(StockMarket::instance->hold[player][3]);
}

void Display::movePlayer(int player, int start, int end)
{
	clock_t time = clock();
	while (start != end)
		if(clock() - time >= GameParameter::moveTime)
		{
			changeColor(WHITE);
			gotoXY(place[start].x + player * 2, place[start].y + 6);
			cout << "  ";
			start++;
			if (start >= 28)	start %= 28;

			changeColor(player);
			gotoXY(place[start].x + player * 2, place[start].y + 6);
			cout << getFullWidthNum(player + 1);
			time = clock();			
		}

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void Display::drawHouse(int player, int pos, int level)
{
	changeColor(player);
	if (level)
	{
		gotoXY(place[pos].x + 3, place[pos].y + 2);
		cout << getFullWidthNum(level);
	}
	else
	{
		gotoXY(place[pos].x + 3, place[pos].y + 1);
		cout << "  ";
		gotoXY(place[pos].x + 2, place[pos].y + 2);
		cout << " ";
		gotoXY(place[pos].x + 5, place[pos].y + 2);
		cout << " ";
		gotoXY(place[pos].x + 1, place[pos].y + 3);
		cout << "      ";
		for (int i = 0; i < 2; i++) {
			gotoXY(place[pos].x + 2, place[pos].y + 4 + i);
			cout << " ";
			gotoXY(place[pos].x + 5, place[pos].y + 4 + i);
			cout << " ";
		}
	}
}

void Display::hint(string str)
{
	changeColor(BACKGROUND);
	gotoXY(DICE_X, DICE_Y - 1);
	cout << "                                        ";
	gotoXY(DICE_X, DICE_Y - 1);
	cout << str;
	gotoXY(DICE_X, DICE_Y);
}

void Display::hint2(string str)
{
	changeColor(BACKGROUND);
	gotoXY(DICE_X, DICE_Y);
	cout << "                                        ";
	gotoXY(DICE_X, DICE_Y);
	cout << str;
	hint3("");
}

void Display::hint3(string str)
{
	changeColor(BACKGROUND);
	gotoXY(DICE_X, DICE_Y + 1);
	cout << "                                        ";
	gotoXY(DICE_X, DICE_Y + 1);
	cout << str;
}

void Display::drawWinner()
{
	changeColor(BACKGROUND);
	system("CLS");
	for (int i = 0; i < Manager::instance->numbersOfPlayer; i++)
		if(Manager::instance->winner[i])
		{
			gotoXY(0, i);
			cout << "Player" << getFullWidthNum(i + 1) << "Wins!";
		}
		
	system("PAUSE");
}

void Display::drawActions()
{
	changeColor(BACKGROUND);
	for (int i = 0; i < 10; i++)
	{
		gotoXY(ACTION_X + 1, ACTION_Y + i);
		cout << actions[i];
	}	
	chooseAction(0);
}

void Display::eraseActions()
{
	changeColor(BACKGROUND);
	for (int i = 0; i < 10; i++)
	{
		gotoXY(ACTION_X, ACTION_Y + i);
		cout << "             ";
	}
}

void Display::chooseAction(int option)
{
	changeColor(BACKGROUND);
	if (option == 8) {
		hint2("15000元，可往前選擇6格內位置。");
		hint3("用完可繼續操作");
	}	
	else if (option == 9)
		hint2("10000元，可放置路障");
	else
		hint2("");

	gotoXY(ACTION_X, ACTION_Y + option);
	cout << '#';
	gotoXY(ACTION_X, ACTION_Y + option);
}

void Display::eraseChosenAction(int option)
{
	changeColor(BACKGROUND);
	gotoXY(ACTION_X, ACTION_Y + option);
	cout << ' ';
}

void Display::chooseGrid(int pos)
{
	pos += Manager::instance->players[Manager::instance->currentPlayerId].position;
	if (pos >= 28)	pos %= 28;

	hint2("");
	changeColor(Manager::instance->currentPlayerId);
	drawName(place[pos], Grid::instance->grids[pos]->name);
}

void Display::eraseChosenGrid(int pos)
{
	pos += Manager::instance->players[Manager::instance->currentPlayerId].position;
	if (pos >= 28)	pos %= 28;

	changeColor(WHITE);
	drawName(place[pos], Grid::instance->grids[pos]->name);
}

void Display::drawOptionInquire()
{
	eraseActions();
	changeColor(BACKGROUND);
	gotoXY(ACTION_X, ACTION_Y);
	cout << "購買土地?";
	confirm = false;
	changeOption();
}

void Display::changeOption()
{
	confirm = !confirm;
	if (confirm)
	{
		changeColor(BACKGROUND);
		gotoXY(ACTION_X, ACTION_Y + 1);
		cout << "是";
		changeColor(WHITE);
		gotoXY(ACTION_X + 4, ACTION_Y + 1);
		cout << "否";
	}
	else 
	{
		changeColor(WHITE);
		gotoXY(ACTION_X, ACTION_Y + 1);
		cout << "是";
		changeColor(BACKGROUND);
		gotoXY(ACTION_X + 4, ACTION_Y + 1);
		cout << "否";
	}
}

void Display::drawHorizotalSelector()
{
	eraseActions();
	changeColor(BACKGROUND);
	gotoXY(ACTION_X, ACTION_Y);
	cout << "選擇數量";
	changeColor(BG_GRAY);
	gotoXY(ACTION_X, ACTION_Y + 2);
	cout << "←        →";
	drawMony(0);
}

void Display::drawMony(int amount)
{
	changeColor(WHITE);
	gotoXY(ACTION_X + 3, ACTION_Y + 2);
	printf("%6d", amount);
}

void Display::eraseMoney(int amount)
{
	changeColor(BACKGROUND);
	gotoXY(ACTION_X + 3, ACTION_Y + 2);
	cout << "      ";
}

void Display::drawStocks()
{
	changeColor(BACKGROUND);
	for (int i = 0; i < StockMarket::instance->IncNum; i++)
	{
		gotoXY(ACTION_X + 1, ACTION_Y + i);
		cout << StockMarket::instance->IncName[i];
	}
	chooseAction(0);
}

void Display::drawStockPrice(int stock)
{
	int price;
	double baseLine = StockMarket::instance->record[0][stock];
	double unit = (StockMarket::instance->histroicalMaximun[stock] - StockMarket::instance->historicalMinimun[stock]) / 40;
	system("CLS");
	changeColor(BLACK);
	for (int i = 0; i < StockMarket::instance->record.size(); i++)
	{
		price = (StockMarket::instance->record[i][stock] - StockMarket::instance->historicalMinimun[stock]) / unit;
		gotoXY(i, 45 - price);
		if (!i || StockMarket::instance->record[i][stock] == StockMarket::instance->record[i - 1][stock]) {
			changeColor(BACKGROUND);		
			cout << '|';
		}
		else if (StockMarket::instance->record[i][stock] > StockMarket::instance->record[i - 1][stock]){
			changeColor(RED);
			cout << ' ';
		}
		else {
			changeColor(GREEN);
			cout << ' ';
		}
		changeColor(BACKGROUND);
		if(i < 40)	gotoXY(ACTION_X, 5 + i);
		else gotoXY(ACTION_X + 15, i - 35);
		printf("%2d: %9lf", i, StockMarket::instance->record[i][stock]);
		gotoXY(i, 48);
		if (i % 5)	cout << '_';
		else {
			cout << '|';
			gotoXY(i, 47);
			cout << i;
		}
	}
	gotoXY(5, 1);
	cout << StockMarket::instance->IncName[stock];
	gotoXY(5, 2);
	printf("最高: %9lf, 最低: %9lf", StockMarket::instance->histroicalMaximun[stock], StockMarket::instance->historicalMinimun[stock]);
	gotoXY(5, 3);
	printf("現在: %9lf", StockMarket::instance->record[StockMarket::instance->record.size() - 1][stock]);
	gotoXY(ACTION_X, 49);
	system("PAUSE");
	system("CLS");
	drawAll();
}

void Display::drawMenu()
{
	SetConsoleTextAttribute(hConsole, 9); // 黑底藍字	
	gotoXY(47, 9);
	cout << "╔═════╗";
	for (int i = 1; i < 8; i += 2) {
		gotoXY(47, 9 + i);
		cout << "║          ║";
		gotoXY(47, 10 + i);
		cout << "╠═════╣";
	}
	gotoXY(47, 17);
	cout << "╚═════╝";

	for (int i = 0; i < 4; i++)
		eraseOption(i);
	chooseOption(0);
}

void Display::eraseMenu()
{
	changeColor(BACKGROUND);
	for (int i = 0; i < 9; i ++) {
		gotoXY(47, 9 + i);
		cout << "              ";
	}
}

void Display::chooseOption(int option)
{
	changeColor(WHITE);
	gotoXY(50, 10 + (option * 2));
	cout << options[option];
}

void Display::eraseOption(int option)
{
	changeColor(BLACK);
	gotoXY(50, 10 + (option * 2));
	cout << options[option];
}

void Display::drawTitle()
{
	//PlaySound(TEXT("ErikSatie-LePiccadilly.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	mciSendString("open \"hapymaher.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3 repeat", NULL, 0, NULL);
	changeColor(BACKGROUND);
	system("CLS");

	SetConsoleTextAttribute(hConsole, 160); // 綠底黑字
	gotoXY(0, 0);
	cout << "╔═════════╗";
	for (int i = 0; i < 3; i++) {
		gotoXY(0, (i * 4) + 1);
		cout << "║                  ║";
		gotoXY(0, (i * 4) + 2);
		cout << "║                  ║";
		gotoXY(0, (i * 4) + 3);
		cout << "║                  ║";
		gotoXY(0, (i * 4) + 4);
		cout << "╠═════════╣";
	}
	gotoXY(0, 12);
	cout << "╚═════════╝";

	for (int i = 0; i < 3; i++) {
		gotoXY(4, (i * 4) + 2);
		cout << startOptions[i];
	}
	gotoXY(4, 3);
	cout << "───────";
}

void Display::eraseTitle()
{
	changeColor(BACKGROUND);
	for (int i = 0; i < 16; i++) {
		gotoXY(0, i);
		cout << "                       ";
	}
}

void Display::chooseTitleOption(int option)
{
	SetConsoleTextAttribute(hConsole, 160); // 綠底黑字
	gotoXY(4, (option * 4) + 3);
	cout << "───────";
}

void Display::eraseTitleOption(int option)
{
	SetConsoleTextAttribute(hConsole, 160); // 綠底黑字
	gotoXY(4, (option * 4) + 3);
	cout << "              ";
}

void Display::drawChooseNumber()
{
	eraseTitle();
	gotoXY(0, 0);
	cout << "選擇人數: ";
	for (int i = 0; i < 3; i++)
		cout << getFullWidthNum(i + 2);
	chooseNumbersOfPlayer(0);
}

void Display::eraseChooseNumber()
{
	changeColor(BACKGROUND);
	gotoXY(0, 0);
	cout << "                   ";
}

void Display::chooseNumbersOfPlayer(int n)
{
	changeColor(WHITE);
	gotoXY(10 + 2 * n, 0);
	cout << getFullWidthNum(n + 2);
	gotoXY(10 + 2 * n, 0);
}

void Display::eraseChosenNumber(int n)
{
	changeColor(BACKGROUND);
	gotoXY(10 + 2 * n, 0);
	cout << getFullWidthNum(n + 2);
	gotoXY(10 + 2 * n, 0);
}

void Display::empty() {}

void Display::gotoXY(int x, int y)
{
	cursor.X = x;
	cursor.Y = y;
	SetConsoleCursorPosition(hConsole, cursor);
}

void Display::changeColor(int n)
{
	switch (n)
	{
	case 0:	SetConsoleTextAttribute(hConsole, 192); break;
	case 1:	SetConsoleTextAttribute(hConsole, 160); break;
	case 2:	SetConsoleTextAttribute(hConsole, 48); break;
	case 3:	SetConsoleTextAttribute(hConsole, 224); break;
	case WHITE: SetConsoleTextAttribute(hConsole, 240); break; // White
	case BLACK:	SetConsoleTextAttribute(hConsole, 15); break; // Black
	case RED: SetConsoleTextAttribute(hConsole, 64); break; // Red
	case GREEN:	SetConsoleTextAttribute(hConsole, 32); break; // Green
	case BACKGROUND: SetConsoleTextAttribute(hConsole, 176); break; // Aoi-Black
	case BG_GRAY: SetConsoleTextAttribute(hConsole, 184); break; // Aoi-Gray
	}
}

void Display::drawLineO(int x)
{
	gotoXY(SPACE_X + x, 4);
	cout << "  ";
	gotoXY(SPACE_X * 2 + GAP_X + x, 4);
	cout << "  ";
	for (int i = 0; i < 10; i++) {
		gotoXY(3 + x, SPACE_Y + i);
		cout << "  ";
		gotoXY((SPACE_X + GAP_X) * 2 + 3 + x, SPACE_Y + i);
		cout << "  ";
	}
	for (int i = 0; i < 10; i++) {
		gotoXY(3 + x, SPACE_Y * 2 + 10 + i);
		cout << "  ";
		gotoXY((SPACE_X + GAP_X) * 2 + 3 + x, SPACE_Y * 2 + 10 + i);
		cout << "  ";
	}
	gotoXY(SPACE_X + x, SPACE_Y * 2 + 20 + 4);
	cout << "  ";
	gotoXY(SPACE_X * 2 + GAP_X + x, SPACE_Y * 2 + 20 + 4);
	cout << "  ";
}

void Display::drawLineP()
{
	gotoXY(SPACE_X * 9 + GAP_X * 8, 4);
	cout << "  ";
	gotoXY(SPACE_X * 10 + GAP_X * 9, 4);
	cout << "  ";
	for (int i = 0; i < 4; i++) {
		gotoXY((SPACE_X + GAP_X) * 8 + 3, SPACE_Y + (SPACE_Y + GAP_Y) * i);
		cout << "  ";
	}
	for (int i = 0; i < 2; i++) {
		gotoXY((SPACE_X + GAP_X) * 10 + 3, SPACE_Y + (SPACE_Y + GAP_Y) * i);
		cout << "  ";
	}
	gotoXY(SPACE_X * 9 + GAP_X * 8, (SPACE_Y + GAP_Y) * 2 + 4);
	cout << "  ";
	gotoXY(SPACE_X * 10 + GAP_X * 9, (SPACE_Y + GAP_Y) * 2 + 4);
	cout << "  ";
}

void Display::drawFrame(Vector2 origin)
{
	gotoXY(origin.x, origin.y);
	cout << "        ";
	for (int j = 1; j <= 5; j++)
	{
		gotoXY(origin.x, origin.y + j);
		cout << " ";
		gotoXY(origin.x + 7, origin.y + j);
		cout << " ";
	}
	gotoXY(origin.x, origin.y + 6);
	cout << "        ";
	gotoXY(origin.x, origin.y);
}

void Display::drawName(Vector2 origin, string name)
{
	gotoXY(origin.x, origin.y + 7);
	switch (name.length())
	{
	case 4:	cout << "  " << name << "  "; break;
	case 6:	cout << "  " << name; break;
	case 8:	cout << name; break;
	}
}

string Display::getFullWidthNum(int num)
{
	switch (num)
	{
	case 0:	return "０";
	case 1:	return "１";	
	case 2:	return "２";	
	case 3:	return "３";		
	case 4:	return "４";
	case 5:	return "５";
	case 6:	return "６";		
	case 7:	return "７";		
	case 8:	return "８";		
	case 9:	return "９";		
	default:return "";
	}
}