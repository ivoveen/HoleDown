#pragma once
#include "Enemy.h"
class Eel :
    public Enemy
{
public:
    Eel(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH);
    ~Eel();
    void tick(float deltaTime);
    void draw();
private:    
    float2 mStartPos;
    const float mSWIMDISTANCE = 5;
    Sprite* mpEel;
    int mMAPWIDTH;
    float mSpriteFrame = 0;
};