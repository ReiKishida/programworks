//=============================================================================
//
// モデル(階段代わりのもの)処理 [model.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "model.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MODEL_TEXTUER	"data/TEXTURE/step.jpg"
#define MODEL_1			"data/MODEL/door.x"
#define MODEL_MOVR		(1.0f)

//=============================================================================
// グローバル変数
//=============================================================================
LPD3DXMESH			g_pMeshModel = NULL;				//メッシュ情報（頂点情報）のポインタ
LPD3DXBUFFER		g_pBuffMatModel = NULL;				//マテリアル情報へのポインタ
DWORD				g_nNumMatModel = 0;					//マテリアル情報の数
LPDIRECT3DTEXTURE9	g_pTextureModel = NULL;				//テクスチャへのポインタ
Model				g_Model;							//モデルの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nNumVtx;						//頂点数
	DWORD sizeFVF;						//頂点フォーマットのサイズ
	BYTE *pVtxBuff;						//頂点バッファのポインタ
	g_Model.vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	g_Model.vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_1,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel);

	//頂点数の取得
	nNumVtx = g_pMeshModel->GetNumVertices();
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファのロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		
		if (g_Model.vtxMin.x > vtx.x)
		{//Xの最小値の書き換え
			g_Model.vtxMin.x = vtx.x;
		}
		if (g_Model.vtxMin.z > vtx.z)
		{//Xの最小値の書き換え
			g_Model.vtxMin.z = vtx.z;
		}
		if (g_Model.vtxMax.x < vtx.x)
		{//Xの最小値の書き換え
			g_Model.vtxMax.x = vtx.x;
		}
		if (g_Model.vtxMax.z < vtx.z)
		{//Xの最小値の書き換え
			g_Model.vtxMax.z = vtx.z;
		}

		pVtxBuff += sizeFVF;//サイズ分ポインタを進める
	}
	//頂点バッファのアンロック
	g_pMeshModel->UnlockVertexBuffer();
	
	//テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, MODEL_TEXTUER, &g_pTextureModel);

	// 位置・向き・色の初期設定
	g_Model.pos = D3DXVECTOR3(120.0f, 0.0f, -350.0f);
	g_Model.rot = D3DXVECTOR3(0.0f, D3DX_PI *-0.5, 0.0f);
	g_Model.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_Model.bUse = true;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	// メッシュの開放
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}
	// マテリアルの開放
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
	
	if (g_pTextureModel != NULL)
	{
		g_pTextureModel->Release();
		g_pTextureModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;						//マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Model.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z);
	D3DXMatrixMultiply(&g_Model.mtxWorld, &g_Model.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
	D3DXMatrixMultiply(&g_Model.mtxWorld, &g_Model.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Model.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat <= (int)g_nNumMatModel;nCntMat++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModel);

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
#endif
}
//=============================================================================
// モデルの取得
//=============================================================================
Model*GetModel(void)
{
	return &g_Model;
}

//=============================================================================
// モデルとプレイヤーのあたり判定
//=============================================================================
bool CollisionModel(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	bool bLand = false;
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if (g_Model.bUse == true)
	{//モデルの使用状況
		bLand = false;
		//左壁あたり判定------------------------------
		if (posOld->x + pPlayer->vtxMaxModel.x < g_Model.pos.x + g_Model.vtxMin.x
			&&posOld->z + pPlayer->vtxMaxModel.z < g_Model.pos.z + g_Model.vtxMax.z
			&&posOld->z + pPlayer->vtxMaxModel.z > g_Model.pos.z + g_Model.vtxMin.z)
		{//前回の位置がモデルよりも左側にいる

			if (pos->x + pPlayer->vtxMaxModel.x < g_Model.pos.x + g_Model.vtxMax.x
				&&pos->z + pPlayer->vtxMaxModel.z < g_Model.pos.x + g_Model.vtxMax.z
				&&pos->x + pPlayer->vtxMaxModel.x > g_Model.pos.x + g_Model.vtxMin.x
				&&pos->z + pPlayer->vtxMaxModel.z > g_Model.pos.z + g_Model.vtxMin.z)
			{//左の壁にめり込んだ
				bLand = true;
				pos->x = posOld->x;
			}
		}
		//奥壁あたり判定------------------------------
		if (posOld->z + pPlayer->vtxMinModel.z > g_Model.pos.z + g_Model.vtxMax.z
			&&posOld->x + pPlayer->vtxMinModel.x > g_Model.pos.x + g_Model.vtxMin.x
			&&posOld->x + pPlayer->vtxMinModel.x < g_Model.pos.x + g_Model.vtxMax.x)
		{//前回の位置がモデルよりも奥側にいる
			if (pos->x + pPlayer->vtxMinModel.x > g_Model.pos.x + g_Model.vtxMin.x
				&&pos->z + pPlayer->vtxMinModel.z > g_Model.pos.z + g_Model.vtxMin.z
				&&pos->x + pPlayer->vtxMinModel.x < g_Model.pos.x + g_Model.vtxMax.x
				&&pos->z + pPlayer->vtxMinModel.z < g_Model.pos.z + g_Model.vtxMax.z)
			{//奥壁にめり込んだ
				bLand = true;
				pos->z = posOld->z;
			}
		}

		//右壁あたり判定------------------------------
		if (posOld->x + pPlayer->vtxMinModel.x > g_Model.pos.x + g_Model.vtxMax.x
			&&posOld->z + pPlayer->vtxMinModel.z > g_Model.pos.z + g_Model.vtxMin.z
			&&posOld->z + pPlayer->vtxMinModel.z < g_Model.pos.z + g_Model.vtxMax.z)
		{//前回の位置がモデルよりも右側にいる
			if (pos->x + pPlayer->vtxMinModel.x < g_Model.pos.x + g_Model.vtxMax.x
				&&pos->z + pPlayer->vtxMinModel.z < g_Model.pos.x + g_Model.vtxMax.z
				&&pos->x + pPlayer->vtxMinModel.x > g_Model.pos.x + g_Model.vtxMin.x
				&&pos->z + pPlayer->vtxMinModel.z > g_Model.pos.z + g_Model.vtxMin.z)
			{//右の壁にめり込んだ
				bLand = true;
				pos->x = posOld->x;
			}
		}

		//手前壁あたり判定------------------------------
		if (posOld->z + pPlayer->vtxMaxModel.z < g_Model.pos.z + g_Model.vtxMin.z
			&&posOld->x + pPlayer->vtxMaxModel.x < g_Model.pos.x + g_Model.vtxMax.x
			&&posOld->x + pPlayer->vtxMaxModel.x > g_Model.pos.x + g_Model.vtxMin.x)
		{//前回の位置がモデルよりも手前側にいる
			if (pos->x + pPlayer->vtxMaxModel.x > g_Model.pos.x + g_Model.vtxMin.x
				&&pos->z + pPlayer->vtxMaxModel.z > g_Model.pos.z + g_Model.vtxMin.z
				&&pos->x + pPlayer->vtxMaxModel.x < g_Model.pos.x + g_Model.vtxMax.x
				&&pos->z + pPlayer->vtxMaxModel.z < g_Model.pos.z + g_Model.vtxMax.z)
			{//手前壁にめり込んだ
				bLand = true;
				pos->z = posOld->z;
			}
		}
	}

	return bLand;
}
