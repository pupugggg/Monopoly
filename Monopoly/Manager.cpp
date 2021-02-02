#include"Manager.h"
#include"GameParameter.h"
#include<fstream>
#include<string>
#include<sstream>
#include"Grid.h"
#include"Bank.h"
#include"InputSystem.h"
#include"Display.h"
#include"StockMarket.h"
using namespace std;

Manager* Manager::instance;

Manager::Manager() {
	Grid::instance = new Grid();
	InputSystem::instance = new InputSystem();
	Display::Init();
	startT = clock();
	endT = clock();
	cycleCounter = 0;
}

void Manager::init()
{
	winner.clear();
	bankrupt.clear();
	players.clear();
	for (int i = 0; i < Grid::instance->grids.size(); i++)
	{
		Grid::instance->grids[i]->toll.clear();
		delete Grid::instance->grids[i];
		Grid::instance->grids.clear();
	}
	gameOver = false;
	ifstream inputStream("basemap.txt");
	if (inputStream.fail())
		exit(EXIT_FAILURE);
	string nameTmp;
	string posTmp;
	string typeTmp;
	inputStream >> Manager::instance->mapName;
	inputStream >> Manager::instance->remain;
	totalRound = remain + 1;
	inputStream >> Manager::instance->numbersOfPlayer;
	for (int i = 0; i < GameParameter::totalGrid; i++)
	{
		inputStream >> posTmp;
		inputStream >> nameTmp;
		inputStream >> typeTmp;
		Grid::instance->grids.push_back(Grid::instance->constructorInvoker(stoi(typeTmp), inputStream, stoi(posTmp), nameTmp));
		Grid::instance->grids[i]->type = stoi(typeTmp);
		Grid::instance->grids[i]->roadBlockIsPlaced = false;
	}
	string sTmp;
	inputStream >> sTmp;
	inputStream >> sTmp;
	currentPlayerId = stoi(sTmp);
	//need to be concered
	inputStream.seekg(sizeof(char)*2, inputStream.cur);
	for (int i = 0; i < numbersOfPlayer; i++)
	{
		getline(inputStream, sTmp);
		vector<int> iTmp;
		istringstream s(sTmp);
		while (getline(s, sTmp,' '))
		{
			iTmp.push_back(stoi(sTmp));
		}
		players.push_back(Player(iTmp[0], iTmp[1], iTmp[2]));
		for (int j = 3; j < iTmp.size(); j += 2)
		{
			Grid::instance->grids[iTmp[j]]->ownerID = i;
			Grid::instance->grids[iTmp[j]]->estateLevel = iTmp[j + 1];
		}
		iTmp.clear();
		bankrupt.push_back(false);
	}
	inputStream.close();
	for (int i = 0; i < numbersOfPlayer; i++)
		winner.push_back(false);
	Bank::instance = new Bank();
	StockMarket::instance = new StockMarket();
	StockMarket::instance->init();
}

void Manager::newGameInit()
{
	winner.clear();
	bankrupt.clear();
	players.clear();
	for (int i = 0; i < Grid::instance->grids.size(); i++)
	{
		Grid::instance->grids[i]->toll.clear();
		delete Grid::instance->grids[i];
		Grid::instance->grids.clear();
	}
	gameOver = false;
	ifstream inputStream("NewGamebasemap.txt");
	if (inputStream.fail())
		exit(EXIT_FAILURE);
	string nameTmp;
	string posTmp;
	string typeTmp;
	inputStream >> Manager::instance->mapName;
	inputStream >> Manager::instance->remain;
	totalRound = remain + 1;
	inputStream >> Manager::instance->numbersOfPlayer;
	if (!selectNumOfPlayer())
	{
		inputStream.close();
		return;
	}
	for (int i = 0; i < GameParameter::totalGrid; i++)
	{
		inputStream >> posTmp;
		inputStream >> nameTmp;
		inputStream >> typeTmp;
		Grid::instance->grids.push_back(Grid::instance->constructorInvoker(stoi(typeTmp), inputStream, stoi(posTmp), nameTmp));
		Grid::instance->grids[i]->type = stoi(typeTmp);
		Grid::instance->grids[i]->roadBlockIsPlaced = false;
	}
	string sTmp;
	inputStream >> sTmp;
	inputStream >> sTmp;
	currentPlayerId = stoi(sTmp);
	//need to be concered
	inputStream.seekg(sizeof(char) * 2, inputStream.cur);
	for (int i = 0; i < numbersOfPlayer; i++)
	{
		getline(inputStream, sTmp);
		vector<int> iTmp;
		istringstream s(sTmp);
		while (getline(s, sTmp, ' '))
		{
			iTmp.push_back(stoi(sTmp));
		}
		players.push_back(Player(iTmp[0], iTmp[1], iTmp[2]));
		for (int j = 3; j < iTmp.size(); j += 2)
		{
			Grid::instance->grids[iTmp[j]]->ownerID = i;
			Grid::instance->grids[iTmp[j]]->estateLevel = iTmp[j + 1];
		}
		iTmp.clear();
		bankrupt.push_back(false);
	}
	inputStream.close();
	for (int i = 0; i < numbersOfPlayer; i++)
		winner.push_back(false);
	Bank::instance = new Bank();
	StockMarket::instance = new StockMarket();
	StockMarket::instance->newGameInit();
}

