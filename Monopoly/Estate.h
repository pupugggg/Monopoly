#pragma once
#include<fstream>
#include<vector>
#include"Grid.h"
class Estate :public Grid{
public:
	Estate();
	Estate(std::ifstream&,int, std::string);
	void enable()override;
};