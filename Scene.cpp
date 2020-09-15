#include "stdafx.h"
#include "./Systems/Device.h"
#include "./Objects/Background.h"
#include "./Objects/Bullet.h"

Background* bg;
vector<Sprite*> sprites;
vector<Bullet*> bullets;

D3DXVECTOR2 position;

void InitScene() {
	bg = new Background(Shaders+L"008_Sprite.fx");

	wstring shaderFile = Shaders+L"008_Sprite.fx";

	Sprite* mario = new Sprite(Textures + L"Mario/All.png", shaderFile, 9, 87, 50, 161);
	mario->Position(300, 145);
	sprites.push_back(mario);

	position = D3DXVECTOR2(400, 300);
}

void DestroyScene() {
	for (Bullet* bullet : bullets)
		SAFE_DELETE(bullet);
	
	for (Sprite* sprite : sprites)
		SAFE_DELETE(sprite);

	SAFE_DELETE(bg);
}

D3DXMATRIX V, P;

void Update() {
	//View
	D3DXVECTOR3 eye(0, 0, -1);
	D3DXVECTOR3 at(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&V, &eye, &at, &up);

	//Projection
	D3DXMatrixOrthoOffCenterLH(&P, 0, (float)Width, 0, (float)Height, -1, 1);

	bg->Update(V, P);

	for (Sprite* sprite : sprites) {
		sprite->Update(V, P);
	}

	if (Key->Press(VK_LEFT))
		position.x -= 200.0f * Timer->Elapsed();
	if (Key->Press(VK_RIGHT))
		position.x += 200.0f * Timer->Elapsed();
	if (Key->Press(VK_UP))
		position.y += 200.0f * Timer->Elapsed();
	if (Key->Press(VK_DOWN))
		position.y -= 200.0f * Timer->Elapsed();

	if (Key->Down(VK_SPACE)) {
		wstring file = Shaders + L"008_Sprite.fx";

		float angle = Math::Random(-179.9f, 179.9f);
		float speed = Math::Random(0.05f, 1.0f);

		Bullet* bullet = new Bullet(file, position, angle, speed);
		bullets.push_back(bullet);
	}

	for (Bullet* bullet : bullets)
		bullet->Update(V, P);
}

int pass = 0;

void Render() {
	D3DXCOLOR bgColor = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		bg->Render();

		for (Sprite* sprite : sprites)
			sprite->Render();

		for (Bullet* bullet : bullets)
			bullet->Render();
	}
	ImGui::Render();
	SwapChain->Present(0, 0);
}