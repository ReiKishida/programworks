//=============================================================================
//
// シャドウ処理 [shadow.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
#include "renderer.h"
#include "shadow.h"
#include "manager.h"
#include "light.h"
//=============================================================================
// シャドウの描画
//=============================================================================
void CShadow::SetShadow(LPD3DXMESH Mesh, DWORD NumMat, D3DXMATRIX mtxWorld,D3DXVECTOR3 *pPos)
{
	// デバイスの確保
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// デバイスの取得

	//影用の射影マトリクスを作成
	D3DXMATRIX mShadow;				// マトリックス
	D3DXVECTOR3 vPoint= *pPos;	// 平面に使う点
	D3DXVECTOR3 vNormal(0, -1, 0);	// 平面に使う法線
	D3DXPLANE GroundPlane;

	// 点と法線から平面を作成する
	D3DXPlaneFromPointNormal(&GroundPlane, pPos, &vNormal);

	// ジオメトリを平面に射影する
	D3DXMatrixShadow(&mShadow, &D3DXVECTOR4(0.7f, -0.8f, 0.54f, 0.0f), &GroundPlane);

	mShadow = mtxWorld*mShadow;
	mShadow._42 += 1;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);	// ステンシル･バッファ有効

	//影用マテリアルの設定
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Emissive.a = 1.0f;
	Material.Emissive.r = 0.0f;
	Material.Emissive.g = 0.0f;
	Material.Emissive.b = 0.0f;
	Material.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	//影としてレンダリング
	pDevice->SetTransform(D3DTS_WORLD, &mShadow);
	pDevice->SetMaterial(&Material);
	for (DWORD nCnt = 0; nCnt < NumMat; nCnt+=2)
	{// 描画を軽く
		pDevice->SetTexture(0, NULL);
		Mesh->DrawSubset(nCnt);
	}
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);	// ステンシル･バッファ無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);



}
