//===========================================================================
//
// メッセージ処理 [Message.cpp]
// Author : TEAM_Fascinator
//
//===========================================================================
#include "main.h"
#include "message.h"
#include "input.h"
#include "inputx.h"
#include "fade.h"
#include "sound.h"

//===========================================================================
// マクロ定義
//===========================================================================
#define MAX_TYPE (5)
#define MESSAGE_TEXTURE_NAME0		"data/TEXTURE/message01.png"	// テクスチャ
#define MESSAGE_TEXTURE_NAME1		"data/TEXTURE/message02.png"	// テクスチャ
#define MESSAGE_TEXTURE_NAME2		"data/TEXTURE/message03.png"	// テクスチャ

#define MESSAGE_SIZE_X				(200.0f)						// 左上X座標
#define MESSAGE_SIZE_Y				(50.0f)						// 左上Y座標
#define MESSAGE_WIDTH				(SCREEN_WIDTH)				// 幅
#define MESSAGE_HEIGHT			(SCREEN_HEIGHT)				// 高さ
#define MESSAGE_TEXTURE_UV_U		(1.0f)						// アニメーションU範囲
#define MESSAGE_TEXTURE_UV_V		(1.0f)						// アニメーションV範囲
#define MESSAGE_SPEED				(0.003f)					// スピード

//===========================================================================
// グローバル変数
//===========================================================================
LPDIRECT3DTEXTURE9 g_pTextureMessage[MAX_TYPE] = {};			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMessage = NULL;		// 頂点バッファへのポインタ
JoyState	g_JoyStateMessage;							// ジョイパッドの状態
MESSAGE g_Message[MAX_TYPE];
//===========================================================================
// 初期化処理
//===========================================================================
void InitMessage(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntMeaage = 0; nCntMeaage < MAX_TYPE; nCntMeaage++)
	{
		g_Message[nCntMeaage].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 300.0f, 0.0f);	// 中心値
		g_Message[nCntMeaage].Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_Message[nCntMeaage].type = MESSAGETYPE_APPER;
		g_Message[nCntMeaage].bUse = false;
	}


	g_JoyStateMessage.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, MESSAGE_TEXTURE_NAME0, &g_pTextureMessage[0]);
	D3DXCreateTextureFromFile(pDevice, MESSAGE_TEXTURE_NAME1, &g_pTextureMessage[1]);
	D3DXCreateTextureFromFile(pDevice, MESSAGE_TEXTURE_NAME2, &g_pTextureMessage[2]);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMessage,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	for (int nCntMeaage = 0; nCntMeaage < MAX_TYPE; nCntMeaage++)
	{
		// 頂点バッファをロック
		g_pVtxBuffMessage->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点設定
		pVtx[0].pos = D3DXVECTOR3(-MESSAGE_SIZE_X, -MESSAGE_SIZE_Y, 0.0f) + g_Message[0].pos;
		pVtx[1].pos = D3DXVECTOR3(MESSAGE_SIZE_X, -MESSAGE_SIZE_Y, 0.0f) + g_Message[0].pos;
		pVtx[2].pos = D3DXVECTOR3(-MESSAGE_SIZE_X, MESSAGE_SIZE_Y, 0.0f) + g_Message[0].pos;
		pVtx[3].pos = D3DXVECTOR3(MESSAGE_SIZE_X, MESSAGE_SIZE_Y, 0.0f) + g_Message[0].pos;

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー設定
		pVtx[0].col = g_Message[0].Col;
		pVtx[1].col = g_Message[0].Col;
		pVtx[2].col = g_Message[0].Col;
		pVtx[3].col = g_Message[0].Col;

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(MESSAGE_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, MESSAGE_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(MESSAGE_TEXTURE_UV_U, MESSAGE_TEXTURE_UV_V);

		pVtx += 4;
		// 頂点バッファをアンロック
		g_pVtxBuffMessage->Unlock();
	}
}

//===========================================================================
// 終了処理
//===========================================================================
void UninitMessage(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TYPE; nCntTex++)
	{
		// テクスチャ破棄
		if (g_pTextureMessage[nCntTex] != NULL)
		{
			g_pTextureMessage[nCntTex]->Release();
			g_pTextureMessage[nCntTex] = NULL;
		}
	}
	// 頂点バッファ破棄
	if (g_pVtxBuffMessage != NULL)
	{
		g_pVtxBuffMessage->Release();
		g_pVtxBuffMessage = NULL;
	}
}

//===========================================================================
// 更新処理
//===========================================================================
void UpdateMessage(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	for (int nCntMessage = 0; nCntMessage < 2; nCntMessage++)
	{
		if (g_Message[nCntMessage].bUse == true)
		{
			

			switch (g_Message[nCntMessage].type)
			{
			case MESSAGETYPE_APPER:
				g_Message[nCntMessage].Col.a += 0.05f;
				if (g_Message[nCntMessage].Col.a >= 1.0f)
				{
					g_Message[nCntMessage].Col.a = 1.0f;
					g_Message[nCntMessage].type = MESSAGETYPE_NORMAL;
				}

				
			case MESSAGETYPE_NORMAL:

				break;
			}
		}
	}
	// 頂点バッファをロック
	g_pVtxBuffMessage->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntMessage = 0; nCntMessage < 2; nCntMessage++)
	{
		// 頂点カラー設定
		pVtx[0].col = g_Message[nCntMessage].Col;
		pVtx[1].col = g_Message[nCntMessage].Col;
		pVtx[2].col = g_Message[nCntMessage].Col;
		pVtx[3].col = g_Message[nCntMessage].Col;

		pVtx += 4;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffMessage->Unlock();

}

//===========================================================================
// 描画処理
//===========================================================================
void DrawMessage(void)
{
	for (int nCntMessage = 0; nCntMessage < MAX_TYPE; nCntMessage++)
	{

		if (g_Message[nCntMessage].bUse == true)
		{

			LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

														// 頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMessage, 0, sizeof(VERTEX_2D));

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_2D);

			

			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureMessage[g_Message[nCntMessage].nType]);

			// 描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMessage * 4, 2);
		}
	}
}

void DeleteMessage(void)
{
	for (int nCntMessage = 0; nCntMessage < 2; nCntMessage++)
	{
		if (g_Message[nCntMessage].bUse == true)
		{
			g_Message[nCntMessage].Col.a -= 0.05f;
			if (g_Message[nCntMessage].Col.a <= 0.0f)
			{
				g_Message[nCntMessage].Col.a = 0.0f;
				g_Message[nCntMessage].bUse = false;
			}
		}
	}
}

MESSAGE *GetMessageBox(void)
{
	return &g_Message[0];
}

void SetMessageBox(D3DXVECTOR3 pos, int ntype)
{
	int nCntMessage;
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMessage = 0; nCntMessage < MAX_TYPE; nCntMessage++)
	{
		if (g_Message[nCntMessage].bUse == false)	
		{
			g_Message[nCntMessage].pos = pos;

			g_Message[nCntMessage].type = MESSAGETYPE_APPER;


			g_Message[nCntMessage].nType = ntype;
			

			g_Message[nCntMessage].bUse = true;	//使用している状態にする

			break;
		}
		pVtx += 4;


	}
	//頂点バッファをアンロック
	g_pVtxBuffMessage->Unlock();
}
