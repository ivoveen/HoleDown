#include "precomp.h"
#include "Player.h"
#include "Camera.h"
//#include <iostream>
#include <Sound.hpp>
#include "Balloon.h"
#include"ScoreManager.h"
#include <lib/json/json.hpp>
Player::Player(Surface* screen, TileMap* tileMap, ScoreManager* scoreManager) {
	mpHarryRun = new Sprite(new Surface("assets/characterSprites/HarryRun.tga"), 8);
	mpHarryClimb = new Sprite(new Surface("assets/characterSprites/HarryClimb.tga"), 10);
	mpHarrySwim = new Sprite(new Surface("assets/characterSprites/HarrySwim.tga"), 5);
	mpHarrySwing = new Sprite(new Surface("assets/characterSprites/HarrySwing.tga"), 2);
	mpHarryMoveUp = new Sprite(new Surface("assets/characterSprites/HarryMoveUp.tga"), 2);
	mpHarryMoveDown = new Sprite(new Surface("assets/characterSprites/HarryMoveDown.tga"), 2);
	mpHarryIdle = new Sprite(new Surface("assets/characterSprites/HarryIdle.tga"), 8);
	mpHarryBored = new Sprite(new Surface("assets/characterSprites/HarryBored.tga"), 6);
	mpHarryDie = new Sprite(new Surface("assets/characterSprites/HarryDie.tga"), 7);

	mpMainSoundTrack_S = new Audio::Sound{ "assets/Sound/BeepBox-Song.mp3" };
	mpCheckpoint_S = new Audio::Sound{ "assets/Sound/collect-5930.mp3" };
	mpRun_S = new Audio::Sound{ "assets/Sound/running-on-grass.mp3" };
	mpClimb_S = new Audio::Sound{ "assets/Sound/ladder.mp3" };
	mpSwim_S = new Audio::Sound{ "assets/Sound/creek-swimming.mp3" };
	mpDie_S = new Audio::Sound{ "assets/Sound/lose_funny_retro_video_game.mp3" };


	mpCheckpoint_S->setVolume(1);
	mpCheckpoint_S->setLooping(false);
	mpMainSoundTrack_S->setVolume(0.1);
	mpMainSoundTrack_S->setLooping(true);
	mpMainSoundTrack_S->play();
	mpRun_S->setVolume(0.8);
	mpRun_S->setLooping(true);
	mpClimb_S->setVolume(0.5);
	mpClimb_S->setLooping(true);
	mpSwim_S->setVolume(0.5);
	mpSwim_S->setLooping(true);
	mpDie_S->setVolume(0.7);
	mpDie_S->setLooping(false);


	mpScreen = screen;
	mpMAPDATA = tileMap->getMAPDATA();
	mTILESIZE = tileMap->getTILESIZE();
	mTILESCALE = tileMap->getTILESCALE();
	mSCALEDTILESIZE = tileMap->getSCALEDTILESIZE();
	mAMOUNTOFTILESINAROW = tileMap->getAMOUNTOFTILESINAROW();
	mMAPWIDTH = tileMap->getMAPWIDTH();
	mMAPHEIGHT = tileMap->getMAPHEIGHT();
	mpTileSet = tileMap->getTileSet();
	mScoreManager = scoreManager;

	lastCheckpointPos = mPos;

	std::ifstream jsonIn("./data.json");
	nlohmann::json jsonNew = nlohmann::json::parse(jsonIn);
	float2 lastSavedCheckPoint = float2(jsonNew["playerPosX"], jsonNew["playerPosY"]);
	if (lastSavedCheckPoint.x != 0 && lastSavedCheckPoint.y != 0) mPos = lastSavedCheckPoint;
	mMOVEMENTSPEED *= jsonNew["playerSpeed"];
	mLADDERCLIMBSPEED *= jsonNew["playerSpeed"];
	mSWIMSPEED *= jsonNew["playerSpeed"];
	mFLOATSPEED *= jsonNew["playerSpeed"];
	jsonIn.close();
}

