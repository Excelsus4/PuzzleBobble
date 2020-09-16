#pragma once
#include "Objects/Bullet.h"

class Hex {
public:
	enum direction {
		one,
		three,
		five,
		seven,
		nine,
		eleven
	};
	Hex(Hex* n9, Hex* n11, Hex* n1, const D3DXVECTOR2& pos);
	~Hex();

	Bullet* bullet;
	Hex* neighbor[6];

	D3DXVECTOR2 position;

public:
	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

public:
	void SetBullet(Bullet* b);
	bool CollisionCheck(const Bullet& b);
};

//-----------------------------------------------------------------------------
// HexMap
//-----------------------------------------------------------------------------
class HexMap {
public:
	HexMap(const int& width, const int& height, const D3DXVECTOR2& pos, const D3DXVECTOR2& gridSize);
	~HexMap();

public:
	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

private:
	vector<Hex*> left;

public:
	bool isFit(const Bullet* b) const;
	void Fit(Bullet* b);
};