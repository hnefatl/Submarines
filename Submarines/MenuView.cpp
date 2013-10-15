#include "MenuView.h"

#include "GameView.h"
#include "InstructionView.h"
#include "LoadGameView.h"

MenuView::MenuView()
{
	Options.push_back(MenuOption("New Game", new GameView(), true));
	Options.push_back(MenuOption("Load Game", new LoadGameView(), false));
	Options.push_back(MenuOption("Instructions", new InstructionView(), false));
	Options.push_back(MenuOption("Exit", NULL, false));
}

bool MenuView::Run()
{

}

bool MenuView::Update()
{

}
void MenuView::Draw()
{

}