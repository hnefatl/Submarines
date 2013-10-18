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

bool GameView::Update()
{
	unsigned int Pressed=_getch();
	switch(Pressed)
	{
	case 224: // Arrow key
		Pressed=_getch();
		switch(Pressed)
		{
			
		}
		break;
	}
	return true;
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
		BoxTemplate.push_back("+------------+");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("|            |");
		BoxTemplate.push_back("+------------+");
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