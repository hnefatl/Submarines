#include "MenuView.h"

#include "GameView.h"
#include "InstructionView.h"
#include "LoadGameView.h"

MenuView::MenuView()
{
	Options.push_back(MenuOption("New Game", new GameView()));
	Options.push_back(MenuOption("Load Game", new LoadGameView()));
	Options.push_back(MenuOption("Instructions", new InstructionView()));
	Options.push_back(MenuOption("Exit", NULL));

	CurrentlySelected=0;
	LastSelected=0;
}

bool MenuView::Run()
{
	Draw(true);
	while(true)
	{
		Draw(false);
		if(!Update())
		{
			return false;
		}
	}
	return true;
}

bool MenuView::Update()
{
	int Pressed=_getch();

	switch(Pressed)
	{
	case 224: // Arrow key
		Pressed=_getch();
		switch(Pressed)
		{
		case 72: // Up arrow
			if(CurrentlySelected>0)
			{
				LastSelected=CurrentlySelected;
				CurrentlySelected--;
			}
			break;
		case 80: // Down arrow
			if(CurrentlySelected<Options.size()-1)
			{
				LastSelected=CurrentlySelected;
				CurrentlySelected++;
			}
			break;
		}
	case 13: // Enter key
		if(Options[CurrentlySelected].RelatedView!=NULL)
		{
			Options[CurrentlySelected].RelatedView->Run(); // Start the new View running
			// Redraw the initial layout
			Draw(true);
		}
		else
		{
			// No view associated with the option, must be the exit option
			return false; // Signal exit
		}
		break;
	case 27: // Escape
		return false; // Signal exit
	}

	return true;
}
void MenuView::Draw(const bool &Initial)
{
	unsigned int Width=80;
	unsigned int Height=25;
	if(Initial)
	{
		// Title
		// Had to add escape characters; the text now looks wonky
		std::cout<<"	   _____       _                          _                 "<<std::endl;
		std::cout<<"	  / ____|     | |                        (_)                "<<std::endl;
		std::cout<<"	 | (___  _   _| |__  _ __ ___   __ _ _ __ _ _ __   ___  ___ "<<std::endl;
		std::cout<<"	  \\___ \\| | | | '_ \\| '_ ` _ \\ / _` | '__| | '_ \\ / _ \\/ __|"<<std::endl;
		std::cout<<"	  ____) | |_| | |_) | | | | | | (_| | |  | | | | |  __/\\__ \\"<<std::endl;
		std::cout<<"	 |_____/ \\__,_|_.__/|_| |_| |_|\\__,_|_|  |_|_| |_|\\___||___/"<<std::endl;

		// Options
		for(unsigned int x=0; x<Options.size(); x++)
		{
			SetCursor((Width/2)-(Options[x].Name.size()/2), 9+x); // Below the title and central
		}
	}
	else
	{
		// Remove old selection
		SetCursor((Width/2)-(Options[LastSelected].Name.size()/2), 9+LastSelected);
		SetColour(GREY, BLACK);
		std::cout<<Options[LastSelected].Name;
		
		// Draw new selection
		SetCursor((Width/2)-(Options[CurrentlySelected].Name.size()/2), 9+CurrentlySelected);
		SetColour(GREEN, BLACK);
		std::cout<<Options[CurrentlySelected].Name;
	}
}