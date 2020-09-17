#pragma once
#include "Objects/Bullet.h"
#include "Objects/Pop.h"

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
	UINT scanIndex;
public:
	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

public:
	void SetBullet(Bullet* b);
	void DestroyBullet(vector<Pop*>* popAnim);
	bool CollisionCheck(const Bullet& b);

	void SetIndexToNearBullets(const UINT& index);
};

//-----------------------------------------------------------------------------
// HexMap
//-----------------------------------------------------------------------------
class HexMap {
public:
	HexMap(const int& width, const int& height, const D3DXVECTOR2& pos, const D3DXVECTOR2& gridSize);
	~HexMap();

public:
	void SetPopAnim(vector<Pop*>* popAnpim);

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

private:
	UINT scanIndex;
	vector<Hex*> left;
	vector<Pop*>* popAnim;

public:
	bool isFit(const Bullet* b) const;
	Hex* Fit(Bullet* b);

	void StartScan();
	int Scan(Hex* start, const int& color);
	
	void Destroy(Hex* start, const int& color);
	void CleanUp();
};