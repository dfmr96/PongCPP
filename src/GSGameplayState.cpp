#include "GSGameplayState.h"
#include "StructsDef.h"
#include "SDL.h"
#include "SDL_image.h"
#include "GameplayStructsDef.h"

namespace gameplay {
	enum SUBSTATE {
		INIT_STATE = 0,
		UPDATE_STATE,
		ENDING_STATE,
	};

	const float pongSpeed = 0.5f;
	float ballSpeed_X;
	float ballSpeed_Y;
	int subState = INIT_STATE;


	int player1Sprite_ResourceID = -1;
	int player2Sprite_ResourceID = -1;
	int ballSprite_ResourceID = -1;


	Boundary topBound;
	Boundary bottomBound;
	Boundary leftBound;
	Boundary rightBound;
	Player player1;
	Player player2;
	Ball ball;


	void LoadAssets(ResourceManager& resource) {
		SDL_Renderer* renderer = resource.renderer;
		SpriteAssets& spritesAssets = *resource.spritesAssets;



		string paletaFilePath = "assets/img/paleta.png";
		SDL_Texture* paletaTexture = IMG_LoadTexture(renderer, paletaFilePath.c_str());
		SDL_Rect paletaDest;
		paletaDest.x = WIDTH >> 4;
		paletaDest.y = (HEIGHT >> 1) - 15;
		paletaDest.w = 8;
		paletaDest.h = 30;

		Sprite player1Sprite;

		player1Sprite.dest = paletaDest;
		player1Sprite.texture = paletaTexture;
		spritesAssets.push_back(player1Sprite);

		player1Sprite_ResourceID = spritesAssets.size() - 1;

		Sprite player2Sprite;

		player2Sprite.dest = paletaDest;
		player2Sprite.dest.x = WIDTH - paletaDest.x;
		player2Sprite.texture = paletaTexture;
		spritesAssets.push_back(player2Sprite);

		player2Sprite_ResourceID = spritesAssets.size() - 1;

		string ballFilePath = "assets/img/pelota.png";
		SDL_Texture* ballTexture = IMG_LoadTexture(renderer, ballFilePath.c_str());
		SDL_Rect ballDest;
		ballDest.x = WIDTH >> 1;
		ballDest.y = HEIGHT >> 1;
		ballDest.w = 8;
		ballDest.h = 8;

		Sprite ballSprite;
		ballSprite.dest = ballDest;
		ballSprite.texture = ballTexture;
		spritesAssets.push_back(ballSprite);

		ballSprite_ResourceID = spritesAssets.size() - 1;

		player1.sprite = &spritesAssets[player1Sprite_ResourceID];
		player2.sprite = &spritesAssets[player2Sprite_ResourceID];
		ball.sprite = &spritesAssets[ballSprite_ResourceID];
	}

	void InitBoundaries() {
		SDL_Rect topBoundRect;
		topBoundRect.x = 0;
		topBoundRect.y = 0;
		topBoundRect.w = WIDTH;
		topBoundRect.h = 1;

		topBound.rect = topBoundRect;

		SDL_Rect bottomBoundRect;
		bottomBoundRect.x = 0;
		bottomBoundRect.y = HEIGHT;
		bottomBoundRect.w = WIDTH;
		bottomBoundRect.h = 1;

		bottomBound.rect = bottomBoundRect;
	}

	void UpdateMovements(float deltaTime, ResourceManager& resource) {
		InputState inputState = *resource.inputState;
		SpriteAssets& spritesAssets = *resource.spritesAssets;

		if (inputState.player1Up) {
			player1.sprite->dest.y -= 0.5 * deltaTime;
		}
		if (inputState.player1Down) {
			player1.sprite->dest.y += 0.5 * deltaTime;
		}
		if (inputState.player2Up) {
			player2.sprite->dest.y -= 0.5 * deltaTime;
		}
		if (inputState.player2Down) {
			player2.sprite->dest.y += 0.5 * deltaTime;
		}

		if (player1.sprite->dest.y <= 0) {
			player1.sprite->dest.y = 0;
		}

		if (player1.sprite->dest.y >= (480 - 30)) {
			player1.sprite->dest.y = 450;
		}

		if (player2.sprite->dest.y <= 0) {
			player2.sprite->dest.y = 0;
		}

		if (player2.sprite->dest.y >= (480 - 30)) {
			player2.sprite->dest.y = 450;
		}
	}

	void BallInitSpeed() {

		srand(time(NULL));
		int xSide = rand() % 2;
		int ySide = rand() % 2;
		float randomX = (10 + rand() % 15) / 100.00f;
		float randomY = (10 +rand() % 15) / 100.00f;

		if (xSide == 0) {
			randomX = -randomX;
		}

		if (ySide == 0) {
			randomY = -randomY;
		}

		ballSpeed_X = randomX;
		ballSpeed_Y = randomY;
	}

	void BallMovement(float deltaTime, ResourceManager& resource) {
		SpriteAssets& spritesAssets = *resource.spritesAssets;

		ball.sprite->dest.x += ballSpeed_X * deltaTime;
		ball.sprite->dest.y += ballSpeed_Y * deltaTime;
	}

	void UpdateColission(float deltaTime, ResourceManager& resource) {
		SpriteAssets& spritesAssets = *resource.spritesAssets;

		SDL_Rect* player1Rect = &player1.sprite->dest;
		SDL_Rect* player2Rect = &player2.sprite->dest;
		SDL_Rect* ballRect = &ball.sprite->dest;

		bool player1hit = SDL_HasIntersection(player1Rect, ballRect);
		bool player2hit = SDL_HasIntersection(player2Rect, ballRect);
		bool topBoundhit = SDL_HasIntersection(ballRect, &topBound.rect);
		bool bottomBoundhit = SDL_HasIntersection(ballRect, &bottomBound.rect);


		if (player1hit || player2hit) {
			ballSpeed_X = -ballSpeed_X;
		}

		if (topBoundhit || bottomBoundhit) {
			ballSpeed_Y = -ballSpeed_Y;
		}
	}
}

using namespace gameplay;

void GSGameplayStateUpdate(float deltaTime, ResourceManager& resource)
{

	switch (subState) {
	case INIT_STATE:
		LoadAssets(resource);
		InitBoundaries();
		BallInitSpeed();
		subState = UPDATE_STATE;
		break;
	case UPDATE_STATE:
		UpdateMovements(deltaTime, resource);
		BallMovement(deltaTime, resource);
		UpdateColission(deltaTime, resource);
		break;
	}
}
