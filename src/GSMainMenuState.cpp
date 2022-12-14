#include "GSMainMenuState.h"
#include "StructsDef.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Utils.h"
#include "Global.h"

namespace mainmenu {
	enum SUBSTATE {
		INIT_STATE = 0,
		PRESSKEY_STATE,
		MAINMENU_STATE,
		ENDING_STATE,
	};

	int subState = INIT_STATE;
	float timer = 1.0f * 1000;
	int mainTitle_resourceID = -1;
	int pressStartText_resourceID = -1;
	int singlePlayerText_resourceID = -1;
	int multiPlayerText_resourceID = -1;
	int bg_resourceID = -1;

	const float BLINK_SPEED = 2.00f;

	bool isOnPlayerSelect = false;
	bool _2PlayersSelect = false;

	void LoadAssets(ResourceManager& resource) {

		//Cargar Titulo Principal
		TextAssets& textAssets = *resource.textAssets;
		SpriteAssets& spriteAssets = *resource.spritesAssets;
		SDL_Renderer* renderer = resource.renderer;
		SpriteAssets& spritesAssets = *(resource.spritesAssets);

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

		string logofilePath = "assets/img/ultimatepong.png";
		SDL_Texture* logoTexture = IMG_LoadTexture(renderer, logofilePath.c_str());
		SDL_Rect logoDest;
		logoDest.x = 0;
		logoDest.y = 0;
		logoDest.w = WIDTH;
		logoDest.h = HEIGHT;

		Sprite logoSprite;
		logoSprite.dest = logoDest;
		logoSprite.texture = logoTexture;
		spritesAssets.push_back(logoSprite);

		mainTitle_resourceID = spritesAssets.size() - 1;

		// CARGAR TEXTOS
		string fontFilePath = "assets/fonts/bit5x3.ttf";
		TTF_Font* bit5x3 = TTF_OpenFont(fontFilePath.c_str(), 24); //this opens a font style and sets a size
		SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
		SDL_Surface* pressStartSurface = TTF_RenderText_Solid(bit5x3, "PRESS START", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_Surface* singlePlayerSurface = TTF_RenderText_Solid(bit5x3, "1 PLAYER", White);
		SDL_Surface* multiPlayerSurface = TTF_RenderText_Solid(bit5x3, "2 PLAYER", White);

		SDL_Texture* pressStartTexture = SDL_CreateTextureFromSurface(renderer, pressStartSurface); //now you can convert it into a texture
		SDL_Texture* singlePlayerTexture = SDL_CreateTextureFromSurface(renderer, singlePlayerSurface);
		SDL_Texture* multiPlayerTexture = SDL_CreateTextureFromSurface(renderer, multiPlayerSurface);

		SDL_Rect pressStartRect; //create a rect
		pressStartRect.w = WIDTH * 0.2; // controls the width of the rect
		pressStartRect.h = HEIGHT * 0.1; // controls the height of the rect
		pressStartRect.x = (WIDTH >> 1) - (pressStartRect.w >> 1);  //controls the rect's x coordinate
		pressStartRect.y = HEIGHT - (HEIGHT >> 2); // controls the rect's y coordinte

		SDL_Rect singlePlayerRect;
		singlePlayerRect.w = WIDTH * 0.1;
		singlePlayerRect.h = HEIGHT * 0.05;
		singlePlayerRect.x = (WIDTH >> 1) - (singlePlayerRect.w >> 1);
		singlePlayerRect.y = HEIGHT - (HEIGHT >> 2);

		SDL_Rect multiPlayerRect;
		multiPlayerRect.w = WIDTH * 0.1;
		multiPlayerRect.h = HEIGHT * 0.05;
		multiPlayerRect.x = (WIDTH >> 1) - (multiPlayerRect.w >> 1);
		multiPlayerRect.y = HEIGHT - (HEIGHT >> 3);

		Text pressStartText;
		pressStartText.font = bit5x3;
		pressStartText.color = White;
		pressStartText.surface = pressStartSurface;
		pressStartText.texture = pressStartTexture;
		pressStartText.dest = pressStartRect;

		Text singlePlayerText;
		singlePlayerText.font = bit5x3;
		singlePlayerText.color = White;
		singlePlayerText.surface = singlePlayerSurface;
		singlePlayerText.texture = singlePlayerTexture;
		singlePlayerText.dest = singlePlayerRect;

		Text multiPlayerText;
		multiPlayerText.font = bit5x3;
		multiPlayerText.color = White;
		multiPlayerText.surface = multiPlayerSurface;
		multiPlayerText.texture = multiPlayerTexture;
		multiPlayerText.dest = multiPlayerRect;

		textAssets.push_back(pressStartText);
		pressStartText_resourceID = textAssets.size() - 1;

		textAssets.push_back(singlePlayerText);
		singlePlayerText_resourceID = textAssets.size() - 1;

		textAssets.push_back(multiPlayerText);
		multiPlayerText_resourceID = textAssets.size() - 1;

	}

	void HideAssets(ResourceManager& resource) {
		TextAssets& textAssets = *resource.textAssets;
		SpriteAssets& spriteAssets = *resource.spritesAssets;

		spriteAssets[mainTitle_resourceID].isVisible = false;
		textAssets[pressStartText_resourceID].isVisible = false;
		textAssets[singlePlayerText_resourceID].isVisible = false;
		textAssets[multiPlayerText_resourceID].isVisible = false;
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
			textAssets[i].isVisible = false;
		}
	}
}

