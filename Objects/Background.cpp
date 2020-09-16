#include "stdafx.h"
#include "Background.h"

Background::Background(wstring shaderFile)
{
	wstring backgroundFile = Textures + L"PuzzleBobble/Background.png";
	wstring spriteFile = Textures + L"PuzzleBobble/PuzzleBobble.png";

	// Background
	backgroundObject[0] = new Sprite(backgroundFile, shaderFile, 16, 16, 336, 240);
	backgroundObject[0]->Position(160, 112);

	// Floor
	backgroundObject[1] = new Sprite(backgroundFile, shaderFile, 16, 248, 336, 256);
	backgroundObject[1]->Position(160, 4);

	// Line
	backgroundObject[2] = new Sprite(spriteFile, shaderFile, 581, 2185, 709, 2201);
	backgroundObject[2]->Position(160, 43);

	// Machine
	backgroundObject[3] = new Sprite(spriteFile, shaderFile, 1, 1805, 65, 1845);
	backgroundObject[3]->Position(160, 28);

	// Bag
	backgroundObject[4] = new Sprite(spriteFile, shaderFile, 781, 1813, 845, 1845);
	backgroundObject[4]->Position(110, 24);
}

Background::~Background()
{
	for (int i = 0; i < num; i++) {
		SAFE_DELETE(backgroundObject[i]);
	}
}

void Background::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	for (int i = 0; i < num; i++) {
		backgroundObject[i]->Update(V,P);
	}
}

void Background::Render()
{
	for (int i = 0; i < num; i++) {
		backgroundObject[i]->Render();
	}
}

