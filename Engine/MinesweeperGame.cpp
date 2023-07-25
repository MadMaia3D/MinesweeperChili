#include "MinesweeperGame.h"

MinesweeperGame::MinesweeperGame(int nMines)
	:
	minefield(position + Vei2(smallBorderSize, topBorderSize), nMines)
{
	frameRect = minefield.GetFieldRect();
	frameRect.top -= topBorderSize;
	frameRect.right += smallBorderSize;
	frameRect.bottom += smallBorderSize;
	frameRect.left -= smallBorderSize;

	//  minefield emboss
	fieldRect = minefield.GetFieldRect().GetExpanded(bevelDepth);

	// top frame
	frameTop = frameRect;
	frameTop.bottom = fieldRect.top;
	frameTop = frameTop.GetExpanded(bevelDepth - smallBorderSize);

	// code for face drawing
	const Vei2 frameTopCenter = frameTop.GetCenter();
	const int faceSize = SpriteCodex::faceSize;
	faceRect = RectI::FromCenter(frameTopCenter, faceSize / 2, faceSize / 2);
}

void MinesweeperGame::InputLeftClick(const Vei2& clickPosition){
	minefield.OnRevealClick(clickPosition);
}

void MinesweeperGame::InputRightClick(const Vei2& clickPosition){
	minefield.OnFlagClick(clickPosition);
}

void MinesweeperGame::Draw(Graphics& gfx) const {
	SpriteCodex::DrawBeveledSquareUp(frameRect, bevelDepth, gfx);

	SpriteCodex::DrawBeveledSquareDown(fieldRect, bevelDepth, gfx);
	minefield.Draw(gfx);

	SpriteCodex::DrawBeveledSquareDown(frameTop, bevelDepth, gfx);

	SpriteCodex::DrawBeveledSquareUp(faceRect, bevelDepth, gfx);
	const Vei2 facePos(faceRect.left, faceRect.top);
	SpriteCodex::DrawFaceUnpressed(facePos, gfx);
}