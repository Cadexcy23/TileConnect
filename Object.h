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



class Object {
public:
	struct masterObject {
		std::string name; // PRIMARY KEY MUST BE UNIQUE
		SDL_Texture* tex; //spriteSheet for animation and connection
		//maybe add a way it can connect to other textures? perhaps by name prefix?
		bool connect; //if the tile should test for connectivity
		bool walkable;
	};

	struct object {
		int conType[4]; //used to determine the way the texture should connect visualy
		int objectType;



		void draw(int x, int y, int w, int h)
		{
			//if empty object
			if (masterObjectList[objectType].name == "empty")
				return;

			Artist artist;


			//get info about the texture
			int tW, tH;
			SDL_QueryTexture(masterObjectList[objectType].tex, NULL, NULL, &tW, &tH);

			//Decide what freame to draw
			int drawFrame = clock() % tH / 144;

			//Rect to use as source
			SDL_Rect obj;
			obj.w = 72;
			obj.h = 72;


			if (masterObjectList[objectType].connect)
			{
				//Top left Quad
				obj.x = conType[0] * 144;
				obj.y = drawFrame * 144;
				artist.drawImage(masterObjectList[objectType].tex, x, y, w / 2, h / 2, &obj);

				//Top right Quad
				obj.x = conType[1] * 144 + 72;
				obj.y = drawFrame * 144;
				artist.drawImage(masterObjectList[objectType].tex, x + w / 2, y, w / 2 + 1, h / 2, &obj);//MIGHT NEED TO ADD A CHECK TO SEE IF WE HAVE A REMAINDER AND DRAW 1PX FARTHER instead of always

				//Bottom left Quad
				obj.x = conType[2] * 144;
				obj.y = drawFrame * 144 + 72;
				artist.drawImage(masterObjectList[objectType].tex, x, y + h / 2, w / 2, h / 2 + 1, &obj);

				//Bottom right Quad
				obj.x = conType[3] * 144 + 72;
				obj.y = drawFrame * 144 + 72;
				artist.drawImage(masterObjectList[objectType].tex, x + w / 2, y + h / 2, w / 2 + 1, h / 2 + 1, &obj);
			}
			else
			{
				obj.w = 144;
				obj.h = 144;
				obj.x = 0;
				obj.y = drawFrame * 144;
				artist.drawImage(masterObjectList[objectType].tex, x, y, w, h, &obj);
			}
		}
	};

	static std::vector<masterObject> masterObjectList;

	enum objectVars { COMMENT, EMPTY, NAME, WALKABLE, CONNECT }; //Add new vars here



	std::pair<objectVars, std::smatch> parse_line(std::istream* dataFile) {
		/*
		read line from file and parse it
		IN:  pointer to iostream data file
		OUT: key(unique identifier int), match (desired string from line)
		*/

		// Key/Regex map
		std::map<objectVars, std::regex> rx_map;
		rx_map[COMMENT] = std::regex("^\\s*//.*"); //Matches Comments
		rx_map[EMPTY] = std::regex("^\\s*$");        //Matches empty strings (with white space)
		rx_map[NAME] = std::regex("^\\s*\\[Name\\]\\s*(.*)"); //Add new vars here
		rx_map[WALKABLE] = std::regex("^\\s*\\[Walkable\\]\\s*(.*)");
		rx_map[CONNECT] = std::regex("^\\s*\\[Connect\\]\\s*(.*)");

		static std::string s; //This must be static or the smatch var will not work
		std::smatch match;    // Regex matches
		objectVars key;         // Match key

		std::getline(*dataFile, s); //Read in a line (string\n) from the file

		for (std::map<objectVars, std::regex>::iterator iter = rx_map.begin(); iter != rx_map.end(); ++iter) {
			if (std::regex_match(s, (*iter).second)) { //If there is a match
				std::regex_search(s, match, (*iter).second); //get matches
				key = (*iter).first; //get the key associated with the match
				break; //Stop searching and return key match pair
			}
		}
		return std::make_pair(key, match); //Return key match pair
	}

