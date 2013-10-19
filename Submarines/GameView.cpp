#include "GameView.h"

GameView::GameView()
	:ConsoleLock(new std::mutex),
	Player(ConsoleLock)
{
	Width=80;
	Height=25;
}
GameView::~GameView()
{
	AddEnemies=false;
	if(EnemyAdder!=NULL)
	{
		delete EnemyAdder;
		EnemyAdder=NULL;
	}

	if(EnemiesLock!=NULL)
	{
		delete EnemiesLock;
		EnemiesLock=NULL;
	}
}

bool GameView::Run()
{
	Turn=1;
	AddEnemies=true;
	EnemyAdder=new std::thread(&GameView::EnemyAdderFunction, this);

	SubmarineSelected=0;
	LastSubmarineSelected=0;

	Draw(true);
	while(true)
	{
		Draw(false);
		if(Update()) // Change entered
		{

		}
	}

	return true;
}

bool GameView::Update()
{
	unsigned int Pressed=_getch();
	switch(Pressed)
	{
	case 224: // Arrow key
		Pressed=_getch();
		EnemiesLock->lock(); // Lock the enemies as it will be used
		switch(Pressed)
		{
		case 72: // Up arrow
			if(Enemies[SubmarineSelected].AttributeSelected>0)
			{
				Enemies[SubmarineSelected].AttributeSelected--;
			}
			break;
		case 80: // Down arrow
			if(Enemies[SubmarineSelected].AttributeSelected<1)
			{
				Enemies[SubmarineSelected].AttributeSelected++;
			}
			break;
		case 75: // Left arrow
			if(SubmarineSelected>0)
			{
				LastSubmarineSelected=SubmarineSelected;
				SubmarineSelected--;
				Enemies[LastSubmarineSelected].SubmarineSelected=false;
				Enemies[SubmarineSelected].SubmarineSelected=true;
			}
			break;
		case 77: // Right arrow
			if(SubmarineSelected<Enemies.size()-1)
			{
				LastSubmarineSelected=SubmarineSelected;
				SubmarineSelected++;
				Enemies[LastSubmarineSelected].SubmarineSelected=false;
				Enemies[SubmarineSelected].SubmarineSelected=true;
			}
			break;
		}
		EnemiesLock->unlock(); // Unlock the enemies after use
		break;
	case 13:
		return true; // Signal change
	default: // Assume input
		Enemies[SubmarineSelected].Input(Pressed); // Delegate to EnemySubmarine, which will pick the correct attribute
		break;
	}
	return false; // Signal no change
}
void GameView::Draw(const bool &Initial)
{
	if(Initial)
	{
		ConsoleLock->lock();
		SetColour(GREY, BLACK);
		Clear();

		// Draw hull meter
		for(unsigned int y=0; y<3; y++)
		{
			for(unsigned int x=0; x<52; (y==0 || y==2)?x++:x+=51)
			{
				SetCursor(((Width/2)-(52/2))+x, 1+y);

				if((y==0 && x==0) || // Top left corner
					(y==0 && x==51) || // Top right corner
					(y==2 && x==0) || // Bottom left corner
					(y==2 && x==51)) // Bottom right corner
				{
					std::cout<<"+";
				}
				else if(y==0 || y==2) // Top and bottom
				{
					std::cout<<"-";
				}
				else // Sides
				{
					std::cout<<"|";
				}
			}
		}

		// Don't draw the hull status yet, wait until after the outlines have been drawn

		std::vector<std::string> BoxTemplate;
		BoxTemplate.push_back("+--------+---+");
		BoxTemplate.push_back("|        |   |");
		BoxTemplate.push_back("+--------+---+");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("+------------+");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("+------------+");

		// Draw the information boxes
		for(unsigned int Box=0; Box<5; Box++)
		{
			unsigned int y=6;
			unsigned int x=(Box*BoxTemplate[0].size())+5;

			x-=(1*Box); // Decrease x to merge the box edges together

			for(unsigned int i=0; i<BoxTemplate.size(); i++)
			{
				SetCursor(x, y+i);
				std::cout<<BoxTemplate[i];
			}
		}

		ConsoleLock->unlock();

		// Draw in the hull status
		Player.DisplayHull((Width/2)-(52/2)+1, 2);
	}
	else
	{
		EnemiesLock->lock();
		unsigned int y=6;
		unsigned int X=0;
		for(unsigned int x=0; x<Enemies.size(); x++)
		{
			Enemies[x].DrawInformation(X, y);
			X+=16;
		}
		EnemiesLock->unlock();
	}
}

void GameView::EnemyAdderFunction()
{
	while(AddEnemies)
	{
		std::this_thread::sleep_for(std::chrono::seconds((rand()%10)+30)); // Sleep for between 30 and 39 seconds

		if(!AddEnemies) // If we've stopped adding enemies
		{
			break;
		}

		if(Enemies.size()>=5) // Maximum amount of enemies
		{
			continue;
		}

		EnemiesLock->lock();
		Enemies.push_back(EnemySubmarine::GenerateSubmarine(ConsoleLock));
		EnemiesLock->unlock();
	}
}