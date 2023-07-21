#pragma once
#include "Graphics.h"

class Minefield {
public:
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
		void SetFlag(bool value);
		void SpawnMine();
		bool HasBomb() const;
		void Draw(const Vei2& pos, Graphics& gfx) const;
	private:
		bool hasBomb = false;
		Status status = Status::Hidden;
	};
public:
	Minefield(int nMemes);
	void OnRevealClick(const Vei2& mousePosition);
	void OnFlagClick(const Vei2& mousePosition);
	void Draw(Graphics& gfx) const;
private:
	void SpawnMines(int nMines);
	bool IsScreenPositionInsideGrid(const Vei2 & screenPosition) const;
	Vei2 ScreenSpaceToGridSpace(const Vei2& screenPosition) const;
	const Tile& GetTileAtPosition(const Vei2 & position) const;
	Tile& GetTileAtPosition(const Vei2 & position);
	RectI GetFieldRect() const;
private:
	const Vei2 fieldPosition = Vei2(50, 50);
	static constexpr int nColumns = 16;
	static constexpr int nRows = 12;
	Tile tiles[nColumns * nRows];
};