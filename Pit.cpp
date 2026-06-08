#include "precomp.h"
#include "Pit.h"
#include "Player.h"
#include "Camera.h"
#include <Sound.hpp>

Pit::Pit(float2 position, Player* player, Camera* camera, Surface* screen, PitType typeOfPit) : GameObject::GameObject(mPosition, mpPlayer, mpCamera, mpScreen) {
	mpScreen = screen;
	mpCamera = camera;
	mpPlayer = player;
	mPitType = typeOfPit;
	mpCroc_S = new Audio::Sound{ "assets/Sound/Crocodile.mp3" };
	mpCroc_S->setVolume(0.5);
	mpCroc_S->setLooping(false);
	if (mPitType == waterPitWithCrocs) mPosition = { position.x - (mCROCPITMAXWIDTH / 2), position.y };
	else 	mPosition = { position.x - (mMAXWIDTH / 2), position.y };
	mpCroc = new Sprite(new Surface("assets/enemySprites/crocodileSheet.tga"), 2);
}

Pit::~Pit() {
	delete mpCroc;
	delete mpCroc_S;
}

void Pit::tick(float deltaTime) {
	switch (mPitType) {
	case shrinkingPit:
		if (currentWidthPercentage < 0 || currentWidthPercentage  > 1) {
			//if the maximum or minimum has been reached switch from growing to shrinking or vice versa
			mShrinkSpeed *= -1;
			currentWidthPercentage = round(currentWidthPercentage);
		}
		if (mShrinkPauseTime < mSHRINKPAUSEMAXTIME && currentWidthPercentage == 0 || currentWidthPercentage == 1) {
			//dont grow or shrink for a period of time when the minimum or maximum has been reached.
			mShrinkPauseTime += deltaTime;
		}
		if (mShrinkPauseTime == 0 || mShrinkPauseTime >= mSHRINKPAUSEMAXTIME) {
			//increase the percentage.
			mShrinkPauseTime = 0;
			currentWidthPercentage += mShrinkSpeed * deltaTime;
		}
		break;
	case waterPitWithCrocs:
		mShrinkPauseTime += deltaTime;
		if (mShrinkPauseTime >= mCROCODILETIME) {
			if (mCrocState == open)mCrocState = closed;
			else mCrocState = open;
			mShrinkPauseTime = 0;

			//play croc sound logic
			float2 camPos = mpCamera->getPos();
			if (mCrocState == open 
				&& mPosition.x - camPos.x > 0
				&& mPosition.x - camPos.x < SCRWIDTH * 1.2
				&& mPosition.y - camPos.y > 0
				&& mPosition.y - camPos.y < SCRHEIGHT * 1.2) {

				mpCroc_S->play();
			}
		}
		break;
	}

	//check for collision
	bool standingOnCroc = false;
	float2 playerPos = mpPlayer->getPos();
	float playerWidth = (mpPlayer->getCentrePos().x - playerPos.x) * 2;
	float playerHeight = (mpPlayer->getCentrePos().y - playerPos.y) * 2;
	int leftX, rightX;
	if (mPitType == waterPitWithCrocs) {
		leftX = static_cast<int>(mPosition.x + ((1 - currentWidthPercentage) * mCROCPITMAXWIDTH / 2));
		rightX = static_cast<int>(mPosition.x + (currentWidthPercentage * mCROCPITMAXWIDTH) + ((1 - currentWidthPercentage) * mCROCPITMAXWIDTH / 2));
	}
	else {
		leftX = static_cast<int>(mPosition.x + ((1 - currentWidthPercentage) * mMAXWIDTH / 2));
		rightX = static_cast<int>(mPosition.x + (currentWidthPercentage * mMAXWIDTH) + ((1 - currentWidthPercentage) * mMAXWIDTH / 2));

	}

	if (playerPos.y + playerHeight > mPosition.y
		&& playerPos.y < mPosition.y + mHeight
		&& playerPos.x + playerWidth > leftX
		&& playerPos.x < rightX
		&& leftX != rightX)
	{

		if (mPitType == waterPitWithCrocs && mCrocState == closed) {
			for (int i = 0; i < 3; i++) {
				if (playerPos.x + playerWidth > mPosition.x + mCROCBORDER + mCROCSPACING * i
					&& playerPos.x < mPosition.x + mCROCBORDER + mCROCSPACING * i + CROCWIDTH) {
					standingOnCroc = true;
				}
			}

		}

		if (!standingOnCroc)mpPlayer->die();


	}
}


void Pit::draw() {
	uint color;
	if (mPitType == pit || mPitType == shrinkingPit) color = 0;
	else color = 255;
	float2 camPos = mpCamera->getPos();
	int leftX = 0, rightX = 0;
	if (mPitType != waterPitWithCrocs) {
		leftX = static_cast<int>(mPosition.x - camPos.x + ((1 - currentWidthPercentage) * mMAXWIDTH / 2));
		rightX = static_cast<int>(mPosition.x - camPos.x + (currentWidthPercentage * mMAXWIDTH) + ((1 - currentWidthPercentage) * mMAXWIDTH / 2));
	}
	else {
		leftX = static_cast<int>(mPosition.x - camPos.x + ((1 - currentWidthPercentage) * mCROCPITMAXWIDTH / 2));
		rightX = static_cast<int>(mPosition.x - camPos.x + (currentWidthPercentage * mCROCPITMAXWIDTH) + ((1 - currentWidthPercentage) * mCROCPITMAXWIDTH / 2));
	}
	if (leftX != rightX)mpScreen->Bar(leftX, static_cast<int>(mPosition.y - camPos.y), rightX, static_cast<int>(mPosition.y + mHeight - camPos.y), color);

	if (mPitType == waterPitWithCrocs) {
		mpCroc->SetFrame(mCrocState);
		for (int i = 0; i < 3; i++) {
			mpCroc->DrawScaled(static_cast<int>(mPosition.x - camPos.x + mCROCBORDER + mCROCSPACING * i), static_cast<int>(mPosition.y - camPos.y - 18), CROCWIDTH, 28, false, mpScreen);
		}
	}
}