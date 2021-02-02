#pragma once
#include<vector>
class Bank
{
public:
	static Bank* instance;
	std::vector<bool> isSquared;
	std::vector<int> arrears;
	std::vector<int> BankStatement;
	std::vector<int> deposit;
	void credits();
	void payBack();
	void depositMoney();
	void withdraw();
	Bank();
	~Bank();
};

