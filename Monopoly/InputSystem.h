#pragma once
enum vaildiInput
{
	upArrow = 0, downArrow = 1, leftArrow = 2, rightArrow = 3, enter = 4, ESC = 5
};

class InputSystem {
public:
	static InputSystem * instance;
	InputSystem();
	void keyUpdate();
	bool keyState[6];
	bool OptionInquire(void(*drawOption)(), void(*drawMenu)(), void(*eraseMenu)());
	int quitableVerticalOptionSelector(int,void (*drawOption)(int),void(*eraseOption)(int), void(*eraseMenu)(),void(*drawMenu)());
	int quitableHorizontalOptionSelector(int, void(*drawOption)(int), void(*eraseOption)(int), void(*eraseMenu)(), void(*drawMenu)());
	int verticalOptionSelector(int, void(*drawOption)(int), void(*eraseOption)(int), void(*eraseMenu)(), void(*drawMenu)());
	int horizontalOptionSelector(int, void(*drawOption)(int), void(*eraseOption)(int), void(*eraseMenu)(), void(*drawMenu)());
};