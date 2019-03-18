//=============================================================================
//
// プレイヤー処理 [Shadow.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "Shadow.h"
#include "input.h"
#include "main.h"
#include "effect.h"
#include "item.h"
#include "score.h"
#include "sound.h"
#include "timer.h"
#include "block.h"
#include "itemcounter.h"
#include "vodkacounter.h"
#include "satsucounter.h"
#include "areycounter.h"
#include "fade.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HALF_X (30.0f)	//X軸半分
#define HALF_Y (100.0f)	//Y軸半分

#define LIMIT_MIN_X (30)	//画面端-X
#define LIMIT_MAX_X (1250)	//画面端X
#define LIMIT_MIN_Y (50)	//画面端-Y
#define LIMIT_MAX_Y (595)	//画面端Y

#define SJADOWANIMATION_SPEED (8)	//アニメーションのスピード

#define JUMP_COUNT (1)	//何段ジャンプか


#define SHADOW_MOVE (7.0f)	//移動量
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
SHADOW g_Shadow;	//プレイヤー

float fJShadowump;	//ジャンプ力
float fShadowGravity;	//重力


int nShadowAnimation_Y;	//テクスチャアニメーション


//=============================================================================
// プレイヤー生成処理
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//デバイスの取得
	pDevice = GetDevice();

	g_Shadow.pos = D3DXVECTOR3(350, 580, 0.0f);//プレイヤー中心座標

	g_Shadow.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量


	g_Shadow.state = SHADOWSTATE_NORMAL;
	g_Shadow.bDisp = true;
	g_Shadow.nCounterState = 0;
	g_Shadow.nCounterAnim = 0;
	g_Shadow.nPatternAnim = 0;
	g_Shadow.fWidth = HALF_X;
	g_Shadow.fHeight = HALF_Y;

	g_Shadow.bUseJump = true;


	nShadowAnimation_Y = 0;

	g_Shadow.bPress = false;

	fJShadowump = SHADOWJUMP_POWER;
	fShadowGravity = SHADOWGRAVITY;

	g_Shadow.nJumpCount = 0;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Shadow.png", &g_pTextureShadow);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight /2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight /2, 0.0f);
	////頂点座標
	//pVtx[0].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}
