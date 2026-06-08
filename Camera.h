#pragma once
#include "TileMap.h"
#include "sprite.h"
#include "surface.h"
class Player;

class Camera
{
public:
	Camera(Surface* screen, Player* playerPos, TileMap* tileMap);
	~Camera();
	void draw(float deltaTime);
	void cameraRespawn(float2 playerPos);
	float2 getPos() { return mPos; };
private:
	void updateCameraPos();
	void renderParalax();
	void renderTileMap();
	void renderTile(int x, int y, int tile, int scaledTileSize);


	int2 mPos = int2(0, 0); //top left corner of the view
	Sprite* waterSurface;
	Surface* mpParalaxLayers[3];
	Surface* mpScreen;
	Player* pPlayer;
	TileMap* mpTileMap;
	float mWaterCycle = 0;
	const int PARALAXSCALE = 3;
	const int2 PLAYEROFFCENTRERANGE = { 200, 100 };
	float2 mCurrentPlayerOffset = { 0, 0 };

	const int* MAPDATA{};
	int TILESIZE = 0;
	int TILESCALE = 0;
	int SCALEDTILESIZE = 0;
	int AMOUNTOFTILESINAROW = 0;
	int MAPWIDTH = 0;
	int MAPHEIGHT = 0;
	Surface* pTileSet;
};

