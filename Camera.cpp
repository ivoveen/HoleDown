#include "precomp.h"
#include "Camera.h"
#include "TileMap.h"
#include "Player.h"
//#include <iostream>

Camera::Camera(Surface* screen, Player* player, TileMap* tileMap) {
	//initialize vars
	mpScreen = screen;
	pPlayer = player;
	mpTileMap = tileMap;
	MAPDATA = tileMap->getMAPDATA();
	TILESIZE = tileMap->getTILESIZE();
	TILESCALE = tileMap->getTILESCALE();
	SCALEDTILESIZE = tileMap->getSCALEDTILESIZE();
	AMOUNTOFTILESINAROW = tileMap->getAMOUNTOFTILESINAROW();
	MAPWIDTH = tileMap->getMAPWIDTH();
	MAPHEIGHT = tileMap->getMAPHEIGHT();
	pTileSet = tileMap->getTileSet();

	float2 playerPos = player->getPos();
	mPos = { static_cast<int>(playerPos.x - (SCRWIDTH / 2)), static_cast<int>(playerPos.y - (SCRHEIGHT / 2)) };

	waterSurface = new Sprite(new Surface("assets/simpleWaveSpriteSheet.png"), 10);
	//load paralax layers
	for (int i = 0; i < 3; i++) {
		mpParalaxLayers[i] = new Surface(SCRWIDTH, SCRHEIGHT  );
		std::string imageAdress = "assets/" + std::to_string(i + 1) + "paralax.png";
		Sprite temp(new Surface(imageAdress.c_str()), 1);
		temp.DrawScaled(0, 0, SCRWIDTH , SCRHEIGHT, false, mpParalaxLayers[i]);
	}
	
}

Camera::~Camera() {
	delete waterSurface;
	for (int i = 0; i < 3; i++) {
		delete *(mpParalaxLayers + i);
	}
}

void Camera::draw(float deltaTime) {
	mWaterCycle += (0.01f * deltaTime);
	if (mWaterCycle >= 10) mWaterCycle = 0;

	updateCameraPos();
	renderParalax();
	renderTileMap(); 
}

void Camera::cameraRespawn(float2 playerPos) {
	mPos = { static_cast<int>(playerPos.x - (SCRWIDTH / 2)), static_cast<int>(playerPos.y - (SCRHEIGHT / 2)) };
}


void Camera::updateCameraPos() {
	float2 playerPos = pPlayer->getPos();
	float2 cameraCentrePos = float2(static_cast<float>(mPos.x + (SCRWIDTH / 2)), static_cast<float>(mPos.y + (SCRHEIGHT / 2)));
	float2 proposedMPos = mPos;

	if (pythagorean( playerPos.x, cameraCentrePos.x) > powf(static_cast<float>(PLAYEROFFCENTRERANGE.x),2) ){
		if (mCurrentPlayerOffset.x == 0)mCurrentPlayerOffset.x = playerPos.x - mPos.x;
		else if (mCurrentPlayerOffset.x >= SCRWIDTH * 3 / 4 ) mCurrentPlayerOffset.x = SCRWIDTH / 2;
		proposedMPos.x = playerPos.x - mCurrentPlayerOffset.x;
	}
	else mCurrentPlayerOffset.x = 0;

	if (pythagorean(playerPos.y, cameraCentrePos.y) > powf(static_cast<float>(PLAYEROFFCENTRERANGE.y), 2)) {
		if (mCurrentPlayerOffset.y == 0)mCurrentPlayerOffset.y = playerPos.y - mPos.y;
		else if (mCurrentPlayerOffset.y >= SCRHEIGHT *3 / 4) mCurrentPlayerOffset.y = SCRHEIGHT / 2;
		proposedMPos.y = playerPos.y - mCurrentPlayerOffset.y;
	}
	else mCurrentPlayerOffset.y = 0;

	if (proposedMPos.x + SCRWIDTH < (MAPWIDTH * SCALEDTILESIZE) && proposedMPos.x > 0) {
		mPos.x = static_cast<int>(proposedMPos.x);
	}
	
	if (proposedMPos.y + SCRHEIGHT < (MAPHEIGHT * SCALEDTILESIZE) && proposedMPos.y > 0) {
		mPos.y = static_cast<int>(proposedMPos.y);
	}
}


void Camera::renderParalax() {	
	int paralaxCutOff[3] = {0,mpParalaxLayers[1]->height / 324  * 249, 685 * mpParalaxLayers[2]->height / 1350 };
	for (int i = 0; i < 3; i++) {
		int2 paralaxPos = { static_cast<int>(-mPos.x / ((3 - i) * 2)), static_cast<int>(-mPos.y / ((3 - i) * 9 )) + i * i * 150 };
		paralaxPos.x += mpParalaxLayers[i]->width * static_cast<int>(-paralaxPos.x / mpParalaxLayers[i]->width);
		mpParalaxLayers[i]->CopyTo(paralaxCutOff[i], mpScreen, paralaxPos.x, paralaxPos.y);
		mpParalaxLayers[i]->CopyTo(paralaxCutOff[i], mpScreen, paralaxPos.x + mpParalaxLayers[i]->width, paralaxPos.y);
	}
}
void Camera::renderTileMap() {
	int tile; //An int identifying which tile should be placed.
	//Select the tiles that are in view
	int firstTileInRow = (mPos.x / SCALEDTILESIZE) + ((mPos.y / SCALEDTILESIZE) * MAPWIDTH); 
	for (int row = 0; row <= SCRHEIGHT / SCALEDTILESIZE + 1; row++) {
		for (int collumn = 0; collumn <= SCRWIDTH / SCALEDTILESIZE + 1; collumn++) {
			tile = MAPDATA[firstTileInRow + collumn];
			int x = ((firstTileInRow + collumn) % MAPWIDTH) * SCALEDTILESIZE - mPos.x;
			int y = static_cast<int>(floor((firstTileInRow + collumn) / MAPWIDTH)) * SCALEDTILESIZE - mPos.y;
			/*screen->Line(x, 0, x, SCRHEIGHT, 255);
			screen->Line(0, y, SCRWIDTH, y, 255);*/
			switch (tile){
			case -1:
				//there is no tile to be drawn
				break;
			case 51:
				waterSurface->SetFrame((((firstTileInRow + collumn % MAPWIDTH) + 1) + static_cast<int>(mWaterCycle)) % 10);
				waterSurface->DrawScaled(x, y, TILESIZE * TILESCALE, TILESIZE * TILESCALE, false, mpScreen);
				break;
			default:
				renderTile(x, y, tile, SCALEDTILESIZE);
				break;
			}
		}
		firstTileInRow += MAPWIDTH; //move on to the next row.
	}
}

void Camera::renderTile(int x, int y, int tile, int scaledTileSize) {
	int tileRow = static_cast<int>(floor(tile / AMOUNTOFTILESINAROW));
	int firstPixelInRow = tileRow * (pTileSet->width * TILESIZE) + (tile % AMOUNTOFTILESINAROW) * TILESIZE;
	for (int yTile = 0; yTile < scaledTileSize; yTile++) {
		for (int xTile = 0; xTile < scaledTileSize; xTile++) {
			uint c = *(pTileSet->pixels + firstPixelInRow + pTileSet->width * static_cast<int>(yTile / TILESCALE) + xTile / TILESCALE);
			if (c != 0x000000) mpScreen->Plot(x + xTile, y + yTile, c); //white is transparent
		}
	}
}

