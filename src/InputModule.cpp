#include "InputModule.h"
#include "StructsDef.h"

void onKeyUp(int keyCode, InputState& input) {
	switch (keyCode) {
	case SDLK_UP:
		input.player2Up = false;
		break;
	case SDLK_DOWN:
		input.player2Down = false;
		break;
	case SDLK_w:
		input.player1Up = false;
		break;
	case SDLK_s:
		input.player1Down = false;
		break;
	case SDLK_RETURN:
		input.start = false;
		break;
	case SDLK_ESCAPE:
		input.back = false;
		break;
	default:
		break;
	}
}

void onKeyDown(int keyCode, InputState& input) {
	switch (keyCode) {
	case SDLK_UP:
		input.player2Up = true;
		break;
	case SDLK_DOWN:
		input.player2Down = true;
		break;
	case SDLK_w:
		input.player1Up = true;
		break;
	case SDLK_s:
		input.player1Down = true;
		break;
	case SDLK_RETURN:
		input.start = true;
		break;
	case SDLK_ESCAPE:
		input.back = true;
		break;
	default:
		break;
	}
}