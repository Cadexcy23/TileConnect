#include "Map.h"
#include "Menu.h"
#include "Controller.h"



Map::map Map::activeMap;
std::vector<Tile::masterTile> Tile::masterTileList;
std::vector<Object::masterObject> Object::masterObjectList;
float Map::mapZoomGoal = 1;
float actualMapZoom = 1;
Artist::pos cameraPos = { 0, 0 };
Artist::pos topLeftScreenPos = { 0, 0 };
int camScrollSpeed = 2;



void resizeMap(int sizeX, int sizeY)
{
	//resize tile grid
	Map::activeMap.tileGrid.resize(sizeX);
	for (int i = 0; i < Map::activeMap.tileGrid.size(); i++)
	{
		Map::activeMap.tileGrid[i].resize(sizeY);
	}
	//resize object grid
	Map::activeMap.objectGrid.resize(sizeX);
	for (int i = 0; i < Map::activeMap.objectGrid.size(); i++)
	{
		Map::activeMap.objectGrid[i].resize(sizeY);
	}

}

int decideVariant(bool left, bool right, bool straight)
{
	if (!left && !right && straight)
	{
		return 0;
	}
	else if (left && right && straight)
	{
		return 1;
	}
	else if (left && right && !straight)
	{
		return 2;
	}
	else if (!left && right)
	{
		return 3;
	}
	else if (left && !right)
	{
		return 4;
	}
	else
	{
		return 0;
	}
}

void updateTileConnections(Artist::pos gridPos)
{
	//get pointer of current tile
	Tile::tile* currentTile = &Map::activeMap.tileGrid[gridPos.x][gridPos.y];

	//if the tile even connects
	if (Tile::masterTileList[currentTile->tileType].connect)
	{
		//Top left Quad
		//set base bools for checking connection
		bool left = false;
		bool right = false;
		bool straight = false;

		//check left
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.tileGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x - 1][gridPos.y].tileType == currentTile->tileType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.tileGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x][gridPos.y - 1].tileType == currentTile->tileType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.tileGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x - 1][gridPos.y - 1].tileType == currentTile->tileType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentTile->conType[0] = decideVariant(left, right, straight);

		//Top right Quad
		//reset flags
		left = false;
		right = false;
		straight = false;

		//check left
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.tileGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x][gridPos.y - 1].tileType == currentTile->tileType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.tileGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x + 1][gridPos.y].tileType == currentTile->tileType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.tileGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x + 1][gridPos.y - 1].tileType == currentTile->tileType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentTile->conType[1] = decideVariant(left, right, straight);

		//Bottom left Quad
		//reset flags
		left = false;
		right = false;
		straight = false;

		//check left
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.tileGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x][gridPos.y + 1].tileType == currentTile->tileType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.tileGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x - 1][gridPos.y].tileType == currentTile->tileType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.tileGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x - 1][gridPos.y + 1].tileType == currentTile->tileType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentTile->conType[2] = decideVariant(left, right, straight);

		//Bottem right Quad
		//reset flags
		left = false;
		right = false;
		straight = false;

		//check left
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.tileGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x + 1][gridPos.y].tileType == currentTile->tileType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.tileGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x][gridPos.y + 1].tileType == currentTile->tileType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.tileGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.tileGrid[0].size())
		{
			if (Map::activeMap.tileGrid[gridPos.x + 1][gridPos.y + 1].tileType == currentTile->tileType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentTile->conType[3] = decideVariant(left, right, straight);
	}
	else
	{
	currentTile->conType[0] = 0;
	currentTile->conType[1] = 0;
	currentTile->conType[2] = 0;
	currentTile->conType[3] = 0;
	}
}

