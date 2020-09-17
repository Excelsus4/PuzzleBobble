#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle, float speed, int color) :
	color(color)
{
	float YIndex = 33 * (color % 4);
	float XIndex = 554 * (color / 4);

	position = start;
	for (int i = 0; i < 7; i++) {
		sprite[i] = new Sprite(Textures + L"PuzzleBobble/PuzzleBobble.png", 
			shaderFile, 1 + XIndex + i*17, 1854 + YIndex, 17 + XIndex + i*17, 1870 + YIndex);
		sprite[i]->Position(position);
	}

	float radian = Math::ToRadian(angle);
	velocity.x = sinf(radian);
	velocity.y = cosf(radian);
	velocity *= speed;

	gCount = Math::Random(0.0f, 6.9f);
}

Bullet::~Bullet()
{
	for(int i = 0; i < 7;i++)
		SAFE_DELETE(sprite[i]);
}

void Bullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	gCount = Math::Modulo(gCount + 0.003f, 7);
	if (position.x < 104)
		Reflection(D3DXVECTOR2(1, 0));
	if (position.x > 216)
		Reflection(D3DXVECTOR2(-1, 0));

	position += velocity;

	sprite[(int)(gCount / 1)]->Position(position);
	sprite[(int)(gCount / 1)]->Update(V, P);
}

void Bullet::Reflection(const D3DXVECTOR2 & n)
{
	velocity = velocity - 2 * D3DXVec2Dot(&velocity, &n) * n;
}

void Bullet::Position(const float & x, const float & y)
{
	Position(D3DXVECTOR2(x, y));
}

void Bullet::Position(const D3DXVECTOR2 & pos)
{
	position = pos;
}

void Bullet::Stop()
{
	velocity = D3DXVECTOR2(0, 0);
}

void Bullet::Render()
{
	sprite[(int)(gCount / 1)]->Render();
}