bool Manager::selectNumOfPlayer()
{
	int option = InputSystem::instance->quitableHorizontalOptionSelector(GameParameter::MaximumPlayer - 1, &Display::chooseNumbersOfPlayer, &Display::eraseChosenNumber, &Display::eraseChooseNumber, &Display::drawChooseNumber) + 1;
	if (!option)
		return false;
	else
	{
		Manager::instance->numbersOfPlayer = option + 1;
		return true;
	}
}

void Manager::start()
{
	cycleCounter = currentPlayerId;
	Display::drawAll();
	while (!gameOver)
	{
		double timeFrame = (double)(endT - startT)/CLOCKS_PER_SEC;
		if (timeFrame >= GameParameter::frameRate)
		{
			update();
			startT = clock();
		}
		endT = clock();
	}
}

void Manager::update()
{
	winConditionCheck();
	if (!gameOver)
	{
		selection();
		gridAction();
		Bank::instance->payBack();
		nextRound();
	}
	else
		nextRound();
	
	
}

void Manager::gridAction()
{
	if (!(players[currentPlayerId].activated)||bankrupt[currentPlayerId])
		return;
	Grid::instance->grids[players[currentPlayerId].position]->enable();
}

void Manager::rolldiceAndMove()
{
	
	if (!(players[currentPlayerId].activated) || bankrupt[currentPlayerId])
	{
		return;
	}
	//Display::hint("請擲骰子");
	//system("pause");
	int origin = players[currentPlayerId].position;
	int distance = rand() % 6 + 1;
	players[currentPlayerId].move(distance);
	Display::hint("您骰到了" + to_string(distance) + "點");
	Display::movePlayer(currentPlayerId, origin, players[currentPlayerId].position);
}

void Manager::nextRound()
{
	if (gameOver)
		return;
	if (!(players[currentPlayerId].activated))
	{
		players[currentPlayerId].inactivatedRound++;
	}
	else if (players[currentPlayerId].inactivatedRound > 1)
	{
		players[currentPlayerId].inactivatedRound = 0;
		players[currentPlayerId].activated = true;
	}
	Display::drawPlayerStatus(currentPlayerId);
	Display::hint3("");
	system("pause");
	Display::hint("");
	Display::hint3("");
	Display::hint2("");
	currentPlayerId++;
	if (currentPlayerId >= numbersOfPlayer)
		currentPlayerId = 0;
	StockMarket::instance->priceFloating();
	cycleCounter++;
	if (cycleCounter >=numbersOfPlayer )
	{
		cycleCounter = 0;
		remain--;
	}
	Display::drawPlayerStatus(currentPlayerId);
}

void Manager::winConditionCheck()
{
	if (!bankrupt[currentPlayerId] && assetEval(currentPlayerId) < 0)
		bankrupt[currentPlayerId] = true;
	if (remain <= 0)
	{
		gameOver = true;
		int candidate=0;
		for (int i = 0; i < numbersOfPlayer; i++)
		{
			if (assetEval(i)>assetEval(candidate))
				candidate = i;
		}
		for (int i = 0; i < numbersOfPlayer; i++)
		{
			if (assetEval(i)==assetEval(candidate))
				winner[i] = true;
		}
		Display::drawWinner();
	}
	else
	{
		int count = 0, candidate = -1;
		for (int i = 0; i < numbersOfPlayer; i++)
		{
			if (bankrupt[i])
				count++;
			else
				candidate = i;
		}
		if (count == numbersOfPlayer - 1)
		{
			winner[candidate]=true;
			gameOver = true;
			Display::drawWinner();
		}
	}
}

int Manager::assetEval(int id)
{
	int totalAsset = 0;
	for (int i = 0; i < GameParameter::totalGrid; i++)
	{
		if (Grid::instance->grids[i]->type == 1 && Grid::instance->grids[i]->ownerID == id)
		{
			totalAsset += Grid::instance->grids[i]->startPrice*(Grid::instance->grids[i]->estateLevel + 1);
		}
	}
		totalAsset += Bank::instance->deposit[id];
	

		for (int i = 0; i < StockMarket::instance->IncNum; i++)
		{
			totalAsset += StockMarket::instance->stockPrice[i] * StockMarket::instance->hold[id][i] * 1000;
		}
		totalAsset += players[id].budget;
	return totalAsset;
}

