#ifndef _VIEW_H
#define _VIEW_H

#include <stack>
#include <conio.h>
#include "ConsoleControl.h"

class View
{
public:
	// Returns true on continuation of program, returns false on signal to exit
	virtual bool Run();
};

#endif