#pragma once
#include "GameObject.h"
class sprite;
class surface;

class Enemy : public GameObject
{
public:
	 Enemy(float2 position, Player* player, Camera* camera, Surface* screen);
	 void CollisionCheck();
	 void TurnAround();
	 void drawHitboxes();

protected:
	float2 mVel = {0,0};
	struct CollidingZone {
		float2 relativePos = {}; //collidingzone position relative to enemy position
		int width = 0;
		int height = 0;
		float radius =0; // if radius != NULL it is a circle collider
		CollidingZone() {};
		CollidingZone(float2 aRelativePos, int aWidth, int aHeight, float aRadius) {
			relativePos = aRelativePos;
			width = aWidth;
			height = aHeight;
			radius = aRadius;
		}
	}mCollidingZones[3];
	const int mNumberOfCollidingZones = 3;


};

