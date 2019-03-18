//=============================================================================
//
// フォグ処理 [fog.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "fog.h"






void InitFog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//フォグが使えるか
	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(D3DCAPS9));


	FLOAT StartPos = 0;
	FLOAT EndPos = 2000;
	FLOAT Fog = 0.001f;

	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(100, 0, 100, 255));
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Fog));
	//pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(100, 0, 100, 255));
	//pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	//pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	//pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&StartPos));
	//pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&EndPos));

}

void UnInitFog(void)
{

}

void UpdateFog(void)
{
}

void DrawFog(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	


	
}
