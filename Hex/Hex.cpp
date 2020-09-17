#include "stdafx.h"
#include "Hex.h"

Hex::Hex(Hex * n9, Hex * n11, Hex * n1, const D3DXVECTOR2& pos) :
	position(pos)
{
	neighbor[nine] = n9;
	if (n9 != nullptr)
		n9->neighbor[three] = this;
	neighbor[eleven] = n11;
	if (n11 != nullptr)
		n11->neighbor[five] = this;
	neighbor[one] = n1;
	if (n1 != nullptr)
		n1->neighbor[seven] = this;
	neighbor[three] = nullptr;
	neighbor[five] = nullptr;
	neighbor[seven] = nullptr;

	bullet = nullptr;
	scanIndex = 0;
}

Hex::~Hex()
{
	SAFE_DELETE(neighbor[three]);
	SAFE_DELETE(bullet);
}

void Hex::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if(bullet)
		bullet->Update(V, P);
	if (neighbor[three])
		neighbor[three]->Update(V, P);
}

void Hex::Render()
{
	if(bullet)
		bullet->Render();
	if (neighbor[three])
		neighbor[three]->Render();
}

void Hex::SetBullet(Bullet * b)
{
	bullet = b;
	b->Position(position);
	b->Stop();
}

void Hex::DestroyBullet(vector<Pop*>* popAnim)
{
	if(bullet)
		popAnim->push_back(new Pop(Shaders + L"008_Sprite.fx", bullet->Position(), bullet->Color()));
	SAFE_DELETE(bullet);
}

bool Hex::CollisionCheck(const Bullet & b)
{
	if (bullet) {
		D3DXVECTOR2 diff = position - b.Position();
		float d = sqrtf(D3DXVec2Dot(&diff, &diff));
		if (d < 16)
 			return true;
	}
	if(neighbor[three])
		return neighbor[three]->CollisionCheck(b);
	return false;
}

void Hex::SetIndexToNearBullets(const UINT & index)
{
	if (scanIndex >= index || bullet == nullptr)
		return;
	else {
		scanIndex = index;
		for (int i = 0; i < 6; i++) {
			if(neighbor[i])
				neighbor[i]->SetIndexToNearBullets(index);
		}
	}
}

//-----------------------------------------------------------------------------
// HexMap
//-----------------------------------------------------------------------------

HexMap::HexMap(const int& width, const int& height, const D3DXVECTOR2& pos, const D3DXVECTOR2& gridSize):
	scanIndex(0)
{
	Hex* newHex;
	Hex* lastHex;
	D3DXVECTOR2 shifter(gridSize.x, 0.0f);
	D3DXVECTOR2 linefeed(gridSize.x / 2, -gridSize.y);

	//First Line
	newHex = new Hex(nullptr, nullptr, nullptr, pos);
	left.push_back(newHex);
	for (int j = 1; j < width; j++) {
		lastHex = newHex;
		newHex = new Hex(lastHex, nullptr, nullptr, lastHex->position + shifter);
	}

	for (int i = 1; i < height; i++) { 
		if (i % 2 == 1) {
			lastHex = left.back();
			newHex = new Hex(nullptr, lastHex, lastHex->neighbor[Hex::three],
				lastHex->position + linefeed);
			left.push_back(newHex);
		}
		else {
			lastHex = left.back();
			newHex = new Hex(nullptr, nullptr, lastHex,
				lastHex->position + linefeed - shifter);
			left.push_back(newHex);
		}
		for (int j = 1; j < width + (width % 2 ? 0 : (i%2?-1:0)); j++) {
			lastHex = newHex;
			newHex = new Hex(lastHex,
				lastHex->neighbor[Hex::one],
				lastHex->neighbor[Hex::one]->neighbor[Hex::three],
				lastHex->position + shifter);
		}
	}
}

HexMap::~HexMap()
{
	for (Hex* h : left) {
		SAFE_DELETE(h);
	}
}

void HexMap::SetPopAnim(vector<Pop*>* p)
{
	popAnim = p;
}

void HexMap::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	for (Hex* h : left) {
		h->Update(V, P);
	}
}

void HexMap::Render()
{
	for (Hex* h : left) {
		h->Render();
	}
}

bool HexMap::isFit(const Bullet * b) const
{
	D3DXVECTOR2 pos = b->Position();

	if (pos.y >= 192)
		return true;
	else {
		for (Hex* h : left) {
			if (h->CollisionCheck(*b))
				return true;
		}
	}
	return false;
}

Hex* HexMap::Fit(Bullet * b)
{
	D3DXVECTOR2 pos = b->Position();
	float d = INFINITY;
	Hex* currentTarget = nullptr;
	for (Hex* h : left) {
		if (d > abs(h->position.y - pos.y)) {
			d = abs(h->position.y - pos.y);
			currentTarget = h;
		}
	}
	assert(currentTarget);
	d = abs(currentTarget->position.x - pos.x);
	Hex* lastAnswer = currentTarget;
	while (currentTarget->neighbor[Hex::three]) {
		currentTarget = currentTarget->neighbor[Hex::three];
		if (d > abs(currentTarget->position.x - pos.x)) {
			d = abs(currentTarget->position.x - pos.x);
			lastAnswer = currentTarget;
		}
	}
	
	lastAnswer->SetBullet(b);
	return lastAnswer;
}

void HexMap::StartScan()
{
	++scanIndex;
}

int HexMap::Scan(Hex * start, const int& color)
{
	// This indicates this hex is already scanned by other route
	if (start->scanIndex >= scanIndex)
		return 0;

	// Update scan Index for this hex node
	start->scanIndex = scanIndex;

	if (start->bullet) {
		// Bullet exist in start
		if (start->bullet->Color() == color) {
			//Bullet is same color
			int sum = 0;
			// Scan through the hexmap from startpoint
			for (int i = 0; i < 6; i++) {
				if (start->neighbor[i]) {
					sum += Scan(start->neighbor[i], color);
				}
			}
			// Return sum
			return sum + 1;
		}
		else {
			//Bullet is different color
			return 0;
		}
	}
	else {
		// No Bullet in start
		return 0;
	}
	
	assert(false);
	return 0;
}

void HexMap::Destroy(Hex * start, const int & color)
{
	if (start->bullet) {
		// Bullet exist in start
		if (start->bullet->Color() == color) {
			start->DestroyBullet(popAnim);

			// Scan through the hexmap from startpoint
			for (int i = 0; i < 6; i++) {
				if (start->neighbor[i]) {
					Destroy(start->neighbor[i], color);
				}
			}
			// Return
			return;
		}
		else {
			//Bullet is different color
			return;
		}
	}
	else {
		// No Bullet in start
		return;
	}
}

void HexMap::CleanUp()
{
	//TODO: Scan thorugh the hexmap and remove any floating bubbles
	++scanIndex;

	// Refresh first line and all that connected to it 
	{
		Hex* c = left[0];
		while (c) {
			c->SetIndexToNearBullets(scanIndex);
			c = c->neighbor[Hex::three];
		}
	}

	//travel
	for (size_t i = 1; i < left.size(); i++) {
		Hex* c = left[i];
		while (c) {
			if (c->scanIndex < scanIndex)
				c->DestroyBullet(popAnim);
			c = c->neighbor[Hex::three];
		}
	}
}
