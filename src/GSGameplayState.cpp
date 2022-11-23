#include "GSGameplayState.h"
#include "StructsDef.h"
#include "SDL.h"
#include "SDL_image.h"
#include "GameplayStructsDef.h"
#include <string>
#include "Utils.h"

namespace gameplay {
	enum SUBSTATE {
		INIT_STATE = 0,
		UPDATE_STATE,
		MATCH_ENDED,
		ENDING_STATE,
	};
	const float pongSpeed = 0.5f;
	float ballSpeed_X = 0;
	float ballSpeed_Y = 0;
	int subState = INIT_STATE;

	int bg_resourceID = -1;
	int player1Sprite_ResourceID = -1;
	int player2Sprite_ResourceID = -1;
	int ballSprite_ResourceID = -1;
	int player1ScoreText_ResourceID = -1;
	int player2ScoreText_ResourceID = -1;
	int player1WinsText_ResourceID = -1;
	int player2WinsText_ResourceID = -1;
	int golMusic_ResourceID = -1;
	int bounceMusic_ResourceID = -1;
	int paletMusic_ResourceID = -1;


	Boundary topBound;
	Boundary bottomBound;
	Boundary leftBound;
	Boundary rightBound;
	Player player1;
	Player player2;

	const float BALL_BLINK_SPEED = 1.0f;
	
	Ball ball;
	bool resetBall = false;
	float ballTimer = 0.00f;
	float collisionTimer = 0.00f;
	float toMainMenuTimer = 0.00f;

	const float PALET_SPEED = 0.5f;
	const float BALL_SPAWN_TIME = 1500.00f;
	const float BALL_COLLISION_TIME = 800.00f;
	const float MAIN_MENU_TIME = 3000.00f;
	const int MAX_POINTS = 5;

