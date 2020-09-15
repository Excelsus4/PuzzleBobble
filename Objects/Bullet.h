#pragma once

class Bullet {
public:
	Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle, float speed);
	virtual ~Bullet();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void Reflection(const D3DXVECTOR2& n);

private:
	D3DXVECTOR2 velocity;
	Sprite* sprite;

	D3DXVECTOR2 position;
};