#include "Minefield.h"
#include "SpriteCodex.h"
#include <random>
#include <algorithm>
#include <assert.h>

void Minefield::Tile::Reveal() {
	assert(status != Status::Revealed);
	status = Status::Revealed;
}

bool Minefield::Tile::IsRevealed() const {
	return status == Status::Revealed;
}

bool Minefield::Tile::IsFlagged() const {
	return status == Status::Flagged;
}

void Minefield::Tile::SetFlag(bool value) {
	assert(status != Status::Revealed);
	if (status == Status::Flagged) {
		status = Status::Hidden;
	}
	else {
		status = Status::Flagged;
	}
}

bool Minefield::Tile::HasBomb() const {
	return hasBomb;
}

void Minefield::Tile::Draw(const Vei2 & pos, Graphics & gfx) const {
	assert(nNeighborMines >= 0 && nNeighborMines < 9);
	switch (status) {
	case Status::Hidden:
		SpriteCodex::DrawTileButton(pos, gfx);
		break;
	case Status::Flagged:
		SpriteCodex::DrawTileButton(pos, gfx);
		SpriteCodex::DrawTileFlag(pos, gfx);
		break;
	case Status::Revealed:
		if (!hasBomb) {
			SpriteCodex::DrawTileNumber(pos, nNeighborMines, gfx);
		}
		else {
			SpriteCodex::DrawTileBomb(pos, gfx);
		}
		break;
	}
}

void Minefield::Tile::SpawnMine() {
	assert(hasBomb == false);
	hasBomb = true;
}

void Minefield::Tile::SetNeighborMinesNumber(int nMines) {
	assert(nMines > -1 && nMines < 9);
	nNeighborMines = nMines;
}

bool Minefield::Tile::HasNeighborMines() const {
	return nNeighborMines > 0;
}

Minefield::Minefield(int nMemes) {
	SpawnMines(nMemes);
	for (int x = 0; x < nColumns; x++) {
		for (int y = 0; y < nRows; y++) {
			Vei2 gridPosition(x, y);
			int nMines = CountNeighborMines(gridPosition);
			Tile& tile = GetTileAtPosition(gridPosition);
			tile.SetNeighborMinesNumber(nMines);
		}
	}
}

void Minefield::OnRevealClick(const Vei2 & mousePosition) {
	if (!IsScreenPositionInsideGrid(mousePosition)) { return; }
	const Vei2 gridPosition = ScreenSpaceToGridSpace(mousePosition);

	Tile& tile = GetTileAtPosition(gridPosition);
	if (!tile.IsRevealed() && !tile.IsFlagged()) {
		tile.Reveal();
	}
}

void Minefield::OnFlagClick(const Vei2 & mousePosition) {
	if (!IsScreenPositionInsideGrid(mousePosition)) { return; }
	const Vei2 gridPosition = ScreenSpaceToGridSpace(mousePosition);

	Tile& tile = GetTileAtPosition(gridPosition);
	if (tile.IsRevealed()) { return; }
	tile.SetFlag(!tile.IsFlagged());

}

void Minefield::Draw(Graphics & gfx) const {
	Color baseColor = SpriteCodex::baseColor;
	RectI boardRect = GetFieldRect();
	gfx.DrawRect(boardRect, baseColor);

	const int tileSize = SpriteCodex::tileSize;

	for (Vei2 gridCoordinate(0, 0); gridCoordinate.y < nRows; gridCoordinate.y++) {
		for (gridCoordinate.x = 0; gridCoordinate.x < nColumns; gridCoordinate.x++) {
			const int xPos = gridCoordinate.x * tileSize + fieldPosition.x;
			const int yPos = gridCoordinate.y * tileSize + fieldPosition.y;
			GetTileAtPosition(gridCoordinate).Draw(Vei2(xPos, yPos), gfx);
		}
	}
}

void Minefield::SpawnMines(int nMines) {
	const int tilesQuantity = nColumns * nRows;
	assert(nMines > 0 && nMines < tilesQuantity);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, nColumns);
	std::uniform_int_distribution<int> yDist(0, nRows);

	for (int i = 0; i < nMines; i++) {
		Vei2 spawnPos;
		do {
			int x = xDist(rng);
			int y = yDist(rng);
			spawnPos = { x, y };
		} while (GetTileAtPosition(spawnPos).HasBomb());
		GetTileAtPosition(spawnPos).SpawnMine();
	}
}

const Minefield::TileArea Minefield::GetSurroundingArea(const Vei2 & gridPosition) const {
	TileArea area;
	area.left = std::max(0, gridPosition.x - 1);
	area.right = std::min(gridPosition.x + 1, nColumns - 1);
	area.top = std::max(0, gridPosition.y - 1);
	area.bottom = std::min(gridPosition.y + 1, nRows - 1);
	return area;
}

int Minefield::CountNeighborMines(const Vei2 & gridPosition) const {
	TileArea area = GetSurroundingArea(gridPosition);
	int minesCount = 0;
	for (int x = area.left; x <= area.right; x++) {
		for (int y = area.top; y <= area.bottom; y++) {
			const Vei2 currentPosition(x, y);
			if (currentPosition == gridPosition) { continue; }
			bool currentPositionHasBomb = GetTileAtPosition(currentPosition).HasBomb();
			if (currentPositionHasBomb) {
				minesCount++;
			}
		}
	}
	return minesCount;
}

bool Minefield::IsScreenPositionInsideGrid(const Vei2 & screenPosition) const {
	RectI boardRect = GetFieldRect();
	return boardRect.ContainsPoint(screenPosition);
}

Vei2 Minefield::ScreenSpaceToGridSpace(const Vei2 & screenPosition) const {
	Vei2 relativePosition = screenPosition - fieldPosition;

	const int tileSize = SpriteCodex::tileSize;

	const int cellX = relativePosition.x / tileSize;
	const int cellY = relativePosition.y / tileSize;

	const Vei2 gridSpacePosition = Vei2(cellX, cellY);

	return gridSpacePosition;
}

const Minefield::Tile& Minefield::GetTileAtPosition(const Vei2& position) const {
	assert(position.x >= 0 && position.x <= nColumns);
	assert(position.y >= 0 && position.y <= nRows);
	return tiles[position.y * nColumns + position.x];
}

Minefield::Tile& Minefield::GetTileAtPosition(const Vei2& position) {
	assert(position.x >= 0 && position.x <= nColumns);
	assert(position.y >= 0 && position.y <= nRows);
	return tiles[position.y * nColumns + position.x];
}

RectI Minefield::GetFieldRect() const {
	const int tileSize = SpriteCodex::tileSize;
	const int fieldWidth = nColumns * tileSize;
	const int fieldHeight = nRows * tileSize;
	return RectI(fieldPosition, fieldWidth, fieldHeight);;
}
