#include "GSLogoState.h"
#include "StructsDef.h"

bool isSinglePlayer = true;

void GSLogoStateUpdate(float deltaTime, ResourceManager& resource) {
	InputState inputState = *resource.inputState;
	GameStages gameStages = *resource.gameStages;
	TextAssets& textAssets = *resource.textAssets;

	SpriteAssets& spriteAssets = *resource.spritesAssets;
	Sprite& player1 = spriteAssets[1];
	Sprite& player2 = spriteAssets[2];

	const float BLINK_SPEED = 2.0f;
	extern float timer;

	extern bool isOnPressStart;
	

	/// START

	spriteAssets[0].isVisible = true; // Mostrar logo

	///UPDATE

	timer -= BLINK_SPEED * deltaTime;

	if (isOnPressStart) {
		// para efecto de parpadeo...
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[0].isVisible = !textAssets[0].isVisible;
		}
	}


	if (inputState.start) {
		isOnPressStart = false;
		textAssets[0].isVisible = false;
		textAssets[1].isVisible = true;
		textAssets[2].isVisible = true;
		isSinglePlayer = true;
	}
	
	if (inputState.back) {
		isOnPressStart = true;
		textAssets[1].isVisible = false;
		textAssets[2].isVisible = false;
	}

	if (inputState.player1Down) {
		isSinglePlayer = false;
	}

	if (inputState.player1Up) {
		isSinglePlayer = true;
	}

	if (isSinglePlayer && !isOnPressStart) {
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[1].isVisible = !textAssets[1].isVisible;
			textAssets[2].isVisible = true;
		}
	}
	else if (!isSinglePlayer && !isOnPressStart) {
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[2].isVisible = !textAssets[2].isVisible;
			textAssets[1].isVisible = true;

		}
	}

	// Si presione cualquier tecla (arriba, abajo, izquierda
	if (inputState.player1Up) {
		player1.dest.y -= 0.5 * deltaTime;
	}
	if (inputState.player1Down) {
		player1.dest.y += 0.5 * deltaTime;
	}
	if (inputState.player2Up) {
		player2.dest.y -= 0.5 * deltaTime;
	}
	if (inputState.player2Down) {
		player2.dest.y += 0.5 * deltaTime;
	}

	if (player1.dest.y <= 0) {
		player1.dest.y = 0;
	}

	if (player1.dest.y >= (480 - 30)) {
		player1.dest.y = 450;
	}

	if (player2.dest.y <= 0) {
		player2.dest.y = 0;
	}

	if (player2.dest.y >= (480 - 30)) {
		player2.dest.y = 450;
	}
}