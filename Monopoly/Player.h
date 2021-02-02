#pragma once
class Player {
public:
	Player();
	Player(int loc,int budg,int id);
	void move(int);
	int position;
	int budget;
	int playerId;
	bool activated;
	int inactivatedRound;
};