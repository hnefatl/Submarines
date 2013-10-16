#ifndef _MENUVIEW_H
#define _MENUVIEW_H

#include "View.h"

#include <string>
#include <vector>

struct MenuOption
{
	MenuOption(const std::string &Name, View *RelatedView=NULL)
	{
		this->Name=Name;
		this->RelatedView=RelatedView;
	}
	~MenuOption()
	{
		if(RelatedView!=NULL)
		{
			delete RelatedView;
			RelatedView=NULL;
		}
	}

	std::string Name;
	View *RelatedView;
};

class MenuView
	: public View
{
public:
	MenuView();

	virtual bool Run();

	bool Update();
	void Draw(const bool &Initial=false);

protected:
	std::vector<MenuOption> Options;

	unsigned int CurrentlySelected;
	unsigned int LastSelected;
};

#endif