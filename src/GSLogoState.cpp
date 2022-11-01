#include "GSLogoState.h"
#include "StructsDef.h"

void GSLogoStateUpdate(float delta, ResourceManager& resource) {

	InputState inputState = *resource.inputState;
	GameStages gameStages = *resource.gameStages;

	SpriteAssets& spriteAssets = *resource.spritesAssets;
	Sprite& player1 = spriteAssets[1];
	Sprite& player2 = spriteAssets[2];


	// Si presione cualquier tecla (arriba, abajo, izquierda
	if (inputState.player1Up) {
		player1.dest.y -= 0.5 * delta;
	}
	if (inputState.player1Down) {
		player1.dest.y += 0.5 * delta;
	}
	if (inputState.player2Up) {
		player2.dest.y -= 0.5 * delta;
	}
	if (inputState.player2Down) {
		player2.dest.y += 0.5 * delta;
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