void updateObjectConnections(Artist::pos gridPos)
{
	//get pointer of current object
	Object::object* currentObject = &Map::activeMap.objectGrid[gridPos.x][gridPos.y];

	//if the object even connects
	if (Object::masterObjectList[currentObject->objectType].connect)
	{
		//Top left Quad
		//set base bools for checking connection
		bool left = false;
		bool right = false;
		bool straight = false;

		//check left
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.objectGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x - 1][gridPos.y].objectType == currentObject->objectType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.objectGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x][gridPos.y - 1].objectType == currentObject->objectType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.objectGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x - 1][gridPos.y - 1].objectType == currentObject->objectType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentObject->conType[0] = decideVariant(left, right, straight);

		//Top right Quad
		//reset flags
		left = false;
		right = false;
		straight = false;

		//check left
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.objectGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x][gridPos.y - 1].objectType == currentObject->objectType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.objectGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x + 1][gridPos.y].objectType == currentObject->objectType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.objectGrid.size() && gridPos.y - 1 >= 0 && gridPos.y - 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x + 1][gridPos.y - 1].objectType == currentObject->objectType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentObject->conType[1] = decideVariant(left, right, straight);

		//Bottom left Quad
		//reset flags
		left = false;
		right = false;
		straight = false;

		//check left
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.objectGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x][gridPos.y + 1].objectType == currentObject->objectType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.objectGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x - 1][gridPos.y].objectType == currentObject->objectType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x - 1 >= 0 && gridPos.x - 1 < Map::activeMap.objectGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x - 1][gridPos.y + 1].objectType == currentObject->objectType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentObject->conType[2] = decideVariant(left, right, straight);

		//Bottem right Quad
		//reset flags
		left = false;
		right = false;
		straight = false;

		//check left
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.objectGrid.size() && gridPos.y >= 0 && gridPos.y < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x + 1][gridPos.y].objectType == currentObject->objectType)
			{
				left = true;
			}
		}
		//check right
		if (gridPos.x >= 0 && gridPos.x < Map::activeMap.objectGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x][gridPos.y + 1].objectType == currentObject->objectType)
			{
				right = true;
			}
		}
		//check straight
		if (gridPos.x + 1 >= 0 && gridPos.x + 1 < Map::activeMap.objectGrid.size() && gridPos.y + 1 >= 0 && gridPos.y + 1 < Map::activeMap.objectGrid[0].size())
		{
			if (Map::activeMap.objectGrid[gridPos.x + 1][gridPos.y + 1].objectType == currentObject->objectType)
			{
				straight = true;
			}
		}
		//decide what variant to use
		currentObject->conType[3] = decideVariant(left, right, straight);
	}
	else
	{
		currentObject->conType[0] = 0;
		currentObject->conType[1] = 0;
		currentObject->conType[2] = 0;
		currentObject->conType[3] = 0;
	}
}


void Map::init()
{
	//TEMP map loading
	Tile tile;
	Object object;
	Entity entity;

	resizeMap(192, 108);

	//load tiles into master list
	std::vector<std::string> allTiles = tile.getTileNames();
	for (int i = 0; i < allTiles.size(); i++)
	{
		tile.getTile(allTiles[i]);
	}
	//load objects onto master list
	std::vector<std::string> allObjects = object.getObjectNames();
	for (int i = 0; i < allObjects.size(); i++)
	{
		object.getObject(allObjects[i]);
	}
	//load all entities into map
	//std::vector<std::string> allEntities = entity.getEntityNames();
	//for (int i = 0; i < allEntities.size(); i++)
	//{
	//	activeMap.entityList.push_back(entity.loadEntityData(allEntities[i]));
	//}

	//set each tile and object
	for (int x = 0; x < activeMap.tileGrid.size(); x++)
	{
		for (int y = 0; y < activeMap.tileGrid[x].size(); y++)
		{
			activeMap.tileGrid[x][y].tileType = rand() % Tile::masterTileList.size();
			activeMap.objectGrid[x][y].objectType = Object::masterObjectList.size() - 1;//set all to clear
			//10% chance of a random object
			if (rand() % 10 == 0)
			{
				if (Tile::masterTileList[activeMap.tileGrid[x][y].tileType].walkable)
				{
					activeMap.objectGrid[x][y].objectType = rand() % (Object::masterObjectList.size() - 1);
				}
			}
		}
	}

	
	

	//TEMP
	for (int x = 0; x < activeMap.tileGrid.size(); x++)
	{
		activeMap.tileGrid[x][0].tileType = tile.getTile("error");
		//activeMap.objectGrid[x][0].objectType = object.getObject("wall");
	}
	for (int x = 0; x < activeMap.tileGrid.size(); x++)
	{
		activeMap.tileGrid[x][activeMap.tileGrid[x].size() - 1].tileType = tile.getTile("error");
		//activeMap.objectGrid[x][activeMap.objectGrid[x].size() - 1].objectType = object.getObject("wall");
	}
	for (int y = 0; y < activeMap.tileGrid[0].size(); y++)
	{
		activeMap.tileGrid[0][y].tileType = tile.getTile("error");
		//activeMap.objectGrid[0][y].objectType = object.getObject("wall");
	}
	for (int y = 0; y < activeMap.tileGrid[0].size(); y++)
	{
		activeMap.tileGrid[activeMap.tileGrid.size() - 1][y].tileType = tile.getTile("error");
		//activeMap.objectGrid[activeMap.objectGrid.size() - 1][y].objectType = object.getObject("wall");
	}



	//update the visual connectivity of each tile and object MUST BE LAST
	for (int x = 0; x < activeMap.tileGrid.size(); x++)
	{
		for (int y = 0; y < activeMap.tileGrid[x].size(); y++)
		{
			updateTileConnections({ x, y });
			updateObjectConnections({ x, y });
		}
	}
}

