#pragma once
#include<vector>
class StockMarket
{
public:
	static StockMarket* instance;
	std::vector<std::string> IncName;
	std::vector<double> stockPrice;
	std::vector<std::vector<int>> hold;
	std::vector<double> floatingRate;
	std::vector<std::vector<double>> record;
	std::vector<double> histroicalMaximun;
	std::vector<double> historicalMinimun;
	void init();
	void newGameInit();
	void priceFloating();
	void buy();
	void sell();
	int IncNum;
	StockMarket();
	~StockMarket();
};

