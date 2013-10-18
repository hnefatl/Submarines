#include <iostream>
#include <ctime>

#include "MenuView.h"

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));

	MenuView().Run();

	return 0;
}