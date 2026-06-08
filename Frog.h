#pragma once
#include "Enemy.h"

namespace Audio {
    class Sound;
};
class Frog :
    public Enemy
{
public:
    Frog(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH);
    ~Frog();
    void tick(float deltaTime);
    void draw();
private:
    enum movementState {
        idle, jump
    } mMovementState = idle;
    const float mFLYDISTANCE = 5;
    const float mJUMPHEIGHT = -100;
    const float mJUMPDISTANCE = 64;
    float2 mStartPos;
    float mWaitedTime = 0;
    const float mTIMEUNTILJUMP = 2000;
    Audio::Sound* mpCroak_S;
    Sprite* mpFrogHop;
    Sprite* mpFrogIdle;
    int mMAPWIDTH;
    float mSpriteFrame = 0;
};