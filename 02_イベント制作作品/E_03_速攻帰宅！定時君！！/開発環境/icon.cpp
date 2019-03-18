//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// アイコン処理 [icon.cpp]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "icon.h"
#include "main.h"
#include "player.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_ICON			(64)							// アイコンの最大数

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	char *pFilename;	// ファイル名
} IconTexInfo;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9		g_pTextureIcon[ICON_TEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;				// 頂点バッファへのポインタ
Icon					g_aIcon[MAX_ICON];					// アイコンの情報

// 読み込むテクスチャ名の設定
IconTexInfo g_apTextureIconInfo[ICON_TEX_MAX] =
{
	{ "data//TEXTURE//rizaruto_001.png" },			// 今回の帰宅時間
	{ "data//TEXTURE//rank1.png" },			// rank1
	{ "data//TEXTURE//rank2.png" },			// rank2
	{ "data//TEXTURE//rank3.png" },			// rank3
	{ "data//TEXTURE//rank4.png" },			// rank4
	{ "data//TEXTURE//rank5.png" },			// rank5sokkoukitakuou
	{ "data//TEXTURE//sokkoukitakuou.png" },// rank5
};

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitIcon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// アイコンの情報の初期化
	for(int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{
		g_aIcon[nCntIcon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aIcon[nCntIcon].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aIcon[nCntIcon].nTexType = ICON_TEX_GOGOME_TIME;
		g_aIcon[nCntIcon].fWidth = 0.0f;
		g_aIcon[nCntIcon].fHeight = 0.0f;
		g_aIcon[nCntIcon].bUse = false;
	}

	// テクスチャの読み込み
	for (int nCntIconTex = 0; nCntIconTex < ICON_TEX_MAX; nCntIconTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureIconInfo[nCntIconTex].pFilename,
			&g_pTextureIcon[nCntIconTex]);
	}

	// 頂点情報の作成
	MakeVertexIcon(pDevice);

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitIcon(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < ICON_TEX_MAX; nCntTex++)
	{
		// テクスチャの開放
		if (g_pTextureIcon[nCntTex] != NULL)
		{
			g_pTextureIcon[nCntTex]->Release();
			g_pTextureIcon[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffIcon != NULL)
	{
		g_pVtxBuffIcon->Release();
		g_pVtxBuffIcon = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateIcon()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawIcon(void)
{
	int nCntIcon;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffIcon, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// アイコンの描画
	for(nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureIcon[g_aIcon[nCntIcon].nTexType]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntIcon * 4, 2);
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ICON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIcon,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffIcon->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// アイコンの設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetIcon(D3DXVECTOR3 pos, int Textype, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
							//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++, pVtx += 4)
	{
		if (g_aIcon[nCntIcon].bUse == false)
		{ // アイコンが使用されていない場合
		  // 頂点位置の設定
			g_aIcon[nCntIcon].pos = pos;
			g_aIcon[nCntIcon].fWidth = fWidth;
			g_aIcon[nCntIcon].fHeight = fHeight;

			// アイコンの種類の設定
			g_aIcon[nCntIcon].nTexType = Textype;

			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x - g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y - g_aIcon[nCntIcon].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x + g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y - g_aIcon[nCntIcon].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x - g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y + g_aIcon[nCntIcon].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIcon[nCntIcon].pos.x + g_aIcon[nCntIcon].fWidth / 2, g_aIcon[nCntIcon].pos.y + g_aIcon[nCntIcon].fHeight / 2, 0.0f);

			g_aIcon[nCntIcon].bUse = true;	// 使用する状態にする
			break;	// このbreakはかなり大切
		}
		// 頂点バッファをアンロックする
		g_pVtxBuffIcon->Unlock();
	}
}

