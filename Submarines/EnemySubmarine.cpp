#include "EnemySubmarine.h"

#include <sstream>

#include "ConsoleControl.h"

EnemySubmarine EnemySubmarine::GenerateSubmarine(std::mutex *ConsoleLock)
{
	EnemySubmarine New;
	New.ConsoleLock=ConsoleLock;

	// Generate data
	// Generate new size
	unsigned int NewSize=rand()%3;
	New.mSize=Size(NewSize);

	// Generate base dimensions
	New.Lat.SetValue((rand()%200)+20); // Between 20m and 219m
	New.Long.SetValue((rand()%200)+20); // Between 20m and 219m
	New.Depth.SetValue((rand()%100)); // From 0m to 99m above
	
	// Generate derived dimensions
	New.Distance.SetValue(sqrt(pow(New.Lat.GetValue(), 2)+pow(New.Long.GetValue(), 2))); // Pythagoras to find flat distance
	New.Direct.SetValue(sqrt(pow(New.Distance.GetValue(), 2)+pow(New.Depth.GetValue(), 2)));

	// Pick two to disable
	unsigned int Temp=rand()%5;
	switch(Temp)
	{
	case 0: New.Lat.Known=false; break;
	case 1: New.Long.Known=false; break;
	case 2: New.Depth.Known=false; break;
	case 3: New.Distance.Known=false; break;
	case 4: New.Direct.Known=false; break;
	}

	return New;
}

void EnemySubmarine::Start(const unsigned int &Firex, const unsigned int &Firey)
{
	FiringThread=new std::thread(&EnemySubmarine::FiringFunction, this, Firex, Firey);
}

void EnemySubmarine::Hit()
{
	switch(mSize)
	{
	case Size::Small: HullStrength-=100; break; // One hit to kill
	case Size::Medium: HullStrength-=50; break; // Two hits to kill
	case Size::Large: HullStrength-=34; break; // Three hits to kill
	}
	// Floor overflows
	if(HullStrength>100)
	{
		HullStrength=0;
	}
}

void EnemySubmarine::DrawHullStatus(const unsigned int &x, const unsigned int &y)
{
	ConsoleLock->lock();
	SetCursor(x, y);
	for(unsigned int x=0; x<HullStrength; x+=10)
	{
		if(x<3) // First 3
		{
			SetColour(RED, RED);
		}
		else if(x>=3 && x<6) // Middle 3
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
void EnemySubmarine::DrawInformation(const unsigned int &x, const unsigned int &y)
{
	std::stringstream ss;

	ConsoleLock->lock();
	SetColour(GREY, BLACK);

	unsigned int yOffset=0;
	for(unsigned int x=0; x<Attributes.size(); x++)
	{
		SetCursor(x, y+yOffset);
		// If the current attribute has been selected and this box is selected, display the attribute in green 
		if(x==AttributeSelected && SubmarineSelected)
		{
			SetColour(GREEN, BLACK);
		}

		std::cout<<Attributes[x].Name<<": ";
		if(Attributes[x].Known)
		{
			// Display the answer
			std::cout<<Attributes[x].Value;
		}
		else
		{
			// Display what's been entered
			std::cout<<Attributes[x].GetValue();
		}

		if(yOffset==2)
		{
			// Add a space between lines for border
			yOffset+=2;
		}
		else
		{
			// New line
			yOffset++;
		}
	}

	ConsoleLock->unlock();
}

void EnemySubmarine::FiringFunction(const unsigned int &x, const unsigned int &y)
{
	while(FiringThreadRun)
	{
		unsigned int Delay=rand()%15+5; // Minimum delay of 5 seconds, maximum delay of 19 seconds

		std::this_thread::sleep_for(std::chrono::seconds(Delay)); // Sleep until required

		if(!FiringThreadRun)
		{
			// Stopped while waiting
			break;
		}

		// Submarine fires
		unsigned int Timer=3000; // Not how long it takes to reach the player - just used for reducing time elapsed
		while(Timer!=0)
		{
			// Ticker on
			ConsoleLock->lock();
			SetCursor(x, y);
			SetColour(RED, RED);
			std::cout<<" ";
			ConsoleLock->unlock();

			// Beep for 100 milliseconds
			Tone(100);

			// Ticker off
			ConsoleLock->lock();
			SetCursor(x, y);
			SetColour(BLACK, BLACK);
			std::cout<<" ";
			ConsoleLock->unlock();

			// Subtract the time taken from the timer
			Timer-=100;
			// Wait
			std::this_thread::sleep_for(std::chrono::milliseconds(Timer));
			if(!FiringThreadRun)
			{
				// Meant to have stopped
				break;
			}
		}
	}
}

void EnemySubmarine::Tone(const unsigned int &Duration, const bool &Async)
{
	if(Async)
	{
		// Launch new (short-lived) thread to beep
		std::thread New(Beep, 600, Duration);
	}
	else
	{
		Beep(600, Duration);
	}
}