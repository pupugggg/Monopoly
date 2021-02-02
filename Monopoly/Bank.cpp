#include "Bank.h"
#include"Manager.h"
#include"InputSystem.h"
#include"GameParameter.h"
#include"Display.h"
Bank* Bank::instance;

Bank::Bank()
{
	for (int i = 0; i < Manager::instance->numbersOfPlayer; i++)
	{
		isSquared.push_back(true);
		arrears.push_back(0);
		BankStatement.push_back(0);
		deposit.push_back(0);
	}
}


Bank::~Bank()
{
}

void Bank::credits()
{
	if (!isSquared[Manager::instance->currentPlayerId])
	{
		Display::hint2("�A�٨S�ٲM�A�ҥH����ɿ�");
		return;
	}
	int amount = InputSystem::instance->quitableHorizontalOptionSelector(GameParameter::maximumCredits+1,Display::drawMony,Display::eraseMoney,Display::eraseActions,Display::drawHorizotalSelector);
	if (amount == -1||amount==0)
		return;
	if (amount > (Manager::instance->players[Manager::instance->currentPlayerId].budget / 2))
	{
		Display::hint2("�̦h�u��ɥX��e�w�⪺�@�b�ƶq������");
		return;
	}
	else
	{
		Manager::instance->players[Manager::instance->currentPlayerId].budget += amount;
		isSquared[Manager::instance->currentPlayerId]=false;
		arrears[Manager::instance->currentPlayerId] += amount;
		BankStatement[Manager::instance->currentPlayerId] += amount;
		Display::hint2("�ɥX" + std::to_string(amount) + "��");
	}
	
}

void Bank::payBack()
{
	if (arrears[Manager::instance->currentPlayerId] == 0)
		return;
	system("pause");
	Manager::instance->players[Manager::instance->currentPlayerId].budget -= BankStatement[Manager::instance->currentPlayerId] / 10;
	arrears[Manager::instance->currentPlayerId]-= BankStatement[Manager::instance->currentPlayerId] / 10;
	Display::hint2("�����F"+std::to_string(BankStatement[Manager::instance->currentPlayerId] / 10)+"���A�ثe�Ű��٦�"+std::to_string(arrears[Manager::instance->currentPlayerId])+"��");
	if (arrears[Manager::instance->currentPlayerId] <= 0)
	{
		arrears[Manager::instance->currentPlayerId] = 0;
		BankStatement[Manager::instance->currentPlayerId] = 0;
		isSquared[Manager::instance->currentPlayerId] = true;
		Display::hint2("�z�H�ٲM�Ű�");
	}
}

void Bank::withdraw()
{
	if (deposit[Manager::instance->currentPlayerId] == 0)
	{
		Display::hint2("�l�B����");
		return;
	}
	int withdrawals = InputSystem::instance->quitableHorizontalOptionSelector(deposit[Manager::instance->currentPlayerId]+1, Display::drawMony, Display::eraseMoney, Display::eraseActions, Display::drawHorizotalSelector);
	if (withdrawals == 0 || withdrawals == -1)
		return;
	Display::hint2("���X�F"+std::to_string(withdrawals)+"��");
	deposit[Manager::instance->currentPlayerId] -= withdrawals;
	Manager::instance->players[Manager::instance->currentPlayerId].budget += withdrawals;
}

void Bank::depositMoney()
{
	if (Manager::instance->players[Manager::instance->currentPlayerId].budget <= 0)
	{
		Display::hint2("�{������");
		return;
	}
	int dep = InputSystem::instance->quitableHorizontalOptionSelector(Manager::instance->players[Manager::instance->currentPlayerId].budget + 1, Display::drawMony, Display::eraseMoney, Display::eraseActions, Display::drawHorizotalSelector);
	if (dep == 0 || dep == -1)
		return;
	Display::hint2("�s�J"+std::to_string(dep)+"��");
	Manager::instance->players[Manager::instance->currentPlayerId].budget -= dep;
	deposit[Manager::instance->currentPlayerId] += dep;
}