	masterObject loadObjectData(std::string objectName) {
		/*
		Secret Genius Tile Vars Loading
		IN: tile name name string
		OUT: tile struct with vars set based on data file
		*/

		// Tile defaults
		masterObject object = { objectName, NULL, NULL, NULL }; //Add new vars here
		std::pair<objectVars, std::smatch> key_match;
		std::string s;

		// Open tile data file
		std::ifstream objectDataFile;
		objectDataFile.open("Resource/objects/objectData"); // Open file for read

		if (!objectDataFile.is_open()) return object; // Return from funciton if file does not exist

		key_match = parse_line(&objectDataFile); //Parse first line

		while (objectDataFile.eof() != 1) { //Loop until end of file

			//if the current line is [Name] something and it matches the given name or default
			if (key_match.first == NAME && (key_match.second.str(1) == objectName || key_match.second.str(1) == "default")) {

				bool nameDefault = (key_match.second.str(1) == "default"); //Makes the following loop not break; on default case

				key_match = parse_line(&objectDataFile); //Parse next line

				// Keep loading parameters until next [Name] or end of file
				while (key_match.first != NAME) {
					//Set variables based on key
					switch (key_match.first) {
						//Add new vars here
					case WALKABLE: object.walkable = std::stoi(key_match.second.str(1));
						break;
					case CONNECT: object.connect = std::stoi(key_match.second.str(1));
						break;
					}

					if (objectDataFile.eof()) break; //Break out if the file ends

					key_match = parse_line(&objectDataFile); //Parse next line
				};
				if (!nameDefault) break; //Break out if Name is not default
			}
			else key_match = parse_line(&objectDataFile); //Parse next line
		};
		objectDataFile.close(); //Close the file $_$ 
		return object;
	}

	masterObject loadMasterObject(std::string objectName)
	{
		Artist artist;
		//maybe make it look into a txt file with tile data?
		//maybe look for the PNG and if we dont find that then load error?
		masterObject tempObject;
		tempObject.name = objectName;

		tempObject = loadObjectData(objectName);

		tempObject.tex = artist.loadTexture("Resource/objects/" + objectName + ".png");
		if (tempObject.tex == NULL)
		{
			tempObject.tex = artist.loadTexture("Resource/objects/error.png");
		}

		return tempObject;
	}

	int getObject(std::string objectName) //gets the ID of the object in the master list
	{


		//look in master list to see if we already have it loaded

		for (int i = 0; i < masterObjectList.size(); i++)
		{
			if (objectName == masterObjectList[i].name)
			{
				return i;
			}
		}

		//if we dont have it loaded load it
		masterObject tempObject = loadMasterObject(objectName);

		//if we get an error object
		if (tempObject.name == "error")
		{
			for (int i = 0; i < masterObjectList.size(); i++)
			{
				//if we find the error tile in the master list just send it that pointer
				if (masterObjectList[i].name == "error")
				{
					return i;
				}
			}
		}
		masterObjectList.push_back(tempObject);
		return masterObjectList.size() - 1;
	}

	std::vector<std::string> getObjectNames() {
		/*
		IN: nothing
		OUT: list of tile names in tileData
		*/
		std::pair<objectVars, std::smatch> key_match;
		std::vector<std::string> objectVector = {};

		// Open tile data file
		std::ifstream objectDataFile;
		objectDataFile.open("Resource/objects/objectData"); // Open file for read

		if (!objectDataFile.is_open()) return objectVector; // Return from funciton if file does not exist

		while (objectDataFile.eof() != 1) { //Loop until end of file

			key_match = parse_line(&objectDataFile); //Parse first line
			//if the current line is [Name]
			if (key_match.first == NAME && key_match.second.str(1) != "default") {
				objectVector.push_back(key_match.second.str(1)); //add the match to the vector
			}
		};
		objectDataFile.close(); //Close the file $_$ 
		return objectVector;
	}
};