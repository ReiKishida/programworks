////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Massage処理 [Massage.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MessageWindow.h"
#include "input.h"
#include "sound.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ブロックの中心座標
	D3DXCOLOR  col;				// 色
	MESSAGETEX nType;				// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
} MessageWindowInfo;
typedef struct
{
	char *pFileName;
}MessageWindowTexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_MESSAGEWNDOW		(10)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexMessageWindow(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMessageWindow[MESSAGETEX_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMessageWindow = NULL;		// 頂点バッファへのポインタ
MessageWindow						g_aMessageWindow[MAX_MESSAGEWNDOW];

// テクスチャ名
MessageWindowTexture  g_aMessageWindowTex[MESSAGETEX_MAX] =
{
	{ "data/TEXTURE/Massage000.png" },			// 最初
	{ "data/TEXTURE/Massage001.png" },			// 最初
	{ "data/TEXTURE/Massage002.png" },			// 最初
	{ "data/TEXTURE/Massage003.png" },			// 柵が上がった
	{ "data/TEXTURE/Massage004.png" },			// 習得おめでとう
	{ "data/TEXTURE/Massage005.png" },			// 何もなし
};

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitMessageWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < MESSAGETEX_MAX; nTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			g_aMessageWindowTex[nTex].pFileName,		// ファイルの名前
			&g_pTextureMessageWindow[nTex]);	// テクスチャへのポインタ
	}
	// 初期化
	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++)
	{
		g_aMessageWindow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心
		g_aMessageWindow[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
		g_aMessageWindow[nCnt].nType = MESSAGETEX_TUTORIAL;						// 種類
		g_aMessageWindow[nCnt].nState = MESSAGESTATE_NONE;
		g_aMessageWindow[nCnt].fWidth = 0;									// 幅
		g_aMessageWindow[nCnt].fHeight = 0;									// 高さ
		g_aMessageWindow[nCnt].bUse = false;								// 使用されているかどうか
	}
	// 頂点情報の作成
	MakeVertexMessageWindow(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitMessageWindow(void)
{
	// テクスチャの開放
	for (int nTex = 0; nTex < MESSAGETEX_MAX; nTex++)
	{
		if (g_pTextureMessageWindow[nTex] != NULL)
		{
			g_pTextureMessageWindow[nTex]->Release();
			g_pTextureMessageWindow[nTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMessageWindow != NULL)
	{
		g_pVtxBuffMessageWindow->Release();
		g_pVtxBuffMessageWindow = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateMessageWindow(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
						// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffMessageWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++, pVtx += 4)
	{
		if (g_aMessageWindow[nCnt].bUse == true)
		{
			
			g_aMessageWindow[nCnt].fWidth += (g_aMessageWindow[nCnt].fDiffWidth - g_aMessageWindow[nCnt].fWidth) * 0.5f;
			if (g_aMessageWindow[nCnt].nState == MESSAGESTATE_SET)
			{
				if (g_aMessageWindow[nCnt].fWidth >= g_aMessageWindow[nCnt].fDiffWidth - 10)
				{
					g_aMessageWindow[nCnt].fHeight += (g_aMessageWindow[nCnt].fDiffHeight - g_aMessageWindow[nCnt].fHeight) * 0.1f;
				}
			}
			if (g_aMessageWindow[nCnt].nState == MESSAGESTATE_DELETE)
			{
				if (g_aMessageWindow[nCnt].fWidth <= g_aMessageWindow[nCnt].fDiffWidth + 1)
				{
					g_aMessageWindow[nCnt].fHeight += (g_aMessageWindow[nCnt].fDiffHeight - g_aMessageWindow[nCnt].fHeight) * 0.1f;
					if (g_aMessageWindow[nCnt].fHeight <= g_aMessageWindow[nCnt].fHeight + 1)
					{
						g_aMessageWindow[nCnt].bUse = false;
					}
				}
			}
			// 頂点情報を設定
			pVtx[0].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;

			// 頂点カラーの設定
			pVtx[0].col = g_aMessageWindow[nCnt].col;
			pVtx[1].col = g_aMessageWindow[nCnt].col;
			pVtx[2].col = g_aMessageWindow[nCnt].col;
			pVtx[3].col = g_aMessageWindow[nCnt].col;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffMessageWindow->Unlock();
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawMessageWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMessageWindow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nTex = 0; nTex < MAX_MESSAGEWNDOW; nTex++)
	{
		if (g_aMessageWindow[nTex].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMessageWindow[g_aMessageWindow[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexMessageWindow(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MESSAGEWNDOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMessageWindow,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffMessageWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++, pVtx += 4)
	{
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// 頂点カラーの設定
		pVtx[0].col = g_aMessageWindow[nCnt].col;
		pVtx[1].col = g_aMessageWindow[nCnt].col;
		pVtx[2].col = g_aMessageWindow[nCnt].col;
		pVtx[3].col = g_aMessageWindow[nCnt].col;
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffMessageWindow->Unlock();
}
//==============================================================================================================================
// ウィンドウの設定
//==============================================================================================================================
void SetMessageWindow(D3DXVECTOR3 pos,float DiffWidth,float DiffHigeht,MESSAGETEX nType)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffMessageWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++, pVtx += 4)
	{
		if (g_aMessageWindow[nCnt].bUse == false)
		{
			g_aMessageWindow[nCnt].pos = pos;				// 中心
			g_aMessageWindow[nCnt].fDiffWidth = DiffWidth;		// 幅
			g_aMessageWindow[nCnt].fDiffHeight = DiffHigeht;		// 高さ

			g_aMessageWindow[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aMessageWindow[nCnt].nType = nType;		// 種類
			g_aMessageWindow[nCnt].fWidth = 0;		// 幅
			g_aMessageWindow[nCnt].fHeight = 10;	// 高さ
			g_aMessageWindow[nCnt].nState = MESSAGESTATE_SET;
			
				PlaySound(SOUND_LABEL_SE_TORI_MESSAGEWINDOW);
			
		
			g_aMessageWindow[nCnt].bUse = true;

			// 頂点情報を設定
			pVtx[0].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, -g_aMessageWindow[nCnt].fHeight/2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aMessageWindow[nCnt].fWidth / 2, g_aMessageWindow[nCnt].fHeight / 2, 0.0f) + g_aMessageWindow[nCnt].pos;

			// 頂点カラーの設定
			pVtx[0].col = g_aMessageWindow[nCnt].col;
			pVtx[1].col = g_aMessageWindow[nCnt].col;
			pVtx[2].col = g_aMessageWindow[nCnt].col;
			pVtx[3].col = g_aMessageWindow[nCnt].col;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffMessageWindow->Unlock();
}
//==============================================================================================================================
// ウィンドウの設定
//==============================================================================================================================
void DeleteMessageWndow(void)
{
	for (int nCnt = 0; nCnt < MAX_MESSAGEWNDOW; nCnt++)
	{
		if (g_aMessageWindow[nCnt].bUse == true)
		{
			g_aMessageWindow[nCnt].fDiffWidth = 0;		// 幅
			g_aMessageWindow[nCnt].fDiffHeight = 0;	// 高さ
			g_aMessageWindow[nCnt].nState = MESSAGESTATE_DELETE;
		}
	}
}
//==============================================================================================================================
// メッセージウィンドウの獲得
//==============================================================================================================================
MessageWindow *GetMessageWindow(void)
{
	return &g_aMessageWindow[0];
}