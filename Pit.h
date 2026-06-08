#pragma once
#include "GameObject.h"
class Player;
class Camera;
namespace Audio {
	class Sound;
};
class Pit : public GameObject
{
public:
	enum PitType {
		pit, shrinkingPit, waterPit, waterPitWithCrocs
	};

	Pit(float2 position, Player* player, Camera* camera, Surface* screen, PitType typeOfPit);
	~Pit();
	void tick(float deltaTime);
	void draw(); 


private:
enum CrocState {
	closed, open
}mCrocState = closed;
	Sprite* mpCroc;
	const int mCROCODILETIME = 4000;
	const int mCROCBORDER = 50;
	const int mCROCSPACING = 128;
	const int mCROCPITMAXWIDTH = 420;
	const int CROCWIDTH = 64;
	const int mMAXWIDTH = 320;
	const int mMINWIDTH = 0;
	const int mHeight = 50;
	const int mSHRINKPAUSEMAXTIME = 1500;
	float mShrinkSpeed = 0.001f;
	float mShrinkPauseTime = 0;
	float currentWidthPercentage = 1;
	PitType mPitType;
	Audio::Sound* mpCroc_S;

};
