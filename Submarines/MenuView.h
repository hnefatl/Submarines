#ifndef _MENUVIEW_H
#define _MENUVIEW_H

#include "View.h"

#include <string>
#include <vector>

class MenuOption
{
public:
	MenuOption()
	{

	}
	MenuOption(const std::string &Name, View *RelatedView=NULL)
	{
		this->Name=Name;
		this->RelatedView=RelatedView;
	}

	void Dispose()
	{
		if(RelatedView!=NULL)
		{
			delete RelatedView;
			RelatedView=NULL;
		}
		if(Name!="")
		{
			Name="";
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
	~MenuView();

	virtual bool Run();

	bool Update();
	void Draw(const bool &Initial=false);

protected:
	std::vector<MenuOption> Options;

	unsigned int CurrentlySelected;
	unsigned int LastSelected;
};

#endif