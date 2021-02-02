#include "StockMarket.h"
#include"Manager.h"
#include"InputSystem.h"
#include "Display.h"
#include<string>
#include<fstream>
StockMarket* StockMarket::instance;

void StockMarket::init() 
{
	std::ifstream inputStream("stock.txt");
	std::string buffer;
	inputStream >> IncNum;
	for (int i = 0; i < IncNum; i++)
	{
		inputStream>>buffer;
		IncName.push_back(buffer);
		inputStream >> buffer;
		stockPrice.push_back(stod(buffer));
	}
	std::vector<int> tmp;
	for (int i = 0; i < Manager::instance->numbersOfPlayer; i++)
	{
		inputStream >> buffer;
		tmp.clear();
		for (int j = 0; j < IncNum; j++)
		{
			inputStream >> buffer;
			tmp.push_back(stoi(buffer));
		}
		hold.push_back(tmp);
	}
	inputStream.close();
	std::vector<double> temp;
	for (int j = 0; j < IncNum; j++)
	{
		temp.push_back(stockPrice[j]);
		historicalMinimun.push_back(stockPrice[j]);
		histroicalMaximun.push_back(stockPrice[j]);
	}
	record.push_back(temp);
}

void StockMarket::newGameInit()
{
	std::ifstream inputStream("NewGamestock.txt");
	std::string buffer;
	inputStream >> IncNum;
	for (int i = 0; i < IncNum; i++)
	{
		inputStream >> buffer;
		IncName.push_back(buffer);
		inputStream >> buffer;
		stockPrice.push_back(stod(buffer));
	}
	std::vector<int> tmp;
	for (int i = 0; i < Manager::instance->numbersOfPlayer; i++)
	{
		inputStream >> buffer;
		tmp.clear();
		for (int j = 0; j < IncNum; j++)
		{
			inputStream >> buffer;
			tmp.push_back(stoi(buffer));
		}
		hold.push_back(tmp);
	}
	inputStream.close();
	std::vector<double> temp;
	for (int j = 0; j < IncNum; j++)
	{
		temp.push_back(stockPrice[j]);
		historicalMinimun.push_back(stockPrice[j]);
		histroicalMaximun.push_back(stockPrice[j]);
	}
	record.push_back(temp);
}

StockMarket::StockMarket()
{
}
StockMarket::~StockMarket()
{
}

void StockMarket::priceFloating()
{

	for (int i = 0; i < IncNum; i++)
	{
		double rnd = 1;
		while(rnd>0.024)
			rnd = (double)rand() / RAND_MAX;
		int chance = rand() % 2;
		floatingRate.push_back(rnd*(chance ? 1 : -1));
	}
	
	for (int i = 0; i < IncNum; i++)
	{
		stockPrice[i] = stockPrice[i] * (1 + floatingRate[i]);
		if (stockPrice[i] > histroicalMaximun[i])
			histroicalMaximun[i] = stockPrice[i];
		if (stockPrice[i] < historicalMinimun[i])
			historicalMinimun[i] = stockPrice[i];
	}
	floatingRate.clear();
	std::vector<double> tmp;
	for (int j = 0; j < IncNum; j++)
	{
		tmp.push_back(stockPrice[j]);
	}
	record.push_back(tmp);
}

void StockMarket::buy()
{
	int selection = InputSystem::instance->quitableVerticalOptionSelector(IncNum, &Display::chooseAction, Display::eraseChosenAction, &Display::eraseActions, &Display::drawStocks);
	if (selection == -1)
		return;
	int amount = InputSystem::instance->quitableHorizontalOptionSelector(11, &Display::drawMony, &Display::eraseMoney, &Display::eraseActions, &Display::drawHorizotalSelector);
	if (amount == -1 || amount == 0)
		return;
	if (Bank::instance->deposit[Manager::instance->currentPlayerId] >= amount * 1000 * stockPrice[selection])
	{
		hold[Manager::instance->currentPlayerId][selection] += amount;
		Bank::instance->deposit[Manager::instance->currentPlayerId] -= amount * 1000 * stockPrice[selection];
		Display::hint2("成功買入" + IncName[selection] + std::to_string(amount) + "張");
		system("pause");
	}
	else
	{
		Display::hint2("存款不足");
		system("pause");
	}
}

void StockMarket::sell()
{
	int selection = InputSystem::instance->quitableVerticalOptionSelector(IncNum, &Display::chooseAction, Display::eraseChosenAction, &Display::eraseActions, &Display::drawStocks);
	if (selection == -1)
		return;
	int amount = InputSystem::instance->quitableHorizontalOptionSelector(11, &Display::drawMony, &Display::eraseMoney, &Display::eraseActions, &Display::drawHorizotalSelector);
	if (amount == -1 || amount == 0)
		return;
	if (hold[Manager::instance->currentPlayerId][selection] >= amount)
	{
		hold[Manager::instance->currentPlayerId][selection] -= amount;
		Bank::instance->deposit[Manager::instance->currentPlayerId] += amount * 1000 * stockPrice[selection];
		Display::hint2("成功賣出" + IncName[selection] + std::to_string(amount) + "張");
		system("pause");
	}
	else
	{
		Display::hint2("沒這麼多張股票");
		system("pause");
	}
}