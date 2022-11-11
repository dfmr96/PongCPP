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
	float ballSpeed_X = 0;
	float ballSpeed_Y = 0;
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
	bool resetBall = false;
	float ballTimer = 0.00f;

	const float BALL_SPAWN_TIME = 1500.00f;



	void LoadAssets(ResourceManager& resource) {
		SDL_Renderer* renderer = resource.renderer;
		SpriteAssets& spritesAssets = *resource.spritesAssets;
		TextAssets& textAssets = *resource.textAssets;

		string fontFilePath = "assets/fonts/bit5x3.ttf";
		TTF_Font* bit5x3 = TTF_OpenFont(fontFilePath.c_str(), 64); //this opens a font style and sets a size
		SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
		SDL_Surface* player1ScoreSurface = TTF_RenderText_Solid(bit5x3, "${player1}", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_Texture* player1ScoreTexture = SDL_CreateTextureFromSurface(renderer, player1ScoreSurface); //now you can convert it into a texture

		Text player1ScoreText;
		player1ScoreText.font = bit5x3;
		player1ScoreText.color = White;
		player1ScoreText.surface = player1ScoreSurface;
		player1ScoreText.texture = player1ScoreTexture;
		player1ScoreText.dest = multiPlayerRect;

		textAssets.push_back(player1ScoreText);
		player1Text_resourceID = textAssets.size() - 1;


		string paletaFilePath = "assets/img/paleta.png";
		SDL_Texture* paletaTexture = IMG_LoadTexture(renderer, paletaFilePath.c_str());
		SDL_Rect paletaDest;
		paletaDest.x = WIDTH >> 4;
		paletaDest.y = (HEIGHT >> 1) - 15;
		paletaDest.w = 8;
		paletaDest.h = 30;

		SDL_Rect paleta2Dest;
		paleta2Dest.x = WIDTH - (WIDTH >> 4);
		paleta2Dest.y = (HEIGHT >> 1) - 15;
		paleta2Dest.w = 8;
		paleta2Dest.h = 30;

		Sprite player1Sprite;

		player1Sprite.dest = paletaDest;
		player1Sprite.texture = paletaTexture;
		spritesAssets.push_back(player1Sprite);

		player1Sprite_ResourceID = spritesAssets.size() - 1;

		Sprite player2Sprite;

		player2Sprite.dest = paleta2Dest;
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

		SDL_Rect leftBoundRect;
		leftBoundRect.x = 0;
		leftBoundRect.y = 0;
		leftBoundRect.w = 5;
		leftBoundRect.h = HEIGHT;

		leftBound.rect = leftBoundRect;

		SDL_Rect rightBoundRect;
		rightBoundRect.x = WIDTH;
		rightBoundRect.y = 0;
		rightBoundRect.w = 5;
		rightBoundRect.h = HEIGHT;

		rightBound.rect = rightBoundRect;
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

	void BallReset(float deltaTime, ResourceManager& resource) {
		SpriteAssets& spritesAssets = *resource.spritesAssets;

		ball.sprite->dest.x = WIDTH >> 1;
		ball.sprite->dest.y = HEIGHT >> 1;

		ballTimer += deltaTime;

		if (ballTimer > BALL_SPAWN_TIME) {
			BallInitSpeed();
			resetBall = false;
			ballTimer = 0.00f;
		}

	}
	void IncreaseBallSpeed() {
		if (ballSpeed_X > 0) {
			ballSpeed_X += 0.01f;
		}
		else {
			ballSpeed_X -= 0.01f;
		}
	}

	void UpdateColission(float deltaTime, ResourceManager& resource) {
		SpriteAssets& spritesAssets = *resource.spritesAssets;

		SDL_Rect* player1Rect = &player1.sprite->dest;
		SDL_Rect* player2Rect = &player2.sprite->dest;
		SDL_Rect* ballRect = &ball.sprite->dest;

		bool player1Hit = SDL_HasIntersection(player1Rect, ballRect);
		bool player2Hit = SDL_HasIntersection(player2Rect, ballRect);
		bool topBoundHit = SDL_HasIntersection(ballRect, &topBound.rect);
		bool bottomBoundHit = SDL_HasIntersection(ballRect, &bottomBound.rect);
		bool leftBoundHit = SDL_HasIntersection(ballRect, &leftBound.rect);
		bool rightBoundHit = SDL_HasIntersection(ballRect, &rightBound.rect);

		if (player1Hit || player2Hit) {
			ballSpeed_X = -ballSpeed_X;
			IncreaseBallSpeed();
		}

		if (topBoundHit || bottomBoundHit) {
			ballSpeed_Y = -ballSpeed_Y;
		}

		if (leftBoundHit || rightBoundHit) {
			ballSpeed_X = 0;
			ballSpeed_Y = 0;
			SDL_Delay(500);
			resetBall = true;
		}
		if (resetBall) {
			BallReset(deltaTime, resource);
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
