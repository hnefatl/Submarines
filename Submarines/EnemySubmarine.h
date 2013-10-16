#ifndef _ENEMYSUBMARINE_H
#define _ENEMYSUBMARINE_H

#include "Submarine.h"
#include <thread>
#include <mutex>

enum Size
{
	Small,
	Medium,
	Large,
};

struct Attribute
{
	Attribute(const double &Value=0, const bool &Required=false)
	{
		this->Value=Value;
		this->Required=Required;
	}

	double Value;
	bool Required;
};

class EnemySubmarine
    : public Submarine
{
public:
    EnemySubmarine(const Size &Size, std::mutex *ConsoleLock, const unsigned int &HullStrength);

	void SetInformation(const Attribute &Depth, const Attribute &Distance, const Attribute &Pitch, const Attribute &Yaw);

	Size GetSize() const;

	void Hit();

	void DrawHullStatus(const unsigned int &x, const unsigned int &y);
	void DrawInformation(const unsigned int &x, const unsigned int &y);

	void FiringFunction(const unsigned int &x, const unsigned int &y);

	void Tone(const unsigned int &Duration, const bool &Async=false);

	// Members
	Size mSize;

	std::mutex *ConsoleLock;
	std::thread *FiringThread;
	bool FiringThreadRun;
	
	Attribute Depth;
	Attribute Distance;
	Attribute Pitch;
	Attribute Yaw;

	std::string Value;
};

#endif
