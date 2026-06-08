// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
//#include <iostream>
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "Rope.h"
#include "TileMap.h"
#include "Pit.h"
#include "ScoreManager.h"
#include <lib/json/json.hpp>



void Game::Init()
{
	std::ifstream jsonIn("./data.json");
	nlohmann::json jsonNew;
	if (!jsonIn.is_open()) {
		std::ofstream jsonOut("./data.json");
		jsonNew["playerSpeed"] = 1;
		jsonNew["shadersOn"] = true;
		jsonNew["lastScore"] = -100;
		jsonNew["playerPosX"] = 0;
		jsonNew["playerPosY"] = 0;
		jsonOut << jsonNew;
		jsonOut.close();
	}
	else {
		jsonNew = nlohmann::json::parse(jsonIn);
		shadersOn = jsonNew["shadersOn"];
	};
	jsonIn.close();

	mScreenUnscaled = new Surface(SCRWIDTH, SCRHEIGHT);
	mUnscaledScreenSprite = new Sprite(mScreenUnscaled, 1);
	myTileMap = new TileMap();
	mScoreManager = new ScoreManager(screen);
	myPlayer = new Player(mScreenUnscaled, myTileMap, mScoreManager);
	mCamera = new Camera(mScreenUnscaled, myPlayer, myTileMap);
	myPlayer->Init(mCamera);
	pGameObjects = myTileMap->getGameObjectData(myPlayer, mCamera, mScreenUnscaled, mScoreManager);
}

void Game::saveScore() {
	nlohmann::json jsonNew;
	std::ifstream jsonIn("./data.json");
	jsonNew = nlohmann::json::parse(jsonIn);
	jsonNew["lastScore"] = mScoreManager->getScore();
	jsonNew["playerPosX"] = myPlayer->getCheckpointPos().x;
	jsonNew["playerPosY"] = myPlayer->getCheckpointPos().y; 
	jsonIn.close();
	std::ofstream jsonOut("./data.json");
	jsonOut << jsonNew;
	jsonOut.close();
}
void Game::Shutdown() {
	saveScore();

	delete mUnscaledScreenSprite;
	delete myPlayer;
	delete myTileMap;
	delete mScoreManager;
	delete mCamera;
}


void Game::MouseDown(int) {

	nlohmann::json jsonNew;
	std::ifstream jsonIn("./data.json");


	jsonNew = nlohmann::json::parse(jsonIn);

	//cout << mousePos.x << " y, " << mousePos.y << "\n";
	switch (mGameState) {
	case start:
		if (mousePos.y > 400 && mousePos.y < 450) {
			//cout << "clicked continue";
			mScoreManager->UpdateScore(jsonNew["lastScore"]);
			myPlayer->setPos( float2(jsonNew["playerPosX"], jsonNew["playerPosY"]));
			mGameState = play;
		}
		else if (mousePos.y > 500 && mousePos.y < 550) {
			//cout << "clicked play";
			Shutdown();
			Init();
			mGameState = play;
		}
		else if (mousePos.y > 600 && mousePos.y < 650) {
			//cout << "clicked settings";
			mGameState = settings;
		}
		break;
	case settings:
		if (mousePos.y > 200 && mousePos.y < 250) {
			//cout << "1x";
			jsonNew.at("playerSpeed") = 1;

		}
		else if (mousePos.y > 300 && mousePos.y < 350) {
			//cout << "2x";
			jsonNew.at("playerSpeed") = 2;
		}
		else if (mousePos.y > 450 && mousePos.y < 500) {
			//cout << "shader yes";
			jsonNew.at("shadersOn") = true;
			shadersOn = true;

		}
		else if (mousePos.y > 550 && mousePos.y < 600) {
			//cout << "shaders no";
			jsonNew.at("shadersOn") = false;
			shadersOn = false;

		}
		else if (mousePos.y > 630 && mousePos.y < 680) {
			mGameState = start;
		}

		//jsonIn << jsonNew;
		break;
	default:
		if (mousePos.y > 500 && mousePos.y < 550) {
			//cout << "clicked play";
			Shutdown();
			Init(); 
			mGameState = play;
		}
		else if (mousePos.y > 600 && mousePos.y < 650) {
			//cout << "clicked menu";
			mGameState = start;
		}
	}
	jsonIn.close();
	std::ofstream jsonOut("./data.json");
	jsonOut << jsonNew;
	jsonOut.close();


}

