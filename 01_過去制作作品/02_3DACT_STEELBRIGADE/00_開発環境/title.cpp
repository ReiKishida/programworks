//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "meshcylinder.h"
#include "titlecamera.h"
#include "light.h"
#include "particle.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TYPE_TITLE (2)
#define TITLE_TEXTURENAME_0	"data/TEXTURE/title.png"		//背景のテクスチャ3
#define TITLE_TEXTURENAME_1	"data/TEXTURE/press_enter2.png"		//背景のテクスチャ3


#define TITLE_POS_X		(400.0f)							//背景左上X座標
#define TITLE_POS_Y		(220.0f)							//背景左上Y座標

#define ENTER_POS_X   (250.0f)		//Enterキー座標
#define ENTER_POS_Y   (80.0f)		//Enterキー座標

#define TITLE_WIDTH		(SCREEN_WIDTH / 2)				//背景の幅
#define TITLE_HEIGHT		(SCREEN_HEIGHT /2)				//背景の高さ
//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTitle[MAX_TYPE_TITLE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;					// 頂点バッファへのポインタ


TITLE g_aTitle[MAX_TYPE_TITLE];

int g_nCntScene; //ランキングへの遷移
int g_nColorChange;
int LengthCounter;
bool bUseTitle;

JoyState g_JoyStateTitle;

