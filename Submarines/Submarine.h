#ifndef _SUBMARINE_H
#define _SUBMARINE_H

class Submarine
{
public:
	Submarine(unsigned int HullStrength=100, unsigned int Torpedoes=20)
	{
		this->HullStrength=HullStrength;
		this->Torpedoes=Torpedoes;
	}

protected:
	unsigned int HullStrength;
	unsigned int Torpedoes;
};

#endif