//=============================================================================
// プレイヤー消去処理
//=============================================================================
void UnInitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}
//=============================================================================
// プレイヤー更新処理
//=============================================================================
void UpdateShadow(void)
{
	g_Shadow.posold = g_Shadow.pos;

	/*BLOCK *pBlock;
	pBlock = GetBlock();*/

	ITEM *pItem;
	pItem = GetItem();

	TIMER pTimer;
	pTimer = GetTimer();

	int pCounter;
	pCounter = GetItemCounter();

	int pVodka;
	pVodka = GetVodkaCounter();

	VERTEX_2D *pVtx;			//頂点情報へのポインタ
								//g_Shadow.move.x = 0;
								//float fShadowGravity;


	D3DXVECTOR3 EffectPos;

	EffectPos.x = g_Shadow.pos.x;
	EffectPos.y = g_Shadow.pos.y - 50;


	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = g_Shadow.pos.x + 10;
	ShadowPos.y = g_Shadow.pos.y - 25;


	//SetShadow(ShadowPos, 35.0f, 1);

	switch (g_Shadow.state)
	{

	case SHADOWSTATE_NORMAL:
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//頂点バッファをアンロックする
		g_pVtxBuffShadow->Unlock();
		break;

	case SHADOWSTATE_DAMAGE:
		g_Shadow.nCounterState--;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		//頂点バッファをアンロックする
		g_pVtxBuffShadow->Unlock();

		if (g_Shadow.nCounterState <= 0)
		{
			g_Shadow.nCounterState = 0;
			g_Shadow.state = SHADOWSTATE_NORMAL;
		}

		break;
	}

	//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 55.0f, 25);

	//=========================================
	//任意のキー(Aキー)が押されたかどうか    //
	//=========================================
	if (g_Shadow.state == SHADOWSTATE_NORMAL)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{

			g_Shadow.move.x -= SHADOW_MOVE;
			g_Shadow.bPress = true;

			nShadowAnimation_Y = 1;
			if (GetKeyboardPress(DIK_RSHIFT) == true)
			{
				SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 25);
				g_Shadow.move.x -= 3;

			}

		}


		//=========================================
		//任意のキー(Dキー)が押されたかどうか    //
		//=========================================
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_Shadow.move.x += SHADOW_MOVE;
			g_Shadow.bPress = true;

			nShadowAnimation_Y = 0;
			if (GetKeyboardPress(DIK_RSHIFT) == true)
			{
				SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 50.0f, 25);
				g_Shadow.move.x += 3;
			}
		}

		else if (g_Shadow.bPress == true)
		{
			g_Shadow.bPress = false;
			if (g_Shadow.nPatternAnim % 2 == 0)
			{
				g_Shadow.nPatternAnim++;
			}
		}



		//=========================================
		//任意のキー(SPACEキー)が押されたかどうか //
		//=========================================

		//if (GetKeyboardPress(DIK_SPACE) == true && g_Shadow.bUseJump == false)
		if (GetKeyboardTrigger(DIK_SPACE) == true && g_Shadow.bUseJump == false)
		{
			//g_Shadow.nJumpCount++;
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_Shadow.bUseJump = true;

			if (g_Shadow.bUseJump == true)
			{

				g_Shadow.move.y += fJShadowump;
				if (g_Shadow.nPatternAnim == 0)
				{
					g_Shadow.nPatternAnim = 1;
				}
				if (g_Shadow.nPatternAnim == 2)
				{
					g_Shadow.nPatternAnim = 3;
				}
			}
			//SetEffect(EffectPos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 30.0f, 10);
		}
	}




	if (g_Shadow.bUseJump == false)//地上にいるとき
	{
		if (g_Shadow.move.x >= 0.1f || g_Shadow.move.x <= -0.1f) //移動量が0ではない
		{
			g_Shadow.nCounterAnim++; //カウンターの加算 爆発の速度
			if (g_Shadow.nCounterAnim % SJADOWANIMATION_SPEED == 0)
			{
				g_Shadow.nPatternAnim = (g_Shadow.nPatternAnim + 1) % 4;//パターンNo.更新
			}
		}
		//else if (g_Shadow.nPatternAnim % 2 != 0)
		else if (g_Shadow.nPatternAnim == 1 || g_Shadow.nPatternAnim == 3)

		{
			g_Shadow.nCounterAnim++; //カウンターの加算 爆発の速度
			if (g_Shadow.nCounterAnim % SJADOWANIMATION_SPEED == 0)
			{
				g_Shadow.nPatternAnim = (g_Shadow.nPatternAnim + 1) % 4;//パターンNo.更新
			}
		}
	}


	g_Shadow.move.y += fShadowGravity;
	if (g_Shadow.move.y > SHADOWGRAVITY_MAX)
	{
		g_Shadow.move.y = SHADOWGRAVITY_MAX;
	}

	g_Shadow.move.x += (0.0f - g_Shadow.move.x) * 0.35f;
	/*g_Shadow.move.y += (0.0f - g_Shadow.move.y) * 0.2f;*/



	g_Shadow.pos.x += g_Shadow.move.x;
	g_Shadow.pos.y += g_Shadow.move.y;




	if (g_Shadow.pos.x >= SCREEN_WIDTH + 20)
	{
		g_Shadow.pos = D3DXVECTOR3(10.0, g_Shadow.pos.y, 0.0);
	}

	if (g_Shadow.pos.x <= 0)
	{
		g_Shadow.pos = D3DXVECTOR3(1280.0, g_Shadow.pos.y, 0.0);
	}



	if (CollisionBlock(&g_Shadow.pos, &g_Shadow.posold, &g_Shadow.move, g_Shadow.fWidth, g_Shadow.fHeight) == true)
	{//接触判定
		g_Shadow.bUseJump = false;

	}
	else
	{//空中にいるとき
		if (g_Shadow.bUseJump == false)
		{
			g_Shadow.bUseJump = true;
		}
	}



	
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Shadow.pos.x - g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Shadow.pos.x + g_Shadow.fWidth, g_Shadow.pos.y - g_Shadow.fHeight / 2, 0.0f);


	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0 + (0.25f*g_Shadow.nPatternAnim), 0.5f * nShadowAnimation_Y);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f*g_Shadow.nPatternAnim), 0.5f* nShadowAnimation_Y);
	pVtx[2].tex = D3DXVECTOR2(0 + (0.25f*g_Shadow.nPatternAnim), 0.5f* (nShadowAnimation_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f*g_Shadow.nPatternAnim), 0.5f* (nShadowAnimation_Y + 1));
	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();


}


//=============================================================================
// プレイヤー描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

							  //デバイスを取得する
	pDevice = GetDevice();

	if (g_Shadow.bDisp == true)
	{

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャ
		pDevice->SetTexture(0, g_pTextureShadow);


		//プレイヤー描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}


bool CollisionShadow(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{



	bool bLand = false; //着地したか

	if (pPos->x >= g_Shadow.pos.x - fWidth && pPos->x <= g_Shadow.pos.x + g_Shadow.fWidth + fWidth
		&& pPos->y >= g_Shadow.pos.y  && pPos->y <= g_Shadow.pos.y + g_Shadow.fHeight + g_Shadow.fWidth)
	{//足場の範囲のとき	

		if (pPosold->y >= g_Shadow.posold.y&& pPos->y <= g_Shadow.pos.y)
		{//下から来た時	
			pPos->y = g_Shadow.pos.y;
			pMove->y = 0.0f;
		}

	}


	return bLand;
}




