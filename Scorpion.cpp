#include "precomp.h"
#include "Scorpion.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"


Scorpion::Scorpion(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH) : Enemy::Enemy(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
	mMAPWIDTH = MAPWIDTH;
	mWidth = 43, mHeight = 40;
	mCollidingZones[0] = CollidingZone(float2(0, 20), mWidth, 20, NULL);
	mCollidingZones[1] = CollidingZone(float2(20, 0), 20, 20, NULL);
	mCollidingZones[2] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);

	mStartPos = position;
	mpScorpion = new Sprite(new Surface("assets/enemySprites/scorpionSheet.tga"), 6);
	mVel = { -0.07,0 };
}

Scorpion::~Scorpion() {
	delete mpScorpion;
}

void Scorpion::tick(float deltaTime) { 
	mDeltaTime = deltaTime; 

	//Is the player within range?
	if (pythagorean(mpPlayer->getCentrePos(), mPosition + float2(static_cast<float>(mWidth / 2), static_cast<float>(mHeight / 2) )) <= mACTIVATIONRANGE) {
		mMovementState = follow;
	}
	else {
		mMovementState = patrol;
	}
	switch (mMovementState) {
	case patrol:
		if (mStartPos.x - mPosition.x > mWidth * mWALKDISTANCE 
			&& mVel.x < 0
			|| mPosition.x > mStartPos.x 
			&& mVel.x > 0) {
			TurnAround();
		}
		break;
	case follow:
		if (mpPlayer->getGrounded()) {
			if (mVel.x <= 0
				&& mPosition.x < mpPlayer->getPos().x
				|| mVel.x > 0
				&& mPosition.x > mpPlayer->getCentrePos().x) {
				TurnAround();
			}
		}
		break;
	}
	mPosition = mPosition + mVel * deltaTime;
	CollisionCheck();
}



void Scorpion::draw() {
	mSpriteFrame += (0.007f * mDeltaTime);
	if (mSpriteFrame >= 6) mSpriteFrame = 0;
	float2 camPos = mpCamera->getPos();
	mpScorpion->SetFrame(static_cast<int>(mSpriteFrame));
	if (mVel.x > 0) mpScorpion->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
	else mpScorpion->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, true, mpScreen);
	
#ifdef _DEBUG
	drawHitboxes();
#endif
	
}