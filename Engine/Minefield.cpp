#include "Minefield.h"
#include "SpriteCodex.h"
#include <random>
#include <assert.h>

void Minefield::Tile::Reveal()
{
	assert(status != Status::Revealed);
	status = Status::Revealed;
}

bool Minefield::Tile::IsRevealed() const
{
	return status == Status::Revealed;
}

bool Minefield::Tile::HasBomb() const
{
	return hasBomb;
}

void Minefield::Tile::Draw(const Vei2 & pos, Graphics & gfx) const
{
	
	switch (status) {
	case Status::Hidden:
		SpriteCodex::DrawTileButton(pos, gfx);
		break;
	case Status::Flagged:
		SpriteCodex::DrawTileFlag(pos, gfx);
		break;
	case Status::Revealed:
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
	assert(hasBomb == false);
	hasBomb = true;
}

Minefield::Minefield(int nMemes)
{
	SpawnMines(nMemes);
}

void Minefield::OnRevealClick(const Vei2 & mousePosition)
{
	if (!IsScreenPositionInsideGrid(mousePosition)) { return; }
	const Vei2 gridPosition = ScreenSpaceToGridSpace(mousePosition);

	Tile& tile = GetTileAtPosition(gridPosition);
	if (!tile.IsRevealed()){
		tile.Reveal();
	}
}

void Minefield::Draw(Graphics & gfx) const
{
	Color baseColor = SpriteCodex::baseColor;
	RectI boardRect = GetFieldRect();
	gfx.DrawRect(boardRect, baseColor);

	const int tileSize = SpriteCodex::tileSize;

	for (Vei2 gridCoordinate(0, 0); gridCoordinate.y < nRows; gridCoordinate.y++) {
		for (gridCoordinate.x = 0; gridCoordinate.x < nColumns; gridCoordinate.x++){
			const int xPos = gridCoordinate.x * tileSize + fieldPosition.x;
			const int yPos = gridCoordinate.y * tileSize + fieldPosition.y;
			GetTileAtPosition(gridCoordinate).Draw(Vei2(xPos, yPos), gfx);
		}
	}
}

void Minefield::SpawnMines(int nMines)
{
	const int tilesQuantity = nColumns * nRows;
	assert(nMines > 0 && nMines < tilesQuantity);

	std::random_device rd;
	std::mt19937 rng(rd() );
	std::uniform_int_distribution<int> xDist(0, nColumns);
	std::uniform_int_distribution<int> yDist(0, nRows);

	for (int i = 0; i < nMines; i++)
	{
		Vei2 spawnPos;
		do {
			int x = xDist(rng);
			int y = yDist(rng);
			spawnPos = {x, y};
		} while (GetTileAtPosition(spawnPos).HasBomb());
		GetTileAtPosition(spawnPos).SpawnMine();
	}	
}

bool Minefield::IsScreenPositionInsideGrid(const Vei2 & screenPosition) const
{
	RectI boardRect = GetFieldRect();
	return boardRect.ContainsPoint(screenPosition);
}

Vei2 Minefield::ScreenSpaceToGridSpace(const Vei2 & screenPosition) const
{
	Vei2 relativePosition = screenPosition - fieldPosition;

	const int tileSize = SpriteCodex::tileSize;

	const int cellX = relativePosition.x / tileSize;
	const int cellY = relativePosition.y / tileSize;

	const Vei2 gridSpacePosition = Vei2(cellX, cellY);

	return gridSpacePosition;
}

const Minefield::Tile& Minefield::GetTileAtPosition(const Vei2& position) const
{
	assert(position.x <= nColumns && position.y <= nRows);
	return tiles[position.y * nColumns + position.x];
}

Minefield::Tile& Minefield::GetTileAtPosition(const Vei2& position)
{
	assert(position.x <= nColumns && position.y <= nRows);
	return tiles[position.y * nColumns + position.x];
}

RectI Minefield::GetFieldRect() const
{
	const int tileSize = SpriteCodex::tileSize;
	const int fieldWidth = nColumns * tileSize;
	const int fieldHeight = nRows * tileSize;
	return RectI(fieldPosition, fieldWidth, fieldHeight);;
}
