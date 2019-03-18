//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET	(128)	// 弾の最大数
#define BULLET_TEXTURENAME	"data/TEXTURE/bullet000.png"



//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];	//弾の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;	//弾のカウント



	// デバイスの取得
	pDevice = GetDevice();

	// 弾の情報の初期化
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].fRadius = 0.0f;

		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME, &g_pTextureBullet);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);

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

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntBullet;
	
	MODE *pMode;
	pMode = GetMode();
	
	

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
	
			if (g_aBullet[nCntBullet].bUse == true)	//弾が使用されている
			{
				
		
				//弾の位置を移動させる
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);

				if (g_aBullet[nCntBullet].pos.y < 0)
				{
					g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
				}
				//一定時間経過
				g_aBullet[nCntBullet].nLife--;
				if (g_aBullet[nCntBullet].nLife <= 0)
				{
					//SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					g_aBullet[nCntBullet].bUse = false;
				}



				if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER) //プレイヤーの弾
				{
					//弾の位置を移動させる
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

					
					ENEMY *pEnemy;
					pEnemy = GetEnemy();
					int nCntEnemy;
					for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)	//敵が使用している
						{
							if (g_aBullet[nCntBullet].pos.x > pEnemy->pos.x - 30
								&& g_aBullet[nCntBullet].pos.x < pEnemy->pos.x + 30
								&& g_aBullet[nCntBullet].pos.y >  pEnemy->pos.y - 30
								&& g_aBullet[nCntBullet].pos.y < pEnemy->pos.y + 30)
							{
								HitEnemy(nCntEnemy, 1);

								g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
							}
						}
					}
				}

				
				
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	// ポリゴンの描画
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)	//弾が使用されている
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}
}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, float fRadius, int nLife)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntBullet;


	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aBullet[nCntBullet].bUse == false)	//弾が使用されていない
		{
			pVtx += (nCntBullet * 4);

			if (type == BULLETTYPE_PLAYER)
			{
				g_aBullet[nCntBullet].pos = pos;
				g_aBullet[nCntBullet].fRadius = fRadius;
				g_aBullet[nCntBullet].nLife = nLife;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].bUse = true;	//使用している状態にする

				
				break;
			}

			
			

			else if (type == BULLETTYPE_ENEMY)
			{
				g_aBullet[nCntBullet].pos = pos;
				g_aBullet[nCntBullet].nLife = nLife;
				g_aBullet[nCntBullet].fRadius = fRadius;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius, 0.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_aBullet[nCntBullet].type = BULLETTYPE_ENEMY;
				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].bUse = true;	//使用している状態にする
				break;

			}
			
		}
		//頂点バッファをアンロック
		g_pVtxBuffBullet->Unlock();
	}
	
}
//=============================================================================
// 弾の取得
//=============================================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}