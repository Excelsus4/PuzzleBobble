#pragma once

class Pop {
public:
	Pop(wstring shaderFile, D3DXVECTOR2 start, int color);
	virtual ~Pop();

	bool Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

private:
	float gCount;
	Sprite* sprite[7];

	D3DXVECTOR2 position;
};