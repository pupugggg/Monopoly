#include"Player.h"
#include"GameParameter.h"
#include"Display.h"
Player::Player() :playerId(-1),position(0),budget(0),activated(true){}

Player::Player(int id,int loc, int budg): playerId(id), position(loc), budget(budg),activated(true),inactivatedRound(0){}

void Player::move(int distance)
{
	if ((position + distance) >= GameParameter::totalGrid)
	{

		for (int i = 1; i <= distance; i++)
		{
			if (position + i < GameParameter::totalGrid)
			{
				if (Grid::instance->grids[position + i]->roadBlockIsPlaced)
				{
					
					Grid::instance->grids[position + i]->roadBlockIsPlaced = false;
					position = position + i;
					Display::hint3("�Q�����d�I");
					return;
				}
			}
			else
			{
				if (Grid::instance->grids[(position + i) % GameParameter::totalGrid]->roadBlockIsPlaced)
				{
					
					Grid::instance->grids[(position + i) % GameParameter::totalGrid]->roadBlockIsPlaced = false;
					position = (position + i) % GameParameter::totalGrid;
					Display::hint2("�V�L�_�I�A��o" + std::to_string(GameParameter::goPrice) + "��");
					Display::hint3("�Q�����d�I");
					budget += GameParameter::goPrice;
					return;
				}
			}
		}

		position = (position + distance) % GameParameter::totalGrid;	
		Display::hint2("�V�L�_�I�A��o"+std::to_string(GameParameter::goPrice)+"��");
		budget += GameParameter::goPrice;

	}
	else if (distance < 0)
	{
		distance = -distance;
		distance %= GameParameter::totalGrid;
		position = (position + (GameParameter::totalGrid - distance)) % GameParameter::totalGrid;
	}
	else
	{
		
		for (int i = 1; i <= distance; i++)
		{
			if (Grid::instance->grids[position + i]->roadBlockIsPlaced)
			{
			
				Grid::instance->grids[position + i]->roadBlockIsPlaced = false;
				position = position + i;
				Display::hint2("�Q�����d�I");
				return;
			}
		}
		position += distance;
	}
}