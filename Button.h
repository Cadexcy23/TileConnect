#pragma once
#ifndef Artist
#include "Artist.h"
#endif
#ifndef vector
#include <vector>
#endif
#ifndef time
#include <time.h>
#endif

class Button {
public:
	// add a bool to track if we alreay clicked abutton so we cant multi click
	struct button {
		int x, y, w, h, frameTime;
		std::vector<SDL_Texture*> texSet;
		//seperate tex set for hover
		//seperate tex set for activate animation NEEDS A WAY TO DELAY
		void(*activate)();
		void draw()
		{
			Artist artist;
			artist.drawImage(texSet[0], x, y, w, h);
		}
		void drawHovered()
		{
			Artist artist;
			int frameToDraw = clock() / frameTime % (texSet.size() - 1);
			//printf("FrameTime: %i, DisplayedFrame: %i \n", clock(), frameToDraw);
			artist.drawImage(texSet[frameToDraw + 1], x, y, w, h);
		}
	};

	button makeButton(std::vector<SDL_Texture*> texSet, int x, int y, int frameTime, void(*activateFunc)(), int w = 0, int h = 0);

	void updateButtonVector(std::vector<button> buttonList);
};