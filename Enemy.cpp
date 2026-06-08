#include "precomp.h"
#include "Enemy.h"
#include "player.h"

Enemy::Enemy(float2 position, Player* player, Camera* camera, Surface* screen) : GameObject::GameObject(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
}


void Enemy::CollisionCheck() {
	for (int i = 0; i < mNumberOfCollidingZones; i++) {
		float2 playerCentrePos = mpPlayer->getCentrePos();
		float2 playerPos = mpPlayer->getPos();
		float playerWidth = (playerCentrePos.x - playerPos.x) * 2;
		float playerHeight = (playerCentrePos.y - playerPos.y) * 2;
		float playerRadius = pythagorean({ playerWidth,0 }, { 0,playerHeight });

		//circle colliders
		if (mCollidingZones[i].radius != NULL) {
			//early out check
			float powfRadius = powf(mCollidingZones[i].radius, 2);
			if (pythagorean(playerCentrePos, mPosition + mCollidingZones[i].relativePos) <= powfRadius + playerRadius) {
				//check if player corners are in circle
				if (pythagorean(playerPos, mPosition + mCollidingZones[i].relativePos) <= powfRadius) mpPlayer->die();
				else if (pythagorean(playerPos + float2(playerWidth, 0), mPosition + mCollidingZones[i].relativePos) <= powfRadius) mpPlayer->die();
				else if (pythagorean(playerPos + float2(0, playerHeight), mPosition + mCollidingZones[i].relativePos) <= powfRadius) mpPlayer->die();
				else if (pythagorean(playerPos + float2(playerWidth, playerHeight), mPosition + mCollidingZones[i].relativePos) <= powfRadius) mpPlayer->die();
			}
		}
		else if(mCollidingZones[i].width != NULL && mCollidingZones[i].height != NULL){
			// box colliders
			//early out check
			float2 centreOfCollidingZone = mPosition + mCollidingZones[i].relativePos + float2(static_cast<float>(mCollidingZones[i].width / 2), static_cast<float>(mCollidingZones[i].height / 2));
			if (pythagorean(playerCentrePos,centreOfCollidingZone ) <= playerRadius) {
				//AABB check for collision
				int leftX = static_cast<int>(mPosition.x);
				int rightX = static_cast<int>(mPosition.x + mCollidingZones[i].width);

				if (playerPos.y + playerHeight > mPosition.y && playerPos.y < mPosition.y + mHeight) {
					if (leftX != rightX && playerPos.x + playerWidth > leftX && playerPos.x < rightX) {
						mpPlayer->die();
					}
				}
			}
		}
	}
}


void Enemy::TurnAround() {
	mVel *= -1;
	float distanceToMiddle = 0;
	for (int i = 0; i < 3; i++) {
		if (mCollidingZones[i].relativePos.x > mWidth / 2) {
			distanceToMiddle = mWidth / 2 - mCollidingZones[i].relativePos.x;
			mCollidingZones[i].relativePos.x = mCollidingZones[i].relativePos.x + distanceToMiddle * 2 - mCollidingZones[i].width;
		}
		else {
			distanceToMiddle = mWidth / 2 - mCollidingZones[i].relativePos.x - mCollidingZones[i].width;
			mCollidingZones[i].relativePos.x = mCollidingZones[i].relativePos.x + distanceToMiddle * 2 + mCollidingZones[i].width;
		}
	}
}


void Enemy::drawHitboxes() {
	float2 camPos = mpCamera->getPos();
	for (int i = 0; i < 3; i++) {
		if (mCollidingZones[i].width != NULL
			&& mCollidingZones[i].height != NULL
			|| mCollidingZones[i].radius != NULL)
		{
			mpScreen->Box(static_cast<int>(mPosition.x + mCollidingZones[i].relativePos.x - camPos.x), static_cast<int>(mPosition.y + mCollidingZones[i].relativePos.y - camPos.y), static_cast<int>(mPosition.x + mCollidingZones[i].relativePos.x + mCollidingZones[i].width - camPos.x), static_cast<int>(mCollidingZones[i].height + mPosition.y + mCollidingZones[i].relativePos.y - camPos.y), 255);
		}
	}
}