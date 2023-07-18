#include "Minefield.h"
#include "SpriteCodex.h"

void Minefield::Tile::Draw(const Vei2 & pos, Graphics & gfx) const
{
	SpriteCodex::DrawTileButton(pos, gfx);
}

void Minefield::Draw(Graphics & gfx) const
{
	Color baseColor = SpriteCodex::baseColor;
	const int tileSize = SpriteCodex::tileSize;
	const int fieldWidth = nColumns * tileSize;
	const int fieldHeight = nRows * tileSize;
	RectI boardRect(fieldPosition, fieldWidth, fieldHeight);
	gfx.DrawRect(boardRect, baseColor);
	for (Vei2 gridCoordinate(0, 0); gridCoordinate.y < nRows; gridCoordinate.y++) {
		for (gridCoordinate.x = 0; gridCoordinate.x < nColumns; gridCoordinate.x++){
			const int xPos = gridCoordinate.x * tileSize + fieldPosition.x;
			const int yPos = gridCoordinate.y * tileSize + fieldPosition.y;
			GetTileAtPosition(gridCoordinate).Draw(Vei2(xPos, yPos), gfx);
		}
	}
}

Minefield::Tile Minefield::GetTileAtPosition(int x, int y) const
{
	return tiles[y * nColumns + x];
}

Minefield::Tile Minefield::GetTileAtPosition(const Vei2& position) const
{
	return GetTileAtPosition(position.x, position.y);
}