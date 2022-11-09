#include "GSLogoState.h"
#include "StructsDef.h"


void GSLogoStateUpdate(float deltaTime, ResourceManager& resource) {
	InputState inputState = *resource.inputState;
	TextAssets& textAssets = *resource.textAssets;
	SpriteAssets& spriteAssets = *resource.spritesAssets;
	GameStages& gameStages = *resource.gameStages;

	const float BLINK_SPEED = 2.0f;
	extern float timer;

	extern bool isOnPressStart;
	bool isSinglePlayer = false;

	// Create Gameplay Stage
	

	/// START

	spriteAssets[0].isVisible = true; // Mostrar logo

	///UPDATE

	timer -= BLINK_SPEED * deltaTime;

	// Si esta mostrando PRESS START -> Que parpadee PRESS START
	if (isOnPressStart) {
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[0].isVisible = !textAssets[0].isVisible;
		}
	}

	//Si es presionado Start -> Esconde logo y press start, se activa la seleccion de jugador

	if (inputState.start && isOnPressStart) {
		isOnPressStart = false;
		textAssets[0].isVisible = false;
		textAssets[1].isVisible = true;
		textAssets[2].isVisible = true;
		isSinglePlayer = true;
	}
	
	// Si esta seleccionando jugador y presiona ESC -> Vuelve a PRESS START

	if (inputState.back && !isOnPressStart) {
		isOnPressStart = true;
		textAssets[1].isVisible = false;
		textAssets[2].isVisible = false;
	}

	// Si presiona abajo es multiplayer

	if (inputState.player1Down) {
		isSinglePlayer = false;
	}

	//Si presiona arriba es singleplayer
	if (inputState.player1Up) {
		isSinglePlayer = true;
	}

	//Si esta en seleccion y es single player parpadea

	if (isSinglePlayer && !isOnPressStart) {
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[1].isVisible = !textAssets[1].isVisible;
			textAssets[2].isVisible = true;
		}
		if (inputState.start) {
			
			GameStage gamePlayStage;
			gamePlayStage.game_stageID = GS_GAMEPLAY;
			gamePlayStage.stage_name = "Gameplay";
			gameStages.push(gamePlayStage);

		}
	}
	else if (!isSinglePlayer && !isOnPressStart) 	//Si esta en seleccion y no es single player parpadea
	{
		if (timer <= 0.0f) {
			timer = 1.0f * 1000;
			textAssets[2].isVisible = !textAssets[2].isVisible;
			textAssets[1].isVisible = true;

		}
	}


/*
	if (inputState.start) {

		GameStage mainMenuGameStage;
		mainMenuGameStage.game_stageID = GS_GAMEPLAY;
		mainMenuGameStage.stage_name = "Gameplay";
		gameStages.push(mainMenuGameStage);
	}
*/
}