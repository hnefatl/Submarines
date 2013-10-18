#ifndef _ENEMYSUBMARINE_H
#define _ENEMYSUBMARINE_H

#include "Submarine.h"

#include <thread>
#include <mutex>
#include <string>
#include <vector>

enum Size
{
	Small=0,
	Medium=1,
	Large=2,
};

struct Attribute
{
	Attribute(const std::string &Name, const double &Value, const bool &Known)
	{
		this->Name=Name;
		this->Value=Value;
		this->Known=Known;
	}

	double GetValue() const
	{
		std::stringstream ss;
		ss.precision(1);
		ss<<Value;
		double Temp;
		ss>>Temp;
		return Temp;
	}
	void SetValue(const double &Value)
	{
		std::stringstream ss;
		ss.precision(1);
		ss<<Value; // 1 d.p.
		ss>>this->Value;
	}
	bool Input(const unsigned int &Pressed)
	{
		if(Pressed==8) // Backspace
		{
			Value.erase(Value.end()-1);
		}
		// Maximum size
		else if(Value.size()==4)
		{
			return false;
		}
		else if(Pressed=='.')
		{
			Value+='.';
		}
		else
		{
			bool Number=false;
			for(char x='0'; x<='9'; x++)
			{
				if(Pressed==x)
				{
					Value+=x;
					Number=true;
				}
			}
			if(!Number)
			{
				// Wasn't a valid input
				return false;
			}
		}
		
		// Valid
		return true;
	}

	std::string Name;
	std::string InputedValue;
	std::string Value;
	bool Known;
};

class EnemySubmarine
	: public Submarine
{
public:
	static EnemySubmarine GenerateSubmarine(std::mutex *ConsoleLock);

	void Start(const unsigned int &Firex, const unsigned int &Firey);

	void SetInformation(const Attribute &Depth, const Attribute &Distance, const Attribute &Pitch,
		const Attribute &Yaw, const Attribute &Lat, const Attribute &Long);

	virtual void Hit();

	void DrawHullStatus(const unsigned int &x, const unsigned int &y);
	void DrawInformation(const unsigned int &x, const unsigned int &y);

	void ChangeValue(const char &Change, const unsigned int &x, const unsigned int &y);

	void FiringFunction(const unsigned int &x, const unsigned int &y);

	void Tone(const unsigned int &Duration, const bool &Async=false);

	// Members
	Size mSize;

	bool SubmarineSelected;
	unsigned int AttributeSelected;
	unsigned int LastAttributeSelected;

	std::mutex *ConsoleLock;
	std::thread *FiringThread;
	bool FiringThreadRun;

	Attribute Lat;
	Attribute Long;
	Attribute Depth;
	Attribute Distance;
	Attribute Direct;
};

#endif
