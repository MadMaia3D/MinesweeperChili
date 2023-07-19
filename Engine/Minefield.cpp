#include "Minefield.h"
#include "SpriteCodex.h"
#include <random>
#include <assert.h>

void Minefield::Tile::Reveal()
{
	status = Status::revealed;
}

bool Minefield::Tile::HasBomb() const
{
	return hasBomb;
}

void Minefield::Tile::Draw(const Vei2 & pos, Graphics & gfx) const
{
	
	switch (status) {
	case Status::unrevealed:
		SpriteCodex::DrawTileButton(pos, gfx);
		break;
	case Status::revealed:
		if (hasBomb) {
			SpriteCodex::DrawTileBomb(pos, gfx);
		}else{
			SpriteCodex::DrawTile0(pos, gfx);
		}
		break;
	}	
}

void Minefield::Tile::SpawnMine()
{
	hasBomb = true;
}

Minefield::Minefield()
{
	SpawnMines(30);
}

void Minefield::LeftClick(const Vei2 & screenPosition)
{
	Vei2 relativePosition = screenPosition - fieldPosition;
	
	const int tileSize = SpriteCodex::tileSize;
	RectI boardRect = GetFieldRect();

	if ( !boardRect.ContainsPoint(screenPosition) ) { return; }

	const int cellX = relativePosition.x / tileSize;
	const int cellY = relativePosition.y / tileSize;

	GetTileAtPosition(Vei2(cellX, cellY)).Reveal();
}

void Minefield::Draw(Graphics & gfx) const
{
	Color baseColor = SpriteCodex::baseColor;
	const int tileSize = SpriteCodex::tileSize;

	RectI boardRect = GetFieldRect();

	gfx.DrawRect(boardRect, baseColor);

	for (Vei2 gridCoordinate(0, 0); gridCoordinate.y < nRows; gridCoordinate.y++) {
		for (gridCoordinate.x = 0; gridCoordinate.x < nColumns; gridCoordinate.x++){
			const int xPos = gridCoordinate.x * tileSize + fieldPosition.x;
			const int yPos = gridCoordinate.y * tileSize + fieldPosition.y;
			GetTileAtPosition(gridCoordinate).Draw(Vei2(xPos, yPos), gfx);
		}
	}
}

void Minefield::SpawnMines(int quantity)
{
	const int tilesQuantity = nColumns * nRows;
	assert(quantity < tilesQuantity);

	std::random_device randomDevice;
	std::mt19937 randomNumberGenerator( randomDevice() );
	std::uniform_int_distribution<int> dist(0, tilesQuantity);

	for (int i = 0; i < quantity; i++)
	{
		int randomTileIndex;
		do {
			randomTileIndex = dist(randomNumberGenerator);
		} while (tiles[randomTileIndex].HasBomb());
		tiles[randomTileIndex].SpawnMine();
	}	
}

const Minefield::Tile& Minefield::GetTileAtPosition(const Vei2& position) const
{
	return tiles[position.y * nColumns + position.x];
}

Minefield::Tile& Minefield::GetTileAtPosition(const Vei2& position)
{
	return tiles[position.y * nColumns + position.x];
}

RectI Minefield::GetFieldRect() const
{
	const int tileSize = SpriteCodex::tileSize;
	const int fieldWidth = nColumns * tileSize;
	const int fieldHeight = nRows * tileSize;
	return RectI(fieldPosition, fieldWidth, fieldHeight);;
}
