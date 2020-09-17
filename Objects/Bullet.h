#pragma once

class Bullet {
public:
	Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle, float speed, int color);
	virtual ~Bullet();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void Reflection(const D3DXVECTOR2& n);

	D3DXVECTOR2 Position() const { return position; }
	void Position(const float& x, const float& y);
	void Position(const D3DXVECTOR2& pos);

	int Color() const { return color; }

	void Stop();
private:
	float gCount;

	D3DXVECTOR2 velocity;
	Sprite* sprite[7];

	D3DXVECTOR2 position;
	int color;
};