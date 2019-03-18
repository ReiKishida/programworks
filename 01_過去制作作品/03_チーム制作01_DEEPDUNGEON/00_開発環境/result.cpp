//=============================================================================
//
// 情報画面処理 [Result.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "bg.h"
#include "sound.h"
#include "inputx.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TYPE_RESULT (6)
#define MAX_RESULT (3)

#define RESULT_TEXTURENAME0	"DATA/TEXTURE/0true.png"	//タイトルのテクスチャ名
#define RESULT_TEXTURENAME1	"DATA/TEXTURE/25.png"	//タイトルのテクスチャ名
#define RESULT_TEXTURENAME2	"DATA/TEXTURE/50.png"	//タイトルのテクスチャ名
#define RESULT_TEXTURENAME3	"DATA/TEXTURE/75.png"	//タイトルのテクスチャ名

//#define RESULT_TEXTURENAME0	"DATA/TEXTURE/tuto.png"	//タイトルのテクスチャ名
//#define RESULT_TEXTURENAME1	"DATA/TEXTURE/tuto.png"	//タイトルのテクスチャ名
//#define RESULT_TEXTURENAME2	"DATA/TEXTURE/tuto.png"	//タイトルのテクスチャ名
//#define RESULT_TEXTURENAME3	"DATA/TEXTURE/tuto.png"	//タイトルのテクスチャ名



#define RESULT_TEXTURENAME98	"DATA/TEXTURE/page.png"	//タイトルのテクスチャ名
#define RESULT_TEXTURENAME99	"DATA/TEXTURE/start.png"	//タイトルのテクスチャ名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_TYPE_RESULT] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//頂点バッファへのポインタ

RESULT g_aResult[MAX_RESULT];

//int 	g_nSelectPase;
int nResultAnimation_X;
int nRResulteturnCount;

