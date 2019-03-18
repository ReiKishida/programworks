////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクト3D処理 [effct3D.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effect3D.h"
#include "player.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}Effect3DInfo;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_EFFECT3D (100)
#define MAX_SPEED	(1.0f)	// 移動量
#define DOWN_SPEED	(0.15f)	// 減速量
#define MAX_HEIGHT	(100)	// 上の上限
#define MIN_HEIGHT	(-50)	// 下の上限

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
Effect3D g_aEffect3D[MAX_EFFECT3D];
Effect3DData g_aEffect3DData[EFFECT3D_MAX];
Effect3DInfo g_aEffect3DFilename[EFFECT3D_MAX]=
{
	{ "data/MODEL/hanma＿efect.x" },
	{ "data/MODEL/handEffectLong.x" },
	{"data/MODEL/sordEffect.x"},
};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitEffect3D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・向きの初期設定
	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		g_aEffect3D[nCntMat].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect3D[nCntMat].nType = EFFECT3D_ARM;
		g_aEffect3D[nCntMat].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect3D[nCntMat].bCollision = true;
		g_aEffect3D[nCntMat].nIdy = 0;
		g_aEffect3D[nCntMat].bUse = false;
	}	
	for (int nCntMat = 0; nCntMat < EFFECT3D_MAX; nCntMat++)
	{
		// モデルデータの読み込み
		g_aEffect3DData[nCntMat].pMesh = NULL;
		g_aEffect3DData[nCntMat].pBuffMat = NULL;
		g_aEffect3DData[nCntMat].nNumMat = 0;

		// Xファイルの読み込み
		D3DXLoadMeshFromX(g_aEffect3DFilename[nCntMat].pFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEffect3DData[nCntMat].pBuffMat,
			NULL,
			&g_aEffect3DData[nCntMat].nNumMat,
			&g_aEffect3DData[nCntMat].pMesh);
		D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aEffect3DData[nCntMat].pBuffMat->GetBufferPointer();
		// テクスチャの配列設定
		g_aEffect3DData[nCntMat].pTextuer = new LPDIRECT3DTEXTURE9[(int)g_aEffect3DData[nCntMat].nNumMat];

		// テクスチャの読み込み
		for (int nCntMatTex = 0; nCntMatTex < (int)g_aEffect3DData[nCntMat].nNumMat; nCntMatTex++)
		{
			g_aEffect3DData[nCntMat].pTextuer[nCntMatTex] = NULL;

			if (pMat[nCntMatTex].pTextureFilename != NULL)
			{
				// テクスチャの設定
				D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
					pMat[nCntMatTex].pTextureFilename,	// ファイルの名前
					&g_aEffect3DData[nCntMat].pTextuer[nCntMatTex]);		// テクスチャへのポインタ
			}
		}
	}
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitEffect3D(void)
{
	for (int nCntMat = 0; nCntMat < EFFECT3D_MAX; nCntMat++)
	{
		// メッシュの開放
		if (g_aEffect3DData[nCntMat].pMesh != NULL)
		{
			g_aEffect3DData[nCntMat].pMesh->Release();
			g_aEffect3DData[nCntMat].pMesh = NULL;
		}
		// マテリアルの開放
		if (g_aEffect3DData[nCntMat].pBuffMat != NULL)
		{
			g_aEffect3DData[nCntMat].pBuffMat->Release();
			g_aEffect3DData[nCntMat].pBuffMat = NULL;
		}
		// テクスチャの開放
		for (int nCntMatTex = 0; nCntMatTex < int(g_aEffect3DData[nCntMat].nNumMat); nCntMatTex++)
		{
			if (g_aEffect3DData[nCntMat].pTextuer[nCntMatTex] != NULL)
			{
				g_aEffect3DData[nCntMat].pTextuer[nCntMatTex]->Release();
				g_aEffect3DData[nCntMat].pTextuer[nCntMatTex] = NULL;
			}
		}
	}

}
//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateEffect3D(void)
{
	Player *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		if (g_aEffect3D[nCntMat].bUse == true)
		{
			switch (g_aEffect3D[nCntMat].nType)
			{
			case EFFECT3D_ARM:	// パンチエフェクト
				if (pPlayer->nMotionType == PLAYER_MOTION_TYPE_ATTACK_BIG&&pPlayer->nWeaponState == PLAYERWEAPON_HAMMER)
				{
					g_aEffect3D[nCntMat].pos = D3DXVECTOR3(pPlayer->aModel[17][pPlayer->nWeaponState].mtxWorld._41, pPlayer->aModel[17][pPlayer->nWeaponState].mtxWorld._42, pPlayer->aModel[17][pPlayer->nWeaponState].mtxWorld._43);
					g_aEffect3D[nCntMat].Destrot.x = pPlayer->aModel[6][pPlayer->nWeaponState].rot.x;
				}
				else
				{
					g_aEffect3D[nCntMat].Destrot.x = 0.0f;
					// 現在のマテリアルを取得
					pDevice->GetMaterial(&matDef);
					// マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
					for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
					{
						pMat[nCntMat2].MatD3D.Diffuse.a -= 0.03f;
						if (pMat[nCntMat2].MatD3D.Diffuse.a <= 0.0f)
						{
							g_aEffect3D[nCntMat].bUse = false;
						}
						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
					}
					// マテリアルをデフォルトに戻す
					pDevice->SetMaterial(&matDef);
				}
				break;
			case EFFECT3D_ARM2:	// ロングパンチエフェクト
					g_aEffect3D[nCntMat].pos += g_aEffect3D[nCntMat].move;
					g_aEffect3D[nCntMat].move.y *= 0.9f;
					// 現在のマテリアルを取得
					pDevice->GetMaterial(&matDef);
					// マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
					for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
					{
						pMat[nCntMat2].MatD3D.Diffuse.a -= 0.001f;

						if (pMat[nCntMat2].MatD3D.Diffuse.a <= 0.0f)
						{
							g_aEffect3D[nCntMat].bUse = false;
						}
						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
					}
					// マテリアルをデフォルトに戻す
					pDevice->SetMaterial(&matDef);
				
				break;
			case EFFECT3D_SORD:	// 剣エフェクト
				if (pPlayer->nMotionType == PLAYER_MOTION_TYPE_ATTACK_BIG&&pPlayer->nWeaponState == PLAYERWEAPON_HAND) 
				{
					g_aEffect3D[nCntMat].pos += g_aEffect3D[nCntMat].move;
					g_aEffect3D[nCntMat].Destrot.y += 0.1f;
					g_aEffect3D[nCntMat].move.y *= 0.9f;

				}
				else
				{
					// 現在のマテリアルを取得
					pDevice->GetMaterial(&matDef);
					// マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
					for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
					{
						pMat[nCntMat2].MatD3D.Diffuse.a -= 0.01f;
						if (pMat[nCntMat2].MatD3D.Diffuse.a <= 0.0f)
						{
							g_aEffect3D[nCntMat].bUse = false;
						}
						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
					}
					// マテリアルをデフォルトに戻す
					pDevice->SetMaterial(&matDef);
				}
				break;
			}
			// Xの差分
			if (g_aEffect3D[nCntMat].Destrot.x > D3DX_PI)
			{// 差分が3.14を超えるなら
				g_aEffect3D[nCntMat].Destrot.x -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Destrot.x < -D3DX_PI)
			{// 差分が-3.14を超えるなら
				g_aEffect3D[nCntMat].Destrot.x += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].Diffrot.x = g_aEffect3D[nCntMat].Destrot.x - g_aEffect3D[nCntMat].rot.x;
			if (g_aEffect3D[nCntMat].Diffrot.x > D3DX_PI)
			{// 差分が3.14を超えるなら
				g_aEffect3D[nCntMat].Diffrot.x -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Diffrot.x < -D3DX_PI)
			{// 差分が-3.14を超えるなら
				g_aEffect3D[nCntMat].Diffrot.x += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].rot.x += g_aEffect3D[nCntMat].Diffrot.x * 0.3f;
			if (g_aEffect3D[nCntMat].rot.x > D3DX_PI)
			{// 差分が3.14を超えるなら
				g_aEffect3D[nCntMat].rot.x -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].rot.x < -D3DX_PI)
			{// 差分が-3.14を超えるなら
				g_aEffect3D[nCntMat].rot.x += D3DX_PI * 2.0f;
			}
			// Ｙの差分
			if (g_aEffect3D[nCntMat].Destrot.y > D3DX_PI)
			{// 差分が3.14を超えるなら
				g_aEffect3D[nCntMat].Destrot.y -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Destrot.y < -D3DX_PI)
			{// 差分が-3.14を超えるなら
				g_aEffect3D[nCntMat].Destrot.y += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].Diffrot.y = g_aEffect3D[nCntMat].Destrot.y - g_aEffect3D[nCntMat].rot.y;
			if (g_aEffect3D[nCntMat].Diffrot.y > D3DX_PI)
			{// 差分が3.14を超えるなら
				g_aEffect3D[nCntMat].Diffrot.y -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].Diffrot.y < -D3DX_PI)
			{// 差分が-3.14を超えるなら
				g_aEffect3D[nCntMat].Diffrot.y += D3DX_PI * 2.0f;
			}
			g_aEffect3D[nCntMat].rot.y += g_aEffect3D[nCntMat].Diffrot.y * 0.3f;
			if (g_aEffect3D[nCntMat].rot.y > D3DX_PI)
			{// 差分が3.14を超えるなら
				g_aEffect3D[nCntMat].rot.y -= D3DX_PI * 2.0f;
			}
			if (g_aEffect3D[nCntMat].rot.y < -D3DX_PI)
			{// 差分が-3.14を超えるなら
				g_aEffect3D[nCntMat].rot.y += D3DX_PI * 2.0f;
			}
		}
	}

}
//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawEffect3D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい
																// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		if (g_aEffect3D[nCntMat].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect3D[nCntMat].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect3D[nCntMat].rot.y, g_aEffect3D[nCntMat].rot.x, g_aEffect3D[nCntMat].rot.z);
			D3DXMatrixMultiply(&g_aEffect3D[nCntMat].mtxWorld, &g_aEffect3D[nCntMat].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect3D[nCntMat].pos.x, g_aEffect3D[nCntMat].pos.y, g_aEffect3D[nCntMat].pos.z);
			D3DXMatrixMultiply(&g_aEffect3D[nCntMat].mtxWorld, &g_aEffect3D[nCntMat].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect3D[nCntMat].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_aEffect3DData[g_aEffect3D[nCntMat].nType].pTextuer[nCntMat2]);			
				// モデル(パーツ)の描画
				g_aEffect3DData[g_aEffect3D[nCntMat].nType].pMesh->DrawSubset(nCntMat2);
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}		
		}	
	}

	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化
																// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================================================================================
