//=============================================================================
//
// 情報画面処理 [Tuto.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "prologue.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "bg.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TYPE_PROLOGUE (3)
#define MAX_PROLOGUE (3)

#define PROLOGUE_TEXTURENAME	"DATA/TEXTURE/prologue.png"	//タイトルのテクスチャ名
#define PROLOGUE_TEXTURENAME_2	"DATA/TEXTURE/page.png"	//タイトルのテクスチャ名
#define PROLOGUE_TEXTURENAME_3	"DATA/TEXTURE/start.png"	//タイトルのテクスチャ名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePrologue[MAX_TYPE_PROLOGUE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPrologue = NULL;	//頂点バッファへのポインタ

PROLOGUE g_aPrologue[MAX_PROLOGUE];

//int 	g_nSelectPase;
int nPrologueAnimation_X;
int nPrologueReturnCount;

int g_nCntProloguePage;

JoyState g_JoyStatePrologue;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitPrologue(void)
{
	g_nCntProloguePage = 0;

	LPDIRECT3DDEVICE9 pDevice;


	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntPrologue = 0; nCntPrologue < MAX_PROLOGUE; nCntPrologue++)
	{
		g_aPrologue[nCntPrologue].state = PROLOGUESTATE_NONE;
		g_aPrologue[nCntPrologue].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPrologue[nCntPrologue].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	nPrologueAnimation_X = 0;
	nPrologueReturnCount = 0;

	g_aPrologue[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aPrologue[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT - 70, 0.0f);
	g_aPrologue[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 330, SCREEN_HEIGHT - 70, 0.0f);



	g_aPrologue[0].bUse = true;
	g_aPrologue[1].bUse = true;
	g_aPrologue[2].bUse = false;

	/*g_nSelectPase = PASE_ONE;*/



	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PROLOGUE_TEXTURENAME, &g_pTexturePrologue[0]);
	D3DXCreateTextureFromFile(pDevice, PROLOGUE_TEXTURENAME_2, &g_pTexturePrologue[1]);
	D3DXCreateTextureFromFile(pDevice, PROLOGUE_TEXTURENAME_3, &g_pTexturePrologue[2]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PROLOGUE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPrologue,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPrologue->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aPrologue[0].pos.x - (SCREEN_WIDTH / 2 ), g_aPrologue[0].pos.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPrologue[0].pos.x + (SCREEN_WIDTH / 2 ), g_aPrologue[0].pos.y - (SCREEN_HEIGHT / 2 ), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPrologue[0].pos.x - (SCREEN_WIDTH / 2), g_aPrologue[0].pos.y + (SCREEN_HEIGHT / 2 ), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPrologue[0].pos.x + (SCREEN_WIDTH / 2), g_aPrologue[0].pos.y + (SCREEN_HEIGHT / 2 ), 0.0f);


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

	pVtx += 4;

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aPrologue[1].pos.x - 300, g_aPrologue[1].pos.y - 50, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPrologue[1].pos.x + 300, g_aPrologue[1].pos.y - 50, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPrologue[1].pos.x - 300, g_aPrologue[1].pos.y + 50, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPrologue[1].pos.x + 300, g_aPrologue[1].pos.y + 50, 0.0f);


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
	pVtx[0].pos = D3DXVECTOR3(g_aPrologue[2].pos.x - 300, g_aPrologue[2].pos.y - 70, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPrologue[2].pos.x + 300, g_aPrologue[2].pos.y - 70, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPrologue[2].pos.x - 300, g_aPrologue[2].pos.y + 70, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPrologue[2].pos.x + 300, g_aPrologue[2].pos.y + 70, 0.0f);

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
	g_pVtxBuffPrologue->Unlock();

	//ゲームパッドの状態
	g_JoyStatePrologue.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitPrologue(void)
{

	for (int nCntPrologue = 0; nCntPrologue < MAX_TYPE_PROLOGUE; nCntPrologue++)
	{
		//テクスチャの破棄
		if (g_pTexturePrologue[nCntPrologue] != NULL)
		{
			g_pTexturePrologue[nCntPrologue]->Release();
			g_pTexturePrologue[nCntPrologue] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPrologue != NULL)
	{
		g_pVtxBuffPrologue->Release();
		g_pVtxBuffPrologue = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdatePrologue(void)
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
		if (g_JoyStatePrologue.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			//g_nSelectPase = (g_nSelectPase + (PASE_MAX - 1)) % PASE_MAX;//選択状態を一つ上げる
			PlaySound(SOUND_LABEL_SE_MEKURI);
			nPrologueAnimation_X = (nPrologueAnimation_X + 1);
			if (nPrologueAnimation_X > 3)
			{
				nPrologueAnimation_X = 3;
			}
		}
		g_JoyStatePrologue.nJoypadState = INPUT_JOYSTATE_PUSH;
	}


	if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_LEFTARROW) == true
		|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_LEFT && pJoyState.bConnectionJoypad == true))
	{
		if (g_JoyStatePrologue.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{


			//g_nSelectPase = (g_nSelectPase + 1) % PASE_MAX;//選択状態を一つ下げる

			PlaySound(SOUND_LABEL_SE_MEKURI);

			nPrologueAnimation_X = (nPrologueAnimation_X - 1);
			if (nPrologueAnimation_X < 0)
			{
				nPrologueAnimation_X = 0;
			}
		}
		g_JoyStatePrologue.nJoypadState = INPUT_JOYSTATE_PUSH;
	}



	if (nPrologueAnimation_X == 3)
	{
		g_aPrologue[0].bUse = true;
		g_aPrologue[1].bUse = false;
		g_aPrologue[2].bUse = true;
	}
	if (nPrologueAnimation_X != 3)
	{
		g_aPrologue[0].bUse = true;
		g_aPrologue[1].bUse = true;
		g_aPrologue[2].bUse = false;
	}





	if (nPrologueAnimation_X == 3)
	{
		if (pFade == FADE_NONE)
		{

			if (GetKeyboardTrigger(DIK_SPACE) == true
				|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true))
			{
				if (g_JoyStatePrologue.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
				{
				PlaySound(SOUND_LABEL_SE_DECISION);

				//モード選択
				//SetMode(MODE_TUTO);
				SetFade(MODE_TITLE);
				}
				g_JoyStatePrologue.nJoypadState = INPUT_JOYSTATE_PUSH;
			}
		}
	}





	if (g_JoyStatePrologue.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStatePrologue.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStatePrologue.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStatePrologue.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}


	if (g_aPrologue[1].bUse == true)
	{
		switch (g_aPrologue[1].state)
		{
		case PROLOGUESTATE_NONE:
			g_aPrologue[1].col.a -= 0.02f;
			if (g_aPrologue[1].col.a <= 0.0f)
			{
				g_aPrologue[1].col.a = 0.0f;
				g_aPrologue[1].state = PROLOGUESTATE_USE;
			}
			break;

		case PROLOGUESTATE_USE:
			g_aPrologue[1].col.a += 0.02f;
			if (g_aPrologue[1].col.a >= 1.0f)
			{
				g_aPrologue[1].col.a = 1.0f;
				g_aPrologue[1].state = PROLOGUESTATE_NONE;
			}
			break;
		}
	}


	if (g_aPrologue[2].bUse == true)
	{
		switch (g_aPrologue[2].state)
		{
		case PROLOGUESTATE_NONE:
			g_aPrologue[2].col.a -= 0.01f;
			if (g_aPrologue[2].col.a <= 0.0f)
			{
				g_aPrologue[2].col.a = 0.0f;
				g_aPrologue[2].state = PROLOGUESTATE_USE;
			}
			break;

		case PROLOGUESTATE_USE:
			g_aPrologue[2].col.a += 0.01f;
			if (g_aPrologue[2].col.a >= 1.0f)
			{
				g_aPrologue[2].col.a = 1.0f;
				g_aPrologue[2].state = PROLOGUESTATE_NONE;
			}
			break;
		}
	}


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPrologue->Lock(0, 0, (void**)&pVtx, 0);



	if (nPrologueAnimation_X < 4)
	{
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.25f * nPrologueAnimation_X, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f* (nPrologueAnimation_X + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.25f* nPrologueAnimation_X, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f* (nPrologueAnimation_X + 1), 1.0f);
	}

	pVtx += 4;

	//頂点カラー
	pVtx[0].col = g_aPrologue[1].col;
	pVtx[1].col = g_aPrologue[1].col;
	pVtx[2].col = g_aPrologue[1].col;
	pVtx[3].col = g_aPrologue[1].col;


	pVtx += 4;

	//頂点カラー
	pVtx[0].col = g_aPrologue[2].col;
	pVtx[1].col = g_aPrologue[2].col;
	pVtx[2].col = g_aPrologue[2].col;
	pVtx[3].col = g_aPrologue[2].col;

	//頂点バッファをアンロック
	g_pVtxBuffPrologue->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawPrologue(void)
{
	

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPrologue, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPrologue = 0; nCntPrologue < MAX_TYPE_PROLOGUE; nCntPrologue++)
	{
		if (g_aPrologue[nCntPrologue].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePrologue[nCntPrologue]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPrologue * 4, 2);

		}
	}
}
