#include "stdafx.h"
#include "./Systems/Device.h"
#include "./Objects/Background.h"
#include "./Objects/Bullet.h"
#include "./Hex/Hex.h"

Background* bg;
vector<Sprite*> sprites;
vector<Bullet*> bullets;
Sprite* arrow;
int nextnextColor;
int nextColor;
float bulletSpeed = 0.15f;
float arrowSpeed = 0.03f;

Bullet* outbag[8];
Bullet* loaded[8];

HexMap* hmap;

void InitScene() {
	wstring spriteFile = Textures + L"PuzzleBobble/PuzzleBobble.png";
	wstring shaderFile = Shaders + L"008_Sprite.fx";

	bg = new Background(shaderFile);

	arrow = new Sprite(spriteFile, shaderFile, 1, 1545, 65, 1609);
	arrow->Position(160, 31);
	arrow->Rotation(15);
	sprites.push_back(arrow);

	Sprite* dragon = new Sprite(spriteFile, shaderFile, 1, 2012, 33, 2044);
	dragon->Position(142, 24);
	sprites.push_back(dragon);

	dragon = new Sprite(spriteFile, shaderFile, 1, 2045, 33, 2077);
	dragon->Position(195, 24);
	sprites.push_back(dragon);

	nextColor = Math::Random(0, 7);
	nextnextColor = Math::Random(0, 7);

	for (int i = 0; i < 8; i++) {
		loaded[i] = new Bullet(shaderFile, D3DXVECTOR2(160, 31), 0.0f, 0.0f, i);
		outbag[i] = new Bullet(shaderFile, D3DXVECTOR2(125, 16), 0.0f, 0.0f, i);
	}

	hmap = new HexMap(8, 12, D3DXVECTOR2(104, 192), D3DXVECTOR2(16, 14));
}

void DestroyScene() {
	SAFE_DELETE(hmap);
	for (auto b : bullets)
		SAFE_DELETE(b);
	for (auto s : sprites)
		SAFE_DELETE(s);
	for (int i = 0; i < 8; i++) {
		SAFE_DELETE(loaded[i]);
		SAFE_DELETE(outbag[i]);
	}
	SAFE_DELETE(bg);
}

D3DXMATRIX V, P;

void Update() {
	if (Key->Press(VK_LEFT)) {
		arrow->Rotation(arrow->Rotation() - arrowSpeed);
	}
	if (Key->Press(VK_RIGHT)) {
		arrow->Rotation(arrow->Rotation() + arrowSpeed);
	}

	//View
	D3DXVECTOR3 eye(0, 0, -1);
	D3DXVECTOR3 at(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&V, &eye, &at, &up);

	//Projection
	D3DXMatrixOrthoOffCenterLH(&P, 0, (float)Width/3, 0, (float)Height/3, -1, 1);

	bg->Update(V, P);

	for (Sprite* sprite : sprites) {
		sprite->Update(V, P);
	}
	
	if (Key->Down(VK_SPACE)) {
		wstring file = Shaders + L"008_Sprite.fx";

		float angle = arrow->Rotation();
		float speed = bulletSpeed;

		Bullet* bullet = new Bullet(file, D3DXVECTOR2(160, 31), angle, speed, nextColor);
		bullets.push_back(bullet);

		nextColor = nextnextColor;
		nextnextColor = Math::Random(0, 7);
	}

	for (auto iter = bullets.begin(); iter != bullets.end(); ) {
		if (hmap->isFit(*iter)) {
			Hex* newBullet = hmap->Fit(*iter);
			hmap->StartScan();
			int sameColor = hmap->Scan(newBullet, newBullet->bullet->Color());
			if (sameColor >= 3) {
				hmap->Destroy(newBullet, newBullet->bullet->Color());
				hmap->CleanUp();
			}
			
			auto lazy = iter;
			iter = bullets.erase(lazy);
		}
		else {
			(*iter)->Update(V, P);
			++iter;
		}
	}

	hmap->Update(V, P);

	loaded[nextColor]->Update(V, P);
	outbag[nextnextColor]->Update(V, P);
}

void Render() {
	D3DXCOLOR bgColor = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		bg->Render();

		for (Sprite* sprite : sprites)
			sprite->Render();

		for (Bullet* bullet : bullets)
			bullet->Render();

		hmap->Render();

		loaded[nextColor]->Render();
		outbag[nextnextColor]->Render();
	}
	ImGui::Render();
	SwapChain->Present(0, 0);
}