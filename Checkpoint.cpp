#include "precomp.h"
#include "Checkpoint.h"
#include "Player.h"
#include "Camera.h"
#include <Sound.hpp>




Checkpoint::Checkpoint(float2 position, Player* player, Camera* camera, Surface* screen) : GameObject::GameObject(mPosition, mpPlayer, mpCamera, mpScreen) {
	mpScreen = screen;
	mpCamera = camera;
	mpPlayer = player;
	mPosition = position;
	mWidth = 48, mHeight = 96;
	mpCheckpointActive = new Sprite(new Surface("assets/checkpointActiveSheet.tga"), 8);
	mpCheckpointInnactive = new Sprite(new Surface("assets/checkpointNotActive.tga"), 1);
	//mpCheckpoint_S = new Audio::Sound{ "assets/Sound/retro-video-game-coin-pickup.mp3" };
}

Checkpoint::~Checkpoint() {
	delete mpCheckpointActive;
	delete mpCheckpointInnactive;
	//delete mpCoin_S;
}

void Checkpoint::tick(float deltaTime) {
	mDeltaTime = deltaTime;

	if (mCheckpointState == innactive) {
		CollisionDetection();
	}
}

void Checkpoint::CollisionDetection() {
	float2 playerCentrePos = mpPlayer->getCentrePos();
	float2 playerPos = mpPlayer->getPos();
	float playerWidth = (playerCentrePos.x - playerPos.x) * 2;
	float playerHeight = (playerCentrePos.y - playerPos.y) * 2;
	float playerRadius = pythagorean({ playerWidth,0 }, { 0,playerHeight });
	float2 centrePos = mPosition + float2(static_cast<float>(mWidth / 2), static_cast<float>(mHeight / 2));

	//early out check
	if (pythagorean(playerCentrePos, centrePos) <= playerRadius) {
		//AABB check for collision
		int leftX = static_cast<int>(mPosition.x);
		int rightX = static_cast<int>(mPosition.x + mWidth);

		if (playerPos.y + playerHeight > mPosition.y && playerPos.y < mPosition.y + mHeight) {
			if (leftX != rightX && playerPos.x + playerWidth > leftX && playerPos.x < rightX) {
				mpPlayer->setCheckpointPos(mPosition);
				mCheckpointState = active;
				//mpCoin_S->setVolume(0.1);
				//mpCoin_S->play();
			}
		}
	}

}


void Checkpoint::draw() {
	float2 camPos = mpCamera->getPos();
	switch (mCheckpointState) {
	case innactive:
		mpCheckpointInnactive->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
		break;
	case active:
		mSpriteFrame += (0.007f * mDeltaTime);
		if (mSpriteFrame >= 8) mSpriteFrame = 0;
		mpCheckpointActive->SetFrame(static_cast<int>(mSpriteFrame));
		mpCheckpointActive->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
		break;
	}
}