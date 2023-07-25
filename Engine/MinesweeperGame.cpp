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
	// draw frame outer
	SpriteCodex::DrawBeveledSquareUp(gameRect, bevelDepth, gfx);

	// draw minefield emboss
	const RectI fieldRect = minefield.GetFieldRect().GetExpanded(bevelDepth);
	SpriteCodex::DrawBeveledSquareDown(fieldRect, bevelDepth, gfx);

	minefield.Draw(gfx);

	// code for top frame drawing
	RectI frameTop = gameRect;
	frameTop.bottom = fieldRect.top;
	frameTop = frameTop.GetExpanded(bevelDepth - smallBorderSize);
	SpriteCodex::DrawBeveledSquareDown(frameTop, bevelDepth, gfx);

	// code for face drawing
	const Vei2 frameTopCenter = frameTop.GetCenter();
	const int faceSize = SpriteCodex::faceSize;
	const RectI faceRect = RectI::FromCenter(frameTopCenter, faceSize / 2, faceSize / 2);
	SpriteCodex::DrawBeveledSquareUp(faceRect, bevelDepth, gfx);
	const Vei2 facePos(faceRect.left, faceRect.top);
	SpriteCodex::DrawFaceUnpressed(facePos, gfx);
}