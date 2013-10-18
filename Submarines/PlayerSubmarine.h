#ifndef _PLAYERSUBMARINE_H
#define _PLAYERSUBMARINE_H

#include "Submarine.h"

#include <thread>
#include <mutex>

class PlayerSubmarine
    : public Submarine
{
public:
	PlayerSubmarine(std::mutex *ConsoleLock);
	~PlayerSubmarine();

	void Start();

	virtual void Hit();

	void DisplayHull(const unsigned int &x, const unsigned int &y);

	void RepairFunction();

	bool FullyRepaired() const;

	// Members
	std::mutex *ConsoleLock;
	std::thread *RepairThread;
	bool Repairing;

	std::mutex *HullStrengthLock;

	unsigned int Hullx, Hully;
};

#endif
