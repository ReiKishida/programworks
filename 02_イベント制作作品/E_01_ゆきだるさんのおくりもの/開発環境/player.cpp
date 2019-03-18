//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : komatsu
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "block.h"
#include "sound.h"
#include "time.h"
#include "fade.h"
#include "game.h"
#include "item.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define PLAYER_TEXTURENAME	"data/TEXTURE/player000.png"	//表示するファイルの名前
#define PLAYER_POS_X		(30.0f)							//プレイヤーのXの長さ
#define PLAYER_POS_Y		(30.0f)							//プレイヤーのYの長さ
#define MAX_PATTERN			(4)								//パターンの数
#define TEXTURE_UPDATE		(20)							//アニメーション速度
#define MOVE_SPEED			(2.0f)							//移動速度
#define TEX_X				(0.25f)							//テクスチャのX
#define TEX_Y				(0.25f)							//テクスチャのY

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
Player g_player;									//プレイヤーの情報

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitPlayer(void)
{
	VERTEX_2D *pVtx;	//頂点バッファへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	g_player.nCounterAnim = 0;							//カウンタを初期化
	g_player.nPatternAnim = 0;							//パターンNo.を初期化
	g_player.nDirectionMove = 0;						//向きを初期化
	g_player.fHeight = PLAYER_POS_Y * 2;				//高さを初期化
	g_player.fWidth = PLAYER_POS_X;						//幅を初期化
	g_player.pos = D3DXVECTOR3(340.0f, 200.0f, 0.0f);	//開始位置の設定
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回位置の初期化
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化
	g_player.bChange = false;
	g_player.bItem = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURENAME, &g_pTexturePlayer);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - g_player.fHeight, g_player.pos.z);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y - g_player.fHeight, g_player.pos.z);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y, g_player.pos.z);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y, g_player.pos.z);

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

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TEX_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_Y);
	pVtx[3].tex = D3DXVECTOR2(TEX_X, TEX_Y);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer-> Release();
		g_pTexturePlayer = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;					//頂点バッファへのポインタ
	TIME pTime = GetTime();			//タイム情報を取得
	g_player.posOld = g_player.pos;		//前回位置の保存
	g_player.nCounterAnim++;			//カウンタ加算

	//頂点バッファをロック
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (pTime.nTime > 0)
	{
		if (GetKeyboardPress(DIK_W) == true)	//Wキー
		{//上移動
			g_player.move.y -= MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)	//Sキー
		{//下移動
			g_player.move.y += MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_A) == true)	//Aキー
		{//左移動
			g_player.move.x -= MOVE_SPEED;		
		}
		else if (GetKeyboardPress(DIK_D) == true)	//Dキー
		{//右移動
			g_player.move.x += MOVE_SPEED;
		}
	}

	if (pTime.nTime > 0)
	{
		//テクスチャ
		if (GetKeyboardPress(DIK_W) == true)
		{//上移動(地上)
			if ((g_player.nCounterAnim % TEXTURE_UPDATE) == 0)	//カウンター初期化
			{
				g_player.nPatternAnim = (g_player.nPatternAnim + 1 % MAX_PATTERN);
			}
			g_player.nDirectionMove = 1;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//下移動(地上)
			if ((g_player.nCounterAnim % TEXTURE_UPDATE) == 0)	//カウンター初期化
			{
				g_player.nPatternAnim = (g_player.nPatternAnim + 1 % MAX_PATTERN);
			}
			g_player.nDirectionMove = 0;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//左移動(地上)
			if ((g_player.nCounterAnim % TEXTURE_UPDATE) == 0)	//カウンター初期化
			{
				g_player.nPatternAnim = (g_player.nPatternAnim + 1 % MAX_PATTERN);
			}
			g_player.nDirectionMove = 2;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//右移動(地上)
			if ((g_player.nCounterAnim % TEXTURE_UPDATE) == 0)	//カウンター初期化
			{
				g_player.nPatternAnim = (g_player.nPatternAnim + 1 % MAX_PATTERN);
			}
			g_player.nDirectionMove = 3;
		}
		else if (g_player.nPatternAnim != 0)
		{//テクスチャ座標を0に戻す
			if ((g_player.nCounterAnim % TEXTURE_UPDATE) == 0)	//カウンター初期化
			{
				g_player.nPatternAnim = 0;
			}
		}
	}

	//テクスチャ更新
	pVtx[0].tex = D3DXVECTOR2(TEX_X * (g_player.nPatternAnim % MAX_PATTERN), 0.25f * g_player.nDirectionMove);
	pVtx[1].tex = D3DXVECTOR2(TEX_X * (g_player.nPatternAnim % MAX_PATTERN) + TEX_X, 0.25f * g_player.nDirectionMove);
	pVtx[2].tex = D3DXVECTOR2(TEX_X * (g_player.nPatternAnim % MAX_PATTERN), 0.25f * g_player.nDirectionMove + 0.25f);
	pVtx[3].tex = D3DXVECTOR2(TEX_X * (g_player.nPatternAnim % MAX_PATTERN) + TEX_X, 0.25f * g_player.nDirectionMove + 0.25f);

	//位置更新
	g_player.pos += g_player.move;

	//画面端
	if (g_player.pos.y < g_player.fHeight + 80.0f)
	{//上端
		g_player.pos.y = g_player.fHeight + 80.0f;
		g_player.move.y = 0.0f;
	}
	else if (g_player.pos.y > SCREEN_HEIGHT)
	{//下端
		g_player.pos.y = SCREEN_HEIGHT;
		g_player.move.y = 0.0f;
	}
	if (g_player.pos.x < g_player.fWidth)
	{//左端
		g_player.pos.x = 0.0f + g_player.fWidth;
		g_player.move.x = 0.0f;
	}
	else if (g_player.pos.x >= SCREEN_WIDTH - g_player.fWidth)
	{//右端
		g_player.pos.x = SCREEN_WIDTH - g_player.fWidth;
		g_player.move.x = 0.0f;
	}

	//減速
	g_player.move.x += (0.0f - g_player.move.x) * 1.0f;
	g_player.move.y += (0.0f - g_player.move.y) * 1.0f;

	Block *pBlock;
	pBlock = GetBlock();

	CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fHeight, g_player.fWidth);

	CollisionItem(&g_player.pos, &g_player.posOld, g_player.fHeight, g_player.fWidth);

	//頂点座標更新
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - g_player.fHeight, g_player.pos.z);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y - g_player.fHeight, g_player.pos.z);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y, g_player.pos.z);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y, g_player.pos.z);

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	//デバイスを取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
Player *GetPlayer(void)
{
	return &g_player;
}