	void LoadAssets(ResourceManager& resource) {
		SDL_Renderer* renderer = resource.renderer;
		SpriteAssets& spritesAssets = *resource.spritesAssets;
		TextAssets& textAssets = *resource.textAssets;
		BgmAssets& musicAssets = *resource.musicAssets;


		////////TEXTOS///////////

		string fontFilePath = "assets/fonts/bit5x3.ttf";
		TTF_Font* bit5x3 = TTF_OpenFont(fontFilePath.c_str(), 36); //this opens a font style and sets a size
		SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
		SDL_Surface* player1ScoreSurface = TTF_RenderText_Solid(bit5x3, "0", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_Texture* player1ScoreTexture = SDL_CreateTextureFromSurface(renderer, player1ScoreSurface); //now you can convert it into a texture
		SDL_Rect player1ScoreRect;
		player1ScoreRect.h = HEIGHT * 0.2;
		player1ScoreRect.w = WIDTH * 0.06;
		player1ScoreRect.x = (WIDTH >> 1) - (player1ScoreRect.w * 3/2);
		player1ScoreRect.y = HEIGHT >> 4;

		Text player1ScoreText;
		player1ScoreText.font = bit5x3;
		player1ScoreText.color = White;
		player1ScoreText.surface = player1ScoreSurface;
		player1ScoreText.texture = player1ScoreTexture;
		player1ScoreText.dest = player1ScoreRect;

		textAssets.push_back(player1ScoreText);
		player1ScoreText_ResourceID = textAssets.size() - 1;

		SDL_Surface* player2ScoreSurface = TTF_RenderText_Solid(bit5x3, "0", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_Texture* player2ScoreTexture = SDL_CreateTextureFromSurface(renderer, player2ScoreSurface); //now you can convert it into a texture
		SDL_Rect player2ScoreRect;
		player2ScoreRect.h = HEIGHT * 0.2;
		player2ScoreRect.w = WIDTH * 0.06;
		player2ScoreRect.x = (WIDTH >> 1) + (player2ScoreRect.w * 1/ 2);
		player2ScoreRect.y = HEIGHT >> 4;

		Text player2ScoreText;
		player2ScoreText.font = bit5x3;
		player2ScoreText.color = White;
		player2ScoreText.surface = player2ScoreSurface;
		player2ScoreText.texture = player2ScoreTexture;
		player2ScoreText.dest = player2ScoreRect;

		textAssets.push_back(player2ScoreText);
		player2ScoreText_ResourceID = textAssets.size() - 1;

		SDL_Surface* player1WinsSurface = TTF_RenderText_Solid(bit5x3, "PLAYER 1 WINS", White);
		SDL_Texture* player1WinsTexture = SDL_CreateTextureFromSurface(renderer, player1WinsSurface);
		SDL_Rect player1WinsRect;
		player1WinsRect.h = HEIGHT * 0.3;
		player1WinsRect.w = WIDTH * 0.6;
		player1WinsRect.x = (WIDTH >> 1) - (player1WinsRect.w * 1/2);
		player1WinsRect.y = (HEIGHT >> 1) - (player1WinsRect.h * 1/2);

		Text player1WinsText;
		player1WinsText.font = bit5x3;
		player1WinsText.color = White;
		player1WinsText.surface = player1WinsSurface;
		player1WinsText.texture = player1WinsTexture;
		player1WinsText.dest = player1WinsRect;

		textAssets.push_back(player1WinsText);
		player1WinsText_ResourceID = textAssets.size() - 1;
		textAssets[player1WinsText_ResourceID].isVisible = false;

		SDL_Surface* player2WinsSurface = TTF_RenderText_Solid(bit5x3, "PLAYER 2 WINS", White);
		SDL_Texture* player2WinsTexture = SDL_CreateTextureFromSurface(renderer, player2WinsSurface);
		SDL_Rect player2WinsRect;
		player2WinsRect.h = HEIGHT * 0.3;
		player2WinsRect.w = WIDTH * 0.6;
		player2WinsRect.x = (WIDTH >> 1) - (player1WinsRect.w * 1 / 2);
		player2WinsRect.y = (HEIGHT >> 1) - (player1WinsRect.h * 1 / 2);

		Text player2WinsText;
		player2WinsText.font = bit5x3;
		player2WinsText.color = White;
		player2WinsText.surface = player2WinsSurface;
		player2WinsText.texture = player2WinsTexture;
		player2WinsText.dest = player2WinsRect;

		textAssets.push_back(player2WinsText);
		player2WinsText_ResourceID = textAssets.size() - 1;
		textAssets[player2WinsText_ResourceID].isVisible = false;

		///////SPRITES////////

		string filePath = "assets/img/background.png";
		SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
		SDL_Rect dest;
		dest.x = 0;
		dest.y = 0;
		dest.w = WIDTH;
		dest.h = HEIGHT;

		Sprite bgSprite;
		bgSprite.dest = dest;
		bgSprite.texture = texture;
		spritesAssets.push_back(bgSprite); // Se agrega al final del vector

		bg_resourceID = spritesAssets.size() - 1; // Entonces obtengo el indice del asset agregado, asi luego lo puedo usar o eliminar.


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

		string golSoundFilePath = "assets/bgm/gol.mp3";
		Mix_Music* golMusic;
		golMusic = Mix_LoadMUS(golSoundFilePath.c_str());

		Bgm bgm02;
		bgm02.music = golMusic;

		musicAssets.push_back(bgm02);

		golMusic_ResourceID = musicAssets.size() - 1;

		string bounceSoundFilePath = "assets/bgm/bounce.mp3";
		Mix_Music* bounceMusic;
		bounceMusic = Mix_LoadMUS(bounceSoundFilePath.c_str());

		Bgm bgm03;
		bgm03.music = bounceMusic;

		musicAssets.push_back(bgm03);

		bounceMusic_ResourceID = musicAssets.size() - 1;

		string paletSoundFilePath = "assets/bgm/palet.mp3";
		Mix_Music* paletMusic;
		paletMusic = Mix_LoadMUS(paletSoundFilePath.c_str());

		Bgm bgm04;
		bgm04.music = paletMusic;

		musicAssets.push_back(bgm04);

		paletMusic_ResourceID = musicAssets.size() - 1;
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
			player1.sprite->dest.y -= PALET_SPEED * deltaTime;
		}
		if (inputState.player1Down) {
			player1.sprite->dest.y += PALET_SPEED * deltaTime;
		}

		if (singleplayer) {
			if (inputState.player2Up) {
				player2.sprite->dest.y -= PALET_SPEED * deltaTime;
			}
			if (inputState.player2Down) {
				player2.sprite->dest.y += PALET_SPEED * deltaTime;
			}
		}
		else {
			if (player2.sprite->dest.y > ball.sprite->dest.y) {
				player2.sprite->dest.y -= PALET_SPEED * deltaTime;
			}
			if (player2.sprite->dest.y < ball.sprite->dest.y) {
				player2.sprite->dest.y += PALET_SPEED * deltaTime;

			}
		}

		if (player1.sprite->dest.y <= 0) {
			player1.sprite->dest.y = 0;
		}

		if (player1.sprite->dest.y >= (HEIGHT - 30)) {
			player1.sprite->dest.y = 450;
		}

		if (player2.sprite->dest.y <= 0) {
			player2.sprite->dest.y = 0;
		}

		if (player2.sprite->dest.y >= (HEIGHT - 30)) {
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

	void BounceBall(ResourceManager& resource) {
		BgmAssets& musicAssets = *resource.musicAssets;

		collisionTimer = 0.00f;
		Mix_PlayMusic(musicAssets[paletMusic_ResourceID].music, 1);
		ballSpeed_X = -ballSpeed_X;
		IncreaseBallSpeed();
	}

	void MatchEnd(ResourceManager& resource, bool player1won) {
		TextAssets& textAssets = *resource.textAssets;
		if (player1won) {
		textAssets[player1WinsText_ResourceID].isVisible = true;
		}
		else {
		textAssets[player2WinsText_ResourceID].isVisible = true;
		}
		subState = MATCH_ENDED;
	}

	void UpdateColission(float deltaTime, ResourceManager& resource) {
		SpriteAssets& spritesAssets = *resource.spritesAssets;
		BgmAssets& musicAssets = *resource.musicAssets;

		SDL_Rect* player1Rect = &player1.sprite->dest;
		SDL_Rect* player2Rect = &player2.sprite->dest;
		SDL_Rect* ballRect = &ball.sprite->dest;

		collisionTimer += deltaTime;

		bool player1Hit = SDL_HasIntersection(player1Rect, ballRect);
		bool player2Hit = SDL_HasIntersection(player2Rect, ballRect);
		bool topBoundHit = SDL_HasIntersection(ballRect, &topBound.rect);
		bool bottomBoundHit = SDL_HasIntersection(ballRect, &bottomBound.rect);
		bool leftBoundHit = SDL_HasIntersection(ballRect, &leftBound.rect);
		bool rightBoundHit = SDL_HasIntersection(ballRect, &rightBound.rect);

		if ((player1Hit || player2Hit) && collisionTimer > BALL_COLLISION_TIME) {
			BounceBall(resource);
		}

		if (topBoundHit || bottomBoundHit) {
			Mix_PlayMusic(musicAssets[bounceMusic_ResourceID].music, 1);
			ballSpeed_Y = -ballSpeed_Y;
		}

		if (leftBoundHit || rightBoundHit) {

			Mix_PlayMusic(musicAssets[golMusic_ResourceID].music, 1);
			if (rightBoundHit) {
				player1.points++;
				changeTextFromSurface(std::to_string(player1.points), resource, player1ScoreText_ResourceID);
				if (player1.points >= MAX_POINTS) {
					MatchEnd(resource, true);
				}
			}

			if (leftBoundHit) {
				player2.points++;
				changeTextFromSurface(std::to_string(player2.points), resource, player2ScoreText_ResourceID);
				if (player2.points >= MAX_POINTS) {
					MatchEnd(resource, false);
				}
			}
			ballSpeed_X = 0;
			ballSpeed_Y = 0;
			SDL_Delay(500);
			resetBall = true;
		}
		if (resetBall) {
			BallReset(deltaTime, resource);
		}
	}
	void unloadAssets(ResourceManager& resource) {
		SpriteAssets& spritesAssets = *resource.spritesAssets;
		TextAssets& textAssets = *resource.textAssets;
		BgmAssets& musicAssets = *resource.musicAssets;

		for (int i = 0; i < spritesAssets.size(); i++) {
			SDL_DestroyTexture(spritesAssets[i].texture);
			spritesAssets.erase(spritesAssets.begin());
		}

		for (int i = 0; i < textAssets.size(); i++) {
			SDL_DestroyTexture(textAssets[i].texture);
			SDL_FreeSurface(textAssets[i].surface);
			textAssets.erase(textAssets.begin());
		}
	}
	
}

using namespace gameplay;

void GSGameplayStateUpdate(float deltaTime, ResourceManager& resource)
{
	GameStages& gameStages = *resource.gameStages;

	switch (subState) {
	case INIT_STATE:
		player1.points = 0;
		player2.points = 0;
		LoadAssets(resource);
		InitBoundaries();
		resetBall = true;
		BallReset(deltaTime,resource);
		subState = UPDATE_STATE;
		break;
	case UPDATE_STATE:
		UpdateMovements(deltaTime, resource);
		BallMovement(deltaTime, resource);
		UpdateColission(deltaTime, resource);
		break;
	case MATCH_ENDED:
		toMainMenuTimer += deltaTime;

		if (toMainMenuTimer > MAIN_MENU_TIME) {
			unloadAssets(resource);
			subState = INIT_STATE;
		}
		break;
	}
}
