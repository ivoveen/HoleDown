#include "precomp.h"
#include "Boulder.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"


Boulder::Boulder(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH) : Enemy::Enemy(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
	mMAPWIDTH = MAPWIDTH;
	mCollidingZones[0] = CollidingZone(float2(static_cast<float>(mWidth/2), static_cast<float>(mHeight/2)),0, 0, 18);
	mCollidingZones[1] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);
	mCollidingZones[2] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);

	mWidth = 40, mHeight = 40;
	mpBoulder = new Sprite(new Surface("assets/enemySprites/boulderSheet.tga"), 4);
	mVel = { -0.3,0 };
}

Boulder::~Boulder() {
	delete mpBoulder;
}

void Boulder::tick(float deltaTime) {
	mDeltaTime = deltaTime;
	if (mTriggered) {
		mPosition = mPosition + mVel * deltaTime;
		if (mPosition.x < 0) mPosition = static_cast<float>(mMAPWIDTH);
	}
	else if(pythagorean(mpPlayer->getPos(), mPosition) <= mACTIVATIONRANGE) {
		mTriggered = true;
	}
	
	CollisionCheck();
}

void Boulder::draw() {
	mSpriteFrame += (0.007f * mDeltaTime);
	if (mSpriteFrame >= 4) mSpriteFrame = 0;

	float2 camPos = mpCamera->getPos();
	mpBoulder->SetFrame(static_cast<int>(mSpriteFrame));
	mpBoulder->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
}