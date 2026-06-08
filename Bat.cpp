#include "precomp.h"
#include "Bat.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"
#include <Sound.hpp>


Bat::Bat(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH) : Enemy::Enemy(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
	mMAPWIDTH = MAPWIDTH;
	mWidth = 56, mHeight = 40;
	mCollidingZones[0] = CollidingZone(float2(0, 0), mWidth, mHeight,0);
	mCollidingZones[1] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);
	mCollidingZones[2] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);
	mpBatFlap_S = new Audio::Sound{ "assets/Sound/wing-flap.mp3" };
	mpBatFlap_S->setVolume(0.4);
	mpBatFlap_S->setLooping(false);

	mStartPos = position;
	mpBat = new Sprite(new Surface("assets/enemySprites/batFly.tga"), 4);
	mVel = { -0.15,0 };
}

Bat::~Bat() {
	delete mpBat;
	delete mpBatFlap_S;
}

void Bat::tick(float deltaTime) {
	mDeltaTime = deltaTime;
	
	mPosition = mPosition + mVel * deltaTime;
	if (mStartPos.x - mPosition.x > mWidth * mFLYDISTANCE && mVel.x < 0 
		|| mPosition.x > mStartPos.x && mVel.x > 0) {
		mVel.x *= -1;
	}
	CollisionCheck();
}


void Bat::draw() {
	mSpriteFrame += (0.007f * mDeltaTime);
	if (mSpriteFrame >= 4) mSpriteFrame = 0;
	float2 camPos = mpCamera->getPos();
	if (static_cast<int>(mSpriteFrame) == 3) {
		if (mPosition.x - camPos.x > 0
			&& mPosition.x - camPos.x < SCRWIDTH
			&& mPosition.y - camPos.y > 0
			&& mPosition.y - camPos.y < SCRHEIGHT) {

			mpBatFlap_S->play();
		}
	}


	mpBat->SetFrame(static_cast<int>(mSpriteFrame));
	if(mVel.x > 0) mpBat->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
	else mpBat->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, true, mpScreen);
#ifdef _DEBUG
	drawHitboxes();
#endif
}