#pragma once

class Background {
public:
	Background(wstring shaderFile);
	~Background();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

private:
	static const int num = 5;
	Sprite* backgroundObject[num];
};