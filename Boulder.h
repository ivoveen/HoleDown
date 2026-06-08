#pragma once
#include "Enemy.h"
class Boulder :
    public Enemy
{
public:
    Boulder(float2 position, Player* player, Camera* camera, Surface* screen, int MAPWIDTH);
    ~Boulder();
    void tick(float deltaTime);
    void draw();
private:
    Sprite* mpBoulder;
    int mMAPWIDTH;
    bool mTriggered = false;
    const int mACTIVATIONRANGE = static_cast<int>(powf(SCRWIDTH * 0.2,2));
    float mSpriteFrame = 0;
};

