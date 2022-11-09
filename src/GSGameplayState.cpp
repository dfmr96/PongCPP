#include "GSGameplayState.h"
#include "StructsDef.h"


void GSGameplayStateUpdate(float deltaTime, ResourceManager& resource)
{
	InputState inputState = *resource.inputState;
	TextAssets& textAssets = *resource.textAssets;
	SpriteAssets& spriteAssets = *resource.spritesAssets;
	GameStages& gameStages = *resource.gameStages;

	Sprite& player1 = spriteAssets[1];
	Sprite& player2 = spriteAssets[2];

	//START

	player1.isVisible = true;
	player2.isVisible = true;

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