//=============================================================================
// 初期化処理
//=============================================================================
void InitTitle(void)
{


	// カメラの初期化処理
	InitTitleCamera();

	// ライトの初期化処理
	InitLight();


	//メッシュシリンダー初期化
	 InitMeshCylinder();

	 SetMeshCylinder(D3DXVECTOR3(0.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, -20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	 SetMeshCylinder(D3DXVECTOR3(0.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);



	LPDIRECT3DDEVICE9 pDevice;


	// デバイスの取得
	pDevice = GetDevice();

	g_nCntScene = 0;

	LengthCounter = 3;

	//タイトル位置
	g_aTitle[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f);
	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 , 520, 0.0f);

	g_aTitle[0].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	g_aTitle[1].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	g_aTitle[0].nType = 0;
	g_aTitle[1].nType = 1;



	g_aTitle[0].fHeight = 300;
	g_aTitle[0].fWidth = 800;



	g_aTitle[1].TitleType = TITLETYPE_NONE;
	g_aTitle[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTitle[1].state = TITLESTATE_NONE;


	g_aTitle[0].bUse = true;
	g_aTitle[1].bUse = false;


	g_aTitle[0].fLength = sqrtf((g_aTitle[0].fWidth / 2 + g_aTitle[0].fWidth / 2) * (g_aTitle[0].fHeight / 2 + g_aTitle[0].fHeight / 2));
	g_aTitle[0].fAngle = atan2f(g_aTitle[0].fWidth / 2, g_aTitle[0].fHeight / 2);


	// テクスチャの読み込み(2体分)
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME_0, &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME_1, &g_pTextureTitle[1]);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(-D3DX_PI + g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength/ 2,
		g_aTitle[0].pos.y + cosf(-D3DX_PI + g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength/2,
		0.0f);


	pVtx[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(D3DX_PI - g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength/2,
		g_aTitle[0].pos.y + cosf(D3DX_PI - g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength/2,
		0.0f);


	pVtx[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(-g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength/2,
		g_aTitle[0].pos.y + cosf(-g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength/2,
		0.0f);


	pVtx[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength/2,
		g_aTitle[0].pos.y + cosf(g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength/2,
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
	g_pVtxBuffTitle->Unlock();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	//頂点の座標
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_POS_X, g_aTitle[1].pos.y - ENTER_POS_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_POS_X, g_aTitle[1].pos.y - ENTER_POS_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_POS_X, g_aTitle[1].pos.y + ENTER_POS_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_POS_X, g_aTitle[1].pos.y + ENTER_POS_Y, 0.0f);

	//1.0fで固定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラー
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);




	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitTitle(void)
{

	 UninitTitleCamera();


	// ライトの初期化処理
	UninitLight();


	 UninitMeshCylinder();	//メッシュシリンダー終了

	// テクスチャの開放
	for (int nCount = 0; nCount < MAX_TYPE_TITLE; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureTitle[nCount] != NULL)
		{
			g_pTextureTitle[nCount]->Release();
			g_pTextureTitle[nCount] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	 UpdateLight();
	 UpdateTitleCamera();


	UpdateMeshCylinder();	//メッシュシリンダー更新


	g_nCntScene++;


	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();


	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (g_nCntScene % 500 == 0)
	{
		if (pFade == FADE_NONE)
		{

			//モード選択
			SetFade(MODE_RANKING);

		}
	}

	if (g_aTitle[1].TitleType == TITLETYPE_NONE)
	{
		switch (g_aTitle[1].state)
		{
		case TITLESTATE_NONE:
			g_aTitle[1].col.a -= 0.02f;
			if (g_aTitle[1].col.a <= 0.1f)
			{
				g_aTitle[1].col.a = 0.1f;
				g_aTitle[1].state = TITLESTATE_USE;
			}
			break;

		case TITLESTATE_USE:
			g_aTitle[1].col.a += 0.02f;
			if (g_aTitle[1].col.a >= 1.0f)
			{
				g_aTitle[1].col.a = 1.0f;
				g_aTitle[1].state = TITLESTATE_NONE;
			}
			break;
		}
	}
	else if (g_aTitle[1].TitleType == TITLETYPE_NEXT)
	{
		switch (g_aTitle[1].state)
		{
		case TITLESTATE_NONE:
			g_aTitle[1].col.r -= 0.9f;
			if (g_aTitle[1].col.r <= 0.1f)
			{
				g_aTitle[1].col.r = 0.1f;
				g_aTitle[1].state = TITLESTATE_USE;
			}
			break;

		case TITLESTATE_USE:
			g_aTitle[1].col.r += 0.9f;
			if (g_aTitle[1].col.r >= 1.0f)
			{
				g_aTitle[1].col.r = 1.0f;
				g_aTitle[1].state = TITLESTATE_NONE;
			}
			break;
		}
	}


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);



		g_aTitle[1].bUse = true;
		if (pFade == FADE_NONE)
		{

			if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
			{
				if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					//モード選択
					SetFade(MODE_TIPS);
					g_aTitle[1].col.r = 1.0f;
					g_aTitle[1].TitleType = TITLETYPE_NEXT;
				}
				g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_PUSH;
			}

			if (GetKeyboardTrigger(DIK_R) == true)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				//モード選択
				SetFade(MODE_RANKING);
			}


		}



		if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_PUSH)
		{
			g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NONE;
		}
		else if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_NONE)
		{
			g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
		}




	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(-D3DX_PI + g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength,
		g_aTitle[0].pos.y + cosf(-D3DX_PI + g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength,
		0.0f);


	pVtx[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(D3DX_PI - g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength,
		g_aTitle[0].pos.y + cosf(D3DX_PI - g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength,
		0.0f);


	pVtx[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(-g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength,
		g_aTitle[0].pos.y + cosf(-g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength,
		0.0f);


	pVtx[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + sinf(g_aTitle[0].fAngle + g_aTitle[0].rot.x) * g_aTitle[0].fLength,
		g_aTitle[0].pos.y + cosf(g_aTitle[0].fAngle + g_aTitle[0].rot.y) * g_aTitle[0].fLength,
		0.0f);

	//頂点カラー
	pVtx[4].col = g_aTitle[1].col;
	pVtx[5].col = g_aTitle[1].col;
	pVtx[6].col = g_aTitle[1].col;
	pVtx[7].col = g_aTitle[1].col;

	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{

	SetTitleCamera();



	DrawMeshCylinder();		//メッシュシリンダー描画
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画


	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntTitle = 0; nCntTitle < MAX_TYPE_TITLE; nCntTitle++)
	{

		if (g_aTitle[nCntTitle].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTitle[g_aTitle[nCntTitle].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
		}
	}

}


