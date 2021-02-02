#include"Manager.h"
int main()
{
	srand(time(NULL));
	Manager::instance = new Manager();
	Manager::instance->title();
	return 0;
}