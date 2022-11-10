#include "GSGameplayState.h"
#include "StructsDef.h"
#include "SDL.h"
#include "SDL_image.h"

namespace gameplay {
	enum SUBSTATE {
		INIT_STATE = 0,
		UPDATE_STATE,
		ENDING_STATE,
	};

	const float pongSpeed = 0.5f;
	const float ballSpeed = 1.00f;
	int subState = INIT_STATE;


	int player1Sprite_ResourceID = -1;
	int player2Sprite_ResourceID = -1;



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

	}

	void UpdateMovements(float deltaTime, ResourceManager& resource) {
		InputState inputState = *resource.inputState;
		SpriteAssets& spritesAssets = *resource.spritesAssets;

		if (inputState.player1Up) {
			spritesAssets[player1Sprite_ResourceID].dest.y -= 0.5 * deltaTime;
		}
		if (inputState.player1Down) {
			spritesAssets[player1Sprite_ResourceID].dest.y += 0.5 * deltaTime;
		}
		if (inputState.player2Up) {
			spritesAssets[player2Sprite_ResourceID].dest.y -= 0.5 * deltaTime;
		}
		if (inputState.player2Down) {
			spritesAssets[player2Sprite_ResourceID].dest.y += 0.5 * deltaTime;
		}

		if (spritesAssets[player1Sprite_ResourceID].dest.y <= 0) {
			spritesAssets[player1Sprite_ResourceID].dest.y = 0;
		}

		if (spritesAssets[player1Sprite_ResourceID].dest.y >= (480 - 30)) {
			spritesAssets[player1Sprite_ResourceID].dest.y = 450;
		}

		if (spritesAssets[player2Sprite_ResourceID].dest.y <= 0) {
			spritesAssets[player2Sprite_ResourceID].dest.y = 0;
		}

		if (spritesAssets[player2Sprite_ResourceID].dest.y >= (480 - 30)) {
			spritesAssets[player2Sprite_ResourceID].dest.y = 450;
		}
	}
}

using namespace gameplay;

void GSGameplayStateUpdate(float deltaTime, ResourceManager& resource)
{

	switch (subState) {
	case INIT_STATE:
		LoadAssets(resource);
		subState = UPDATE_STATE;
		break;
	case UPDATE_STATE:
		UpdateMovements(deltaTime, resource);
		break;
	}
}
