//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "bullet.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_EXPLOSION	(128)	// 爆発の最大数
#define EXPLOSION_TEXTURENAME	"data/TEXTURE/explosion000.png"	//読み込むテクスチャ
#define UPDATE_ANIM ((g_aExplosion[nCntExplosion].nPatternAnim % 8) * 0.125f)	//アニメーションパターン
#define TEX_PATTERN	(8)	//パターンの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	int nCountAnim;		//アニメーションカウンター
	int nPatternAnim;	//アニメーションパターンNo.
	bool bUse;			//使用しているかどうか
} Explosion;

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];	//爆発の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	// デバイスの取得
	pDevice = GetDevice();

	// 爆発の情報の初期化
	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_aExplosion[nCntExplosion].nCountAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEXTURENAME, &g_pTextureExplosion);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 100.0f, g_aExplosion[nCntExplosion].pos.y - 100.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 100.0f, g_aExplosion[nCntExplosion].pos.y - 100.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 100.0f, g_aExplosion[nCntExplosion].pos.y + 100.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 100.0f, g_aExplosion[nCntExplosion].pos.y + 100.0f, 0.0f);
		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntExplosion;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].nCountAnim++;

		if ((g_aExplosion[nCntExplosion].nCountAnim % 5) == 0)
		{
			if (g_aExplosion[nCntExplosion].bUse == true)	//爆発が使用されている
			{
				//パターンNo.の更新
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 8;

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);

				if (g_aExplosion[nCntExplosion].nPatternAnim <= 0)
				{//パターンが最後まで終わった
					g_aExplosion[nCntExplosion].bUse = false;	//使用していない状態にする
				}
			}
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	// ポリゴンの描画
	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)	//弾が使用されている
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntExplosion, 2);
		}
	}
}

//=============================================================================
// 爆発の設定処理
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntExplosion;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)	//爆発が使用されていない
		{
			g_aExplosion[nCntExplosion].pos = pos;

			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 100.0f, g_aExplosion[nCntExplosion].pos.y - 100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 100.0f, g_aExplosion[nCntExplosion].pos.y - 100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 100.0f, g_aExplosion[nCntExplosion].pos.y + 100.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 100.0f, g_aExplosion[nCntExplosion].pos.y + 100.0f, 0.0f);

			g_aExplosion[nCntExplosion].col = col;

			//頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aExplosion[nCntExplosion].nCountAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);

			g_aExplosion[nCntExplosion].bUse = true;	//使用している状態にする
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}