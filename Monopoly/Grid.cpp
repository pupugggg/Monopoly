#include "Grid.h"
#include"Estate.h"
#include<fstream>
#include"Manager.h"
#include<cstdlib>
#include<ctime>
#include"GameParameter.h"
#include"Display.h"
#include"InputSystem.h"
//Starting Point
/********************************************************************************************/

StartingPoint ::StartingPoint(){}

StartingPoint::StartingPoint(int pos, std::string n)
{
	position = pos;
	name = n;
	ownerID = -1;
	estateLevel = -1;
}

void StartingPoint::enable()
{
}

/********************************************************************************************/

//Chance
/********************************************************************************************/

Chance::Chance(){}

Chance::Chance(int pos, std::string n)
{
	position = pos;
	name = n;
	ownerID = -1;
	estateLevel = -1;
}

void Chance::enable()
{
	int rnd = rand() % 10;
	if (rnd < 5)
	{
		selector();
		funcPtr();
	}
	else
	{
		Display::hint2("啥都沒發生");
	}
}

void  Chance::goToRndPos()
{
	Display::hint2("前往隨機位置");
	int rnd= rand() % GameParameter::totalGrid;
	GameParameter::moveTime = 300;
	Display::movePlayer(Manager::instance->currentPlayerId, Manager::instance->players[Manager::instance->currentPlayerId].position, rnd);
	GameParameter::moveTime = 450;
	Manager::instance->players[Manager::instance->currentPlayerId].position = rnd;
}
void  Chance::gainRndMoney()
{
	Display::hint2("隨機獲得金錢");
	Manager::instance->players[Manager::instance->currentPlayerId].budget += rand() % (Manager::instance->players[Manager::instance->currentPlayerId].budget/10) + 1;
}
void  Chance::rest()
{
	Display::hint2("休息一回合");
	Manager::instance->players[Manager::instance->currentPlayerId].activated = false;
}
void  Chance::freeTravel()
{
	//need display
	Display::hint2("自由選擇位置");
	Display::chooseGrid(0);
	int prePos = InputSystem::instance->horizontalOptionSelector(GameParameter::totalGrid, Display::chooseGrid, Display::eraseChosenGrid, Display::empty, Display::empty);
	Display::eraseChosenGrid(prePos);
	Display::movePlayer(Manager::instance->currentPlayerId, Manager::instance->players[Manager::instance->currentPlayerId].position,prePos);
	Manager::instance->players[Manager::instance->currentPlayerId].position = prePos;
}
void Chance::selector()
{
	int rnd = rand() % 4;
	switch (rnd)
	{
	case 0:
		funcPtr = &goToRndPos;
		break;
	case 1:
		funcPtr = &gainRndMoney;
		break;
	case 2:
		funcPtr = &rest;
		break;
	case 3:
		funcPtr = &freeTravel;
		break;
	}
}

/********************************************************************************************/

//Community
/********************************************************************************************/

Community::Community() {}

Community::Community(int pos, std::string n)
{
	position = pos;
	name = n;
	ownerID = -1;
	estateLevel = -1;
}

void Community::enable()
{
	int rnd = rand() % 10;
	if (rnd < 5)
	{
		selector();
		funcPtr();
	}
	else
	{
		Display::hint2("啥都沒發生");
	}
}

void  Community::goToRndPos()
{
	Display::hint2("前往隨機位置");
	int rnd = rand() % GameParameter::totalGrid;
	GameParameter::moveTime = 300;
	Display::movePlayer(Manager::instance->currentPlayerId, Manager::instance->players[Manager::instance->currentPlayerId].position, rnd);
	GameParameter::moveTime = 450;
	Manager::instance->players[Manager::instance->currentPlayerId].position = rnd;
}
void  Community::gainRndMoney()
{
	Display::hint2("隨機獲得金錢");
	Manager::instance->players[Manager::instance->currentPlayerId].budget += rand() % (Manager::instance->players[Manager::instance->currentPlayerId].budget / 10) + 1;
}
void  Community::rest()
{
	Display::hint2("休息一回合");
	Manager::instance->players[Manager::instance->currentPlayerId].activated = false;
}
void  Community::freeTravel()
{
	Display::hint2("自由選擇位置");
	Display::chooseGrid(0);
	int prePos = InputSystem::instance->horizontalOptionSelector(GameParameter::totalGrid, Display::chooseGrid, Display::eraseChosenGrid, Display::empty, Display::empty);
	Display::eraseChosenGrid(prePos);
	Display::movePlayer(Manager::instance->currentPlayerId, Manager::instance->players[Manager::instance->currentPlayerId].position, prePos);
	Manager::instance->players[Manager::instance->currentPlayerId].position = prePos;
}

void Community::selector()
{
	int rnd = rand() % 4;
	switch (rnd)
	{
	case 0:
		funcPtr = &goToRndPos;
		break;
	case 1:
		funcPtr = &gainRndMoney;
		break;
	case 2:
		funcPtr = &rest;
		break;
	case 3:
		funcPtr = &freeTravel;
		break;
	}
}

/********************************************************************************************/

//Grid
/********************************************************************************************/

Grid* Grid::instance;

Grid::Grid() {}

Grid::~Grid() {}

Grid* Grid::constructorInvoker(int type, std::ifstream& inpStr, int pos, std::string n)
{
	switch (type)
	{
	case 0:
		return new StartingPoint(pos,n);
		break;
	case 1:
		return new Estate(inpStr,pos,n);
		break;
	case -1:
		return new Community(pos,n);
		break;
	case -2:
		return new Chance(pos,n);
		break;
	default:
		break;
	}
	return NULL;
}
void Grid::enable() {}
/********************************************************************************************/