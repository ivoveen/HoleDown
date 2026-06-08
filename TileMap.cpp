#include "precomp.h"
#include "TileMap.h"
#include "GameObject.h"
#include "Rope.h"
#include "Pit.h"
#include "Boulder.h"
#include "Bat.h"
#include "Eel.h"
#include "Scorpion.h"
#include "Frog.h"
#include "Coin.h"
#include "Balloon.h"
#include "Checkpoint.h"
#include <fstream>      // std::ifstream

//this simple char loader function called LoadText was not written by me. It was written by Brian and sent on teams
//However, I do understand how this function works.

TileMap::TileMap() {
	TileSet = new Surface("assets/nature_platformer_tileset.tga");
	AMOUNTOFTILESINAROW = (TileSet->width / TILESIZE);


	// load in map from file
	const char* text = ReadFile(pMAPFILE);
	//convert c string to int array
	pMAPDATA = ConvertCStringToInt(text, true);

	const char* objectText = ReadFile(pGAMEOBJECTSFILE);
	pGAMEOBJECTDATA = ConvertCStringToInt(objectText, false);

}
TileMap::~TileMap() {
	//GameObject** hi = mpGameObjects;
	delete TileSet;
	for (int i = 0; i < 238; i++)
	{
		delete *(mpGameObjects+ i);
	}
}

//I copied this ReadFile function from a simple char loader Brian sent on teams
//this is the message it was sent in: https://teams.microsoft.com/l/message/19:DSuROJlM1MwLXswhFWVIS1JOdnOpqe696fJQmuMw-s81@thread.tacv2/1694006253136?tenantId=0a33589b-0036-4fe8-a829-3ed0926af886&groupId=c716272b-8f9c-4d18-800e-1e2807521a00&parentMessageId=1694006253136&teamName=2023-24A%20FGA1.P1%20PR&channelName=General&createdTime=1694006253136&allowXTenantAccess=false
const char* TileMap::ReadFile(char const* filename) {
	ifstream myfile(filename);

	static std::string line; // need to keep this in scope when it returns.
	if (myfile.is_open())
	{
		getline(myfile, line, (char)myfile.eof()); // *this.eof isn't ideal but it works
		myfile.close();
	}
	myfile.close();
	return line.c_str();
}

const int* TileMap::ConvertCStringToInt(const char cString[], bool tileMap) {
	//int data[150 * 100];
	int j = 0;
	int prevNum = -1;
	for (int i = 0; cString[i] != 0; i++) {
		if (cString[i] != 44 && cString[i] != 10) {
			if (cString[i] == 45) {
				//in case a - is read assume -1 and dont read the 1.
				i++;
				prevNum = -1;
			}
			else {
				//also handle numbers with double digits.
				if (prevNum == -1) prevNum = cString[i] - 48;
				else prevNum = prevNum * 10 + (cString[i] - 48);
			}
		}
		else {
			if (tileMap)tileMapData[j] = prevNum;
			else gameObjectData[j] = prevNum;
			j++;
			prevNum = -1;
		}
	}
	if (tileMap)return tileMapData;
	else return gameObjectData;
}


GameObject** TileMap::getGameObjectData(Player* player, Camera* camera, Surface* screen, ScoreManager* scoreManager) {
	int count = 0;
	for (int i = 0; i < MAPWIDTH; i++) {
		for (int j = 0; j < MAPHEIGHT; j++) {
			switch (pGAMEOBJECTDATA[i + (j * MAPWIDTH)]) {
			case -1:
				count--;
				break;
			case 0:
				mpGameObjects[count] = new Rope({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) }, player, camera, screen);
				break;
			case 1:
				mpGameObjects[count] = new Balloon({ static_cast<float>(i * SCALEDTILESIZE) + 7, static_cast<float>(j * SCALEDTILESIZE) - 36 }, player, camera, screen);
				break;
			case 2:
				mpGameObjects[count] = new Pit({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE - 2) }, player, camera, screen, Pit::pit);
				break;
			case 3:
				mpGameObjects[count] = new Pit({ static_cast<float>(i * SCALEDTILESIZE),static_cast<float>(j * SCALEDTILESIZE - 2) }, player, camera, screen, Pit::shrinkingPit);
				break;
			case 4:
				mpGameObjects[count] = new Pit({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE - 2) }, player, camera, screen, Pit::waterPit);
				break;
			case 5:
				mpGameObjects[count] = new Pit({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE - 2) }, player, camera, screen, Pit::waterPitWithCrocs);
				break;
			case 6:
				mpGameObjects[count] = new Bat({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE + 20) }, player, camera, screen, MAPWIDTH);
				break;
			case 7:
				mpGameObjects[count] = new Frog({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) }, player, camera, screen, MAPWIDTH);
				break;
			case 8:
				mpGameObjects[count] = new Boulder({ static_cast<float>(i * SCALEDTILESIZE) + 26, static_cast<float>(j * SCALEDTILESIZE) + 26 }, player, camera, screen, MAPWIDTH);
				break;
			case 9:
				mpGameObjects[count] = new Eel({ static_cast<float>(i * SCALEDTILESIZE) + 26, static_cast<float>(j * SCALEDTILESIZE) + 26 }, player, camera, screen, MAPWIDTH);
				break;
			case 10:
				mpGameObjects[count] = new Scorpion({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) + 24 }, player, camera, screen, MAPWIDTH);
				break;
			case 11:
				mpGameObjects[count] = new Coin({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) }, player, camera, screen, scoreManager, Coin::yellow);
				break;
			case 12:
				mpGameObjects[count] = new Coin({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) }, player, camera, screen, scoreManager, Coin::red);
				break;
			case 13:
				mpGameObjects[count] = new Coin({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) }, player, camera, screen, scoreManager, Coin::blue);
				break;
			case 14:
				mpGameObjects[count] = new Coin({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) }, player, camera, screen, scoreManager, Coin::green);
				break;
			case 15:
				mpGameObjects[count] = new Checkpoint({ static_cast<float>(i * SCALEDTILESIZE), static_cast<float>(j * SCALEDTILESIZE) - 32}, player, camera, screen);
				break;
			}
			count++;
			//int test2 = pGAMEOBJECTDATA[i + (j * MAPWIDTH)];
		}
	}
	return mpGameObjects;
}