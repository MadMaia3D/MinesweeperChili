#pragma once
#include "Graphics.h"
#include "SpriteCodex.h"

class Minefield {
public:
	struct TileArea {
		int top;
		int left;
		int bottom;
		int right;
	};
	enum class GameState : unsigned char {
		NotStarted,
		Playing,
		GameOver
	};
	class Tile {
	public:
		enum class Status : unsigned char{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void Reveal();
		bool IsRevealed() const;
		bool IsFlagged() const;
		bool HasBomb() const;
		void SetFlag(bool value);
		void SpawnMine();
		void SetNeighborMinesNumber(int nMines);
		int GetnNeighborMines() const;
		bool HasNeighborMines() const;
		void Draw(const Vei2& pos, Graphics& gfx) const;
		void DrawOnGameOver(const Vei2& pos, Graphics& gfx) const;
	private:
		bool hasBomb = false;
		Status status = Status::Hidden;
		int nNeighborMines = -1;
	};
public:
	Minefield(int nMemes);
	void OnRevealClick(const Vei2& mousePosition);
	void OnFlagClick(const Vei2& mousePosition);
	void Draw(Graphics& gfx) const;
private:
	void RevealMine(const Vei2& gridPosition);
	void SpawnMines(int nMines);
	void SetNeighborMinesNumber();
	int GetNeighborFlagsCount(const Vei2& gridPosition) const;
	void RevealNeighbors(const Vei2& gridPosition);
	Vei2 GenerateHintPosition() const;
	const TileArea GetSurroundingArea(const Vei2& gridPosition) const;
	int CountNeighborMines(const Vei2& gridPosition) const;
	bool IsScreenPositionInsideGrid(const Vei2 & screenPosition) const;
	Vei2 ScreenSpaceToGridSpace(const Vei2& screenPosition) const;
	const Tile& GetTileAtPosition(const Vei2 & position) const;
	Tile& GetTileAtPosition(const Vei2 & position);
	RectI GetFieldRect() const;
private:
	GameState gameState = GameState::NotStarted;
	static constexpr int nColumns = 16;
	static constexpr int nRows = 12;
	static constexpr int fieldPositionX = (Graphics::ScreenWidth) / 2 - (SpriteCodex::tileSize * nColumns) / 2;
	static constexpr int fieldPositionY = (Graphics::ScreenHeight) / 2 - (SpriteCodex::tileSize * nRows) / 2;
	const Vei2 fieldPosition = Vei2(fieldPositionX, fieldPositionY);
	Tile tiles[nColumns * nRows];
	Vei2 hintPosition = Vei2(-1, -1);
};