#pragma once

/*
Leo Waters
Constants used through out the game
*/

const int WindowWidth = 800;
const int WindowHeight = 600;
const int WindowWidthOffset = WindowWidth/2;
const int WindowHeightOffset = WindowHeight / 2;

const int TileSize = 80;

const int MaxEnemyCount = 40;

const int MaxSpellCount = 20;

const int TILE_WALL  = 1;
const int TILE_SPAWN = 5;
const int TILE_HATCH = 6;

enum Anchor {
	topLeft = 0,
	topCenter = 1,
	topRight = 2,
	midLeft = 3,
	midCenter = 4,
	midRight = 5,
	bottomLeft = 6,
	bottomCenter = 7,
	bottomRight = 8
};
