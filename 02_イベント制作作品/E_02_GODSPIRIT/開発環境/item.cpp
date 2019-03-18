//=============================================================================
//
// アイテムの処理 [item.cpp]
// Author : 佐藤亮太
//
//=============================================================================
#include "item.h"
#include "fade.h"
#include "player.h"
#include "shadow.h"
#include "game.h"
#include "meshField.h"
#include "model.h"
#include "score.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_KOUTOKU_TEXTURE	("data/TEXTURE/koutoku_Item.png")	// 幸徳のテクスチャのアドレス
#define MAX_ITEM				(128)								// アイテムの最大数
#define ITEM_KOUTOKU_WIDTH		(25.0f / 2.0f)						// 幸徳の幅
#define ITEM_KOUTOKU_HEIGHT		(25.0f)								// 幸徳の高さ
#define ITEM_KOUTOKU_SCORE		(100)								// 幸徳の得点
#define KOUTOKU_PATTEN			(1.0f)								// 幸徳のテクスチャアニメーションのパターン数
#define ITEM_GRAVITY			(-1.5f)								// 重力
#define ITEM_INERTIA			(0.10f)								// 慣性
#define ITEM_JUMP				(3.0f)								// ジャンプ量
#define ITEM_JUMP_DOWN			(0.3f)								// アイテムのジャンプ力の減少量
#define ITEM_GET_RADIUS			(400.0f)							// プレイヤーが近づいたら吸いついてくる範囲
#define ITEM_GET_PLAYER_RADIUS	(50.0f)								// プレイヤーの当たり判定の半径
#define ITEM_DROP_MOVE			(1.0f)								// 吸い付いてくる速度
#define ITEM_ADD_SPIRIT			(3)									// アイテム取得時のスピリット増加量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;		// テクスチャへのポインタ
Item					g_Item[MAX_ITEM];			// アイテムの情報
float					g_fItemGetRadius;			// 近づいたら吸い付いてくる範囲