Player::~Player() {
	delete mpHarryRun;
	delete mpHarryClimb;
	delete mpHarrySwim;
	delete mpHarrySwing;
	delete mpHarryMoveUp;
	delete mpHarryMoveDown;
	delete mpHarryIdle;
	delete mpHarryBored;
	delete mpHarryDie;
	delete mpRun_S;
	delete mpDie_S;
	delete mpClimb_S;
	delete mpSwim_S;
	delete mpMainSoundTrack_S;
	delete mpCheckpoint_S;
}
void Player::setCheckpointPos(float2 pos) {
	lastCheckpointPos = pos;
	mpCheckpoint_S->play();
}


void Player::Init(Camera* camera) {
	mpCamera = camera;
}

void Player::tick(float deltaTime) {
	movement(deltaTime);
}

void Player::movement(float deltaTime) {
	//check what tile I am currently colliding with
	if (mMovementMode != swinging && mMovementMode != dead) {
		int x1 = static_cast<int>(floor((mPos.x - mINTERACTRANGE) / mSCALEDTILESIZE)), y1 = static_cast<int>(floor((mPos.y - mINTERACTRANGE) / mSCALEDTILESIZE));
		int x2 = static_cast<int>(floor((mPos.x + mINTERACTRANGE + mWidth) / mSCALEDTILESIZE)), y2 = static_cast<int>(floor((mPos.y + mINTERACTRANGE + mHeight) / mSCALEDTILESIZE));
		for (int i = x1; i <= x2; i++) for (int j = y1; j <= y2; j++) {
			int tile = mpMAPDATA[(j)*mMAPWIDTH + i];
			if (tile == 21 || tile == 28 || tile == 35) {
				//colliding with ladder
				if (mGrounded && mMovementMode != climbing) {
					if (GetKeyState(VK_DOWN) & 0x8000 && mMovementMode != swimming || GetKeyState(VK_UP) & 0x8000) {
						if (mHeight < mWidth) flipPlayerDimensions();
						mMovementMode = climbing;
						mPos.x = (i)*mSCALEDTILESIZE + (mSCALEDTILESIZE - mWidth) / 2;
						if (mpMAPDATA[(y1)*mMAPWIDTH + i] == -1)mPos.y = (j)*mSCALEDTILESIZE - (mHeight / 2);
						mVel.x = 0;
					}
				}
			}
			else if (mMovementMode != climbing && tile == 51 || tile == 40 && mMovementMode != climbing) {
				//swimming
				if (mpMAPDATA[(j - 1) * mMAPWIDTH + i] == 40 && mHeight > mWidth) flipPlayerDimensions(), mMovementMode = swimming;
			}
		}
	}
	switch (mMovementMode) {
	case walking:
		walkingMovement(deltaTime);
		break;
	case climbing:
		climbingMovement();
		break;
	case swinging:
		swingingMovement();
		break;
	case swimming:
		swimmingMovement();
		break;
	case balloon:
		balloonMovement();
		break;
	}

	mPos = handleCollision(mPos + mVel * deltaTime); // check collision for the future position


	//manage sound effects
	switch (mMovementMode) {
	case walking:

		if (mVel.x != 0
			&& mGrounded
			&& !mpRun_S->isPlaying()) {
			mpRun_S->play();
			mpClimb_S->stop();
			mpSwim_S->stop();
		}
		if (mVel.x == 0
			&& mpRun_S->isPlaying()
			|| !mGrounded
			&& mpRun_S->isPlaying()) {
			mpRun_S->stop();
		}
		break;
	case climbing:
		if (mVel.y != 0
			&& !mpClimb_S->isPlaying()) {
			mpClimb_S->play();
			mpRun_S->stop();
			mpSwim_S->stop();
		}
		if (mVel.y == 0
			&& mpClimb_S->isPlaying()) {
			mpClimb_S->stop();
		}
		break;
	case swimming:
		mpClimb_S->stop();
		mpRun_S->stop();
		mpSwim_S->play();
		break;
	}

	//cout << "X: " << pos.x << " , Y: " << pos.y << "\n";
}



