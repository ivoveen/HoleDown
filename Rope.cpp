#include "precomp.h"
#include "Rope.h"
#include "Player.h"
//#include <iostream> 

Rope::Rope(float2 position, Player* player, Camera* camera, Surface* screen) : GameObject::GameObject(mPosition, mpPlayer, mpCamera, mpScreen) {
	mPosition = position;
	mpPlayer = player;
	mpCamera = camera;
	mpScreen = screen;
	for (int i = 0; i < mROPEPARTS; i++) {
		//mpRopePoints[i] = new Rope::ropePoint{ {mPosition.x + (i * (mMAXDIST)), mPosition.y },{mPosition.x + (i * (mMAXDIST)), mPosition.y },false };
		mpRopePoints[i] = new Rope::ropePoint{ mPosition + normalize(float2(1,1)) * i * (mMAXDIST - 5),mPosition + normalize(float2(1,1)) * i * (mMAXDIST - 5),false };

	}
	mpRopePoints[0]->anchored = true;
}
Rope::~Rope() {
	for (int i = 0; i < mROPEPARTS; i++) {
		delete* (mpRopePoints + i);
	}
}

void Rope::tick(float deltaTime) {
	simulate(deltaTime);
	if (pythagorean(mpPlayer->getCentrePos(), getCentrePos()) < powf(mROPEPARTS * mMAXDIST + mINTERACTRANGE, 2)) {
		if (pythagorean(mpPlayer->getCentrePos(), mpRopePoints[mROPEPARTS - 1]->position) < powf(mINTERACTRANGE,2)) mpPlayer->hitTheRope(mpRopePoints[mROPEPARTS - 1]->position, mpRopePoints[mROPEPARTS - 1]->prevPosition);
	}
}

void Rope::simulate(float deltaTime) {
	if ((mpRopePoints[1]->position.x - mpRopePoints[1]->prevPosition.x) < 0) {
		if (mMovingRight && mMostRightPoint - mpRopePoints[1]->position.x < 20) {
			mMostRightPoint = mpRopePoints[1]->position.x;
			mMovingRight = false;
		//	cout << "most right \n";
		}
	}
	else {
		if (!mMovingRight) {
			//cout << "most left \n";
			if (mMostRightPoint - mpRopePoints[1]->prevPosition.x > 1){
				 currentSwingDistance = mMostRightPoint - mpRopePoints[1]->prevPosition.x; 
			}
			if (mSwingDistance >= currentSwingDistance) boost = true;// cout << "boost true \n";
			else boost = false; //cout << "boost false \n"; 
			mMovingRight = true;
		}
	}
	for (int i = 0; i < mROPEPARTS; i++) {
		if (!mpRopePoints[i]->anchored) {
			float2 oldPosition = mpRopePoints[i]->position;
			mpRopePoints[i]->position += (mpRopePoints[i]->position - mpRopePoints[i]->prevPosition);
			if (boost)mpRopePoints[i]->position += (mpRopePoints[i]->position - mpRopePoints[i]->prevPosition) * 0.00012f * deltaTime;
			mpRopePoints[i]->position.y += mGRAVITY * deltaTime * deltaTime * 0.5f;
			mpRopePoints[i]->prevPosition = oldPosition;
		}
	}
	for (int j = 0; j < mNUMITERATIONS; j++) {
		for (int i = 0; i < mROPEPARTS - 1; i++) {
			float2 lineCentre = (mpRopePoints[i + 1]->position + mpRopePoints[i]->position) / 2;
			float2 lineDirection = normalize(mpRopePoints[i + 1]->position - mpRopePoints[i]->position);
			if (!mpRopePoints[i]->anchored) mpRopePoints[i]->position = lineCentre - (lineDirection * (mMAXDIST / 2));
			if (!mpRopePoints[i + 1]->anchored) mpRopePoints[i + 1]->position = lineCentre + (lineDirection * (mMAXDIST / 2));
		}
	}
}
void Rope::draw() { 
	float2 camPos = mpCamera->getPos();
	for (int i = 0; i < mROPEPARTS - 1; i++) {
		mpScreen->Line(mpRopePoints[i]->position.x - camPos.x, mpRopePoints[i]->position.y - camPos.y, mpRopePoints[i + 1]->position.x - camPos.x, mpRopePoints[i + 1]->position.y - camPos.y, 0);
	}
}