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
#ifndef fstream
#include <fstream>
#endif
#ifndef regex
#include <regex>
#endif
#ifndef map
#include <map>
#endif


class Tile {
public:
	struct masterTile {
		std::string name; // PRIMARY KEY MUST BE UNIQUE
		SDL_Texture* tex; //spriteSheet for animation
		//maybe add a way it can connect to other textures? perhaps by name prefix?
		bool connect; //if the tile should test for connectivity
		bool walkable;
	};

	struct tile {
		int conType[4]; //used to determine the way the texture should connect visualy
		int tileType;

		

		void draw(int x, int y, int w, int h)
		{
			Artist artist;

			//get info about the texture
			int tW, tH;
			SDL_QueryTexture(masterTileList[tileType].tex, NULL, NULL, &tW, &tH);

			//Decide what freame to draw
			int drawFrame = clock() % tH / 128;

			//Rect to use as source
			SDL_Rect tileRec;
			tileRec.w = 64;
			tileRec.h = 64;

			if (masterTileList[tileType].connect)
			{
				//Top left Quad
				tileRec.x = conType[0] * 128;
				tileRec.y = drawFrame * 128;
				artist.drawImage(masterTileList[tileType].tex, x, y, w / 2, h / 2, &tileRec);

				//Top right Quad
				tileRec.x = conType[1] * 128 + 64;
				tileRec.y = drawFrame * 128;
				artist.drawImage(masterTileList[tileType].tex, x + w / 2, y, w / 2 + 1, h / 2, &tileRec);//MIGHT NEED TO ADD A CHECK TO SEE IF WE HAVE A REMAINDER AND DRAW 1PX FARTHER instead of always

				//Bottom left Quad
				tileRec.x = conType[2] * 128;
				tileRec.y = drawFrame * 128 + 64;
				artist.drawImage(masterTileList[tileType].tex, x, y + h / 2, w / 2, h / 2 + 1, &tileRec);

				//Bottom right Quad
				tileRec.x = conType[3] * 128 + 64;
				tileRec.y = drawFrame * 128 + 64;
				artist.drawImage(masterTileList[tileType].tex, x + w / 2, y + h / 2, w / 2 + 1, h / 2 + 1, &tileRec);
			}
			else
			{
				tileRec.w = 128;
				tileRec.h = 128;
				tileRec.x = 0;
				tileRec.y = drawFrame * 128;
				artist.drawImage(masterTileList[tileType].tex, x, y, w, h, &tileRec);
			}
		}
	};
	
	static std::vector<masterTile> masterTileList;
	
	enum tileVars { COMMENT, EMPTY, NAME, WALKABLE, CONNECT }; //Add new vars here



	std::pair<tileVars, std::smatch> parse_line(std::istream* dataFile) {
		/*
		read line from file and parse it
		IN:  pointer to iostream data file
		OUT: key(unique identifier int), match (desired string from line)
		*/

		// Key/Regex map
		std::map<tileVars, std::regex> rx_map;
		rx_map[COMMENT] = std::regex("^\\s*//.*"); //Matches Comments
		rx_map[EMPTY] = std::regex("^\\s*$");        //Matches empty strings (with white space)
		rx_map[NAME] = std::regex("^\\s*\\[Name\\]\\s*(.*)"); //Add new vars here
		rx_map[WALKABLE] = std::regex("^\\s*\\[Walkable\\]\\s*(.*)");
		rx_map[CONNECT] = std::regex("^\\s*\\[Connect\\]\\s*(.*)");

		static std::string s; //This must be static or the smatch var will not work
		std::smatch match;    // Regex matches
		tileVars key;         // Match key

		std::getline(*dataFile, s); //Read in a line (string\n) from the file

		for (std::map<tileVars, std::regex>::iterator iter = rx_map.begin(); iter != rx_map.end(); ++iter) {
			if (std::regex_match(s, (*iter).second)) { //If there is a match
				std::regex_search(s, match, (*iter).second); //get matches
				key = (*iter).first; //get the key associated with the match
				break; //Stop searching and return key match pair
			}
		}
		return std::make_pair(key, match); //Return key match pair
	}
	