void Manager::selection()
{
	if (bankrupt[currentPlayerId]|| !(players[currentPlayerId].activated))
	{
		Display::hint("玩家無法操作");
		return;
	}
	int option = -1;
	while (option)
	{
		option = InputSystem::instance->verticalOptionSelector(10, Display::chooseAction, Display::eraseChosenAction, Display::eraseActions, Display::drawActions);
		switch (option)
		{
		case 0:
			rolldiceAndMove();
			break;
		case 1:
			stockPrice();
			break;
		case 2:
			StockMarket::instance->buy();
			break;
		case 3:
			StockMarket::instance->sell();
			break;
		case 4:
			Bank::instance->credits();
			break;
		case 5:
			Bank::instance->depositMoney();
			break;
		case 6:
			Bank::instance->withdraw();
			break;
		case 7:
			openMenu();
			break;
		case 8:
			superDice();
			break;
		case 9:
			roadBlock();
			break;
		}
		Display::drawPlayerStatus(currentPlayerId);
	}
	
}

void Manager::stockPrice()
{
	int selection = InputSystem::instance->quitableVerticalOptionSelector(StockMarket::instance->IncNum, &Display::chooseAction, Display::eraseChosenAction, &Display::eraseActions, &Display::drawStocks);
	if (selection == -1) 
		return;

	Display::drawStockPrice(selection);
}

void Manager::openMenu()
{
	int option = InputSystem::instance->quitableVerticalOptionSelector(4, &Display::chooseOption, &Display::eraseOption, &Display::eraseMenu, &Display::drawMenu);
	switch (option)
	{
	case 0:
		title();
		break;
	case 1:
		save();
		break;
	case 2:
		load();
		break;
	case 3:
		quit();
		break;
	default:
		break;
	}
}
void Manager::title()
{
	int option=0;
	do
	{
		int option = InputSystem::instance->verticalOptionSelector(3, &Display::chooseTitleOption, &Display::eraseTitleOption, &Display::eraseTitle, &Display::drawTitle);
		switch (option)
		{
		case 0:
			newGameInit();
			start();
			break;
		case 1:
			load();
			break;
		case 2:
			quit();
			break;
		default:
			break;
		}
	} while (option != 2);
}
void Manager::save()
{
	ofstream outputStream("basemap.txt");
	outputStream << Manager::instance->mapName + " " + to_string(remain) + " " + to_string(numbersOfPlayer) << endl;
	for (int i = 0; i < GameParameter::totalGrid; i++)
	{
		if (Grid::instance->grids[i]->position < 10)
			outputStream << "0" + to_string(Grid::instance->grids[i]->position)+" ";
		else
			outputStream<< to_string(Grid::instance->grids[i]->position)+" ";
		outputStream <<Grid::instance->grids[i]->name+ " " + to_string(Grid::instance->grids[i]->type);
		if (Grid::instance->grids[i]->type == 1)
		{
			outputStream << " "+to_string(Grid::instance->grids[i]->startPrice);
			for (int j = 0; j < Grid::instance->grids[i]->toll.size(); j++)
				outputStream << " " + to_string(Grid::instance->grids[i]->toll[j]);
		}
		outputStream << endl;
	}
	outputStream << "playerstate " + to_string(currentPlayerId) << endl;
	for (int i = 0; i < numbersOfPlayer; i++)
	{
		outputStream << to_string(i) + " " + to_string(players[i].position) + " " + to_string(players[i].budget);
		for (int j = 0; j < GameParameter::totalGrid; j++)
		{
			if (Grid::instance->grids[j]->ownerID == i)
				outputStream << " " + to_string(Grid::instance->grids[j]->position) + " " + to_string(Grid::instance->grids[j]->estateLevel);
		}
		outputStream << endl;
	}
	outputStream.flush();
	outputStream.close();
}
void Manager::load()
{
	init();
	start();
}
void Manager::quit()
{
	exit(EXIT_SUCCESS);
}

void Manager::roadBlock()
{
	if (players[currentPlayerId].budget < GameParameter::roadBlockCost)
	{
		Display::hint2("窮B還想買路障阿");
		system("pause");
		return;
	}
	if (Grid::instance->grids[players[currentPlayerId].position]->roadBlockIsPlaced)
	{
		Display::hint2("此處已放置過路障");
		system("pause");
		return;
	}
	Grid::instance->grids[players[currentPlayerId].position]->roadBlockIsPlaced = true;
	players[currentPlayerId].budget -= GameParameter::roadBlockCost;
	Display::hint2("路障放置成功");
	system("pause");
}

void Manager::superDice()
{
	if (players[currentPlayerId].budget < GameParameter::roadBlockCost)
	{
		Display::hint2("金錢不足，無法購買萬能骰子");
		system("pause");
		return;
	}

	Display::chooseGrid(0);
	int pos = InputSystem::instance->quitableHorizontalOptionSelector(7, &Display::chooseGrid, &Display::eraseChosenGrid, &Display::empty, &Display::empty);
	if (pos == -1 || pos == 0) {
		Display::eraseChosenGrid(0);
		return;
	}
		
	Display::eraseChosenGrid(pos);
	Display::movePlayer(currentPlayerId, players[currentPlayerId].position, players[currentPlayerId].position + pos);
	players[currentPlayerId].budget -= GameParameter::superDiceCost;
	players[currentPlayerId].move(pos);
}