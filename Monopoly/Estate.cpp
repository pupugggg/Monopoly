#include"Estate.h"
#include"InputSystem.h"
#include"Manager.h"
#include"GameParameter.h"
#include"Display.h"
Estate::Estate()
{

}

Estate::Estate(std::ifstream& inpStr,int pos, std::string n)
{
	int temp;
	name = n;
	position = pos;
	ownerID = -1;
	estateLevel = -1;
	inpStr >> startPrice;
	for (int i = 0; i < 4; i++)
	{
		inpStr >> temp;
		toll.push_back(temp);
	}
}

void Estate::enable()
{
	if (Manager::instance->currentPlayerId == ownerID || ownerID==GameParameter::inActivated)
	{
		//level up
		if (!(estateLevel == GameParameter::maxEstateLevel))
		{
			if (InputSystem::instance->OptionInquire(Display::changeOption,Display::drawOptionInquire,Display::eraseActions))
			{
				if (Manager::instance->players[Manager::instance->currentPlayerId].budget >= startPrice)
				{
					Manager::instance->players[Manager::instance->currentPlayerId].budget -= startPrice;
					estateLevel++;
					ownerID = Manager::instance->currentPlayerId;
					Display::drawHouse(ownerID, Manager::instance->players[ownerID].position, estateLevel);
					Display::hint2("購買成功");
				}
				else
				{
					Display::hint2("金錢不足");
				}
			}
		}
		else 
		{
			Display::hint2("地產以達到最高等級，因此無法升級");
		}
	}
	else
	{
	
		//show toll info
		Manager::instance->players[Manager::instance->currentPlayerId].budget -= toll[estateLevel];
		Manager::instance->players[ownerID].budget+= toll[estateLevel];
		Display::hint2("您進入了玩家" + std::to_string(Grid::instance->grids[Manager::instance->players[Manager::instance->currentPlayerId].position]->ownerID + 1) + "的領地，收取過路費"+ std::to_string(toll[estateLevel]) +"元");
	}
}