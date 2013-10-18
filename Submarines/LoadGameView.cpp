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
		// Remove extension
		FileNames[x]=std::string(FileNames[x].begin(), FileNames[x].end()-4);
		// Trim to width
		if(FileNames[x].size()>Width-3)
		{
			FileNames[x].erase(FileNames[x].begin()+Width-3, FileNames[x].end());
		}
	}

	Exit=false;
}

bool LoadGameView::Run()
{
	LastSelected=0;
	CurrentlySelected=0;
	Exit=false;
	Draw(true);
	while(true)
	{
		Draw(false);
		if(!Update())
		{
			// Escape has been pressed
			if(Exit)
			{
				return true;
			}
			// If there are available saves
			if(FileNames.size()>0)
			{
				// Create a new game
				GameView Game=GameView();
				// Load the data
				if(!LoadGameData(&Game, FileNames[CurrentlySelected]))
				{
					// Loading failed
					SetColour(GREY, BLACK);
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
}

bool LoadGameView::Update()
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
			if(CurrentlySelected<FileNames.size()-1)
			{
				LastSelected=CurrentlySelected;
				CurrentlySelected++;
			}
			break;
		}
		break;
	case 13: // Enter
		return false;
	case 27: // Escape
		Exit=true;
		return false;
	}

	return true;
}
void LoadGameView::Draw(const bool &Initial) const
{
	if(Initial) // First drawing - draw the general layout
	{
		SetColour(GREY, BLACK);
		Clear();

		// Draw borders
		SetCursor(1, 1);
		for(unsigned int y=0; y<=Height; y++)
		{
			for(unsigned int x=0; x<=Width; (y==0 || y==Height)?x++:x+=Width) // Conditional logic: if we're at the top or bottom,
				// iterate throughthe full width, otherwise just do
					// the two sides.
			{
				// Position
				SetCursor(x+1, y+1);

				// Draw corners
				if(	   (y==0 && x==0) // Top left corner
					|| (y==0 && x==Width) // Top right corner
					|| (y==Height && x==0) // Bottom left corner
					|| (y==Height && x==Width)) //Bottom left corner
				{
					std::cout<<"+";
				}
				else if(y==0 || y==Height) // Top/bottom
				{
					std::cout<<"-";
				}
				else if(x==0 || x==Width) // Sides
				{
					std::cout<<"|";
				}
			}
			std::cout<<std::endl;
		}

		// Draw controls
		SetColour(GREY, BLACK);
		SetCursor(Width+7, 1);
		std::cout<<"Enter: Select save";
		SetCursor(Width+7, 2);
		std::cout<<"Escape: Return to menu";
		SetCursor(Width+7, 3);
		std::cout<<"Up/Down keys: Change selection";


		// Draw files
		SetColour(GREY, BLACK);
		for(unsigned int x=0; x<FileNames.size(); x++)
		{
			SetCursor(3, 2+x);
			std::cout<<FileNames[x];
		}
	}
	else // Subsequent drawings - draw the specific elements
	{
		// If there are any saves
		if(FileNames.size()>0)
		{
			// Overwrite old selection
			SetCursor(3, 2+LastSelected); // Place cursor inside the borders, at the correct height
			SetColour(GREY, BLACK);
			std::cout<<FileNames[LastSelected];

			// Draw new selection
			SetCursor(3, 2+CurrentlySelected); // Place cursor inside the borders, at the correct height
			SetColour(GREEN, BLACK);
			std::cout<<FileNames[CurrentlySelected];
		}
	}
}

std::vector<std::string> LoadGameView::GetFileNames(const std::string &Directory) const
{
	std::vector<std::string> FileNames;
	std::string Pattern=Directory+"\\*.sav";
	WIN32_FIND_DATA FindFileData;
	HANDLE Finder=NULL;

	Finder=FindFirstFile(Pattern.c_str(), &FindFileData);
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
	std::ifstream In=std::ifstream((FileName+".sav").c_str());
	if(!In.good())
	{
		return false;
	}



	return true;
}