int g_nCntResultPase;
JoyState g_JoyStateResult;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitResult(void)
{




	g_nCntResultPase = 0;

	LPDIRECT3DDEVICE9 pDevice;


	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//g_aResult[nCntResult].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT /2, 0.0f);
		g_aResult[nCntResult].state = RESULTSTATE_NONE;
		g_aResult[nCntResult].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aResult[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	nResultAnimation_X = 0;
	nRResulteturnCount = 0;

	g_aResult[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aResult[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 2 + 350, 0.0f);
	g_aResult[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 2 + 300, 0.0f);



	g_aResult[0].bUse = true;
	g_aResult[1].bUse = true;
	g_aResult[2].bUse = false;

	/*g_nSelectPase = PASE_ONE;*/



	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME0, &g_pTextureResult[0]);//tru
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME1, &g_pTextureResult[1]);//25
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME2, &g_pTextureResult[2]);//50
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME3, &g_pTextureResult[3]);//75

	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME98, &g_pTextureResult[4]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME99, &g_pTextureResult[5]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aResult[0].pos.x - SCREEN_WIDTH / 2, g_aResult[0].pos.y - SCREEN_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[0].pos.x + SCREEN_WIDTH / 2, g_aResult[0].pos.y - SCREEN_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[0].pos.x - SCREEN_WIDTH / 2, g_aResult[0].pos.y + SCREEN_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[0].pos.x + SCREEN_WIDTH / 2, g_aResult[0].pos.y + SCREEN_HEIGHT / 2, 0.0f);


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
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	pVtx += 4;

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aResult[1].pos.x - 200, g_aResult[1].pos.y - 20, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[1].pos.x + 200, g_aResult[1].pos.y - 20, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[1].pos.x - 200, g_aResult[1].pos.y + 20, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[1].pos.x + 200, g_aResult[1].pos.y + 20, 0.0f);


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

	pVtx += 4;


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aResult[2].pos.x - 200, g_aResult[2].pos.y - 20, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[2].pos.x + 200, g_aResult[2].pos.y - 20, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[2].pos.x - 200, g_aResult[2].pos.y + 20, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[2].pos.x + 200, g_aResult[2].pos.y + 20, 0.0f);

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
	//頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();

	//ゲームパッドの状態
	g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitResult(void)
{

	for (int nCntResult = 0; nCntResult < MAX_TYPE_RESULT; nCntResult++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateResult(void)
{

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();
	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);
	FADE pFade;
	pFade = GetFade();

	VERTEX_2D*pVtx;	//頂点情報へのポインタ



	

	if (GetKeyboardTrigger(DIK_D) == true || GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_RIGHTARROW) == true
		|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_RIGHT && pJoyState.bConnectionJoypad == true))
	{
		if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			//g_nSelectPase = (g_nSelectPase + (PASE_MAX - 1)) % PASE_MAX;//選択状態を一つ上げる
			PlaySound(SOUND_LABEL_SE_MEKURI);
			nResultAnimation_X = (nResultAnimation_X + 1);
			if (nResultAnimation_X > 3)
			{
				nResultAnimation_X = 3;
			}
		}
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;

	}


	if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_LEFTARROW) == true
		|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_LEFT && pJoyState.bConnectionJoypad == true))
	{

		if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{


			PlaySound(SOUND_LABEL_SE_MEKURI);

			nResultAnimation_X = (nResultAnimation_X - 1);
			if (nResultAnimation_X < 0)
			{
				nResultAnimation_X = 0;
			}
		}
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;
	}


	if (nResultAnimation_X == 3)
	{
		g_aResult[0].bUse = true;
		g_aResult[1].bUse = false;
		g_aResult[2].bUse = true;
	}
	if (nResultAnimation_X != 3)
	{
		g_aResult[0].bUse = true;
		g_aResult[1].bUse = true;
		g_aResult[2].bUse = false;
	}





	if (nResultAnimation_X == 3)
	{
		if (pFade == FADE_NONE)
		{

			if (GetKeyboardTrigger(DIK_SPACE) == true
				|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true))
			{
				if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
				{

					PlaySound(SOUND_LABEL_SE_DECISION);

					//モード選択
					SetFade(MODE_TITLE);
				}
				g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;
			}
		}
	}



	if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}





	if (g_aResult[1].bUse == true)
	{
		switch (g_aResult[1].state)
		{
		case RESULTSTATE_NONE:
			g_aResult[1].col.a -= 0.02f;
			if (g_aResult[1].col.a <= 0.0f)
			{
				g_aResult[1].col.a = 0.0f;
				g_aResult[1].state = RESULTSTATE_USE;
			}
			break;

		case RESULTSTATE_USE:
			g_aResult[1].col.a += 0.02f;
			if (g_aResult[1].col.a >= 1.0f)
			{
				g_aResult[1].col.a = 1.0f;
				g_aResult[1].state = RESULTSTATE_NONE;
			}
			break;
		}
	}


	if (g_aResult[2].bUse == true)
	{
		switch (g_aResult[2].state)
		{
		case RESULTSTATE_NONE:
			g_aResult[2].col.a -= 0.01f;
			if (g_aResult[2].col.a <= 0.0f)
			{
				g_aResult[2].col.a = 0.0f;
				g_aResult[2].state = RESULTSTATE_USE;
			}
			break;

		case RESULTSTATE_USE:
			g_aResult[2].col.a += 0.01f;
			if (g_aResult[2].col.a >= 1.0f)
			{
				g_aResult[2].col.a = 1.0f;
				g_aResult[2].state = RESULTSTATE_NONE;
			}
			break;
		}
	}


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);



	if (nResultAnimation_X < 4)
	{
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.25f * nResultAnimation_X, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f* (nResultAnimation_X + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.25f* nResultAnimation_X, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f* (nResultAnimation_X + 1), 1.0f);
	}

	pVtx += 4;

	//頂点カラー
	pVtx[0].col = g_aResult[1].col;
	pVtx[1].col = g_aResult[1].col;
	pVtx[2].col = g_aResult[1].col;
	pVtx[3].col = g_aResult[1].col;


	pVtx += 4;

	//頂点カラー
	pVtx[0].col = g_aResult[2].col;
	pVtx[1].col = g_aResult[2].col;
	pVtx[2].col = g_aResult[2].col;
	pVtx[3].col = g_aResult[2].col;

	//頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawResult(void)
{



	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	GAMESTATE pGame;
	pGame = *GetGameState2();


								//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aResult[2].bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[5]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

	}
	if (g_aResult[1].bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[4]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	}
		if (g_aResult[0].bUse == true)
		{
			
			if (pGame == GAMESTATE_CLEAR_TRUE)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureResult[0]);
			}
			else if (pGame == GAMESTATE_CLEAR_25)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureResult[1]);
			}
			else if (pGame == GAMESTATE_CLEAR_50)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureResult[2]);
			}
			else if (pGame == GAMESTATE_CLEAR_75)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureResult[3]);
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		}
	
}
