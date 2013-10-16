#include "LoadGameView.h"

#include <Windows.h>
#include <fstream>

LoadGameView::LoadGameView()
{
	LastSelected=0;
	CurrentlySelected=0;

	Height=10;
	Width=30;

	// Load file names
	FileNames=GetFileNames("Saves");
	// Trim to correct length
	for(unsigned int x=0; x<FileNames.size(); x++)
	{
		if(FileNames[x].size()>Width)
		{
			FileNames[x].erase(FileNames[x].begin()+Width, FileNames[x].end());
		}
	}
}

bool LoadGameView::Run()
{
	Draw(true);
	while(true)
	{
		Draw(false);
		if(!Update())
		{
			// Create a new game
			GameView Game=GameView();
			// Load the data
			if(!LoadGameData(&Game, FileNames[CurrentlySelected]))
			{
				// Loading failed
				Clear();
				// Display error message
				std::cout<<"The game failed to load."<<std::endl;
				std::cout<<"Press any key to continue...";
				_getch();

				Draw(true); // Redraw loading window
			}
			else
			{
				// Run the loaded game
				return Game.Run();
			}
		}
	}
}

bool LoadGameView::Update()
{
	char Pressed=_getch();

	switch(Pressed)
	{
	case 224: // Arrow key
		Pressed=_getch();
		switch(Pressed)
		{
		case 72: // Up arrow
			LastSelected=CurrentlySelected;
			CurrentlySelected--;
			break;
		case 80: // Down arrow
			LastSelected=CurrentlySelected;
			CurrentlySelected++;
			break;
		}
		break;
	case 13: // Enter
		return false;
	}

	return true;
}
void LoadGameView::Draw(const bool &Initial) const
{
	if(Initial) // First drawing - draw the general layout
	{
		Clear();

		// Place margin
		SetCursor(1, 1);
		for(unsigned int y=0; y<=Height; y++)
		{
			for(unsigned int x=0; x<=Width; (y==0 || y==Height)?x++:x+=Width) // Conditional logic: if we're at the top or bottom,
				// iterate throughthe full width, otherwise just do
					// the two sides.
			{
				// Draw corners
				if(	   (y==0 && x==0) // Top left corner
					|| (y==0 && x==Width) // Top right corner
					|| (y==Height-1 && x==0) // Bottom left corner
					|| (y==Height-1 && x==Width)) //Bottom left corner
				{
					std::cout<<"+";
				}
				else if(y==0 || y==Height) // Sides
				{
					std::cout<<"|";
				}
				else if(x==0 || x==Width) // Top/bottom 
				{
					std::cout<<"-";
				}
			}
			std::cout<<std::endl;
		}
	}
	else // Subsequent drawings - draw the specific elements
	{
		// Overwrite old selection
		SetCursor(2, 2+LastSelected); // Place cursor inside the borders, at the correct height
		SetColour(GREY, BLACK);
		std::cout<<FileNames[LastSelected];

		// Draw new selection
		SetCursor(2, 2+CurrentlySelected); // Place cursor inside the borders, at the correct height
		SetColour(GREEN, BLACK);
		std::cout<<FileNames[CurrentlySelected];
	}
}

std::vector<std::string> LoadGameView::GetFileNames(const std::string &Directory) const
{
	std::vector<std::string> FileNames;
	std::string Pattern=Directory+"\\*.sav";

	WIN32_FIND_DATA FindFileData;
	HANDLE Finder;

	Finder=FindFirstFile(LPCWSTR(Pattern.c_str()), &FindFileData);
	if(Finder==INVALID_HANDLE_VALUE)
	{
		FindClose(Finder);
		return FileNames;
	}
	FileNames.push_back((char *)FindFileData.cFileName);

	while(FindNextFile(Finder, &FindFileData))
	{
		FileNames.push_back((char *)FindFileData.cFileName);
	}

	FindClose(Finder);
	return FileNames;
}
bool LoadGameView::LoadGameData(GameView *const Game, const std::string &FileName) const
{
	std::ifstream In=std::ifstream(FileName.c_str());
	if(!In.good())
	{
		return false;
	}



	return true;
}