void Map::controller()
{
	if (Menu::gameState == Menu::IN_GAME)
	{
		if (Controller::mouseWheelMovment.y == -1)
		{
			mapZoomGoal -= 0.05;
			if (mapZoomGoal < 0.5)
			{
				mapZoomGoal = 0.5;
			}
		}
		if (Controller::mouseWheelMovment.y == 1)
		{
			mapZoomGoal += 0.05;
			if (mapZoomGoal > 4)// 2
			{
				mapZoomGoal = 4;
			}
		}
		if (Controller::keyboardStates[SDL_SCANCODE_Z] == 1)
		{
			mapZoomGoal = 1;
		}
		if (Controller::mouseStates[1])
		{
			float changeX = (Controller::mouseX - Controller::lastMousePos.x) / actualMapZoom;
			float changeY = (Controller::mouseY - Controller::lastMousePos.y) / actualMapZoom;
			if (abs(changeX) > 0 && abs(changeX) < 1)
			{
				if (changeX > 0)
				{
					changeX = 1;
				}
				else
				{
					changeX = -1;
				}
			}
			if (abs(changeY) > 0 && abs(changeY) < 1)
			{
				if (changeY > 0)
				{
					changeY = 1;
				}
				else
				{
					changeY = -1;
				}
			}
			cameraPos.x -= changeX;
			cameraPos.y -= changeY;
		}

		//60 a sec lock
		if (Controller::FPSLock)
		{
			if (Controller::keyboardStates[SDL_SCANCODE_W])
			{
				cameraPos.y-= camScrollSpeed;
			}
			if (Controller::keyboardStates[SDL_SCANCODE_S])
			{
				cameraPos.y+= camScrollSpeed;
			}
			if (Controller::keyboardStates[SDL_SCANCODE_A])
			{
				cameraPos.x-= camScrollSpeed;
			}
			if (Controller::keyboardStates[SDL_SCANCODE_D])
			{
				cameraPos.x+= camScrollSpeed;
			}
		}
	}
}

void updateZoom()
{
	if (Controller::FPSLock)
	{
		float changeNeeded = actualMapZoom - Map::mapZoomGoal;
		changeNeeded *= 0.2;
		if (abs(changeNeeded) < 0.001)
		{
			changeNeeded = actualMapZoom - Map::mapZoomGoal;
		}
		actualMapZoom -= changeNeeded;

		//keep track of where the top left of the screen relative to the in game units should be
		int activeTileSize = 128 * actualMapZoom;
		float activeZoom = float(activeTileSize) / 128;
		topLeftScreenPos.x = cameraPos.x * activeZoom - Artist::SCREEN_WIDTH / 2;
		topLeftScreenPos.y = cameraPos.y * activeZoom - Artist::SCREEN_HEIGHT / 2;
	}
}

