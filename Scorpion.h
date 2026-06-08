#pragma once
#include "Enemy.h"
class Scorpion :
    public Enemy
{
public:
    Scorpion(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH);
    ~Scorpion();
    void tick(float deltaTime);
    void draw();
private:
    float2 mStartPos;
    enum movementState {
        patrol, follow
    } mMovementState = patrol;
    const float mACTIVATIONRANGE = powf(500,2);
    const float mWALKDISTANCE = 5;
    Sprite* mpScorpion;
    int mMAPWIDTH;
    float mSpriteFrame = 0;
};