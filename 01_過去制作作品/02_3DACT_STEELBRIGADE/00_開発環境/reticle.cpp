//=============================================================================
//
// アイテム処理 [Reticle.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "reticle.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RETICLE		"data/TEXTURE/a.png"	// 読み込むテクスチャファイル名


//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureReticle = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffReticle = NULL;					// 頂点バッファへのポインタ

RETICLE g_Reticle;	//ブロック



//=============================================================================
// 初期化処理
//=============================================================================
void InitReticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();
	

	// アイテムの情報の初期化
	
		g_Reticle.pos = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 -55.0f, 0.0f);
		g_Reticle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
		g_Reticle.fWidth = 150.0f;
		g_Reticle.fHeight = 150.0f;

		g_Reticle.fLength = sqrtf((g_Reticle.fWidth / 2 + g_Reticle.fWidth / 2) * (g_Reticle.fHeight / 2 + g_Reticle.fHeight / 2));

		g_Reticle.fAngle = atan2f(g_Reticle.fWidth / 2, g_Reticle.fHeight / 2);


	


	
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RETICLE, &g_pTextureReticle);




	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffReticle,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

	
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[1].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[2].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[3].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	
	// 頂点バッファをアンロックする
	g_pVtxBuffReticle->Unlock();


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitReticle(void)
{

	
	//テクスチャの破棄
	if (g_pTextureReticle != NULL)
	{
		g_pTextureReticle->Release();
		g_pTextureReticle = NULL;
	}
	


	// 頂点バッファの開放
	if (g_pVtxBuffReticle != NULL)
	{
		g_pVtxBuffReticle->Release();
		g_pVtxBuffReticle = NULL;
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateReticle(void)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	CAMERA *pCamera;
	pCamera = GetCamera();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

	g_Reticle.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2 - 60.0f)+ pCamera->fPosR, 0.0f);

		g_Reticle.rot.x += 0.02f;
			g_Reticle.rot.y += 0.02f;



		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-D3DX_PI + g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[1].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(D3DX_PI - g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[2].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(-g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(-g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);


		pVtx[3].pos = D3DXVECTOR3(g_Reticle.pos.x + sinf(g_Reticle.fAngle + g_Reticle.rot.x) * g_Reticle.fLength,
			g_Reticle.pos.y + cosf(g_Reticle.fAngle + g_Reticle.rot.y) * g_Reticle.fLength,
			0.0f);



	

	// 頂点バッファをアンロックする
	g_pVtxBuffReticle->Unlock();

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawReticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffReticle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureReticle);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	
}

RETICLE GetReticle(void)
{
	return g_Reticle;
}




