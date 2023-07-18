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
		void Draw(const Vei2& pos, Graphics& gfx) const;
	private:
		bool hasBomb = false;
		Status status = Status::unrevealed;
	};
public:
	void Draw(Graphics& gfx) const;
	Tile GetTileAtPosition(int x, int y) const;
	Tile GetTileAtPosition(const Vei2 & position) const;
private:
	const Vei2 fieldPosition = Vei2(50, 50);
	static constexpr int nColumns = 16;
	static constexpr int nRows = 12;
	static constexpr int Mines = 10;
	Tile tiles[nColumns * nRows];
};