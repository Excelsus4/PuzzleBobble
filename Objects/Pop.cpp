#include "stdafx.h"
#include "Pop.h"

Pop::Pop(wstring shaderFile, D3DXVECTOR2 start, int color):
	gCount(Math::Random(0.0f,0.5f))
{
	float YIndex = 33 * (color % 4);
	float XIndex = 554 * (color / 4);

	position = start;
	for (int i = 0; i < 7; i++) {
		sprite[i] = new Sprite(Textures + L"PuzzleBobble/PuzzleBobble.png",
			shaderFile, 171 + XIndex + i * 33, 1846 + YIndex, 202 + XIndex + i * 33, 1877 + YIndex);
		sprite[i]->Position(position);
	}
}

Pop::~Pop()
{
	for (int i = 0; i < 7; i++)
		SAFE_DELETE(sprite[i]);
}

bool Pop::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	gCount = gCount + 0.007f;
	if (gCount >= 7)
		return true;

	sprite[(int)(gCount / 1)]->Position(position);
	sprite[(int)(gCount / 1)]->Update(V, P);

	return false;
}

void Pop::Render()
{
	sprite[(int)(gCount / 1)]->Render();
}
