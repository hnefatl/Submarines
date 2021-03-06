#include "EnemySubmarine.h"

#include <sstream>

#include "ConsoleControl.h"

EnemySubmarine EnemySubmarine::GenerateSubmarine(std::mutex *ConsoleLock)
{
	EnemySubmarine New;
	New.ConsoleLock=ConsoleLock;
	New.SubmarineSelected=false;
	New.AttributeSelected=0;
	New.LastAttributeSelected=0;

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
	std::vector<Attribute *> Attributes;
	Attributes.reserve(5);
	Attributes.push_back(&New.Lat);
	Attributes.push_back(&New.Long);
	Attributes.push_back(&New.Depth);
	Attributes.push_back(&New.Distance);
	Attributes.push_back(&New.Direct);

	// Set them all to be known initially
	for(unsigned int x=0; x<Attributes.size(); x++)
	{
		Attributes[x]->Known=true;
	}

	// Remove two from the list of knowns
	while(Attributes.size()!=3)
	{
		unsigned int Temp=rand()%Attributes.size();
		Attributes[Temp]->Known=false;
		Attributes.erase(Attributes.begin()+Temp);
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
	// Create temporary lists for convenience
	std::vector<Attribute *> Attributes;
	Attributes.reserve(5);
	Attributes.push_back(&Lat);
	Attributes.push_back(&Long);
	Attributes.push_back(&Depth);
	Attributes.push_back(&Distance);
	Attributes.push_back(&Direct);
	std::vector<std::string> Names;
	Names.push_back("Lat");
	Names.push_back("Long");
	Names.push_back("Depth");
	Names.push_back("Depth");
	Names.push_back("Distance");
	

	ConsoleLock->lock();
	unsigned int yOffset=0;
	for(unsigned int x=0; x<Attributes.size(); x++)
	{
		SetCursor(x, y+yOffset);
		// If the current attribute has been selected and this box is selected, display the attribute in green 
		if(x==AttributeSelected && SubmarineSelected)
		{
			SetColour(GREEN, BLACK);
		}

		std::cout<<Names[x]<<": ";
		if(Attributes[x]->Known)
		{
			// Display the answer
			std::cout<<Attributes[x]->Value;
		}
		else
		{
			// Display what's been entered
			std::cout<<Attributes[x]->GetValue();
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

bool EnemySubmarine::Input(const unsigned int &Input)
{
	if(AttributeSelected==0)
	{
		return Yaw.Input(Input);
	}
	else
	{
		return Pitch.Input(Input);
	}
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