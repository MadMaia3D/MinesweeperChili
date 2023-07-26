#include "Minefield.h"
#include <random>
#include <algorithm>
#include <assert.h>

void Minefield::Tile::Reveal() {
	assert(status != TileState::Revealed);
	status = TileState::Revealed;
}

bool Minefield::Tile::IsRevealed() const {
	return status == TileState::Revealed;
}

bool Minefield::Tile::IsFlagged() const {
	return status == TileState::Flagged;
}

void Minefield::Tile::SetFlag(bool value) {
	assert(status != TileState::Revealed);
	if (status == TileState::Flagged) {
		status = TileState::Hidden;
	}
	else {
		status = TileState::Flagged;
	}
}

bool Minefield::Tile::HasBomb() const {
	return hasBomb;
}

void Minefield::Tile::Draw(const Vei2 & pos, Graphics & gfx) const {
	assert(nNeighborMines >= 0 && nNeighborMines < 9);
	switch (status) {
	case TileState::Hidden:
		SpriteCodex::DrawTileButton(pos, gfx);
		break;
	case TileState::Flagged:
		SpriteCodex::DrawTileButton(pos, gfx);
		SpriteCodex::DrawTileFlag(pos, gfx);
		break;
	case TileState::Revealed:
		if (!hasBomb) {
			SpriteCodex::DrawTileNumber(pos, nNeighborMines, gfx);
		}
		else {
			SpriteCodex::DrawTileBomb(pos, gfx);
		}
		break;
	}
}

void Minefield::Tile::DrawOnGameOver(const Vei2 & pos, Graphics & gfx) const {
	assert(nNeighborMines >= 0 && nNeighborMines < 9);
	switch (status) {
	case TileState::Hidden:
		SpriteCodex::DrawTileButton(pos, gfx);
		if (hasBomb) {
			SpriteCodex::DrawTileBomb(pos, gfx);
		}
		break;
	case TileState::Flagged:
		SpriteCodex::DrawTileButton(pos, gfx);
		if (hasBomb) {
			SpriteCodex::DrawTileFlag(pos, gfx);
		}
		else {
			SpriteCodex::DrawTileCrossRed(pos, gfx);
		}
		break;
	case TileState::Revealed:
		if (!hasBomb) {
			SpriteCodex::DrawTileNumber(pos, nNeighborMines, gfx);
		}
		else {
			SpriteCodex::DrawTileBombRed(pos, gfx);
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

int Minefield::Tile::GetnNeighborMines() const {
	return nNeighborMines;
}

bool Minefield::Tile::HasNeighborMines() const {
	return nNeighborMines > 0;
}

Minefield::Minefield(Vei2 position, int nMines)
	:
	fieldPosition(position)
{
	SpawnMines(nMines);
	SetNeighborMinesNumber();
	hintPosition = GenerateHintPosition();
}

void Minefield::SetNeighborMinesNumber() {
	for (int x = 0; x < nColumns; x++) {
		for (int y = 0; y < nRows; y++) {
			Vei2 gridPosition(x, y);
			int nMines = CountNeighborMines(gridPosition);
			Tile& tile = GetTileAtPosition(gridPosition);
			tile.SetNeighborMinesNumber(nMines);
		}
	}
}

int Minefield::GetNeighborFlagsCount(const Vei2 & gridPosition) const {
	TileArea area = GetSurroundingArea(gridPosition);
	int flagsCount = 0;
	for (int x = area.left; x <= area.right; x++) {
		for (int y = area.top; y <= area.bottom; y++) {
			const Vei2 currentPosition(x, y);
			if (currentPosition == gridPosition) { continue; }
			bool currentPositionHasFlag = GetTileAtPosition(currentPosition).IsFlagged();
			if (currentPositionHasFlag) {
				flagsCount++;
			}
		}
	}
	return flagsCount;
}

void Minefield::RevealNeighbors(const Vei2 & gridPosition) {
	TileArea area = GetSurroundingArea(gridPosition);
	for (int x = area.left; x <= area.right; x++) {
		for (int y = area.top; y <= area.bottom; y++) {
			Vei2 currentPosition(x, y);
			if (!GetTileAtPosition(currentPosition).IsRevealed()) {
				RevealMine(currentPosition);
			}
		}
	}
}

Vei2 Minefield::GenerateHintPosition() const {
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, nColumns - 1);
	std::uniform_int_distribution<int> yDist(0, nRows - 1);

	static constexpr int maxAttempts = 1000;
	Vei2 tipPosition = Vei2(-1, -1);
	for (int attempts = 0; attempts < maxAttempts; attempts++) {
		tipPosition.x = xDist(rng);
		tipPosition.y = yDist(rng);
		Tile tileAtPosition = GetTileAtPosition(tipPosition);
		if (tileAtPosition.HasNeighborMines() || tileAtPosition.HasBomb()) {
			continue;
		}
		return tipPosition;
	}
	return tipPosition;
}

void Minefield::OnRevealClick(const Vei2 & mousePosition) {
	if (gameState == GameState::GameOver || !IsScreenPositionInsideGrid(mousePosition)) {
		return;
	}
	const Vei2 gridPosition = ScreenSpaceToGridSpace(mousePosition);

	if (gameState == GameState::NotStarted && gridPosition != hintPosition) {
		return;
	}

	RevealMine(gridPosition);
}

void Minefield::OnFlagClick(const Vei2 & mousePosition) {
	if (gameState != GameState::Playing || !IsScreenPositionInsideGrid(mousePosition)) {
		return;
	}
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
			if (gameState != GameState::GameOver) {
				GetTileAtPosition(gridCoordinate).Draw(Vei2(xPos, yPos), gfx);
			}
			else {
				GetTileAtPosition(gridCoordinate).DrawOnGameOver(Vei2(xPos, yPos), gfx);
			}
		}
	}

	if (gameState == GameState::NotStarted && hintPosition != Vei2(-1, -1)) {
		const int xPos = hintPosition.x * tileSize + fieldPosition.x;
		const int yPos = hintPosition.y * tileSize + fieldPosition.y;
		SpriteCodex::DrawTileGreenCross(Vei2(xPos, yPos), gfx);
	}
}

void Minefield::RevealMine(const Vei2 & gridPosition) {
	Tile& tile = GetTileAtPosition(gridPosition);
	if (tile.IsFlagged()) {	return;	}

	if (!tile.IsRevealed()) {
		tile.Reveal();
	} else if (tile.GetnNeighborMines() == GetNeighborFlagsCount(gridPosition)) {
		RevealNeighbors(gridPosition);
	} else {
		return;
	}

	if (gameState == GameState::NotStarted) {
		gameState = GameState::Playing;
	}
	if (tile.HasBomb()) {
		gameState = GameState::GameOver;
	}
	else if (!tile.HasNeighborMines()) {
		RevealNeighbors(gridPosition);
	}

	if (IsGameWon()) {
		gameState = GameState::Win;
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

bool Minefield::IsGameWon() const {
	for (const Tile& tile : tiles) {
		if (tile.IsRevealed() && tile.HasBomb()) {
			return false;
		}
		if (!tile.IsRevealed() && !tile.HasBomb()) {
			return false;
		}
	}
	return true;
}

const Minefield::GameState Minefield::GetGameState() const {
	return gameState;
}

RectI Minefield::GetFieldRect() const {
	const int tileSize = SpriteCodex::tileSize;
	const int fieldWidth = nColumns * tileSize;
	const int fieldHeight = nRows * tileSize;
	return RectI(fieldPosition, fieldWidth, fieldHeight);;
}
