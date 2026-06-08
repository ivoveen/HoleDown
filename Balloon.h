#pragma once
#include "GameObject.h"
class Camera;
class Player;
class Balloon : public GameObject
{
public:

	Balloon(float2 position, Player* player, Camera* camera, Surface* screen);
	~Balloon();
	void tick(float deltaTime);
	void draw();
	void setPos(float2 position) {
		mPosition = position;
	}
	void pop() {
		popped = true;
	}
private:
	Sprite* mpBalloon;
	float2 mStartPos;
	float mInteractRange = powf(60, 2);
	float mSpriteFrame = 0;
	bool popped = false;
};

