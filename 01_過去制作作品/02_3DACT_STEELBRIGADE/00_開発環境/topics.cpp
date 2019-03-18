//=============================================================================
//
// トピック処理 [Topics.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "topics.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "cursor.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_TOPICS		(6)								// トピックの種類
#define TOPICS_TEXTURENAME_0	"data/TEXTURE/mail.png"		//トピックのテクスチャ1
#define TOPICS_TEXTURENAME_1	"data/TEXTURE/news.png"		//トピックのテクスチャ2
#define TOPICS_TEXTURENAME_2	"data/TEXTURE/circle.png"		//トピックのテクスチャ2
#define TOPICS_TEXTURENAME_3	"data/TEXTURE/mail_01.png"		//トピックのテクスチャ2
#define TOPICS_TEXTURENAME_4	"data/TEXTURE/mail_flame2.png"		//トピックのテクスチャ2
#define TOPICS_TEXTURENAME_5	"data/TEXTURE/mail_flame3.png"		//トピックのテクスチャ2




//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureTopics[MAX_TYPE_TOPICS] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTopics = NULL;					// 頂点バッファへのポインタ
TOPICS g_aTopics[MAX_TOPICS];	//トピック
JoyState g_JoyStateTopics;



//=============================================================================
// 初期化処理
//=============================================================================
void InitTopics(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTopics;	//トピックのカウント

	// デバイスの取得
	pDevice = GetDevice();

	//ゲームパッドの状態
	g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


	
	// トピックの情報の初期化
	for(int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		g_aTopics[nCntTopics].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);	
		g_aTopics[nCntTopics].nType = 0;
		g_aTopics[nCntTopics].bUse = false;
		g_aTopics[nCntTopics].fHeight = 0;
		g_aTopics[nCntTopics].fWidth = 0;
		g_aTopics[nCntTopics].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTopics[nCntTopics].state = MAILSTATE_UNREAD;
		g_aTopics[nCntTopics].FallenPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	}

	

	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_0, &g_apTextureTopics[0]); //空
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_1, &g_apTextureTopics[1]); //メール１
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_2, &g_apTextureTopics[2]); //メール2
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_3, &g_apTextureTopics[3]); //メール2
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_4, &g_apTextureTopics[4]); //メール2
	D3DXCreateTextureFromFile(pDevice, TOPICS_TEXTURENAME_5, &g_apTextureTopics[5]); //メール2


	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TOPICS,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffTopics,
									NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTopics->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
		
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
				pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		
			pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTopics->Unlock();


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitTopics(void)
{
	
	int nCntTex;

	// テクスチャの開放
	for(nCntTex = 0; nCntTex < MAX_TYPE_TOPICS; nCntTex++)
	{
			//テクスチャの破棄
			if (g_apTextureTopics[nCntTex] != NULL)
			{
				g_apTextureTopics[nCntTex]->Release();
				g_apTextureTopics[nCntTex] = NULL;
			}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffTopics != NULL)
	{
		g_pVtxBuffTopics->Release();
		g_pVtxBuffTopics = NULL;
	}

	

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTopics(void)
{
	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	int nCntTex = 0;

	CURSOR pCursor;
	pCursor = GetCursor000();

	if (pCursor.bUpdate == false && GetKeyboardTrigger(DIK_RETURN) == TRUE && g_aTopics[4].bUse == true
		||(state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true && g_aTopics[4].bUse == true && pCursor.bUpdate == true ))
	{
		if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pCursor.bUpdate = true;
			g_aTopics[4].bUse = false;
			//g_aTopics[4].state = MAILSTATE_ALREADY;
		}
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	if (pCursor.bUpdate == false && GetKeyboardTrigger(DIK_RETURN) == TRUE && g_aTopics[5].bUse == true
		|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true && g_aTopics[5].bUse == true && pCursor.bUpdate == true))
	{
		if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pCursor.bUpdate = true;
			g_aTopics[5].bUse = false;
			//g_aTopics[5].state = MAILSTATE_ALREADY;
		}
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	if (pCursor.bUpdate == false && GetKeyboardTrigger(DIK_RETURN) == TRUE && g_aTopics[6].bUse == true
		|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true && g_aTopics[6].bUse == true && pCursor.bUpdate == true))
	{
		if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pCursor.bUpdate = true;
			g_aTopics[6].bUse = false;
			//g_aTopics[6].state = MAILSTATE_ALREADY;
		}
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTopics->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x, g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);

		if (g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_0_1 || g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_1_1 || g_aTopics[nCntTopics].type == TOPICSTYPE_NEWS_0_1)
		{
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		else
		{
			if (g_aTopics[nCntTopics].state == MAILSTATE_UNREAD)
			{
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
			}	

		}

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTopics->Unlock();

	if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateTopics.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateTopics.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTopics(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画



	for (int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
		if (g_aTopics[nCntTopics].bUse == true)
		{
	
			// デバイスの取得
			pDevice = GetDevice();

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffTopics, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

	
		
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTopics[g_aTopics[nCntTopics].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTopics * 4, 2);
		}
	}
}

void DeleteTopics(int nIndexPolygon)
{
	g_aTopics[nIndexPolygon].bUse = false;
}





//=============================================================================
// トピックの設定
//=============================================================================
void SetTopics(D3DXVECTOR3 pos, TOPICSTYPE Type ,int nType, float fHeight, float fWidth)
{
	int nCntTopics;
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTopics->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{
			if (g_aTopics[nCntTopics].bUse == false)	//トピックが使用されていない
			{
				g_aTopics[nCntTopics].pos = pos;
			
				g_aTopics[nCntTopics].state = MAILSTATE_UNREAD;

					g_aTopics[nCntTopics].fHeight = fHeight;
					g_aTopics[nCntTopics].fWidth = fHeight;
			

					if (Type == TOPICSTYPE_MAIL_0_1 || Type == TOPICSTYPE_MAIL_1_1 || Type == TOPICSTYPE_NEWS_0_1)
					{
						//テクスチャ座標
						pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
					}

					else
					{
						//テクスチャ座標
						pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
					}


					g_aTopics[nCntTopics].nType = nType;
					g_aTopics[nCntTopics].type = Type;

				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y , 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x , g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth, g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight, 0.0f);

	
				g_aTopics[nCntTopics].bUse = true;	//使用している状態にする

				break;
			}
			pVtx +=  4;


		}
		//頂点バッファをアンロック
	g_pVtxBuffTopics->Unlock();
}

