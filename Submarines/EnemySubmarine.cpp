#include "EnemySubmarine.h"

#include <sstream>

#include "ConsoleControl.h"

EnemySubmarine::EnemySubmarine(const Size &Size, std::mutex *ConsoleLock, const unsigned int &HullStrength)
	:Submarine(HullStrength)
{
	this->mSize=Size;
	this->ConsoleLock=ConsoleLock;
}

void EnemySubmarine::SetInformation(const Attribute &Depth, const Attribute &Distance, const Attribute &Pitch, const Attribute &Yaw)
{
	this->Depth=Depth;
	this->Distance=Distance;
	this->Pitch=Pitch;
	this->Yaw=Yaw;
}

Size EnemySubmarine::GetSize() const
{
	return mSize;
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

	SetCursor(x, y);
	ss<<Depth.Value;
	std::cout<<"Depth: "<<(Depth.Value!=0?ss.str():(Depth.Required?"     ":"?????"));
	ss=std::stringstream();

	SetCursor(x, y+1);
	ss<<Distance.Value;
	std::cout<<"Distance: "<<(Distance.Value!=0?ss.str():(Distance.Required?"     ":"?????"));
	ss=std::stringstream();

	SetCursor(x, y+2);
	ss<<Pitch.Value;
	std::cout<<"Pitch: "<<(Pitch.Value!=0?ss.str():(Pitch.Required?"     ":"?????"));
	ss=std::stringstream();

	SetCursor(x, y+3);
	ss<<Yaw.Value;
	std::cout<<"Yaw: "<<(Yaw.Value!=0?ss.str():(Yaw.Required?"     ":"?????"));

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