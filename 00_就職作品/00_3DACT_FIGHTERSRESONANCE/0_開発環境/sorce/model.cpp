//=============================================================================
//
// シーンX処理[3Dポリゴン] [model.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "model.h"
#include "scene2D.h"
#include "debugproc.h"
#include "scene_meshorbit.h"
#include "collision.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR (256)
#define GRAVITY_NUM (0.7f)
#define MODEL_FILENAME ("data/TEXT/model_data.txt")

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CMeshOrbit *CModel::m_pMeshOrbit = NULL;
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CModel::CModel()
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CModel::~CModel()
{
}


//=============================================================================
// 関数名：プレイヤーのパーツ読み込み
// 関数の概要：パーツを読み込む
//=============================================================================
void CModel::Load(void)
{

}

//=============================================================================
// 関数名：プレイヤーのパーツの破棄
// 関数の概要：パーツを読み込む
//=============================================================================
void CModel::Unload(void)
{

}


//=============================================================================
// 関数名：Xファイルシーンの生成
// 関数の概要：Xファイルシーンを生成する
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel  *pModel;
	pModel = new CModel;//シーンクラスの生成
	if (pModel != NULL)
	{
		pModel->Init();
		pModel->m_pos = pos;
		pModel->m_rot = rot;
	}
	return pModel;
}

//=============================================================================
// 関数名：Xファイルシーンの初期化処理
// 関数の概要：Xファイル情報の設定
//=============================================================================
HRESULT CModel::Init(void)
{



	return S_OK;
}

//=============================================================================
// 関数名：Xファイルシーンの終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CModel::Uninit(void)
{


}

//=============================================================================
// 関数名：Xファイルシーンの更新処理
// 関数の概要：--
//=============================================================================
void CModel::Update(void)
{
}


//=============================================================================
// 関数名：Xファイルシーンの描画処理
// 関数の概要：--
//=============================================================================
void CModel::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//親のマトリックス
	D3DXMATRIX mtxParent;


	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	//D3DXMATERIAL *m_pMat;		//マテリアルへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	//親のマトリックスと掛け合わせる
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_pMesh != NULL)
	{
		// マテリアルデータへのポインタを取得
		m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, m_pTexture);

			// モデル(パーツ)の描画
			m_pMesh->DrawSubset(nCntMat);
		}
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// 関数名：位置情報の代入
// 関数の概要：位置情報を代入する
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//=============================================================================
// 関数名：角度情報の代入
// 関数の概要：角度情報を代入する
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 関数名：ワールドマトリックスの代入
// 関数の概要：ワールドマトリックス情報を代入する
//=============================================================================
void CModel::SetMtx(D3DXMATRIX mtx)
{
	m_mtxWorld = m_mtxWorld;
}

//=============================================================================
// 関数名：軌跡の生成
// 関数の概要：軌跡の生成とワールドマトリックス情報を代入する。
//=============================================================================
void CModel::CreateOrbit(void)
{
	m_pMeshOrbit = CMeshOrbit::Create();
	m_pMeshOrbit->SetMtxParent(&m_mtxWorld);
}

//=============================================================================
// 関数名：軌跡の生成
// 関数の概要：軌跡の生成とワールドマトリックス情報を代入する。
//=============================================================================
void CModel::CreateAttackCollision(void)
{
}

//=============================================================================
// 関数名：ワールドマトリックスの取得
// 関数の概要：ワールドマトリックス情報を取得する
//=============================================================================
D3DXMATRIX CModel::GetMatrix(void)
{
	return m_mtxWorld;
}

//=============================================================================
// 関数名：親のモデル
// 関数の概要：親のモデルの設定
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}


//==========================================================================================
// 関数名：モデルの割り当て
// 関数の概要：メッシュ情報、マテリアル情報、マテリアルの数、テクスチャを割り当てる
//===========================================================================================
void CModel::BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat, LPDIRECT3DTEXTURE9 tex)
{
	m_pMesh = mesh;
	m_pBuffMat = buffmat;
	m_nNumMat = nummat;
	m_pTexture = tex;

}


//=============================================================================
// 関数名：色の設定
// 関数の概要：色を設定する
//=============================================================================
void CModel::SetColor(D3DXCOLOR col)
{
	// マテリアルデータへのポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_pMat->MatD3D.Diffuse.r = col.r;
	m_pMat->MatD3D.Diffuse.g = col.g;
	m_pMat->MatD3D.Diffuse.b = col.b;
	m_pMat->MatD3D.Diffuse.a = col.a;

	m_pMat->MatD3D.Ambient.r = col.r;
	m_pMat->MatD3D.Ambient.g = col.g;
	m_pMat->MatD3D.Ambient.b = col.b;
	m_pMat->MatD3D.Ambient.a = col.a;

}

