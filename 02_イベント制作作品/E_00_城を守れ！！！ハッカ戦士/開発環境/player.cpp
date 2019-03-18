//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "main.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PATTERN (10)
#define POS_X (0.2f * (g_nPatternAnim % 5))
#define POS_Y (0.5f * (g_nPatternAnim / 5))
#define MOVE (1.2f)

#define HALF_X (60.0f)	//X軸半分
#define HALF_Y (60.0f)	//Y軸半分

#define LIMIT_MIN_X (45)	//画面端-X
#define LIMIT_MAX_X (1250)	//画面端X
#define LIMIT_MIN_Y (25)	//画面端-Y
#define LIMIT_MAX_Y (700)	//画面端Y




//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//頂点バッファへのポインタ
PLAYER g_Player;	//プレイヤー
int g_nCounterAnim;									//アニメーションカウンター
int g_nPatternAnim;									//アニメーションパターンNo.
int nCoolTime;
int nWeapon_Select;	//武装
int g_aaa = 0;

//=============================================================================
// プレイヤー生成処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	g_nCounterAnim = 0;//カウントタイマーを0にする
	g_nPatternAnim = 0;//描写カウントを0にする
	//g_Player.pos = D3DXVECTOR3(200, 535, 0.0f);//プレイヤー中心座標
	g_Player.pos = D3DXVECTOR3(600, 535, 0.0f);//プレイヤー中心座標

	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量

	nWeapon_Select = WEAPON_VALCAN;

	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.bDisp = true;
	g_Player.nCounterState = 0;
	g_Player.nCntStamina = 0;


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HackingCharaMotion.png", &g_pTexturePolygon);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標

	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - HALF_X, g_Player.pos.y - HALF_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + HALF_X, g_Player.pos.y - HALF_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - HALF_X, g_Player.pos.y + HALF_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + HALF_X, g_Player.pos.y + HALF_Y, 0.0f);


	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

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
	g_pVtxBuffPolygon->Unlock();
}
//=============================================================================
// プレイヤー消去処理
//=============================================================================
void UnInitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}
//=============================================================================
// プレイヤー更新処理
//=============================================================================
void UpdatePlayer(void)
{
	int nReload = 0;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	g_nCounterAnim++;			//カウンター加算

	MODE *pMode;
	pMode = GetMode();

	switch (g_Player.state)
	{
		case PLAYERSTATE_NORMAL:
			
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロックする
			g_pVtxBuffPolygon->Unlock();
			break;

		

		case PLAYERSTATE_CHARGE:
			
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);

			// 頂点バッファをアンロックする
			g_pVtxBuffPolygon->Unlock();
			if (GetKeyboardRelease(DIK_RETURN) == true)
			{
				g_Player.state = PLAYERSTATE_NORMAL;
				g_Player.nCntCharge = 0;
				PlaySound(SOUND_LABEL_SE_SLASH2);


			}
			break;

		case PLAYERSTATE_TIRED:
			
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f);
			// 頂点バッファをアンロックする
			g_pVtxBuffPolygon->Unlock();
		
			g_Player.nCounterState--;
			if (g_Player.nCounterState <= 0)
			{
				g_Player.state = PLAYERSTATE_NORMAL;
				g_Player.nCntStamina = 0;
			}
			
			break;

		
		
	}


	
	
