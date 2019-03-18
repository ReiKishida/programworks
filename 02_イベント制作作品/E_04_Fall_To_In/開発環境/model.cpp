//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// モデル処理 [model.cpp]
// Author:圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "model.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "motion.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MODEL_NAME			"data//MODEL//01_head.x"		// 読み込むテクスチャのファイル名
#define MODEL_MOVE_SPEED	(1.0f)
#define MOVE_INERTIA		(0.4f)

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 静的メンバ変数宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// コンストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CModel::CModel()
{
	// 値をクリア

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// デストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CModel::~CModel()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CModel::Init(void)
{
	/*m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;	*/
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;

	for(int nCnt = 0; nCnt < MAX_MODEL_TEX; nCnt++)
	{
		m_pTexture[nCnt] = NULL;
	}


	m_pParent = NULL;

	m_mtxWorld = {};

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Uninit(void)
{
	//UnLoad();
}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Update(void)
{
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Draw(D3DXMATRIX mtxParent)
{
	// デバイスの取得
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	if (m_pParent != NULL)
	{ // 親パーツがNULLじゃなかった場合親パーツのワールドマトリックスを取得する
		mtxParent = m_pParent->GetMtxWorld();
	}

	// 親のワールドマトリックスを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//pMat->pTextureFilename[0]
	//g_pTextureModel
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);


}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーション用描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Draw(D3DXMATRIX mtxParent, D3DXVECTOR3 posMotion, D3DXVECTOR3 rotMotion)
{
	// デバイスの取得
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, (m_rot.y + rotMotion.y), (m_rot.x + rotMotion.x), (m_rot.z + rotMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, (m_pos.x + posMotion.x), (m_pos.y + posMotion.y), (m_pos.z + posMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	if (m_pParent != NULL)
	{ // 親パーツがNULLじゃなかった場合親パーツのワールドマトリックスを取得する
		mtxParent = m_pParent->GetMtxWorld();
	}

	// 親のワールドマトリックスを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//pMat->pTextureFilename[0]
	//g_pTextureModel
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーション用描画処理(モデル情報持ち込み
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Draw(D3DXMATRIX mtxParent, D3DXVECTOR3 posMotion, D3DXVECTOR3 rotMotion, CMotionData::Model model)
{
	// デバイスの取得
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, (m_rot.y + rotMotion.y), (m_rot.x + rotMotion.x), (m_rot.z + rotMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, (m_pos.x + posMotion.x), (m_pos.y + posMotion.y), (m_pos.z + posMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	if (m_pParent != NULL)
	{ // 親パーツがNULLじゃなかった場合親パーツのワールドマトリックスを取得する
		mtxParent = m_pParent->GetMtxWorld();
	}

	// 親のワールドマトリックスを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

	//pMat->pTextureFilename[0]
	//g_pTextureModel
	for (int nCntMat = 0; nCntMat < (int)model.nNumMat; nCntMat++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, model.pTexture[nCntMat]);

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデル(パーツ)の描画
		model.pMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 生成処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CModel *CModel::Create(const D3DXVECTOR3 pos)
{
	CModel *pModel = NULL;

	pModel = new CModel;	// CModel型のインスタンス生成
	if (pModel != NULL)
	{
		// 初期化
		pModel->Init();
		// モデル読み込み
		//pModel->Load();

		// 位置設定
		pModel->m_pos = pos;
	}
	else
	{ // メモリ確保失敗
		MessageBox(NULL, "シーンXのメモリ確保に失敗しました", "エラー", (MB_OK));
	}

	return pModel;
}
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モデル上書き処理
// Author:yuki tanaka
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::BindModel(const LPD3DXMESH mesh, const LPD3DXBUFFER buffMat, const DWORD numMat)
{
	//m_pMesh = mesh;
	//m_pBuffMat = buffMat;
	//m_nNumMat = numMat;

}
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//// モデル読み込み処理
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//HRESULT CModel::Load(void)
//{
//	// デバイスの取得
//	CRenderer *pRenderer = NULL;
//	LPDIRECT3DDEVICE9 pDevice = NULL;
//	pRenderer = CManager::GetRenderer();
//
//	if (pRenderer != NULL)
//	{
//		pDevice = pRenderer->GetDevice();
//	}
//
//	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ
//
//	// モデルの読み込み
//	D3DXLoadMeshFromX(MODEL_NAME,
//		D3DXMESH_SYSTEMMEM,
//		pDevice,
//		NULL,
//		&m_pBuffMat,
//		NULL,
//		&m_nNumMat,
//		&m_pMesh);
//
//	// マテリアルデータへのポインタを取得
//	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
//	{
//		if (pMat[nCntMat].pTextureFilename != NULL)
//		{
//			D3DXCreateTextureFromFile(pDevice,
//				pMat[nCntMat].pTextureFilename,
//				&m_pTexture[nCntMat]);
//		}
//	}
//
//	return S_OK;
//}
//
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//// テクスチャ開放処理
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//void CModel::UnLoad(void)
//{
//	// メッシュの開放
//	if (m_pMesh != NULL)
//	{
//		m_pMesh->Release();
//		m_pMesh = NULL;
//	}
//	// マテリアルの開放
//	if (m_pBuffMat != NULL)
//	{
//		m_pBuffMat->Release();
//		m_pBuffMat = NULL;
//	}
//}