void Player::flipPlayerDimensions() {
	float temp = mWidth;
	mWidth = mHeight;
	mHeight = temp;
}
void Player::walkingMovement(float deltaTime) {
	if (GetKeyState(VK_RIGHT) & 0x8000 && mGrounded) {
		mVel.x = mMOVEMENTSPEED; //cout << "keypressed";
	}
	else if (GetKeyState(VK_LEFT) & 0x8000 && mGrounded) {
		mVel.x = -mMOVEMENTSPEED; //cout << "keypressed";
	}
	else if (mGrounded) {
		updateLastDirection();
		mVel.x = 0;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && mGrounded) {
		mVel.y -= mJUMPSPEED;
		mGrounded = false;
	}
	mVel.y = mVel.y + mGRAVITY * deltaTime;
	if (mVel.y > mMAXFALLSPEED && !jumpedOffRope) mVel.y = mMAXFALLSPEED;
	if (jumpedOffRope) mVel.x -= mVel.x * 0.005f;
}
void Player::swimmingMovement() {
	mGrounded = true;
	int topLeftOfPLayerIndex = static_cast<int>(floor(mPos.x / mSCALEDTILESIZE) + floor((mPos.y) / mSCALEDTILESIZE) * mMAPWIDTH);
	int middleOfPLayerIndex = static_cast<int>(floor(mPos.x / mSCALEDTILESIZE) + floor((mPos.y + (mHeight / 2)) / mSCALEDTILESIZE) * mMAPWIDTH);
	if (mpMAPDATA[middleOfPLayerIndex] == 51) {
		mVel.y = -mBOUYANCY;
	}
	if (mpMAPDATA[middleOfPLayerIndex] == 40 && mpMAPDATA[topLeftOfPLayerIndex] == 40) {
		mVel.y = mBOUYANCY;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000 || GetKeyState(VK_LEFT) & 0x8000) {
		int direction = 0;
		if (GetKeyState(VK_LEFT) & 0x8000) direction = -1, mVel.x = -mMOVEMENTSPEED;
		else direction = static_cast<int>(1 * ceil(mWidth / mSCALEDTILESIZE)), mVel.x = mMOVEMENTSPEED;
		if (mpMAPDATA[topLeftOfPLayerIndex + direction] != 40 && mpMAPDATA[topLeftOfPLayerIndex + direction] != 51) {
			//hitting a wall
			if (mpMAPDATA[middleOfPLayerIndex + direction - 2 * mMAPWIDTH] == -1) {
				//at the top of the wall
				mVel.y = -mMOVEMENTSPEED * 3;
				mVel.x = mMOVEMENTSPEED;
				mCollisionFriction = false;
				flipPlayerDimensions();
				mMovementMode = walking;
			}
		}
	}
	else if (GetKeyState(VK_LEFT) & 0x8000) {
		mVel.x = -mMOVEMENTSPEED; //cout << "keypressed";
	}
	else {
		updateLastDirection();
		mVel.x = 0;

	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		mVel.y = mMOVEMENTSPEED / 2;
	}
}

void Player::swingingMovement() {

	//mPos = mRopePointPosition; 
	if (lastDirection == left)mPos = mRopePointPosition;
	else if (lastDirection == right)mPos = float2(mRopePointPosition.x - mWidth, mRopePointPosition.y);

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		mVel.y = (mRopePointPosition.y - mPrevRopePointPosition.y) / 6;
		mVel.x = (mRopePointPosition.x - mPrevRopePointPosition.x) / 8;

		mMovementMode = walking;
		mCollisionFriction = false;
		jumpedOffRope = true;
	}
}

