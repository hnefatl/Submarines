#ifndef _INSTRUCTIONVIEW_H
#define _INSTRUCTIONVIEW_H

#include "View.h"
#include <vector>
#include <string>

class InstructionView
	: public View
{
public:
	InstructionView();
	void LoadInstructions();

	virtual bool Run();

	bool Update();
	void Draw();

protected:
	std::vector<std::string> Instructions;
	unsigned int Page;
	bool PageChanged;
};

#endif