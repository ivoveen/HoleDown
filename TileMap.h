#pragma once
class Player;
class Camera;
class GameObject;
class ScoreManager;

class TileMap
{
public:
	const int* getMAPDATA() const { return pMAPDATA; };
	GameObject** getGameObjectData(Player* player, Camera* camera, Surface* screen, ScoreManager* scoreManager);

	const int getTILESIZE() const { return TILESIZE; };
	const int getTILESCALE() const { return TILESCALE; };
	const int getSCALEDTILESIZE() const { return SCALEDTILESIZE; };
	const int getAMOUNTOFTILESINAROW() const { return AMOUNTOFTILESINAROW; };
	const int getMAPWIDTH() const { return MAPWIDTH; };
	const int getMAPHEIGHT() const { return MAPHEIGHT; };
	Surface* getTileSet() { return TileSet; };

	TileMap();
	~TileMap();
	const char* ReadFile(char const* filename);
	const int* ConvertCStringToInt(const char* cString, bool tileMap);
private:
	Surface* TileSet;
	const int* pMAPDATA;
	int tileMapData[150 * 100];
	const int* pGAMEOBJECTDATA;
	int gameObjectData[150 * 100];
	GameObject* mpGameObjects[238]{};
	const int TILESIZE = 16;
	const int TILESCALE = 4;
	const int SCALEDTILESIZE = TILESIZE * TILESCALE;
	int AMOUNTOFTILESINAROW =0;
	const int MAPWIDTH = 150;
	const int MAPHEIGHT = 100;

	const char* pMAPFILE = "assets/pitfall2mapWithGameObjects_TileMap.csv";
	const char* pGAMEOBJECTSFILE = "assets/pitfall2mapWithGameObjects_GameObjects.csv";

};