void Player::climbingMovement() {
	//If there are tiles next to the player, the player cant jump off.
	//if the players feet reaches a ladder hole he gets off with a jump. 
	//if the player reaches the bottom or top of a ladder he cant climb more.
	int currentTileIndex = static_cast<int>(floor(mPos.x / mSCALEDTILESIZE) + floor((mPos.y) / mSCALEDTILESIZE) * mMAPWIDTH);
	int tileBelowPlayer = mpMAPDATA[static_cast<int>(floor(mPos.x / mSCALEDTILESIZE) + floor((mPos.y + mHeight) / mSCALEDTILESIZE) * mMAPWIDTH)];
	int middleOfPLayerIndex = static_cast<int>(floor(mPos.x / mSCALEDTILESIZE) + floor((mPos.y + (mHeight / 2)) / mSCALEDTILESIZE) * mMAPWIDTH);
	if (GetKeyState(VK_UP) & 0x8000 || GetKeyState(VK_DOWN) & 0x8000 && mMovementMode != swimming) {
		if (GetKeyState(VK_UP) & 0x8000) mVel.y = -1;
		else mVel.y = 1;
		if (mpMAPDATA[middleOfPLayerIndex] == -1
			&& mVel.y == -1) mVel.y = 0; //top of the ladder reached
		if (tileBelowPlayer != -1
			&& tileBelowPlayer != 21
			&& tileBelowPlayer != 28
			&& tileBelowPlayer != 35
			&& mVel.y == 1) {
			mVel.y = 0, mMovementMode = walking; //bottom of the ladder reached
		}
		mVel.y *= mLADDERCLIMBSPEED;
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000 || GetKeyState(VK_LEFT) & 0x8000) {
		if (GetKeyState(VK_RIGHT) & 0x8000) mVel.x = 1;
		else mVel.x = -1;
		mVel.y = 0, mMovementMode = walking;
		int tileNextToHead = mpMAPDATA[currentTileIndex + static_cast<int>(mVel.x)];
		int tileNextToMiddle = mpMAPDATA[middleOfPLayerIndex + static_cast<int>(mVel.x)];

		if (mpMAPDATA[middleOfPLayerIndex + mMAPWIDTH] == 21) {
			mVel.y = -2.0f; //top of the ladder get off
			mCollisionFriction = false;
			mGrounded = false;
		}
		else if (tileNextToHead != -1
			&& tileNextToHead != 51
			&& tileNextToHead != 40
			|| tileNextToMiddle != -1
			&& tileNextToMiddle != 51
			&& tileNextToMiddle != 40) {
			mVel.x = 0; //tile next to player, get off obstructed
			mMovementMode = climbing;
		}
		mVel *= mMOVEMENTSPEED;

	}
	else {
		mVel.y = 0;
		mVel.x = 0;
	}
}

void Player::GrabbedBalloon(float2 balloonPos, Balloon* pBalloon, int height, int width) {
	if (mMovementMode != balloon) {
		if (mPos.x + mWidth / 2 < balloonPos.x)lastDirection = right;
		else lastDirection = left;

		mPos = balloonPos;
		mpBalloon = pBalloon;
		mMovementMode = balloon;
		mHeight = static_cast<float>(height);
		mWidth += width / 2;
	}
}

void Player::balloonMovement() {
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		mVel.x = mMOVEMENTSPEED / 2; //cout << "keypressed";
	}
	else if (GetKeyState(VK_LEFT) & 0x8000) {
		mVel.x = -mMOVEMENTSPEED / 2; //cout << "keypressed";
	}
	mVel.y = mFLOATSPEED;
	if (lastDirection == left) mpBalloon->setPos(mPos);
	else mpBalloon->setPos(mPos + float2(mWidth / 2, 0));
}

void Player::leaveBalloon() {
	mHeight = mSTARTHEIGHT;
	mWidth = mSTARTWIDTH;
	mMovementMode = walking;
	mGrounded = false;
	mpBalloon->pop();
}

Player::side Player::TileMapCollision(int i, int j) {
	//not in use
	int tile = mpMAPDATA[i + j * mMAPWIDTH];
	if (tile != -1
		&& tile != 21
		&& tile != 28
		&& tile != 35
		&& tile != 40
		&& tile != 51) {
		if (ceil((mPos.y + mHeight) / mSCALEDTILESIZE) <= j) return down;
		if (ceil(mPos.x / mSCALEDTILESIZE) >= i) return left;
		if (ceil((mPos.x + mWidth) / mSCALEDTILESIZE) <= i) return right;
		if (ceil(mPos.y / mSCALEDTILESIZE) >= j) return up;
		return none;
	}
	else return none;
}



Player::side Player::AABBCollision(float2 futurePlayerPos, int x3, int y3, int x4, int y4) {
	if (futurePlayerPos.x >= x4
		|| futurePlayerPos.x + mWidth <= x3
		|| futurePlayerPos.y + mHeight <= y3
		|| futurePlayerPos.y >= y4) {
		return none;
	}
	//which side of the player collided. Using the players previous position.
	if (mPos.y + mHeight <= y3) return down;
	if (mPos.x >= x4) return left;
	if (mPos.x + mWidth <= x3) return right;
	if (mPos.y >= y4) return up;

	return none;
}

