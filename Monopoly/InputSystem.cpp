#include"InputSystem.h"
#include<conio.h>
#include"GameParameter.h"
#include"Display.h"
using namespace std;

InputSystem* InputSystem::instance;

void InputSystem::keyUpdate()
{
	for (int i = 0; i < 6; i++)
		keyState[i] = false;
	char tmp = _getch();
	if (tmp == 27)
	{
		keyState[vaildiInput::ESC] = true;
#ifndef GET_CH_ONCE
		tmp = _getch();
#endif 
	}
	else if (tmp == 13)
	{
		keyState[vaildiInput::enter] = true;
#ifndef GET_CH_ONCE
		tmp = _getch();
#endif 
	}
	else
	{
		switch (tmp = _getch())
		{
		case 72:
			//up
			keyState[vaildiInput::upArrow] = true;
			break;
			case 80:
			//down
			keyState[vaildiInput::downArrow] = true;
			break;
			case 77:
			//right
			keyState[vaildiInput::rightArrow] = true;
			break;
			case 75:
			//left
			keyState[vaildiInput::leftArrow] = true;
			break;
			}
		}
}

bool InputSystem::OptionInquire(void(*drawOption)(),void (*drawMenu)(),void(*eraseMenu)())
{
	drawMenu();
	bool selection=false;
	
	do
	{
		keyUpdate();
		if (keyState[vaildiInput::leftArrow] || keyState[vaildiInput::rightArrow])
		{
			selection = !selection;
			drawOption();
		}
	}while (!keyState[vaildiInput::enter] && !keyState[vaildiInput::ESC]);
	eraseMenu();
	if (keyState[vaildiInput::ESC])
	{
		return false;
	}
	else
		return selection;
}

int InputSystem::quitableVerticalOptionSelector(int numberOfOptions, void(*drawOption)(int), void(*eraseOption)(int), void(*eraseMenu)(), void(*drawMenu)())
{
	drawMenu();
	int decision = 0;
	do
	{
		keyUpdate();
		if (keyState[vaildiInput::downArrow])
		{
			eraseOption(decision);
			decision++;
			
		}
		else if (keyState[vaildiInput::upArrow])
		{
			eraseOption(decision);
			decision--;
			
		}
		if (decision < 0)
			decision = numberOfOptions - 1;
		if (decision >= numberOfOptions)
			decision = 0;
		drawOption(decision);
	} while (!keyState[vaildiInput::enter] && !keyState[vaildiInput::ESC]);
	eraseMenu();
	if (keyState[vaildiInput::ESC])
		return -1;
	else
		return decision;
}

int InputSystem::quitableHorizontalOptionSelector(int numberOfOptions, void(*drawOption)(int), void(*eraseOption)(int), void(*eraseMenu)(), void(*drawMenu)())
{
	drawMenu();
	int decision=0;
	do
	{
		keyUpdate();
		if (keyState[vaildiInput::leftArrow])
		{
			eraseOption(decision);
			decision--;
			
		}
		else if (keyState[vaildiInput::rightArrow])
		{
			eraseOption(decision);
			decision++;
			
		}
		if (decision < 0)
			decision = numberOfOptions - 1;
		if (decision >= numberOfOptions)
			decision = 0;
		drawOption(decision);
	}while (!keyState[vaildiInput::enter] && !keyState[vaildiInput::ESC]);
	eraseMenu();
	if (keyState[vaildiInput::ESC])
		return -1;
	else
		return decision;
}

int InputSystem::verticalOptionSelector(int numberOfOptions, void(*drawOption)(int), void(*eraseOption)(int), void(*eraseMenu)(), void(*drawMenu)())
{
	drawMenu();
	int decision = 0;
	do
	{
		keyUpdate();
		if (keyState[vaildiInput::downArrow])
		{
			eraseOption(decision);
			decision++;
			
		}
		else if (keyState[vaildiInput::upArrow])
		{
			eraseOption(decision);
			decision--;
			
		}
		if (decision < 0)
			decision = numberOfOptions - 1;
		if (decision >= numberOfOptions)
			decision = 0;
		drawOption(decision);
	} while (!keyState[vaildiInput::enter]);
	eraseMenu();
		return decision;
}

int InputSystem::horizontalOptionSelector(int numberOfOptions, void(*drawOption)(int), void(*eraseOption)(int), void(*eraseMenu)(), void(*drawMenu)())
{
	drawMenu();
	int decision = 0;
	do
	{
		keyUpdate();
		if (keyState[vaildiInput::leftArrow])
		{
			eraseOption(decision);
			decision--;
			
		}
		else if (keyState[vaildiInput::rightArrow])
		{
			eraseOption(decision);
			decision++;
			
		}
		if (decision < 0)
			decision = numberOfOptions - 1;
		if (decision >= numberOfOptions)
			decision = 0;
		drawOption(decision);
	} while (!keyState[vaildiInput::enter]);
	eraseMenu();
		return decision;
}

InputSystem::InputSystem()
{
	for (int i = 0; i < 6; i++)
		keyState[i] = false;
}