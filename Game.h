// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#pragma once
class Player;
class Camera;
class TileMap;
class GameObject;
class ScoreManager;


class Game : public TheApp
{
public:

	// game flow methods
	void Init();
	void setNumberOfGameObjects(int numberOfObjects) { mNumberOfGameObjects = numberOfObjects; }
	bool getShadersOn() { return shadersOn; }
	void Tick( float deltaTime );
	void drawScaledScreen();
	void saveScore();
	void Shutdown();
	// input handling
	void MouseUp(int) {};
	void MouseDown(int);
	void MouseMove( int x, int y ) { mousePos.x = x, mousePos.y = y; }
	void MouseWheel( float ) { /* implement if you want to handle the mouse wheel */ }
	void KeyUp( int ) { /* implement if you want to handle keys */ }
	void KeyDown( int ) { /* implement if you want to handle keys */ }
	// data members
	int2 mousePos;
	GameObject** pGameObjects;
private:
	enum GameState {
		start, settings, play, lose, win
	}mGameState = start;
	ScoreManager* mScoreManager;
	Camera* mCamera;
	Sprite* mUnscaledScreenSprite;
	Surface* mScreenUnscaled;
	Player* myPlayer;
	TileMap* myTileMap;
	const float mMAXSCALE = 1.0f;
	const float mMINSCALE = 1.3f;
	const float mWINSCORE = 10000;
	float mFactor = 0;
	float mScale = mMINSCALE;
	int mFrameCount = 0;
	int mNumberOfGameObjects = 238;
	const float mLOCALSPACERADIUS = SCRWIDTH;
	float totalTime = 0;
	bool shadersOn = true;
};

// namespace Tmpl8