#pragma once

#include "StructsDef.h"

typedef struct _Player {
	Sprite* sprite;
	int points = 0;

}Player;

typedef struct _Speed {
	float x;
	float y;
}Speed;

typedef struct _Ball {
	Sprite* sprite;
	Speed speed;
}Ball;

typedef struct _Boundary {
	SDL_Rect rect;
}Boundary;