//=============================================================================
// 初期化処理
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,								// 3Dデバイスのアドレス
							  ITEM_KOUTOKU_TEXTURE,					// 読み込むテクスチャのファイルのアドレス
							  &g_pTextureItem);						// テクスチャのポインタ

	// アイテム情報の初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムの総数分ループ
		g_Item[nCntItem].pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置を初期化
		g_Item[nCntItem].posOld			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置を初期化
		g_Item[nCntItem].InitPos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 初期位置
		g_Item[nCntItem].move			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量を初期化
		g_Item[nCntItem].fJampPower		 = 0.0f;							// ジャンプ力を初期化

		g_Item[nCntItem].nCounterAnime	 = 0;								// カウンターを初期化
		g_Item[nCntItem].nPattanAnime	 = 0;								// パターンを初期化
		g_Item[nCntItem].fMaxPattanAnime = 0;								// パターンの最大数を初期化
		g_Item[nCntItem].nIdyShadow		 = 0;								// 影の番号を初期化
		g_Item[nCntItem].ItemType		 = ITEM_KOUTOKU;					// アイテムの種類を初期化
		g_Item[nCntItem].fWidth			 = 0.0f;							// アイテムの幅を初期化
		g_Item[nCntItem].fHeight		 = 0.0f;							// アイテムの高さを初期化
										 
		g_Item[nCntItem].bUse			 = false;							// 使用していない状態にする
	}

	g_fItemGetRadius = ITEM_GET_RADIUS * ITEM_GET_RADIUS;					// 吸い付いてくる範囲を計算

	// 頂点情報の作成
	MakeVertexItem(pDevice);

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	// 頂点バッファの開放(破棄)
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	// テクスチャの破棄
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;															// 頂点情報のポインタ

	Player *pPlayer = GetPlayer();												// プレイヤー情報の取得

	// 頂点バッファをロックし、頂点でデータへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
	{// アイテムの数分ループ

		if (g_Item[nCntItem].bUse == true)
		{// 使われているとき

			// キャラクターとアイテムの距離の計算
			float PosX = (g_Item[nCntItem].pos.x - pPlayer->pos.x) * (g_Item[nCntItem].pos.x - pPlayer->pos.x);		// (アイテムの位置－キャラの位置)を二乗する						
			float PosY = (g_Item[nCntItem].pos.y - pPlayer->pos.y) * (g_Item[nCntItem].pos.y - pPlayer->pos.y);		// (アイテムの位置－キャラの位置)を二乗する
			float PosZ = (g_Item[nCntItem].pos.z - pPlayer->pos.z) * (g_Item[nCntItem].pos.z - pPlayer->pos.z);		// (アイテムの位置－キャラの位置)を二乗する
			g_Item[nCntItem].fPosLenght = PosX + PosY + PosZ;														// 計算した値を足して、距離を計算する

			//-----------------------------------------------
			// 位置の更新
			//-----------------------------------------------
			g_Item[nCntItem].posOld = g_Item[nCntItem].pos;									// 前回の位置を保存

			g_Item[nCntItem].move.y += ITEM_GRAVITY;										// 重力の加算

			if (g_Item[nCntItem].fJampPower > 0.0f)
			{// ジャンプ力がまだあったら
				g_Item[nCntItem].move.y += g_Item[nCntItem].fJampPower;						// ジャンプ量を移動量に加算する(上昇していく)
				g_Item[nCntItem].fJampPower -= ITEM_JUMP_DOWN;								// ジャンプ力を下げていく
			}

			if (g_Item[nCntItem].fPosLenght < g_fItemGetRadius)
			{// 吸い付いてくる範囲の中にいたら
				float fAngle;																// プレイヤーとアイテムの角度を入れる変数
				float fAngleY;																// プレイヤーとアイテムの角度を入れる変数

				// アイテムからプレイヤーへの角度を計算
				fAngle = atan2f(pPlayer->pos.x - g_Item[nCntItem].pos.x, pPlayer->pos.z - g_Item[nCntItem].pos.z);
				fAngleY = atanf((pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._42) - g_Item[nCntItem].pos.y);											// y軸の角度を計算

				// プレイヤーのほうに移動
				g_Item[nCntItem].move.x += sinf(fAngle) * ITEM_DROP_MOVE;
				g_Item[nCntItem].move.y += sinf(fAngleY) * 3.0f;
				g_Item[nCntItem].move.z += cosf(fAngle) * ITEM_DROP_MOVE;
																																														// 
			}

			// 慣性
			//g_Item[nCntItem].move.x += (0.0f - g_Item[nCntItem].move.x) * ITEM_INERTIA;
			//g_Item[nCntItem].move.z += (0.0f - g_Item[nCntItem].move.z) * ITEM_INERTIA;


			g_Item[nCntItem].pos += g_Item[nCntItem].move;									// 位置の更新

			//-----------------------------------------------
			// 当たり判定
			//-----------------------------------------------
			if(CollisionMeshField(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, &g_Item[nCntItem].pMeshField) == true)
			{// 地面の上についたら
				g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動させないようにする
				Shadow *pShadow = GetShadow();
				pShadow[g_Item[nCntItem].nIdyShadow].pos.y = g_Item[nCntItem].pos.y + 0.1f;
				pShadow[g_Item[nCntItem].nIdyShadow].col.a = 1.0f;
			}		
			
			if (CollisionModelMinMax(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, &g_Item[nCntItem].pModel) == 1)
			{
				g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動させないようにする
				Shadow *pShadow = GetShadow();
				pShadow[g_Item[nCntItem].nIdyShadow].pos.y = g_Item[nCntItem].pos.y + 0.1f;
				pShadow[g_Item[nCntItem].nIdyShadow].col.a = 1.0f;
			}

			//-----------------------------------------------
			// テクスチャアニメーションの更新
			//-----------------------------------------------
			if (g_Item[nCntItem].nCounterAnime % 10 == 0)
			{// 5フレームごとに
				// テクスチャのコマ数を進める
				g_Item[nCntItem].nPattanAnime = (g_Item[nCntItem].nPattanAnime + 1) % int(g_Item[nCntItem].fMaxPattanAnime);

				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f), 20, 0);
				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 20, 0);
				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 20, 0);
				//SetParticle(g_Item[nCntItem].pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 20, 0);

				// テクスチャ座標の更新
				pVtx[0].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)											 + 0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (1.0f / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)											 + 0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (1.0f / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 1.0f);
			}

			g_Item[nCntItem].nCounterAnime++;											// カウンターを毎フレーム加算

			SetPositionShadow(g_Item[nCntItem].nIdyShadow, g_Item[nCntItem].pos, g_Item[nCntItem].InitPos);		// 影の位置の更新
		}

	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxView, mtxTrans;				// 計算用のマトリクス

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// "REF"リファレンス,基準値
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// "FUNC"ファンクション,"GREATER"より大きい,比較方法を入れる

	// Zバッファの設定
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// "FALSE"で描画の順番を無視して、常に手前に描画する
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// "FALSE"で描画する順番を遠くから描画するようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// 描画する順番を決める

	// ライティングモードの影響を受けないようにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ライティングモード有効

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{// 使われているとき

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorld);

			// ビューマトリクスの取得
			pDevice->GetTransform(D3DTS_VIEW,			// ビューマトリクスを取得して
								  &mtxView);			// 取得した値を代入する

			// 逆行列の設定
			g_Item[nCntItem].mtxWorld._11 = mtxView._11;
			g_Item[nCntItem].mtxWorld._12 = mtxView._21;
			g_Item[nCntItem].mtxWorld._13 = mtxView._31;
			g_Item[nCntItem].mtxWorld._21 = mtxView._12;
			g_Item[nCntItem].mtxWorld._22 = mtxView._22;
			g_Item[nCntItem].mtxWorld._23 = mtxView._32;
			g_Item[nCntItem].mtxWorld._31 = mtxView._13;
			g_Item[nCntItem].mtxWorld._32 = mtxView._23;
			g_Item[nCntItem].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
								  g_Item[nCntItem].pos.x,					// 位置はX座標から入れていく
								  g_Item[nCntItem].pos.y,
								  g_Item[nCntItem].pos.z);

			D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorld,					// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
							   &g_Item[nCntItem].mtxWorld,
							   &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItem);

			// アイテムの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntItem * 4,						// 描画最初の頂点								
				2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zバッファの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライティングモードの設定を元に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ITEM,			// 確保するバッファのサイズ（sizeof(VERTEX_3D)*必要な数）
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,															// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;															// 頂点情報のポインタ

	// 頂点バッファをロックし、頂点でデータへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, 0.0f,					  0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, 0.0f,					  0.0f);

		// 法線の設定			※数値が1を超えてはいけない
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.8f, 0.8f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																	  + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																      + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 1.0f);

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// アイテムの設定
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ItemType type)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;															// 頂点情報のポインタ

	// 頂点バッファをロックし、頂点でデータへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
	{// 最大数分ループ
		if (g_Item[nCntItem].bUse == false)
		{// 使われていなかったら
			g_Item[nCntItem].pos = pos;						// 位置を設定
			g_Item[nCntItem].posOld = pos;					// 前回の位置を設定
			g_Item[nCntItem].InitPos = pos;					// 初期位置
			g_Item[nCntItem].move = move;					// 移動量を設定
			g_Item[nCntItem].ItemType = type;				// アイテムの種類を設定
			g_Item[nCntItem].fJampPower = ITEM_JUMP;		// ジャンプ力を設定
			g_Item[nCntItem].nCounterAnime = 0;				// カウンターを設定
			g_Item[nCntItem].nPattanAnime = 0;				// パターンを設定
			g_Item[nCntItem].bUse = true;					// 使用している状態に設定

			switch (g_Item[nCntItem].ItemType)
			{
			case ITEM_KOUTOKU:			// 幸徳
				g_Item[nCntItem].fWidth = ITEM_KOUTOKU_WIDTH;		// 幅を設定
				g_Item[nCntItem].fHeight = ITEM_KOUTOKU_HEIGHT;		// 高さを設定
				g_Item[nCntItem].fMaxPattanAnime = KOUTOKU_PATTEN;		// パターンの最大数を初期化

				break;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_Item[nCntItem].fWidth, 0.0f,					  0.0f);
			pVtx[3].pos = D3DXVECTOR3( g_Item[nCntItem].fWidth, 0.0f,					  0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																	  + 0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime)																	  + 0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + (g_Item[nCntItem].nPattanAnime / g_Item[nCntItem].fMaxPattanAnime) + 0.0f, 1.0f);


			// 影を設定
			g_Item[nCntItem].nIdyShadow = SetShadow(g_Item[nCntItem].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),g_Item[nCntItem].fWidth,g_Item[nCntItem].fHeight);

			break;										// ループを抜ける
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// アイテムの当たり判定
//=============================================================================
void CollisionItem(D3DXVECTOR3 mtxWorld,float fRadius)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// 最大数分ループ
		if (g_Item[nCntItem].bUse == true)
		{// 使われていなかったら

		 	// キャラクターとアイテムの距離の計算
			float PosX = (g_Item[nCntItem].pos.x - mtxWorld.x);									// (アイテムの位置－キャラの位置)を二乗する						
			float PosY = ((g_Item[nCntItem].pos.y + ITEM_KOUTOKU_HEIGHT / 2) - mtxWorld.y);		// (アイテムの位置－キャラの位置)を二乗する
			float PosZ = (g_Item[nCntItem].pos.z - mtxWorld.z);									// (アイテムの位置－キャラの位置)を二乗する
			float fItemLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);															// 計算した値を足して、距離を計算する

			// キャラとアイテムの半径を計算
			float fRadiusLenght;																			// (アイテムの半径－キャラの半径)を二乗して距離を計算する

			fRadiusLenght = (g_Item[nCntItem].fWidth + ITEM_GET_PLAYER_RADIUS) * 
							(g_Item[nCntItem].fWidth + ITEM_GET_PLAYER_RADIUS);					// アイテムとキャラの半径を計算

			//--------------------------------------
			// 距離と半径の比較
			//--------------------------------------
			if (fItemLenght <  fRadiusLenght)
			{// アイテムとキャラが半径よりも近かったら

				DeleteItem(nCntItem, g_Item[nCntItem].ItemType);			// アイテム取得

			}
		}
	}
}

//=============================================================================
// アイテムの取得処理
//=============================================================================
void DeleteItem(int nCntItem, ItemType type)
{
	switch (type)
	{
	case ITEM_KOUTOKU:								// 幸徳
		AddScore(1000);
		PlayerSpiritAdd(ITEM_ADD_SPIRIT);
		break;
	}
	PlaySound(SOUND_LABEL_SE_ITEMGET);
	g_Item[nCntItem].bUse = false;				// アイテムの削除
	DeleteShadow(g_Item[nCntItem].nIdyShadow);	// 影の削除
}