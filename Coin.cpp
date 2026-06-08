#include "precomp.h"
#include "Coin.h"
#include "Player.h"
#include "Camera.h"
#include "ScoreManager.h"
#include <Sound.hpp>




Coin::Coin(float2 position, Player* player, Camera* camera, Surface* screen, ScoreManager* scoreManager, CoinType typeOfCoin) : GameObject::GameObject(mPosition, mpPlayer, mpCamera, mpScreen) {
	mpScreen = screen;
	mpCamera = camera;
	mpPlayer = player;
	mpScoreManager = scoreManager;
	mCoinType = typeOfCoin;
	mPosition = position;
	mpCoin = new Sprite(new Surface("assets/coinsSheet.tga"), 4);
	mpCoin->SetFrame(mCoinType);
	mStartPosition = mPosition;
	mpCoin_S = new Audio::Sound{ "assets/Sound/retro-video-game-coin-pickup.mp3" };
}

Coin::~Coin() {
	delete mpCoin;
	delete mpCoin_S;
}

void Coin::tick(float deltaTime) {
	mElapsedTime += deltaTime;


	if (mCoinState == normal) {
		mPosition.y = mStartPosition.y + sin(-mElapsedTime / 500) * mMAXBOBHEIGHT;
		if (mElapsedTime / 500 > 2 * PI) mElapsedTime = 0;
		CollisionDetection();
	}
	else {
		mWidth -= static_cast<int>(ceil(deltaTime / mDISAPPEARSPEED));
		mHeight -= static_cast<int>(ceil(deltaTime / mDISAPPEARSPEED));
		mPosition += float2(deltaTime / mDISAPPEARSPEED, deltaTime / mDISAPPEARSPEED);
	}
}

void Coin::CollisionDetection() {
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
				Disappear();
				mpCoin_S->setVolume(0.1);
				mpCoin_S->play();
			}
		}
	}

}

void Coin::Disappear() {
	mpScoreManager->UpdateScore(coinValues[mCoinType]);
	mCoinState = collected;
	mElapsedTime = 0;
}

void Coin::draw() {
	float2 camPos = mpCamera->getPos();
	mpCoin->DrawScaled(static_cast<int>(mPosition.x - camPos.x), static_cast<int>(mPosition.y - camPos.y), mWidth, mHeight, false, mpScreen);
}