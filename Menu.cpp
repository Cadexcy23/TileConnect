#include "Menu.h"
#include "Artist.h"
#include <time.h>
#include "Button.h"
#include "Controller.h"
#include "Map.h"




int Menu::gameState = MAIN_MENU;
std::vector<Button::button> buttonList;
SDL_Texture* gBG = NULL;
SDL_Texture* gWatermark = NULL;









//BUTTON FUNCS
void dummyFunc()
{
	Artist artist;
	Button button; //ALL TEMP
	std::vector<SDL_Texture*> newButtonTexSet;
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/huh.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/huhHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, rand() % (Artist::SCREEN_WIDTH - 200), rand() % (Artist::SCREEN_HEIGHT - 100), 16.6, dummyFunc));
}

void closeGame()
{
	Controller::quit = true;
}

void startNewGame()//will need some variables in future
{
	Map map;
	Menu::gameState = Menu::IN_GAME;
	map.init();
}

void loadMainMenuButtons();

void loadOptionsMenuButtons()
{
	Artist artist;
	Button button;

	buttonList.clear();

	std::vector<SDL_Texture*> newButtonTexSet;
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/optionsTitle.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/optionsTitle.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, 0, 0, 16.6, dummyFunc));

	newButtonTexSet.clear();
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/back.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/backHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, 0, 700, 16.6, loadMainMenuButtons));
}

void loadNewGameMenuButtons()
{
	Artist artist;
	Button button;

	buttonList.clear();

	std::vector<SDL_Texture*> newButtonTexSet;
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/newTitle.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/newTitle.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, 0, 0, 16.6, dummyFunc));

	newButtonTexSet.clear();
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/back.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/backHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, 0, 700, 16.6, loadMainMenuButtons));

	newButtonTexSet.clear();
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/start.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/startHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, Artist::SCREEN_WIDTH - 200, 700, 16.6, startNewGame));
}

void loadMainMenuButtons()
{
	Artist artist;
	Button button;

	buttonList.clear();

	std::vector<SDL_Texture*> newButtonTexSet;
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/mainTitle.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/mainTitle.png"));
	int w;
	SDL_QueryTexture(newButtonTexSet[0], NULL, NULL, &w, NULL);
	buttonList.push_back(button.makeButton(newButtonTexSet, Artist::SCREEN_WIDTH / 2 - w / 2, 0, 16.6, dummyFunc));

	newButtonTexSet.clear();
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/new.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/newHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, Artist::SCREEN_WIDTH / 2 - 100, 256, 16.6, loadNewGameMenuButtons));

	newButtonTexSet.clear();
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/options.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/optionsHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, Artist::SCREEN_WIDTH / 2 - 100, 356, 16.6, loadOptionsMenuButtons));

	newButtonTexSet.clear();
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/huh.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/huhHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, Artist::SCREEN_WIDTH - 200, 700, 16.6, dummyFunc));

	newButtonTexSet.clear();
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/exit.png"));
	newButtonTexSet.push_back(artist.loadTexture("Resource/mainMenu/exitHover.png"));
	buttonList.push_back(button.makeButton(newButtonTexSet, 0, 700, 16.6, closeGame));
}



void Menu::init()
{
	srand(clock());
	Artist artist;
	
	gBG = artist.loadTexture("Resource/mainMenu/background.png");
	
	
	loadMainMenuButtons();
}



void drawBG()
{
	Artist artist;
	int drawOffset = clock() % Artist::SCREEN_WIDTH;
	drawOffset *= -1;
	artist.drawImage(gBG, drawOffset, 0);
	artist.drawImage(gBG, drawOffset + Artist::SCREEN_WIDTH, 0);
	artist.drawLetters("V 0.0.0.0.0.0.1", 1620, 1059, Artist::smallFont);
}



void Menu::draw()
{
	if (gameState == MAIN_MENU)
	{
		Button button;

		drawBG();
		button.updateButtonVector(buttonList);
	}
}