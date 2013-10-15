#ifndef _MENUVIEW_H
#define _MENUVIEW_H

#include "View.h"

#include <string>
#include <vector>

struct MenuOption
{
	MenuOption(const std::string &Name, View *RelatedView=NULL, const bool &Selected=false)
	{
		this->Name=Name;
		this->RelatedView=RelatedView;
		this->Selected=Selected;
	}
	~MenuOption()
	{
		if(RelatedView!=NULL)
		{
			delete RelatedView;
			RelatedView=NULL;
		}
	}

	bool Selected;
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
	void Draw();

protected:
	std::vector<MenuOption> Options;
};

#endif