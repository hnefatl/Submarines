#ifndef _GAMEVIEW_H
#define _GAMEVIEW_H

#include "View.h"

class GameView
	: public View
{
public:
	GameView();

	virtual bool Run();

	bool Update();
	void Draw();

protected:

};

#endif