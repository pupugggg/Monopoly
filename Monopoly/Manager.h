#pragma once
#include"Player.h"
#include<ctime>
#include<vector>
class Manager {
public:
	Manager();
	static Manager* instance;
	void init();
	void newGameInit();
	bool selectNumOfPlayer();
	void start();
	void update();
	void winConditionCheck();
	void rolldiceAndMove();
	void nextRound();
	void selection();
	void gridAction();
	void stockPrice();
	void openMenu();
	void title();
	void save();
	void load();
	void quit();
	void superDice();
	void roadBlock();
	int assetEval(int);
	int numbersOfPlayer;
	int remain;
	int totalRound;
	int currentPlayerId;
	std::vector<bool> winner;
	int cycleCounter;
	std::vector<bool> bankrupt;
	std::string mapName;
	std::vector<Player> players;
	clock_t startT, endT;
	bool gameOver;
};