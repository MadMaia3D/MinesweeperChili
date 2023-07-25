#include "MinesweeperGame.h"

MinesweeperGame::MinesweeperGame(int nMines)
	:
	minefield(position + Vei2(smallBorderSize, topBorderSize), nMines)
{
	gameRect = minefield.GetFieldRect();
	gameRect.top -= topBorderSize;
	gameRect.right += smallBorderSize;
	gameRect.bottom += smallBorderSize;
	gameRect.left -= smallBorderSize;
}

void MinesweeperGame::InputLeftClick(const Vei2& clickPosition){
	minefield.OnRevealClick(clickPosition);
}

void MinesweeperGame::InputRightClick(const Vei2& clickPosition){
	minefield.OnFlagClick(clickPosition);
}

void MinesweeperGame::Draw(Graphics& gfx) const {
	static constexpr int bevelDepth = 4;
	SpriteCodex::DrawBeveledSquareUp(gameRect, bevelDepth, gfx);
	const RectI fieldRect = minefield.GetFieldRect().GetExpanded(bevelDepth);
	SpriteCodex::DrawBeveledSquareDown(fieldRect, bevelDepth, gfx);

	minefield.Draw(gfx);

	RectI frameTop = gameRect;
	frameTop.bottom = fieldRect.top;
	frameTop = frameTop.GetExpanded(bevelDepth - smallBorderSize);
	SpriteCodex::DrawBeveledSquareDown(frameTop, bevelDepth, gfx);

}