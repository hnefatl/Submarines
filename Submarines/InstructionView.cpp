#include "InstructionView.h"

#include <iostream>
#include <fstream>

#include "ConsoleControl.h"

InstructionView::InstructionView()
{
	LoadInstructions();
	Page=0;
	PageChanged=true;
}

void InstructionView::LoadInstructions()
{
	std::ifstream In=std::ifstream("Instructions.txt", std::ios::binary | std::ios::beg | std::ios::in);
	if(!In.good())
	{
		Instructions.push_back("");
		Instructions[0]="You deleted the instructions file. You now don't know how to play.";
	}
	else
	{
		// Read instructions
		std::string Temp;
		unsigned int LineCounter=0;
		Instructions.push_back("");
		while(std::getline(In, Temp))
		{
			LineCounter++;
			Instructions.back()+=Temp+'\n';
			Temp="";

			if(LineCounter>=22)
			{
				LineCounter=0;
				Instructions.push_back("");
			}
		}
		In.close();
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
	int Pressed=_getch();

	switch(Pressed)
	{
	case 224:
		Pressed=_getch();
		switch(Pressed)
		{
		case 77: // Right arrow
			if(Page<Instructions.size()-1)
			{
				Page++;
				PageChanged=true;
			}
			break;
		case 75: // Left arrow
			if(Page>0)
			{
				Page--;
				PageChanged=true;
			}
			break;
		}
		break;
	case 27: // Escape
		return false; // Signal return
	}

	return true;
}
void InstructionView::Draw()
{
	if(PageChanged)
	{
		PageChanged=false;
		SetColour(GREY, BLACK);
		Clear();
		std::cout<<Instructions[Page]<<std::endl;
		SetCursor(0, 24);
		SetColour(GREEN, BLACK);
		std::cout<<"Use the arrow keys to navigate the instructions.";
	}
}