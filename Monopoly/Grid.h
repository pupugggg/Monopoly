#pragma once
#include<iostream>
#include<vector>
//Grid is the basic class of the Grid units. It is used to implement dynamic binding.
class Grid
{
public:
	static Grid* instance;
	std::vector<Grid*> grids;
	Grid* constructorInvoker(int type, std::ifstream& inpStr, int, std::string);
	Grid();
	~Grid();
	virtual void enable();
	 std::string name;
	 int type;
	 int ownerID;
	 int estateLevel;
	 int position;
	 int startPrice;
	 std::vector<int> toll;
	 bool roadBlockIsPlaced;
};

class Chance :public Grid {
public:
	void (*funcPtr)();
	static void goToRndPos();
	static void gainRndMoney();
	static void rest();
	static void freeTravel();
	void selector();
	Chance();
	Chance(int, std::string);
	virtual void enable()override;
};

class StartingPoint :public Grid {
public:
	StartingPoint();
	StartingPoint(int, std::string);
	virtual void enable()override;
};

class Community :public Grid {
public:
	void(*funcPtr)();
	static void goToRndPos();
	static void gainRndMoney();
	static void rest();
	static void freeTravel();
	void selector();
	Community();
	Community(int, std::string);
	virtual void enable()override;
};
