#ifndef _INSTRUCTIONVIEW_H
#define _INSTRUCTIONVIEW_H

#include "View.h"
#include <string>

class InstructionView
	: public View
{
public:
	InstructionView();

	virtual bool Run();

	bool Update();
	void Draw();

protected:
	std::string Instructions;
};

#endif