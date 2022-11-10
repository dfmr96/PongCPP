#include "GSLogoState.h"
#include "StructsDef.h"
#include "SDL.h"
#include "SDL_image.h"

namespace logo {
	enum SUBSTATE {
		INIT_STATE = 0,
		LOGO_STATE,
		ENDING_STATE,
	};

	int subState = INIT_STATE;
	float timer = 0;
	const float LOGO_TIME = 2000.0f;
	int logo_resourceID = -1;
}

using namespace logo;

void loadResource(ResourceManager& resource) {
	SDL_Renderer* renderer = resource.renderer;
	SpriteAssets& spritesAssets = *(resource.spritesAssets);

	string uadeLogoFilePath = "assets/img/uadelogo.png";
	SDL_Texture* uadeLogoTexture = IMG_LoadTexture(renderer, uadeLogoFilePath.c_str());
	SDL_Rect uadeLogoDest;
	uadeLogoDest.x = WIDTH >> 2;
	uadeLogoDest.y = HEIGHT >> 2;
	uadeLogoDest.w = WIDTH >> 1;
	uadeLogoDest.h = HEIGHT >> 1;

	Sprite uadeLogoSprite;
	uadeLogoSprite.dest = uadeLogoDest;
	uadeLogoSprite.texture = uadeLogoTexture;
	spritesAssets.push_back(uadeLogoSprite);

	logo_resourceID = spritesAssets.size() - 1;
}

void unloadResource(ResourceManager& resource) {
	SpriteAssets& spritesAssets = *resource.spritesAssets;
	SDL_DestroyTexture(spritesAssets[logo_resourceID].texture);
	spritesAssets.erase(spritesAssets.begin() + logo_resourceID);
}

void GSLogoStateUpdate(float delta, ResourceManager& resource) {
	InputState& inputState = *resource.inputState;
	GameStages& gameStages = *resource.gameStages;

	switch (subState) {
	case INIT_STATE:
		loadResource(resource);
		subState = LOGO_STATE;
		break;
	case LOGO_STATE:
		timer += delta;
		if (timer >= LOGO_TIME) {
			subState = ENDING_STATE;
		}
		break;
	case ENDING_STATE:
		subState = INIT_STATE;
		unloadResource(resource);
		gameStages.pop();

		GameStage mainMenuGameStage;
		mainMenuGameStage.game_stageID = GS_MAIN_MENU;
		mainMenuGameStage.stage_name = "MainMenu";
		gameStages.push(mainMenuGameStage);

		break;
	}
}