// モデルの設定
//==============================================================================================================================
void SetEffect3D(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 rot, EFFECT3DTYPE nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
	Player *pPlayer = GetPlayer();

	for (int nCntMat = 0; nCntMat < MAX_EFFECT3D; nCntMat++)
	{
		if (g_aEffect3D[nCntMat].bUse == false)
		{
			g_aEffect3D[nCntMat].pos = pos;
			g_aEffect3D[nCntMat].rot = rot;
			g_aEffect3D[nCntMat].Destrot = rot;
			g_aEffect3D[nCntMat].move = move;
			g_aEffect3D[nCntMat].nType = nType;
			g_aEffect3D[nCntMat].nIdy = nCntMat;

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEffect3DData[g_aEffect3D[nCntMat].nType].pBuffMat->GetBufferPointer();
			for (int nCntMat2 = 0; nCntMat2 < (int)g_aEffect3DData[g_aEffect3D[nCntMat].nType].nNumMat; nCntMat2++)
			{
				pMat[nCntMat2].MatD3D.Diffuse.a = 1.0f;
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
			if (g_aEffect3D[nCntMat].nType == EFFECT3D_ARM2)
			{
				g_aEffect3D[nCntMat].Destrot.y = float(atan2(g_aEffect3D[nCntMat].pos.x - pPlayer->pos.x, g_aEffect3D[nCntMat].pos.z - pPlayer->pos.z));
			}
			g_aEffect3D[nCntMat].bUse = true;
			break;
		}
	}
}
//==============================================================================================================================
// モデルの取得
//==============================================================================================================================
Effect3D *GetEffect3D(void)
{
	return &g_aEffect3D[0];
}
Effect3DData *GetEffect3DData(void)
{
	return &g_aEffect3DData[0];
}
