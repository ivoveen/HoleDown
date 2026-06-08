#include "precomp.h"
#include "Eel.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"


Eel::Eel(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH) : Enemy::Enemy(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
	mMAPWIDTH = MAPWIDTH;
	mWidth = 64, mHeight = 14;
	mCollidingZones[0] = CollidingZone(float2(0, 0), mWidth, mHeight, 0);
	mCollidingZones[1] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);
	mCollidingZones[2] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);

	mStartPos = position;
	mpEel = new Sprite(new Surface("assets/enemySprites/electricEelSheet.tga"), 5);
	mVel = { -0.15,0 };
}

Eel::~Eel() {
	delete mpEel;
}

void Eel::tick(float deltaTime) {
	mDeltaTime = deltaTime;

	mPosition = mPosition + mVel * deltaTime;
	if (mStartPos.x - mPosition.x > mWidth * mSWIMDISTANCE && mVel.x < 0
		|| mPosition.x > mStartPos.x && mVel.x > 0) {
		mVel.x *= -1;
	}
	CollisionCheck();
}


void Eel::draw() {
	mSpriteFrame += (0.007f * mDeltaTime);
	if (mSpriteFrame >= 5) mSpriteFrame = 0;
	float2 camPos = mpCamera->getPos();
	mpEel->SetFrame(static_cast<int>(mSpriteFrame));
	if (mVel.x > 0) mpEel->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
	else mpEel->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, true, mpScreen);
#ifdef _DEBUG
	drawHitboxes();
#endif
}