#pragma once
#include "Graphics.h"

class Minefield {
public:
	class Tile {
	public:
		enum class Status : unsigned char{
			unrevealed,
			flagged,
			revealed
		};
	public:
		void Reveal();
		bool HasBomb() const;
		void Draw(const Vei2& pos, Graphics& gfx) const;
		void SpawnMine();
	private:
		bool hasBomb = false;
		Status status = Status::unrevealed;
	};
public:
	Minefield();
	void LeftClick(const Vei2& screenPosition);
	void Draw(Graphics& gfx) const;
private:
	void SpawnMines(int quantity);
	const Tile& GetTileAtPosition(const Vei2 & position) const;
	Tile& GetTileAtPosition(const Vei2 & position);
	RectI GetFieldRect() const;
private:
	const Vei2 fieldPosition = Vei2(50, 50);
	static constexpr int nColumns = 16;
	static constexpr int nRows = 12;
	static constexpr int Mines = 10;
	Tile tiles[nColumns * nRows];
};