using namespace mainmenu;

void GSMainMenuStateUpdate(float deltaTime, ResourceManager& resource)
{
	InputState inputState = *resource.inputState;
	TextAssets& textAssets = *resource.textAssets;
	SpriteAssets& spriteAssets = *resource.spritesAssets;
	GameStages& gameStages = *resource.gameStages;


	timer -= BLINK_SPEED * deltaTime;

	switch (subState) {
	case INIT_STATE:
		LoadAssets(resource);
		textAssets[singlePlayerText_resourceID].isVisible = false;
		textAssets[multiPlayerText_resourceID].isVisible = false;
		subState = PRESSKEY_STATE;
		break;
	case PRESSKEY_STATE:
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[pressStartText_resourceID].isVisible = !textAssets[pressStartText_resourceID].isVisible;
		}

		if (inputState.start) {

			subState = MAINMENU_STATE;
			isOnPlayerSelect = true;
			textAssets[pressStartText_resourceID].isVisible = false;
			textAssets[singlePlayerText_resourceID].isVisible = true;
			textAssets[multiPlayerText_resourceID].isVisible = true;
			SDL_Delay(500);
		}

		break;
	case MAINMENU_STATE:

		if (inputState.player2Down) {
			_2PlayersSelect = true;
		}
		if (inputState.player2Up) {
			_2PlayersSelect = false;
		}
		if (!textAssets[pressStartText_resourceID].isVisible) {
			if (!_2PlayersSelect) {
				textAssets[multiPlayerText_resourceID].isVisible = true;
				if (timer <= 0.00f) {
					timer = 1.0f * 1000;
					textAssets[singlePlayerText_resourceID].isVisible = !textAssets[singlePlayerText_resourceID].isVisible;
				}
				if (inputState.start) {
					singleplayer = true;
					subState = ENDING_STATE;
				}
				
			}
			else {
				textAssets[singlePlayerText_resourceID].isVisible = true;
				if (timer <= 0.00f) {
					timer = 1.0f * 1000;
					textAssets[multiPlayerText_resourceID].isVisible = !textAssets[multiPlayerText_resourceID].isVisible;
				}
				if (inputState.start) {
					singleplayer = false;
					subState = ENDING_STATE;
				}
			}
		}
		
		break;

	case ENDING_STATE:
		unloadAssets(resource);
		subState = INIT_STATE;
		gameStages.pop();

		GameStage gamePlayStage;
		gamePlayStage.game_stageID = GS_GAMEPLAY;
		gamePlayStage.stage_name = "Gameplay";
		gameStages.push(gamePlayStage);
		break;
	}
}