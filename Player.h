#pragma once
#include "Camera.h"

namespace Audio {
	class Sound;
};
class Balloon;
class ScoreManager;
class Player :
	TileMap
{

public:
	Player(Surface* screen, TileMap* tileMap, ScoreManager* scoreManager);
	~Player();

	void Init(Camera* camera);
	void tick(float deltaTime);
	void draw(float deltaTime);
	void die();
	float2 getPos() { return mPos; };
	void setPos(float2 pos) { mPos = pos; };
	float2 getCentrePos() { return float2(mPos.x + (mWidth / 2), mPos.y + (mHeight / 2)); };
	float2 getmVel() { return mVel; };
	int getMovementMode() { return mMovementMode; };
	float getGrounded() { return mGrounded; };
	void setCheckpointPos(float2 pos);
	float2 getCheckpointPos() { return lastCheckpointPos; }

	void leaveBalloon();
	
	void GrabbedBalloon(float2 balloonPos, Balloon* pBalloon, int height, int width);
	void hitTheRope(float2 ropePointPosition, float2 prevRopePointPosition) 
	{ 
		if (!jumpedOffRope) {
			mRopePointPosition = ropePointPosition; mPrevRopePointPosition = prevRopePointPosition; mMovementMode = swinging;
		}
	};
	enum movementMode { walking, climbing, swimming, swinging, balloon, dead }mMovementMode = walking;
private:
	enum side { none, left, up, right, down }lastDirection = right;
	void movement(float deltaTime);
	void walkingMovement(float deltaTime);
	void climbingMovement();
	void swingingMovement();
	void swimmingMovement();
	void balloonMovement();
	void StopAllSound();

	void flipPlayerDimensions();

	float2 handleCollision(float2 futurePlayerPos);
	side AABBCollision(float2 futurePlayerPos, int x3, int y3, int x4, int y4);
	side TileMapCollision(int i, int j);
	void updateLastDirection();

	ScoreManager* mScoreManager;
	Surface* mpScreen;
	Camera* mpCamera;

	float2 lastCheckpointPos = 0;
	float2 mRopePointPosition = float2(0.0f, 0.0f);
	float2 mPrevRopePointPosition = float2(0.0f, 0.0f);
	//player settings
	float2 mVel = { 0, 0 }; //players velocity
	float2 mPos = { 9000, 700 }; //players position in the world 
	const float mGRAVITY = 0.0020f;
	float mMOVEMENTSPEED = 0.3f;
	const float mJUMPSPEED = 0.5f;
	float mLADDERCLIMBSPEED = 0.15f;
	float mSWIMSPEED = mMOVEMENTSPEED / 0.8f;
	const float mSINKSPEED = 0.1f;
	const float mMAXFALLSPEED = 0.45f;
	const float mBOUYANCY = -0.05f;
	const int mINTERACTRANGE = 32;
	float mFLOATSPEED = -0.1f;
	float mWidth = 48;
	float mHeight = 48 * 2;
	const float mSTARTHEIGHT = mHeight; // the height needs to be stored so it can be reapplied after a balloon popped.
	const float mSTARTWIDTH = mWidth; // the width needs to be stored so it can be reapplied after a balloon popped.

	bool mGrounded = false;
	bool mCollisionFriction = true;
	bool jumpedOffRope = false;
	
	
	//map data
	const int* mpMAPDATA{};
	int mTILESIZE = 0;
	int mTILESCALE = 0;
	int mSCALEDTILESIZE = 0;
	int mAMOUNTOFTILESINAROW = 0;
	int mMAPWIDTH = 0;
	int mMAPHEIGHT = 0;
	Surface* mpTileSet;

	//balloon data
	Balloon* mpBalloon;

	//Player sprites
	float mSpriteFrame = 0; //number that increments to 10 and resets.
	float mTimeSinceMoved = 0.0f;
	Sprite* mpHarryRun;
	Sprite* mpHarryClimb;
	Sprite* mpHarrySwim;
	Sprite* mpHarrySwing;
	Sprite* mpHarryMoveUp;
	Sprite* mpHarryMoveDown;
	Sprite* mpHarryIdle;
	Sprite* mpHarryBored;
	Sprite* mpHarryDie;

	Audio::Sound* mpMainSoundTrack_S;
	Audio::Sound* mpCheckpoint_S;
	Audio::Sound* mpRun_S;
	Audio::Sound* mpSwim_S;
	Audio::Sound* mpClimb_S;
	Audio::Sound* mpDie_S;
};

