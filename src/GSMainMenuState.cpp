#include "GSMainMenuState.h"
#include "StructsDef.h"
#include "SDL.h"
#include "SDL_image.h"

namespace mainmenu {
	enum SUBSTATE {
		INIT_STATE = 0,
		PRESSKEY_STATE,
		MAINMENU_STATE,
		ENDING_STATE,
	};

	int subState = INIT_STATE;
	float timer = 1.0f * 1000;

	void ShowAssets(ResourceManager& resource) {
		TextAssets& textAssets = *resource.textAssets;
		SpriteAssets& spriteAssets = *resource.spritesAssets;

		spriteAssets[0].isVisible = true; // Mostrar logo
	}

	void HideAssets(ResourceManager& resource) {
		TextAssets& textAssets = *resource.textAssets;
		SpriteAssets& spriteAssets = *resource.spritesAssets;

		spriteAssets[0].isVisible = false;
	}
}

using namespace mainmenu;

void GSMainMenuStateUpdate(float deltaTime, ResourceManager& resource)
{
	InputState inputState = *resource.inputState;
	TextAssets& textAssets = *resource.textAssets;
	SpriteAssets& spriteAssets = *resource.spritesAssets;
	GameStages& gameStages = *resource.gameStages;

	const float BLINK_SPEED = 1.0f;

	extern bool isOnPressStart;
	bool isSinglePlayer = false;


	timer -= BLINK_SPEED * deltaTime;

	switch (subState) {
	case INIT_STATE:
		ShowAssets(resource);
		subState = PRESSKEY_STATE;
		break;
	case PRESSKEY_STATE:
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[0].isVisible = !textAssets[0].isVisible;
		}

		if (inputState.start) {
			textAssets[0].isVisible = false;
			//textAssets[1].isVisible = true;
			//textAssets[2].isVisible = true;
			subState = ENDING_STATE;
		}
		break;
	case ENDING_STATE:
		HideAssets(resource);
		GameStage gamePlayStage;
		gamePlayStage.game_stageID = GS_GAMEPLAY;
		gameStages.push(gamePlayStage);
		break;
	}
}