float2 Player::handleCollision(float2 futurePlayerPos) {
	Player::side collisionSide = none;
	int collumn = static_cast<int>(floor(futurePlayerPos.x / mSCALEDTILESIZE));
	int row = static_cast<int>(floor(futurePlayerPos.y / mSCALEDTILESIZE));

	for (int j = 0; j <= ceil(mHeight / mSCALEDTILESIZE); j++) {
		for (int i = 0; i <= ceil(mWidth / mSCALEDTILESIZE); i++) {
			int tile = mpMAPDATA[(row + j) * mMAPWIDTH + collumn + i];
			if (tile != -1
				&& tile != 21
				&& tile != 28
				&& tile != 35
				&& tile != 40
				&& tile != 51) {
				int x3 = (collumn + i) * mSCALEDTILESIZE;
				int y3 = (row + j) * mSCALEDTILESIZE;
				int x4 = (collumn + i) * mSCALEDTILESIZE + mSCALEDTILESIZE;
				int y4 = (row + j) * mSCALEDTILESIZE + mSCALEDTILESIZE;
				collisionSide = AABBCollision(futurePlayerPos, x3, y3, x4, y4);
				if (collisionSide != none) {
					//apply normal force
					switch (collisionSide) {
					case down:
						futurePlayerPos.y -= futurePlayerPos.y + mHeight - y3;
						mVel.y = 0;
						mGrounded = true;
						mCollisionFriction = true;
						jumpedOffRope = false;
						//cout << tile;
						break;
					case up:
						if (mMovementMode == balloon) leaveBalloon();
						futurePlayerPos.y += y4 - futurePlayerPos.y;
						mVel.y = 0;
						break;
					case right:
						futurePlayerPos.x -= futurePlayerPos.x + mWidth - x3;
						updateLastDirection();
						if (mCollisionFriction)mVel.x = 0;
						break;
					case left:
						futurePlayerPos.x += x4 - futurePlayerPos.x;
						updateLastDirection();
						if (mCollisionFriction)mVel.x = 0;
						break;
					}
				}
			}
		}
	}
	/*int x = (collumn + i) * SCALEDTILESIZE - pos.x + (SCRWIDTH / 2);
	int y = (row + j) * SCALEDTILESIZE - pos.y + (SCRHEIGHT / 2);
	if (collisionSide == none) screen->Box(x, y, x + SCALEDTILESIZE, y + SCALEDTILESIZE, 500000);
	else screen->Bar(x, y, x + SCALEDTILESIZE, y + SCALEDTILESIZE, 500000000);*/


	//cout << "pos X: " << pos.x << " pos Y: " << pos.y << " futurepos X: " << futurePlayerPos.x << " futurepos Y: " << futurePlayerPos.y << " Hit? " << collisionSide << " Row: " << row << "\n";
	if (futurePlayerPos.y - mPos.y > 0) mGrounded = false;
	return futurePlayerPos;

}

void Player::updateLastDirection() {
	if (mVel.x > 0)lastDirection = right;
	if (mVel.x < 0)lastDirection = left;
}

void Player::StopAllSound() {
	mpRun_S->stop();
	mpDie_S->stop();
	mpClimb_S->stop();
	mpSwim_S->stop();
}

void Player::die() {
	if (mMovementMode == balloon) {
		leaveBalloon();
	}
	else if (mMovementMode != dead) {
		StopAllSound();
		mpDie_S->play();
		mScoreManager->UpdateScore(-250);
		if (mMovementMode == swimming) flipPlayerDimensions();
		mMovementMode = dead;
		mVel = {};
		mSpriteFrame = 0;
	}
}

