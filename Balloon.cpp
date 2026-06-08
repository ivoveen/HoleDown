#include "precomp.h"
#include "Balloon.h"
#include "Player.h"
//#include <iostream> 

Balloon::Balloon(float2 position, Player* player, Camera* camera, Surface* screen) : GameObject::GameObject(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mStartPos = mPosition;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
	mpBalloon= new Sprite(new Surface("assets/balloonSheet.tga"), 6);
	mWidth = 50;
	mHeight = 100;
}

Balloon::~Balloon() {
	delete mpBalloon;
}
void Balloon::tick(float deltaTime) {
	if (popped) {
		mPosition = mStartPos;
		popped = false;
	}
	float2 endOfRope = mPosition + float2(static_cast<float>(mWidth / 2), static_cast<float>(mHeight/2));
	if (pythagorean(mpPlayer->getCentrePos(), endOfRope) < mInteractRange) {
		mpPlayer->GrabbedBalloon(mPosition, this, mHeight, mWidth);
	}
	mDeltaTime = deltaTime;
}

void Balloon::draw() {

		mSpriteFrame += (0.007f * mDeltaTime);
		if (mSpriteFrame >= 6) mSpriteFrame = 0;

		float2 camPos = mpCamera->getPos();
		mpBalloon->SetFrame(static_cast<int>(mSpriteFrame));
		mpBalloon->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
	
}