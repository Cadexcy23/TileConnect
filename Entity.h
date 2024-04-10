#pragma once
#ifndef Artist
#include "Artist.h"
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


class Entity {
public:
	struct entity {
		std::string name;
		SDL_Texture* tex;
		Artist::pos pos;
		int direction;
		int team;

		void draw(int x, int y, int w, int h)
		{
			Artist artist;

			//get info about the texture
			int tW, tH;
			SDL_QueryTexture(tex, NULL, NULL, &tW, &tH);

			//Decide what frame to draw
			int drawFrame = clock() % tH / 144;

			//Rect to use as source
			SDL_Rect ent;
			ent.w = 144;
			ent.h = 144;
			ent.x = direction * 144;
			ent.y = drawFrame * 144;
			artist.drawImage(tex, x, y, w, h, &ent);
		}
	};

	enum entityVars { COMMENT, EMPTY, NAME, XPOS, YPOS, DIRECTION, TEAM }; //Add new vars here



	std::pair<entityVars, std::smatch> parse_line(std::istream* dataFile) {
		/*
		read line from file and parse it
		IN:  pointer to iostream data file
		OUT: key(unique identifier int), match (desired string from line)
		*/

		// Key/Regex map
		std::map<entityVars, std::regex> rx_map;
		rx_map[COMMENT] = std::regex("^\\s*//.*"); //Matches Comments
		rx_map[EMPTY] = std::regex("^\\s*$");        //Matches empty strings (with white space)
		rx_map[NAME] = std::regex("^\\s*\\[Name\\]\\s*(.*)"); //Add new vars here
		rx_map[XPOS] = std::regex("^\\s*\\[Xpos\\]\\s*(.*)");
		rx_map[YPOS] = std::regex("^\\s*\\[Ypos\\]\\s*(.*)");
		rx_map[DIRECTION] = std::regex("^\\s*\\[Direction\\]\\s*(.*)");
		rx_map[TEAM] = std::regex("^\\s*\\[Team\\]\\s*(.*)");

		static std::string s; //This must be static or the smatch var will not work
		std::smatch match;    // Regex matches
		entityVars key;         // Match key

		std::getline(*dataFile, s); //Read in a line (string\n) from the file

		for (std::map<entityVars, std::regex>::iterator iter = rx_map.begin(); iter != rx_map.end(); ++iter) {
			if (std::regex_match(s, (*iter).second)) { //If there is a match
				std::regex_search(s, match, (*iter).second); //get matches
				key = (*iter).first; //get the key associated with the match
				break; //Stop searching and return key match pair
			}
		}
		return std::make_pair(key, match); //Return key match pair
	}

	entity loadEntityData(std::string entityName) {
		/*
		Secret Genius Tile Vars Loading
		IN: tile name name string
		OUT: tile struct with vars set based on data file
		*/

		Artist artist;

		// Entity defaults
		entity ent = { entityName, NULL, NULL, NULL }; //Add new vars here
		std::pair<entityVars, std::smatch> key_match;
		std::string s;

		// Entity texture
		ent.tex = artist.loadTexture("Resource/entities/" + entityName + ".png");
		if (ent.tex == NULL)
		{
			ent.tex = artist.loadTexture("Resource/entities/error.png");
		}

		// Open tile data file
		std::ifstream entityDataFile;
		entityDataFile.open("Resource/entities/entityData"); // Open file for read

		if (!entityDataFile.is_open()) return ent; // Return from funciton if file does not exist

		key_match = parse_line(&entityDataFile); //Parse first line

		while (entityDataFile.eof() != 1) { //Loop until end of file

			//if the current line is [Name] something and it matches the given name or default
			if (key_match.first == NAME && (key_match.second.str(1) == entityName || key_match.second.str(1) == "default")) {

				bool nameDefault = (key_match.second.str(1) == "default"); //Makes the following loop not break; on default case

				key_match = parse_line(&entityDataFile); //Parse next line

				// Keep loading parameters until next [Name] or end of file
				while (key_match.first != NAME) {
					//Set variables based on key
					switch (key_match.first) {
						//Add new vars here
					case XPOS: ent.pos.x = std::stoi(key_match.second.str(1));
						break;
					case YPOS: ent.pos.y = std::stoi(key_match.second.str(1));
						break;
					case DIRECTION: ent.direction = std::stoi(key_match.second.str(1));
						break;
					case TEAM: ent.team = std::stoi(key_match.second.str(1));
						break;
					}

					if (entityDataFile.eof()) break; //Break out if the file ends

					key_match = parse_line(&entityDataFile); //Parse next line
				};
				if (!nameDefault) break; //Break out if Name is not default
			}
			else key_match = parse_line(&entityDataFile); //Parse next line
		};
		entityDataFile.close(); //Close the file $_$ 
		return ent;
	}

	//masterObject loadMasterObject(std::string objectName)
	//{
	//	Artist artist;
	//	//maybe make it look into a txt file with tile data?
	//	//maybe look for the PNG and if we dont find that then load error?
	//	masterObject tempObject;
	//	tempObject.name = objectName;
	//
	//	tempObject = loadEntityData(objectName);
	//
	//	tempObject.tex = artist.loadTexture("Resource/objects/" + objectName + ".png");
	//	if (tempObject.tex == NULL)
	//	{
	//		tempObject.tex = artist.loadTexture("Resource/objects/error.png");
	//	}
	//
	//	return tempObject;
	//}

	//int getEntity(std::string entityName) //gets the ID of the object in the master list
	//{
	//
	//
	//	//look in master list to see if we already have it loaded
	//
	//	//for (int i = 0; i < masterObjectList.size(); i++)
	//	//{
	//	//	if (objectName == masterObjectList[i].name)
	//	//	{
	//	//		return i;
	//	//	}
	//	//}
	//
	//	//if we dont have it loaded load it
	//	entity tempEntity = loadEntityData(entityName);
	//
	//	//if we get an error tile
	//	if (tempEntity.name == "error")
	//	{
	//		for (int i = 0; i < masterObjectList.size(); i++)
	//		{
	//			//if we find the error tile in the master list just send it that pointer
	//			if (masterObjectList[i].name == "error")
	//			{
	//				return i;
	//			}
	//		}
	//	}
	//	//masterObjectList.push_back(tempEntity);
	//	return masterObjectList.size() - 1;
	//}

	std::vector<std::string> getEntityNames() {
		/*
		IN: nothing
		OUT: list of entity names in tileData
		*/
		std::pair<entityVars, std::smatch> key_match;
		std::vector<std::string> entityVector = {};

		// Open tile data file
		std::ifstream entityDataFile;
		entityDataFile.open("Resource/entities/entityData"); // Open file for read

		if (!entityDataFile.is_open()) return entityVector; // Return from funciton if file does not exist

		while (entityDataFile.eof() != 1) { //Loop until end of file

			key_match = parse_line(&entityDataFile); //Parse first line
			//if the current line is [Name]
			if (key_match.first == NAME && key_match.second.str(1) != "default") {
				entityVector.push_back(key_match.second.str(1)); //add the match to the vector
			}
		};
		entityDataFile.close(); //Close the file $_$ 
		return entityVector;
	}
};