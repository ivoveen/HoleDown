#include "precomp.h"
#include "Frog.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"
#include <Sound.hpp>

Frog::Frog(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH) : Enemy::Enemy(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
	mMAPWIDTH = MAPWIDTH;
	mWidth = 64, mHeight = 64;
	mCollidingZones[0] = CollidingZone(float2(8, 22), 56, 42, 0);
	mCollidingZones[1] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);
	mCollidingZones[2] = CollidingZone(float2(NULL, NULL), NULL, NULL, NULL);

	mStartPos = position;
	mpCroak_S = new Audio::Sound{ "assets/Sound/frog-croak.mp3" };
	mpCroak_S->setVolume(0.5);
	mpCroak_S->setLooping(false);

	mpFrogHop = new Sprite(new Surface("assets/enemySprites/frogHop.tga"), 7);
	mpFrogIdle = new Sprite(new Surface("assets/enemySprites/frogIdle.tga"), 8);
	mVel = { -1,0 };
}
Frog::~Frog() {
	delete mpFrogHop;
	delete mpFrogIdle;
	delete mpCroak_S;
}

void Frog::tick(float deltaTime) {
	mDeltaTime = deltaTime;
	mWaitedTime += deltaTime;

	switch (mMovementState){
	case idle:
	


		if (mWaitedTime >= mTIMEUNTILJUMP) {
			mWaitedTime = 0;
			mMovementState = jump;
		}
		break;
	case jump:
		if (mWaitedTime / 400 <= PI) {
			mPosition.y = mStartPos.y + (sin(mWaitedTime / 400) * mJUMPHEIGHT);
			mPosition.x = mStartPos.x + ((cos(mWaitedTime / 400)-1) * mJUMPDISTANCE) * -mVel.x;
		}
		else {
			mStartPos.x = mPosition.x;
			mPosition.y = mStartPos.y;
			TurnAround();
			mMovementState = idle;
			mSpriteFrame = 0;
			mWaitedTime = 0;
		}
		break;
	}
	CollisionCheck();
}


void Frog::draw() {
	mSpriteFrame += (0.007f * mDeltaTime);
	if (mSpriteFrame >= 8) mSpriteFrame = 0;
	float2 camPos = mpCamera->getPos();

	switch (mMovementState) {
	case idle:
		if (static_cast<int>(mSpriteFrame) == 3) {
			if (mPosition.x - camPos.x > 0 
				&& mPosition.x - camPos.x < SCRWIDTH 
				&& mPosition.y - camPos.y > 0 
				&& mPosition.y - camPos.y < SCRHEIGHT){

				mpCroak_S->play();
			}
		}


		mpFrogIdle->SetFrame(static_cast<int>(mSpriteFrame));
		if (mVel.x > 0) mpFrogIdle->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
		else mpFrogIdle->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, true, mpScreen);
		break;
	case jump:
		//mpFrogHop->SetFrame(static_cast<int>(1));
		if (mWaitedTime / 500 < PI / 2 - PI / 8) {
			//going up
			mpFrogHop->SetFrame(3);
		}
		else if (mWaitedTime / 500 < PI / 2 + PI / 8) {
			//going up
			mpFrogHop->SetFrame(4);
		}
		else {
			//going down
			mpFrogHop->SetFrame(5);
		}
		if (mVel.x > 0) mpFrogHop->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
		else mpFrogHop->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, true, mpScreen);
		break;
	}


#ifdef _DEBUG
	drawHitboxes();
#endif
}