//=============================================================================
// トピックの当たり判定
//=============================================================================
TOPICSTYPE CollisionTopics(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosold)
{
	TOPICSTYPE Type = TOPICSTYPE_NONE;


	for (int nCntTopics = 0; nCntTopics < MAX_TOPICS; nCntTopics++)
	{


		if (g_aTopics[nCntTopics].bUse == true)
		{

			if (pPos->x >= g_aTopics[nCntTopics].pos.x - g_aTopics[nCntTopics].fWidth
				&& pPos->x <= g_aTopics[nCntTopics].pos.x + g_aTopics[nCntTopics].fWidth
				&&pPos->y >= g_aTopics[nCntTopics].pos.y - g_aTopics[nCntTopics].fHeight
				&& pPos->y <= g_aTopics[nCntTopics].pos.y + g_aTopics[nCntTopics].fHeight
				)
			{

				if (g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_0)
				{
					Type = TOPICSTYPE_MAIL_0;
				}
				if (g_aTopics[nCntTopics].type == TOPICSTYPE_MAIL_1)
				{
					Type = TOPICSTYPE_MAIL_1;
				}
				if (g_aTopics[nCntTopics].type == TOPICSTYPE_NEWS_0)
				{
					Type = TOPICSTYPE_NEWS_0;
				}
				if (g_aTopics[nCntTopics].type == TOPICSTYPE_MISSION_0)
				{
					Type = TOPICSTYPE_MISSION_0;
				}
			}

		}
	}
	return Type;
}


//=============================================================================
// トピックの取得
//=============================================================================
TOPICS *GetTopics(void)
{
	return &g_aTopics[0];
}