void Map::draw()
{
	if (Menu::gameState == Menu::IN_GAME)
	{
		updateZoom();

		//size of tiles based on zoom
		int tileSize = 128 * actualMapZoom;
		
		//amount of tiles to draw for X and Y
		Artist::pos cullingSize = { Artist::SCREEN_WIDTH / tileSize + 2, Artist::SCREEN_HEIGHT / tileSize + 2 };

		//tiles
		for (int x = 0; x < cullingSize.x; x++)
		{
			for (int y = 0; y < cullingSize.y; y++)
			{
				//the finalized camera position
				Artist::pos finalCamPos = { topLeftScreenPos.x , topLeftScreenPos.y };
				//the finalized tile we are going to draw
				Artist::pos tileToDraw = { x + finalCamPos.x / tileSize , y + finalCamPos.y / tileSize };
				//the finalized screen pos we are going to draw the tile
				Artist::pos tileDrawPos = { x * tileSize - finalCamPos.x % tileSize , y * tileSize - finalCamPos.y % tileSize };

				//make sure we are in the map
				if (tileToDraw.x >= 0 && tileToDraw.x < activeMap.tileGrid.size() && tileToDraw.y >= 0 && tileToDraw.y < activeMap.tileGrid[0].size())
				{
					activeMap.tileGrid[tileToDraw.x][tileToDraw.y].draw(tileDrawPos.x, tileDrawPos.y, tileSize, tileSize);
				}
			}
		}

		//entities, working but maybe just a little ugly
		//get the actual number the tiles are a multiple of
		float activeZoom = float(tileSize) / 128;
		//the render size of the entity
		int entitySize = 144 * activeZoom;//maybe only add 1 if we need too and how to know that is TBD
		//the finalized camera position
		//the offset of the tile up left
		int entityOffset = entitySize / 18;
		Artist::pos finalCamPos = { topLeftScreenPos.x , topLeftScreenPos.y };
		for (int i = 0; i < activeMap.entityList.size(); i++)
		{
			//the finalized tile we are going to draw
			Artist::pos entityToDraw = { activeMap.entityList[i].pos.x + finalCamPos.x / tileSize , activeMap.entityList[i].pos.y + finalCamPos.y / tileSize };
			//the finalized screen pos we are going to draw the tile
			//Artist::pos entityDrawPos = { activeMap.entityList[i].pos.x * tileSize - finalCamPos.x % tileSize , activeMap.entityList[i].pos.y * tileSize - finalCamPos.y % tileSize };
			Artist::pos entityDrawPos = { activeMap.entityList[i].pos.x * tileSize - finalCamPos.x , activeMap.entityList[i].pos.y * tileSize - finalCamPos.y };

			activeMap.entityList[i].draw(entityDrawPos.x - entityOffset, entityDrawPos.y - entityOffset, entitySize, entitySize);
		}

		//objects, working but maybe just a little ugly
		//the render size of the entity
		int objectSize = 144 * activeZoom + 1;//maybe only add 1 if we need too and how to know that is TBD
		//the finalized camera position
		//the offset of the tile up left
		int objectOffset = objectSize / 18 + 1;
		for (int x = 0; x < cullingSize.x; x++)
		{
			for (int y = 0; y < cullingSize.y; y++)
			{
				//the finalized tile we are going to draw
				Artist::pos objectToDraw = { x + finalCamPos.x / tileSize , y + finalCamPos.y / tileSize };
				//the finalized screen pos we are going to draw the tile
				Artist::pos objectDrawPos = { x * tileSize - finalCamPos.x % tileSize , y * tileSize - finalCamPos.y % tileSize };

				//make sure we are in the map
				if (objectToDraw.x >= 0 && objectToDraw.x < activeMap.tileGrid.size() && objectToDraw.y >= 0 && objectToDraw.y < activeMap.tileGrid[0].size())
				{
					activeMap.objectGrid[objectToDraw.x][objectToDraw.y].draw(objectDrawPos.x - objectOffset, objectDrawPos.y - objectOffset, objectSize, objectSize);
				}
			}
		}

	}
}