	masterTile loadTileData(std::string tileName) {
		/*
		Secret Genius Tile Vars Loading
		IN: tile name name string
		OUT: tile struct with vars set based on data file
		*/

		// Tile defaults
		masterTile tile = { tileName, NULL, NULL, NULL }; //Add new vars here
		std::pair<tileVars, std::smatch> key_match;
		std::string s;

		// Open tile data file
		std::ifstream tileDataFile;
		tileDataFile.open("Resource/tiles/tileData"); // Open file for read

		if (!tileDataFile.is_open()) return tile; // Return from funciton if file does not exist

		key_match = parse_line(&tileDataFile); //Parse first line

		while (tileDataFile.eof() != 1) { //Loop until end of file

			//if the current line is [Name] something and it matches the given name or default
			if (key_match.first == NAME && (key_match.second.str(1) == tileName || key_match.second.str(1) == "default")) {

				bool nameDefault = (key_match.second.str(1) == "default"); //Makes the following loop not break; on default case

				key_match = parse_line(&tileDataFile); //Parse next line

				// Keep loading parameters until next [Name] or end of file
				while (key_match.first != NAME) {
					//Set variables based on key
					switch (key_match.first) {
						//Add new vars here
					case WALKABLE: tile.walkable = std::stoi(key_match.second.str(1));
						break;
					case CONNECT: tile.connect = std::stoi(key_match.second.str(1));
						break;
					}

					if (tileDataFile.eof()) break; //Break out if the file ends

					key_match = parse_line(&tileDataFile); //Parse next line
				};
				if (!nameDefault) break; //Break out if Name is not default
			}
			else key_match = parse_line(&tileDataFile); //Parse next line
		};
		tileDataFile.close(); //Close the file $_$ 
		return tile;
	}

	masterTile loadMasterTile(std::string tileName)
	{
		Artist artist;
		//maybe make it look into a txt file with tile data?
		//maybe look for the PNG and if we dont find that then load error?
		masterTile tempTile;
		tempTile.name = tileName;

		tempTile = loadTileData(tileName);

		tempTile.tex = artist.loadTexture("Resource/tiles/" + tileName + ".png");
		if (tempTile.tex == NULL)
		{
			tempTile.tex = artist.loadTexture("Resource/tiles/error.png");
		}


		
		//else
		//{
		//	//FAILSAFE
		//	tempTile.name = "error";
		//	tempTile.walkable = true;
		//	tempTile.connect = false;
		//}
		return tempTile;
	}
	
	int getTile(std::string tileName) //gets the ID of the object in the master list
	{
		

		//look in master list to see if we already have it loaded
	
		for (int i = 0; i < masterTileList.size(); i++)
		{
			if (tileName == masterTileList[i].name)
			{
				return i;
			}
		}
	
		//if we dont have it loaded load it
		masterTile tempTile = loadMasterTile(tileName);

		//if we get an error tile
		if (tempTile.name == "error")
		{
			for (int i = 0; i < masterTileList.size(); i++)
			{
				//if we find the error tile in the master list just send it that pointer
				if (masterTileList[i].name == "error")
				{
					return i;
				}
			}
		}
		masterTileList.push_back(tempTile);
		return masterTileList.size() - 1;
	}

	std::vector<std::string> getTileNames() {
		/*
		IN: nothing
		OUT: list of tile names in tileData
		*/
		std::pair<tileVars, std::smatch> key_match;
		std::vector<std::string> tileVector = {};

		// Open tile data file
		std::ifstream tileDataFile;
		tileDataFile.open("Resource/tiles/tileData"); // Open file for read

		if (!tileDataFile.is_open()) return tileVector; // Return from funciton if file does not exist

		while (tileDataFile.eof() != 1) { //Loop until end of file

			key_match = parse_line(&tileDataFile); //Parse first line
			//if the current line is [Name]
			if (key_match.first == NAME && key_match.second.str(1) != "default") {
				tileVector.push_back(key_match.second.str(1)); //add the match to the vector
			}
		};
		tileDataFile.close(); //Close the file $_$ 
		return tileVector;
	}
};