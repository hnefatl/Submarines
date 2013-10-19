#ifndef _GAMEVIEW_H
#define _GAMEVIEW_H

#include "View.h"

#include <vector>
#include <thread>
#include <mutex>

#include "PlayerSubmarine.h"
#include "EnemySubmarine.h"

class GameView
	: public View
{
public:
	GameView();
	~GameView();

	virtual bool Run();

	bool Update();
	void Draw(const bool &Initial=false);

	void EnemyAdderFunction();

protected:
	unsigned int Turn;

	unsigned int Width;
	unsigned int Height;

	std::mutex *ConsoleLock;

	PlayerSubmarine Player;
	std::vector<EnemySubmarine> Enemies;
	std::mutex *EnemiesLock;
	unsigned int SubmarineSelected;
	unsigned int LastSubmarineSelected;
	
	std::thread *EnemyAdder;
	bool AddEnemies;
};

#endif