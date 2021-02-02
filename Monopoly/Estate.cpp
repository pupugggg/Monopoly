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
					Display::hint2("�ʶR���\");
				}
				else
				{
					Display::hint2("��������");
				}
			}
		}
		else 
		{
			Display::hint2("�a���H�F��̰����šA�]���L�k�ɯ�");
		}
	}
	else
	{
	
		//show toll info
		Manager::instance->players[Manager::instance->currentPlayerId].budget -= toll[estateLevel];
		Manager::instance->players[ownerID].budget+= toll[estateLevel];
		Display::hint2("�z�i�J�F���a" + std::to_string(Grid::instance->grids[Manager::instance->players[Manager::instance->currentPlayerId].position]->ownerID + 1) + "����a�A�����L���O"+ std::to_string(toll[estateLevel]) +"��");
	}
}