#pragma once

#include "Graphics.h"
#include "Vei2.h"

class SpriteCodex
{
public:
	// width and height of all tiles
	static constexpr int tileSize = 24;
	// base color for all tiles
	static constexpr Color baseColor = { 192,192,192 };
	static constexpr Color shadowColor = { 128, 128, 128};
	// 16x16 tile sprites assume (192,192,192) background and top left origin
	static void DrawTile0( const Vei2& pos,Graphics& gfx );
	static void DrawTile1( const Vei2& pos,Graphics& gfx );
	static void DrawTile2( const Vei2& pos,Graphics& gfx );
	static void DrawTile3( const Vei2& pos,Graphics& gfx );
	static void DrawTile4( const Vei2& pos,Graphics& gfx );
	static void DrawTile5( const Vei2& pos,Graphics& gfx );
	static void DrawTile6( const Vei2& pos,Graphics& gfx );
	static void DrawTile7( const Vei2& pos,Graphics& gfx );
	static void DrawTile8( const Vei2& pos,Graphics& gfx );
	static void DrawTileNumber(const Vei2& pos, int number, Graphics& gfx);
	static void DrawTileButton( const Vei2& pos,Graphics& gfx );
	static void DrawTileCrossRed( const Vei2& pos,Graphics& gfx );
	static void DrawTileGreenCross(const Vei2& pos, Graphics& gfx);
	static void DrawTileFlag( const Vei2& pos,Graphics& gfx );
	static void DrawTileBomb( const Vei2& pos,Graphics& gfx );
	static void DrawTileBombRed( const Vei2& pos,Graphics& gfx );
	// code for beveled rectangle drawing
	static void DrawBeveledSquareUp(Vei2 pos, int width, int height, int bevel, Graphics& gfx);
	static void DrawBeveledSquareUp(RectI rect, int bevel, Graphics& gfx);
	static void DrawBeveledSquareDown(Vei2 pos, int width, int height, int bevel, Graphics& gfx);
	static void DrawBeveledSquareDown(RectI rect, int bevel, Graphics& gfx);
	// code for border drawing
private:
	static void DrawBevelTop(Vei2 pos, int width, int depth, Color color, Graphics& gfx);
	static void DrawBevelRight(Vei2 pos, int width, int depth, Color color, Graphics& gfx);
	static void DrawBevelBottom(Vei2 pos, int width, int depth, Color color, Graphics& gfx);
	static void DrawBevelLeft(Vei2 pos, int width, int depth, Color color, Graphics& gfx);
};