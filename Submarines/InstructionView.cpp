#include "InstructionView.h"

#include <iostream>
#include <fstream>

#include "ConsoleControl.h"

InstructionView::InstructionView()
{
	std::ifstream In=std::ifstream("Instructions.txt", std::ios::binary | std::ios::beg | std::ios::in);
	if(!In.good())
	{
		Instructions="You deleted the instructions file. You now don't know how to play.";
	}
	else
	{
		// Find file size
		std::streamsize Size=0;
		std::streamsize Temp=0;
		do
		{
			In.seekg(std::ios::ate);
			Size=In.tellg();
			In.seekg(std::ios::beg);
			Temp=In.tellg();
		} while(Size!=Temp);

		// Read instructions
		In.read(&Instructions[0], Size);
		In.close();

		// Terminate string
		Instructions[(unsigned int)Size]='\0';
	}
}

bool InstructionView::Run()
{
	while(true)
	{
		Draw();
		if(!Update())
		{
			// Don't need to exit, just need to backtrack
			return true;
		}
	}
}

bool InstructionView::Update()
{
	// Just wait for a keypress, no matter which one
	_getch();

	return true;
}
void InstructionView::Draw()
{
	Clear();

	std::cout<<Instructions<<std::endl;
	std::cout<<"Press any key to continue...";
}