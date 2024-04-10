#pragma once
#ifndef Artist
#include "Artist.h"
#endif
#ifndef vector
#include <vector>
#endif

class Menu {
public:
	enum gameStates {
		MAIN_MENU,
		IN_GAME,
		CUTSCENE
	};

	static int gameState;
	void init();
	void draw();
};