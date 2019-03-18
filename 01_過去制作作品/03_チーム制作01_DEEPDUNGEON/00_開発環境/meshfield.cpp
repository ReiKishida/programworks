//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : komatsu keisuke
//
//=============================================================================
#include "meshfield.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MESHFIELD_TEXTURENAME	"data/TEXTURE/Field.jpg"	//表示するファイルの名前
#define HORIZONTAL_MESHFIELD	(2)							//横の分割数	
#define VERTICAL_MESHFIELD		(2)							//縦の分割数
#define MESHFIELD_DEPTH			(500.0f)					//奥行き
#define MESHFIELD_WIDTH			(500.0f)					//幅
#define ALL_VERTEX	((HORIZONTAL_MESHFIELD + 1) * (VERTICAL_MESHFIELD + 1))		//全頂点
#define ALL_POLYGON	((HORIZONTAL_MESHFIELD * VERTICAL_MESHFIELD) * 2 + (VERTICAL_MESHFIELD - 1) * 4)	//全ポリゴン
#define ALL_INDEX	(ALL_POLYGON + 2)						//全インデックス

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
MeshField g_MeshField;									//メッシュフィールドの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_3D *pVtx;
	WORD *pIdx;

	//ポリゴンの初期化
	g_MeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.fWidth = MESHFIELD_WIDTH;
	g_MeshField.fDepth = MESHFIELD_DEPTH;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURENAME, &g_pTextureMeshField);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ALL_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ALL_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファのロック

	//頂点情報
	for (int nCntVertical = 0; nCntVertical < (VERTICAL_MESHFIELD + 1) * 2; nCntVertical += (VERTICAL_MESHFIELD + 1))
	{//縦軸
		for (int nCntHorizontal = 0; nCntHorizontal <= HORIZONTAL_MESHFIELD; nCntHorizontal++)
		{//横軸
			pVtx[nCntVertical + nCntHorizontal].pos = D3DXVECTOR3(-g_MeshField.fWidth + (nCntHorizontal) * g_MeshField.fWidth, 0.0f, g_MeshField.fDepth - (nCntVertical / (HORIZONTAL_MESHFIELD + 1) * g_MeshField.fDepth));
			pVtx[nCntVertical + nCntHorizontal + (HORIZONTAL_MESHFIELD + 1)].pos = D3DXVECTOR3(-g_MeshField.fWidth + (nCntHorizontal * g_MeshField.fWidth), 0.0f, 0.0f - (nCntVertical / (HORIZONTAL_MESHFIELD + 1) * g_MeshField.fDepth));
		}
	}

	for (int nCntMF = 0; nCntMF < ALL_VERTEX; nCntMF++)
	{
		//法線ベクトルの設定
		pVtx[nCntMF].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー
		pVtx[nCntMF].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);

	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffMeshField->Unlock();	//頂点バッファのアンロック

	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);	//インデックスバッファのロック

	int nCntIndex = 0;

	for (int nCntVertical = 0; nCntVertical < VERTICAL_MESHFIELD; nCntVertical++)
	{//縦軸
		for (int nCntHorizontal = 0; nCntHorizontal <= HORIZONTAL_MESHFIELD; nCntHorizontal++, nCntIndex++)
		{//横軸
			pIdx[0] = (HORIZONTAL_MESHFIELD + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			if (nCntHorizontal == HORIZONTAL_MESHFIELD && nCntVertical < VERTICAL_MESHFIELD - 1)
			{
				pIdx += 2;
				pIdx[0] = nCntIndex;
				pIdx[1] = (HORIZONTAL_MESHFIELD + 1) + nCntIndex + (VERTICAL_MESHFIELD - 1);
			}
			pIdx += 2;
		}
	}
	
	 //pIdx[0] = 3; //pIdx[1] = 0;
	 //pIdx[2] = 4; //pIdx[3] = 1;
	 //pIdx[4] = 5; //pIdx[5] = 2;
	 //pIdx[6] = 2; //pIdx[7] = 6;
	 //pIdx[8] = 6; //pIdx[9] = 3;
	//pIdx[10] = 7;//pIdx[11] = 4;
	//pIdx[12] = 8;//pIdx[13] = 5;

	g_pIdxBuffMeshField->Unlock();	//インデックスバッファのアンロック
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshField.mtxWorldMeshField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.rot.x, g_MeshField.rot.y, g_MeshField.rot.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshField.mtxWorldMeshField);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデバイスのデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ALL_VERTEX, 0, ALL_POLYGON);
}