#include "precomp.h"
#include "GameObject.h"
#include "Camera.h"


float2 GameObject::getCentrePos() {
	return float2(mPosition.x + (mWidth / 2), mPosition.y + (mHeight / 2));
}

void GameObject::tick(float deltaTime) {
	mDeltaTime = deltaTime;
}

void GameObject::draw(){
	/*float2 camPos = mpCamera->getPos();
	mpScreen->Bar(mPosition.x - camPos.x, mPosition.y - camPos.y, mPosition.x + mWidth - camPos.x, mPosition.y + mHeight - camPos.y, 255);*/
}