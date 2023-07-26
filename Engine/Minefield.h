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
		GameOver,
		Win
	};
	class Tile {
	public:
		enum class TileState : unsigned char{
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
		TileState status = TileState::Hidden;
		int nNeighborMines = -1;
	};
public:
	Minefield( Vei2 position, int nMines);
	void OnRevealClick(const Vei2& mousePosition);
	void OnFlagClick(const Vei2& mousePosition);
	void Draw(Graphics& gfx) const;
	RectI GetFieldRect() const;
	const GameState GetGameState() const;
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
	bool IsGameWon() const;
private:
	GameState gameState = GameState::NotStarted;
	static constexpr int nColumns = 30;
	static constexpr int nRows = 16;
	const Vei2 fieldPosition;
	Tile tiles[nColumns * nRows];
	Vei2 hintPosition = Vei2(-1, -1);
};