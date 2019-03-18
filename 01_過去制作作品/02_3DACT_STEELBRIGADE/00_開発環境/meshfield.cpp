//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "meshfield.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESH_VERTEX ((g_Mesh_x + 1) * (g_Mesh_z + 1))
#define MESH_INDEX ( (g_Mesh_x * g_Mesh_z )* 2 + (4 * (g_Mesh_z - 1)) + 2 )
#define MESH_POLYGON ( (g_Mesh_x * g_Mesh_z) * 2 + (4 * (g_Mesh_z - 1)) )

#define MESH_WIDTH (200.0f)
#define MESH_DEPTH (200.0f)

#define MESH_X (10)
#define MESH_Z (21)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexMeshField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL; //インデックスバッファへのポインタ

D3DXVECTOR3 g_posMeshField;		//位置(中心座標)
D3DXVECTOR3 g_rotMeshField;		//向き
D3DXMATRIX g_mtxWorldMeshField;	//ワールドマトリックス

int vertex;
int polygon;
int index;

int g_Mesh_x, g_Mesh_z;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_Mesh_x = MESH_X;
	g_Mesh_z = MESH_Z;



	MakeVertexMeshField(pDevice);
	MakeIndexMeshField(pDevice);

	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);




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

	// 頂点バッファの開放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの解放
	if(g_pIdxBuffMeshField != NULL)
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
	D3DXMATRIX mtxrot, mtxtrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, g_pTextureMeshField);


	// メッシュフィールドの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0, 0, MESH_VERTEX, 0, MESH_POLYGON);



}


////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field002.jpg", &g_pTextureMeshField);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESH_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);



	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntMesh_Z = 0; nCntMesh_Z <= g_Mesh_z; nCntMesh_Z++)
		{
			for (int nCntMesh_X = 0; nCntMesh_X <= g_Mesh_x; nCntMesh_X++)
			{

				pVtx[0].pos = D3DXVECTOR3(-MESH_WIDTH + (nCntMesh_X * MESH_WIDTH), 0.0f, MESH_DEPTH - (nCntMesh_Z * MESH_DEPTH));

				//法線の設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.9f, 0.4f, 0.3f, 1.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(nCntMesh_X * 1.0f, nCntMesh_Z * 1.0f);

				pVtx += 1;



			}
		}




	//頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();



}

void MakeIndexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESH_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_Z = 0; nCntIndex_Z < g_Mesh_z; nCntIndex_Z++)
	{
		for (int nCntIndex_X = 0; nCntIndex_X <= g_Mesh_x; nCntIndex_X++, nCntIndex++)
		{
			pIdx[0] = (g_Mesh_x + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex_X == g_Mesh_x && nCntIndex_Z < g_Mesh_z -1)
			{
				pIdx[0] = nCntIndex ;
				pIdx[1] = (g_Mesh_x + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffMeshField->Unlock();

}