void Game::Tick(float deltaTime)
{
	int score = mScoreManager->getScore();
	if (score < 0 && mGameState == play) {
		mGameState = lose;
		myPlayer->setCheckpointPos(float2(0,0));
		saveScore();
	}
	else if (score > mWINSCORE && mGameState == play) mGameState = win;

	std::ifstream jsonIn("./data.json");
	nlohmann::json jsonNew = nlohmann::json::parse(jsonIn);
	float lastScore = jsonNew["lastScore"];
	jsonIn.close();

	if (deltaTime < 50) {
		screen->Clear(0);
		switch (mGameState)
		{
		case start:
			


			
			screen->Print("Hole Drop", 100, 100, 255 + (255 << 8) + (255 << 16), 12); // white
			screen->Print("Get a score of 10k to win!", 100, 210, 255 << 8, 6); // green

			if (mousePos.y > 400 && mousePos.y < 450 && lastScore >= 0) {
				screen->Print("Continue", 100, 400, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("Play", 100, 500, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Settings", 100, 600, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 500 && mousePos.y < 550) {
				if (lastScore >= 0) screen->Print("Continue", 100, 400, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Play", 100, 500, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("Settings", 100, 600, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 600 && mousePos.y < 650) {
				if (lastScore >= 0) screen->Print("Continue", 100, 400, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Play", 100, 500, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Settings", 100, 600, 255 + (255 << 8) + (255 << 16), 10);
			}
			else {
				if (lastScore >= 0) screen->Print("Continue", 100, 400, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Play", 100, 500, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Settings", 100, 600, 255 + (255 << 8) + (255 << 16), 8);
			}
			break;
		case settings:
			screen->Print("Settings:", 100, 50, 255 + (255 << 8) + (255 << 16), 12); // white
			screen->Print("player speed multiplier:", 100, 140, 255 << 8, 6); // green
			screen->Print("Would you like the shader on:", 100, 380, 255 << 8, 6); // green

			if (mousePos.y > 200 && mousePos.y < 250) {
				screen->Print("1x", 100, 200, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("2x", 100, 300, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("yes", 100, 450, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("no", 100, 550, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Back to menu", 100, 630, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 300 && mousePos.y < 350) {
				screen->Print("1x", 100, 200, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("2x", 100, 300, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("yes", 100, 450, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("no", 100, 550, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Back to menu", 100, 630, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 450 && mousePos.y < 500) {
				screen->Print("1x", 100, 200, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("2x", 100, 300, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("yes", 100, 450, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("no", 100, 550, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Back to menu", 100, 630, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 550 && mousePos.y < 600) {
				screen->Print("1x", 100, 200, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("2x", 100, 300, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("yes", 100, 450, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("no", 100, 550, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("Back to menu", 100, 630, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 630 && mousePos.y < 680) {
				screen->Print("1x", 100, 200, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("2x", 100, 300, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("yes", 100, 450, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("no", 100, 550, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Back to menu", 100, 630, 255 + (255 << 8) + (255 << 16), 10);
			}
			else {
				screen->Print("1x", 100, 200, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("2x", 100, 300, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("yes", 100, 450, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("no", 100, 550, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("Back to menu", 100, 630, 255 + (255 << 8) + (255 << 16), 8);

			}
			break;
		case play:
			//the main game loop
			myPlayer->tick(deltaTime);
			mCamera->draw(deltaTime);
			myPlayer->draw(deltaTime);
			for (int i = 0; i < mNumberOfGameObjects; i++) {
				pGameObjects[i]->tick(deltaTime);
				pGameObjects[i]->draw();
			}
			drawScaledScreen();
			mScoreManager->Draw();
			break;
		case lose:
			screen->Print("You Lose!", 100, 100, 255 + (255 << 8) + (255 << 16), 12);
			screen->Print("Try to keep your score positive.", 100, 210, 255 << 16, 6);
			if (mousePos.y > 500 && mousePos.y < 550) {
				screen->Print("Play again", 100, 500, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("menu", 100, 600, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 600 && mousePos.y < 650) {
				screen->Print("Play again", 100, 500, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("menu", 100, 600, 255 + (255 << 8) + (255 << 16), 10);
			}
			else {
				screen->Print("Play again", 100, 500, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("menu", 100, 600, 255 + (255 << 8) + (255 << 16), 8);
			}
			break;
		case win:
			screen->Print("You Win!", 100, 100, 255 + (255 << 8) + (255 << 16), 12);
			screen->Print("Score: ", 100, 240, 255 + (255 << 8) + (255 << 16), 6);

			std::string str = std::to_string(score);
			screen->Print(str.c_str(), 320, 240, 255 << 8, 8);

			if (mousePos.y > 500 && mousePos.y < 550) {
				screen->Print("Play again", 100, 500, 255 + (255 << 8) + (255 << 16), 10);
				screen->Print("menu", 100, 600, 255 + (255 << 8) + (255 << 16), 8);
			}
			else if (mousePos.y > 600 && mousePos.y < 650) {
				screen->Print("Play again", 100, 500, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("menu", 100, 600, 255 + (255 << 8) + (255 << 16), 10);
			}
			else {
				screen->Print("Play again", 100, 500, 255 + (255 << 8) + (255 << 16), 8);
				screen->Print("menu", 100, 600, 255 + (255 << 8) + (255 << 16), 8);
			}
			break;
		}
	}
}

void Game::drawScaledScreen() {
	//mScale += 0.001f;
	if (myPlayer->getMovementMode() == Player::swinging && mFactor < 1) {
		mFactor += 0.01f;
	}
	else if (mFactor > 0) {
		mFactor -= 0.01f;
	}
	mScale = Interpolate(mMINSCALE, mMAXSCALE, mFactor);
	int scaledWidth = static_cast<int>(SCRWIDTH * mScale), scaledHeight = static_cast<int>(SCRHEIGHT * mScale);
	mUnscaledScreenSprite->DrawScaled((SCRWIDTH - scaledWidth) >> 1, (SCRHEIGHT - scaledHeight) >> 1, scaledWidth, scaledHeight, false, screen);
}