void Player::draw(float deltaTime) {
	if (mMovementMode == climbing && mVel.y == 0) {
	}
	else {
		mSpriteFrame += (0.007f * deltaTime);
		if (mSpriteFrame >= 10) mSpriteFrame = 0;
	}
	if (mVel.x == 0 && mVel.y == 0 && mGrounded) mTimeSinceMoved += deltaTime;
	else mTimeSinceMoved = 0.0f;


	int2 playerViewPos;
	playerViewPos.x = static_cast<int>(mPos.x - mpCamera->getPos().x);
	playerViewPos.y = static_cast<int>(mPos.y - mpCamera->getPos().y);

	//hitbox
#ifdef _DEBUG
	mpScreen->Box(playerViewPos.x, playerViewPos.y, playerViewPos.x + static_cast<int>(mWidth), static_cast<int>(playerViewPos.y + mHeight), 255);
	mpScreen->Box(playerViewPos.x - mINTERACTRANGE, playerViewPos.y - mINTERACTRANGE, playerViewPos.x + static_cast<int>(mWidth + mINTERACTRANGE), static_cast<int>(playerViewPos.y + mHeight + mINTERACTRANGE), 255000);
#endif
	//correct position for sprite
	playerViewPos.x -= 24 * 2, playerViewPos.y -= 24 * 2;
	//sprites

	//choose which sprite should be rendered
	switch (mMovementMode) {
	case walking:
		if (mVel.x == 0 && mVel.y == 0 && mGrounded) {
			//idle
			if (mTimeSinceMoved < 10000) {

				mpHarryIdle->SetFrame(static_cast<int>(mSpriteFrame) % 8);
				if (lastDirection == right)mpHarryIdle->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
				if (lastDirection == left)mpHarryIdle->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);
			}
			else {
				mpHarryBored->SetFrame(static_cast<int>(mSpriteFrame) % 6);
				if (lastDirection == right)mpHarryBored->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
				if (lastDirection == left)mpHarryBored->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);
			}
		}
		else if (mVel.y == 0 && mGrounded) {
			//running
			mpHarryRun->SetFrame(static_cast<int>(mSpriteFrame) % 7);
			if (mVel.x >= 0)mpHarryRun->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
			else mpHarryRun->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);
		}
		else if (mVel.y <= 0) {
			//moving upwards
			mpHarryMoveUp->SetFrame(static_cast<int>(mSpriteFrame) % 2);
			if (mVel.x > 0 || lastDirection == right && mVel.x == 0) mpHarryMoveUp->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
			else if (mVel.x < 0 || lastDirection == left && mVel.x == 0) mpHarryMoveUp->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);

		}
		else if (mVel.y >= 0) {
			//falling down
			mpHarryMoveDown->SetFrame(static_cast<int>(mSpriteFrame) % 2);
			if (mVel.x > 0 || lastDirection == right && mVel.x == 0) mpHarryMoveDown->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
			else if (mVel.x < 0 || lastDirection == left && mVel.x == 0)mpHarryMoveDown->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);
		}
		break;
	case climbing:
		//ladder
		mpHarryClimb->SetFrame(static_cast<int>(mSpriteFrame) % 10);
		mpHarryClimb->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
		break;
	case swinging:
		if (mVel.x < 0)mpHarrySwing->SetFrame(0);
		if (mVel.x > 0)mpHarrySwing->SetFrame(1);
		if (lastDirection == right)mpHarrySwing->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
		if (lastDirection == left)mpHarrySwing->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);

		//swinging
		break;
	case swimming:
		//swimming
		playerViewPos.y -= 24 * 2;
		playerViewPos.x += 24;
		mpHarrySwim->SetFrame(static_cast<int>(mSpriteFrame) % 5);

		if (mVel.x > 0 || lastDirection == right && mVel.x == 0) mpHarrySwim->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
		else if (mVel.x < 0 || lastDirection == left && mVel.x == 0)mpHarrySwim->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);

		break;
	case balloon:
		if (mVel.x < 0)mpHarrySwing->SetFrame(0);
		if (mVel.x > 0)mpHarrySwing->SetFrame(1);
		if (lastDirection == right)mpHarrySwing->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
		if (lastDirection == left)mpHarrySwing->DrawScaled(static_cast<int>(playerViewPos.x + mWidth / 2), playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);

		break;
	case dead:
		if (static_cast<int>(mSpriteFrame) > 7) {
			//respawn
			mPos = lastCheckpointPos;
			mMovementMode = walking;
			mpCamera->cameraRespawn(mPos);
		}

		mpHarryDie->SetFrame(static_cast<int>(mSpriteFrame) % 7);
		if (mVel.x >= 0)mpHarryDie->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, false, mpScreen);
		else mpHarryDie->DrawScaled(playerViewPos.x, playerViewPos.y, 48 * 3, 48 * 3, true, mpScreen);
		break;
	}
}