#if 0
		//===================================================
		//任意のキー(RETURNキー)が押されたかどうか(トリガー) //
		//===================================================

	D3DXVECTOR3 PlayerSlash;

	PlayerSlash.x = g_Player.pos.x + 55;

	PlayerSlash.y = g_Player.pos.y;

	if (g_Player.bDisp == true)
	{
		if (g_Player.state == PLAYERSTATE_NORMAL)
		{
			//VERTEX_2D *pVtx;			//頂点情報へのポインタ

			

			if (GetKeyboardPress(DIK_RETURN) == true)
			{

				/*g_Player.nCntStamina++;
				if (g_Player.nCntStamina > 18)
				{
					g_Player.nCounterState = 60;
					g_Player.state = PLAYERSTATE_TIRED;
				}*/

				SetBullet(PlayerSlash, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER, 10.0f, 5);

				//g_Player.nCounterAnim++; //カウンターの加算 爆発の速度
				//if (g_Player.nCounterAnim % 4 == 0)
				//{
				//	g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 8;//パターンNo.更新
				//}



				//// 頂点バッファをロックし、頂点情報へのポインタを取得
				//g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);


				//pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - 80, g_Player.pos.y - 80, 0.0f);
				//pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + 80, g_Player.pos.y - 80, 0.0f);
				//pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - 80, g_Player.pos.y + 80, 0.0f);
				//pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + 80, g_Player.pos.y + 80, 0.0f);

				//pVtx[0].tex = D3DXVECTOR2(0.125f*g_Player.nPatternAnim, 0.0f);
				//pVtx[1].tex = D3DXVECTOR2(0.125f*g_Player.nPatternAnim + 0.125f, 0.0f);
				//pVtx[2].tex = D3DXVECTOR2(0.125f*g_Player.nPatternAnim, 1.0f);
				//pVtx[3].tex = D3DXVECTOR2(0.125f*g_Player.nPatternAnim + 0.125f, 1.0f);

				//// 頂点バッファをアンロックする
				//g_pVtxBuffPolygon->Unlock();

				//if (g_Player.nPatternAnim > 5)
				//{
				//	PlaySound(SOUND_LABEL_SE_SLASH);
				//}

				//if (g_Player.nPatternAnim >= 8)
				//{
				//	g_Player.nPatternAnim = 0;
				//	g_Player.bDisp = false;
				//}

#endif

#if 1

				D3DXVECTOR3 PlayerSlash;

				PlayerSlash.x = g_Player.pos.x + 70;

				PlayerSlash.y = g_Player.pos.y;
					//攻撃モーション
					//
					//	
					g_Player.state = PLAYERSTATE_A;
					if (GetKeyboardPress(DIK_RETURN) == false)
					{
						g_aaa = 0;
						g_nPatternAnim = 0;
					}
					if (GetKeyboardPress(DIK_RETURN) == true)
					{
						g_aaa++;
						g_nPatternAnim++;

						/*if (g_aaa > 5)
						{*/
							SetBullet(PlayerSlash, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER, 20.0f, 4);
							PlaySound(SOUND_LABEL_SE_SLASH2);
						//}

						if (g_aaa > 8)
						{
							g_Player.state = PLAYERSTATE_B;
						}
					}
					switch (g_Player.state)
					{
					case PLAYERSTATE_A:
						if (GetKeyboardPress(DIK_RETURN) == true)
						{
							g_nPatternAnim = ((g_nPatternAnim)+1) % 8;//パターンNo.更新

						}
						break;
					case PLAYERSTATE_B:
					{
						g_nPatternAnim = 0;
						break;
					}

					}




					// 頂点バッファをロックし、頂点情報へのポインタを取得
					g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnim*0.125f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnim*0.125f + 0.125f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnim*0.125f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnim*0.125f + 0.125f, 1.0f);
					////頂点座標の設定 / 大きさ
					//pVtx[0].pos = D3DXVECTOR3(-25 + g_posPlayer.x, g_posPlayer.y - 20, 0.0f);
					//pVtx[1].pos = D3DXVECTOR3(25 + g_posPlayer.x, g_posPlayer.y - 20, 0.0f);
					//pVtx[2].pos = D3DXVECTOR3(-25 + g_posPlayer.x, g_posPlayer.y + 60, 0.0f);
					//pVtx[3].pos = D3DXVECTOR3(25 + g_posPlayer.x, g_posPlayer.y + 60, 0.0f);

					////}

					//g_player.pos = g_posPlayer;
					//g_player.move = g_movePlayer;

					//頂点バッファをアンロック
					g_pVtxBuffPolygon->Unlock();

				}

#endif

			

		

	
				
		
	
			//=====================================================
			//任意のキー(RETURNキー)が押されたかどうか(チャージ) //
			//====================================================
			/*if (GetKeyboardPress(DIK_RETURN) == true)
			{

				g_Player.nCntCharge++;
				PlaySound(SOUND_LABEL_SE_CHARGE2);
				if (g_Player.nCntCharge >= 200)
				{
					g_Player.state = PLAYERSTATE_CHARGE;
					StopSound(SOUND_LABEL_SE_CHARGE2);
				}

			}

			if (GetKeyboardPress(DIK_RETURN) != true)
			{
				g_Player.nCntCharge = 0;
			}
		*/

			
	
////頂点バッファをロックし、頂点データへのポインタを取得
//g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
//
//pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - HALF_X, g_Player.pos.y - HALF_Y, 0.0f);
//pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + HALF_X, g_Player.pos.y - HALF_Y, 0.0f);
//pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - HALF_X, g_Player.pos.y + HALF_Y, 0.0f);
//pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + HALF_X, g_Player.pos.y + HALF_Y, 0.0f);
//
//
////頂点バッファをアンロックする
//g_pVtxBuffPolygon->Unlock();



//=============================================================================
// プレイヤー描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	if (g_Player.bDisp == true)
	{
	
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ
	pDevice->SetTexture(0, g_pTexturePolygon);

	
		//プレイヤー描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}


//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void HitPlayer(int nDamage)
{
}


//=============================================================================
// プレイヤーの取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}


