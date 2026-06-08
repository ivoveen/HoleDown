#pragma once
#include "Enemy.h"
namespace Audio {
	class Sound;
};

class Bat :
	public Enemy
{
public:
	Bat(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH);
	~Bat();
	void tick(float deltaTime);
	void draw();
private:
	float2 mStartPos;
	const float mFLYDISTANCE = 5;
	Sprite* mpBat;
	int mMAPWIDTH;
	float mSpriteFrame = 0;
	Audio::Sound* mpBatFlap_S;

};