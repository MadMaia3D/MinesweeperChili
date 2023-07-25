#pragma once
#include "Minefield.h"

class MinesweeperGame {
public:
	MinesweeperGame(int nMines);
	void InputLeftClick(const Vei2& clickPosition);
	void InputRightClick(const Vei2& clickPosition);
	void Draw(Graphics& gfx) const;
private:
	Vei2 position = Vei2(10, 10);
	Minefield minefield;
	int nFieldMines;
	static constexpr int topBorderSize = 100;
	static constexpr int smallBorderSize = 20;
	RectI gameRect;
};