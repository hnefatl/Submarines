#ifndef _LOADGAMEVIEW_H
#define _LOADGAMEVIEW_H

#include "View.h"

#include "GameView.h"

#include <string>
#include <vector>

class LoadGameView
	: public View
{
public:
	LoadGameView();

	virtual bool Run();

	bool Update();
	void Draw(const bool &Initial=false) const;

protected:
	std::vector<std::string> GetFileNames(const std::string &Directory) const;
	bool LoadGameData(GameView *const Game) const;

	std::vector<std::string> FileNames;
	unsigned int LastSelected;
	unsigned int CurrentlySelected;

	unsigned int Width, Height;
};

#endif