//=============================================================================
//
// 情報画面処理 [char_select.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "main.h"
#include "char_select.h"
#include "fade.h"
#include "input.h"
#include "inputx.h"
#include "bg.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SELECT_TEXTURENAME_1	"DATA/TEXTURE/select.png"		//キャラ選択の画像
#define SELECT_TEXTURENAME_2	"DATA/TEXTURE/s.jpeg"			//キャラ選択の背景
#define SELECT_TEXTURENAME_3	"DATA/TEXTURE/player100.png"	//キャラ名(ソルジャー)
#define SELECT_TEXTURENAME_4	"DATA/TEXTURE/player101.png"	//キャラ名(ウォリアー)
#define SELECT_TEXTURENAME_5	"DATA/TEXTURE/player102.png"	//キャラ名(ソーサラー)
#define MAX_TYPE_SELECT			(5)								//使うテクスチャの数
#define MAX_SELECT				(5)								//表示するテクスチャの数
#define WIDTH					(1000)							//幅
#define HEIGHT					(670)							//高さ
#define CHARACTER_WIDTH			(130)							//キャラの幅
#define CHARACTER_HEIGHT		(35)							//キャラの高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCharSelect[MAX_TYPE_SELECT] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCharSelect = NULL;				//頂点バッファへのポインタ
CHARACTER_TYPE g_CharaType = CHARACTER_TYPE_SORCERER;				//初期キャラタイプ
SELECT g_aCharSelect[MAX_SELECT];									//セレクトの状態
JoyState g_JoyStateCharSelect;										//ジョイパッドの状態
int g_nCharSelectPage;												//選択番号

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitCharSelect(void)
{
	InitBG();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	for (int nCntCharSelect = 0; nCntCharSelect < MAX_SELECT; nCntCharSelect++)
	{
		g_aCharSelect[nCntCharSelect].state = SELECTSTATE_NONE;
		g_aCharSelect[nCntCharSelect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCharSelect[nCntCharSelect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	g_JoyStateCharSelect.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	g_aCharSelect[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 125, SCREEN_HEIGHT / 2, 0.0f);
	g_aCharSelect[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 510, 400, 0.0f);
	g_aCharSelect[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 520, 300, 0.0f);
	g_aCharSelect[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 520, 400, 0.0f);
	g_aCharSelect[4].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 520, 500, 0.0f);

	g_aCharSelect[0].bUse = true;
	g_aCharSelect[1].bUse = true;
	g_aCharSelect[2].bUse = true;
	g_aCharSelect[3].bUse = true;
	g_aCharSelect[4].bUse = true;

	g_nCharSelectPage = PAGE_ONE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SELECT_TEXTURENAME_1, &g_pTextureCharSelect[0]);
	D3DXCreateTextureFromFile(pDevice, SELECT_TEXTURENAME_2, &g_pTextureCharSelect[1]);
	D3DXCreateTextureFromFile(pDevice, SELECT_TEXTURENAME_3, &g_pTextureCharSelect[2]);
	D3DXCreateTextureFromFile(pDevice, SELECT_TEXTURENAME_4, &g_pTextureCharSelect[3]);
	D3DXCreateTextureFromFile(pDevice, SELECT_TEXTURENAME_5, &g_pTextureCharSelect[4]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SELECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCharSelect,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCharSelect->Lock(0, 0, (void**)&pVtx, 0);

	//キャラ詳細画像===================================================================================================
	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aCharSelect[0].pos.x - WIDTH / 2, g_aCharSelect[0].pos.y - HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharSelect[0].pos.x + WIDTH / 2, g_aCharSelect[0].pos.y - HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharSelect[0].pos.x - WIDTH / 2, g_aCharSelect[0].pos.y + HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharSelect[0].pos.x + WIDTH / 2, g_aCharSelect[0].pos.y + HEIGHT / 2, 0.0f);

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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.33f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.33f);

	pVtx += 4;

	//背景=============================================================================================================
	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aCharSelect[1].pos.x - 120, g_aCharSelect[1].pos.y - 180, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharSelect[1].pos.x + 120, g_aCharSelect[1].pos.y - 180, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharSelect[1].pos.x - 120, g_aCharSelect[1].pos.y + 180, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharSelect[1].pos.x + 120, g_aCharSelect[1].pos.y + 180, 0.0f);

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

	//キャラ１=========================================================================================================
	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aCharSelect[2].pos.x - CHARACTER_WIDTH, g_aCharSelect[2].pos.y - CHARACTER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharSelect[2].pos.x + CHARACTER_WIDTH, g_aCharSelect[2].pos.y - CHARACTER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharSelect[2].pos.x - CHARACTER_WIDTH, g_aCharSelect[2].pos.y + CHARACTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharSelect[2].pos.x + CHARACTER_WIDTH, g_aCharSelect[2].pos.y + CHARACTER_HEIGHT, 0.0f);

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

	//キャラ２=========================================================================================================
	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aCharSelect[3].pos.x - CHARACTER_WIDTH, g_aCharSelect[3].pos.y - CHARACTER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharSelect[3].pos.x + CHARACTER_WIDTH, g_aCharSelect[3].pos.y - CHARACTER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharSelect[3].pos.x - CHARACTER_WIDTH, g_aCharSelect[3].pos.y + CHARACTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharSelect[3].pos.x + CHARACTER_WIDTH, g_aCharSelect[3].pos.y + CHARACTER_HEIGHT, 0.0f);

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

	//キャラ３=========================================================================================================
	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aCharSelect[4].pos.x - CHARACTER_WIDTH, g_aCharSelect[4].pos.y - CHARACTER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharSelect[4].pos.x + CHARACTER_WIDTH, g_aCharSelect[4].pos.y - CHARACTER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharSelect[4].pos.x - CHARACTER_WIDTH, g_aCharSelect[4].pos.y + CHARACTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharSelect[4].pos.x + CHARACTER_WIDTH, g_aCharSelect[4].pos.y + CHARACTER_HEIGHT, 0.0f);

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
	g_pVtxBuffCharSelect->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitCharSelect(void)
{
	UninitBG();
	for (int nCntCharSelect = 0; nCntCharSelect < MAX_TYPE_SELECT; nCntCharSelect++)
	{
		//テクスチャの破棄
		if (g_pTextureCharSelect[nCntCharSelect] != NULL)
		{
			g_pTextureCharSelect[nCntCharSelect]->Release();
			g_pTextureCharSelect[nCntCharSelect] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCharSelect != NULL)
	{
		g_pVtxBuffCharSelect->Release();
		g_pVtxBuffCharSelect = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateCharSelect(void)
{
	UpdateBG();
	JoyState pJoystate = GetJoystate();
	FADE pFade = GetFade();
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && pJoystate.bConnectionJoypad == true))
	{
		if (g_JoyStateCharSelect.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			PlaySound(SOUND_LABEL_SE_SELECT);

			g_nCharSelectPage = (g_nCharSelectPage + (PAGE_MAX - 1)) % PAGE_MAX;	//選択状態を一つ上げる		
		}
		g_JoyStateCharSelect.nJoypadState = INPUT_JOYSTATE_PUSH;
	}


	if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && pJoystate.bConnectionJoypad == true))
	{
		if (g_JoyStateCharSelect.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			PlaySound(SOUND_LABEL_SE_SELECT);

			g_nCharSelectPage = (g_nCharSelectPage + 1) % PAGE_MAX;	//選択状態を一つ下げる
		}
		g_JoyStateCharSelect.nJoypadState = INPUT_JOYSTATE_PUSH;
	}

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
		{
			if (g_JoyStateCharSelect.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				// 決定音
				PlaySound(SOUND_LABEL_SE_DECISION);

				if (g_nCharSelectPage == 0)
				{//キャラタイプ：ソルジャー
					g_CharaType = CHARACTER_TYPE_SOLDIER;
				}
				else if (g_nCharSelectPage == 1)
				{//キャラタイプ：ウォリアー
					g_CharaType = CHARACTER_TYPE_WARRIOR;
				}
				else if (g_nCharSelectPage == 2)
				{//キャラタイプ：ソーサラー
					g_CharaType = CHARACTER_TYPE_SORCERER;
				}

				SetFade(MODE_TUTO);
				//SetStageMode(STAGEMODE_STAGE1);
			}
			g_JoyStateCharSelect.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
	}

	switch (g_nCharSelectPage)
	{
	case PAGE_ONE:
		g_aCharSelect[2].select = true;
		g_aCharSelect[3].select = false;
		g_aCharSelect[4].select = false;

		switch (g_aCharSelect[2].state)
		{
		case SELECTSTATE_NONE:
			g_aCharSelect[2].col.a -= 0.02f;
			if (g_aCharSelect[2].col.a <= 0.0f)
			{//使ってないとき
				g_aCharSelect[2].col.a = 0.0f;
				g_aCharSelect[2].state = SELECTSTATE_USE;
			}
			break;
		case SELECTSTATE_USE:
			g_aCharSelect[2].col.a += 0.02f;
			if (g_aCharSelect[2].col.a >= 1.0f)
			{//使ってるとき
				g_aCharSelect[2].col.a = 1.0f;
				g_aCharSelect[2].state = SELECTSTATE_NONE;
			}
			break;
		}
		break;
	case PAGE_TWO:
		g_aCharSelect[2].select = false;
		g_aCharSelect[3].select = true;
		g_aCharSelect[4].select = false;
		switch (g_aCharSelect[3].state)
		{
		case SELECTSTATE_NONE:
			g_aCharSelect[3].col.a -= 0.01f;
			if (g_aCharSelect[3].col.a <= 0.0f)
			{
				g_aCharSelect[3].col.a = 0.0f;
				g_aCharSelect[3].state = SELECTSTATE_USE;
			}
			break;
		case SELECTSTATE_USE:
			g_aCharSelect[3].col.a += 0.01f;
			if (g_aCharSelect[3].col.a >= 1.0f)
			{
				g_aCharSelect[3].col.a = 1.0f;
				g_aCharSelect[3].state = SELECTSTATE_NONE;
			}
			break;
		}
		break;
	case PAGE_THREE:
		g_aCharSelect[2].select = false;
		g_aCharSelect[3].select = false;
		g_aCharSelect[4].select = true;
		switch (g_aCharSelect[4].state)
		{
		case SELECTSTATE_NONE:
			g_aCharSelect[4].col.a -= 0.01f;
			if (g_aCharSelect[4].col.a <= 0.0f)
			{
				g_aCharSelect[4].col.a = 0.0f;
				g_aCharSelect[4].state = SELECTSTATE_USE;
			}
			break;
		case SELECTSTATE_USE:
			g_aCharSelect[4].col.a += 0.01f;
			if (g_aCharSelect[4].col.a >= 1.0f)
			{
				g_aCharSelect[4].col.a = 1.0f;
				g_aCharSelect[4].state = SELECTSTATE_NONE;
			}
			break;
		}
		break;
	}

	if (g_aCharSelect[2].select != true)
	{
		g_aCharSelect[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}

	if (g_aCharSelect[3].select != true)
	{
		g_aCharSelect[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}

	if (g_aCharSelect[4].select != true)
	{
		g_aCharSelect[4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCharSelect->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCharSelectPage == 0)
	{
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.33f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.33f);
	}
	if (g_nCharSelectPage == 1)
	{
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.34f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.34f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.66f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.66f);
	}
	if (g_nCharSelectPage == 2)
	{
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.68f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.68f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	pVtx += 4;
	pVtx += 4;

	//頂点カラー
	pVtx[0].col = g_aCharSelect[2].col;
	pVtx[1].col = g_aCharSelect[2].col;
	pVtx[2].col = g_aCharSelect[2].col;
	pVtx[3].col = g_aCharSelect[2].col;
	pVtx += 4;

	//頂点カラー
	pVtx[0].col = g_aCharSelect[3].col;
	pVtx[1].col = g_aCharSelect[3].col;
	pVtx[2].col = g_aCharSelect[3].col;
	pVtx[3].col = g_aCharSelect[3].col;
	pVtx += 4;

	//頂点カラー
	pVtx[0].col = g_aCharSelect[4].col;
	pVtx[1].col = g_aCharSelect[4].col;
	pVtx[2].col = g_aCharSelect[4].col;
	pVtx[3].col = g_aCharSelect[4].col;

	//頂点バッファをアンロック
	g_pVtxBuffCharSelect->Unlock();

	if (g_JoyStateCharSelect.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateCharSelect.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateCharSelect.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateCharSelect.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawCharSelect(void)
{
	DrawBG();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスを取得する

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCharSelect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCharSelect = 0; nCntCharSelect < MAX_TYPE_SELECT; nCntCharSelect++)
	{
		if (g_aCharSelect[nCntCharSelect].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCharSelect[nCntCharSelect]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCharSelect * 4, 2);
		}
	}
}

//=============================================================================
// キャラ選択情報の取得
//=============================================================================
CHARACTER_TYPE GetCharaType(void)
{
	return g_CharaType;
}