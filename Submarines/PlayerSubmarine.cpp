#include "PlayerSubmarine.h"

#include "ConsoleControl.h"

PlayerSubmarine::PlayerSubmarine(std::mutex *ConsoleLock) 
{
	this->ConsoleLock=ConsoleLock;
}
PlayerSubmarine::~PlayerSubmarine()
{
	Repairing=false;
	if(RepairThread!=NULL)
	{
		delete RepairThread;
		RepairThread=NULL;
	}
	if(HullStrengthLock!=NULL)
	{
		delete HullStrengthLock;
		HullStrengthLock=NULL;
	}
}

void PlayerSubmarine::Start()
{
	this->RepairThread=new std::thread(&PlayerSubmarine::RepairFunction, this);
}

void PlayerSubmarine::Hit()
{
	HullStrengthLock->lock();
	HullStrength-=25;
	if(HullStrength>0)
	{
		HullStrength=0;
	}
	HullStrengthLock->unlock();
}

void PlayerSubmarine::DisplayHull(const unsigned int &x, const unsigned int &y)
{
	// Store coordinates used
	Hullx=x;
	Hully=y;
	ConsoleLock->lock();

	for(unsigned int i=0; i<HullStrength/2; i++)
	{
		SetCursor(x+i, y);
		if(i<16)
		{
			SetColour(RED, RED);
		}
		else if(i>=16 && i<33)
		{
			SetColour(GOLD, GOLD);
		}
		else
		{
			SetColour(GREEN, GREEN);
		}
		std::cout<<" ";
	}

	ConsoleLock->unlock();
}

void PlayerSubmarine::RepairFunction()
{
	while(Repairing)
	{
		std::this_thread::sleep_for(std::chrono::seconds(rand()%5+4)); // Sleep for between 4 and 9 seconds

		if(Repairing) // If still meant to repair
		{
			// Repair the hull
			HullStrengthLock->lock();
			HullStrength+=2;
			HullStrengthLock->unlock();

			// Draw the new hull status
			DisplayHull(Hullx, Hully);
		}
	}
}

bool PlayerSubmarine::FullyRepaired() const
{
	return HullStrength>=100;
}