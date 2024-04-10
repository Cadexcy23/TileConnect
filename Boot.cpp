#include <stdio.h>
#include "Artist.h"
#include "Controller.h"
#include "Mixer.h"
#include "Menu.h"


int main(int argc, char* args[])
{
	//Declare Artist and controller
	Artist Artist;
	Controller controller;
	Mixer mixer;
	Menu menu;

	//Start up SDL and create window
	if (!Artist.init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!Artist.loadMedia())
		{
			printf("Failed to load media!\n");
		}
		if (!mixer.loadSoundMedia())
		{
			printf("Failed to load media!\n");
		}
		
		
		//loading setting ect
		controller.loadController();
		menu.init();


		//While application is running
		while (!Controller::quit)
		{
			//Clear screen
			Artist.clearScreen();


			//Checks for user input
			controller.controller();

			//Updates game world
			//Logic.update();

			//Draw everything
			Artist.draw();

			




			//Update screen
			Artist.updateScreen();
		}
		
	}

	//Free resources and close SDL
	Artist.close();

	return 0;
}