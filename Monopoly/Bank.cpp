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
		Display::hint2("你還沒還清，所以不能借錢");
		return;
	}
	int amount = InputSystem::instance->quitableHorizontalOptionSelector(GameParameter::maximumCredits+1,Display::drawMony,Display::eraseMoney,Display::eraseActions,Display::drawHorizotalSelector);
	if (amount == -1||amount==0)
		return;
	if (amount > (Manager::instance->players[Manager::instance->currentPlayerId].budget / 2))
	{
		Display::hint2("最多只能借出當前預算的一半數量的金錢");
		return;
	}
	else
	{
		Manager::instance->players[Manager::instance->currentPlayerId].budget += amount;
		isSquared[Manager::instance->currentPlayerId]=false;
		arrears[Manager::instance->currentPlayerId] += amount;
		BankStatement[Manager::instance->currentPlayerId] += amount;
		Display::hint2("借出" + std::to_string(amount) + "元");
	}
	
}

void Bank::payBack()
{
	if (arrears[Manager::instance->currentPlayerId] == 0)
		return;
	system("pause");
	Manager::instance->players[Manager::instance->currentPlayerId].budget -= BankStatement[Manager::instance->currentPlayerId] / 10;
	arrears[Manager::instance->currentPlayerId]-= BankStatement[Manager::instance->currentPlayerId] / 10;
	Display::hint2("索取了"+std::to_string(BankStatement[Manager::instance->currentPlayerId] / 10)+"元，目前債務還有"+std::to_string(arrears[Manager::instance->currentPlayerId])+"元");
	if (arrears[Manager::instance->currentPlayerId] <= 0)
	{
		arrears[Manager::instance->currentPlayerId] = 0;
		BankStatement[Manager::instance->currentPlayerId] = 0;
		isSquared[Manager::instance->currentPlayerId] = true;
		Display::hint2("您以還清債務");
	}
}

void Bank::withdraw()
{
	if (deposit[Manager::instance->currentPlayerId] == 0)
	{
		Display::hint2("餘額不足");
		return;
	}
	int withdrawals = InputSystem::instance->quitableHorizontalOptionSelector(deposit[Manager::instance->currentPlayerId]+1, Display::drawMony, Display::eraseMoney, Display::eraseActions, Display::drawHorizotalSelector);
	if (withdrawals == 0 || withdrawals == -1)
		return;
	Display::hint2("提出了"+std::to_string(withdrawals)+"元");
	deposit[Manager::instance->currentPlayerId] -= withdrawals;
	Manager::instance->players[Manager::instance->currentPlayerId].budget += withdrawals;
}

void Bank::depositMoney()
{
	if (Manager::instance->players[Manager::instance->currentPlayerId].budget <= 0)
	{
		Display::hint2("現金不足");
		return;
	}
	int dep = InputSystem::instance->quitableHorizontalOptionSelector(Manager::instance->players[Manager::instance->currentPlayerId].budget + 1, Display::drawMony, Display::eraseMoney, Display::eraseActions, Display::drawHorizotalSelector);
	if (dep == 0 || dep == -1)
		return;
	Display::hint2("存入"+std::to_string(dep)+"元");
	Manager::instance->players[Manager::instance->currentPlayerId].budget -= dep;
	deposit[Manager::instance->currentPlayerId] += dep;
}