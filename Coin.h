#pragma once
#include "GameObject.h"
class Player;
class Camera;
class ScoreManager;
namespace Audio {
	class Sound;
};
class Coin : public GameObject
{
public:
	enum CoinType {
		yellow, red, blue, green
	};

	Coin(float2 position, Player* player, Camera* camera, Surface* screen, ScoreManager* scoreManager, CoinType typeOfCoin);
	~Coin();

	void tick(float deltaTime);
	void CollisionDetection();
	void draw();


private:
	void Disappear();
	enum CoinState {
		normal, collected
	} mCoinState = normal;

	ScoreManager* mpScoreManager;
	int coinValues[4] = {
		100,250,500,1000
	};
	const float mRADIUS = powf(20,2);
	const float mDISAPPEARSPEED = 150;
	float2 mStartPosition;
	Sprite* mpCoin;
	const int mMAXBOBHEIGHT = 15;
	float mElapsedTime = 0.001f;
	CoinType mCoinType;
	Audio::Sound* mpCoin_S;

};
