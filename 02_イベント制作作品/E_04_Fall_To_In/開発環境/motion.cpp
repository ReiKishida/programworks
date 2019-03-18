//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// モーション処理 [motion.cpp]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "sceneX.h"
#include "calculation.h"
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_LOAD_LINE (2560)
#define BLEND_FRAME		(5)		// モーションのブレンドに必要なフレーム数

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 静的メンバ変数
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
//CMotion::MotionData CMotion::m_apMotion[TYPE_MAX];				// モーションへのポインタ
//
//char *CMotion::m_apMotionInfo[TYPE_MAX] =
//{
//	// アドレス
//	"data//TEXT//motion.txt",		// サンプル00
//};

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// コンストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotion::CMotion(int nPriority) : CScene(nPriority)
{
	// 値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pMotion = NULL;
	m_pCollision = NULL;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// デストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotion::~CMotion()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotion::Init(void)
{
	// オブジェクトタイプを設定
	//CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	// 初期値を設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = m_pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fSpeed = 0.5f;

	m_nMotionType = 0;
	m_nMotionTypeOld = m_nMotionType;

	// モーション情報を取得
	CMotionData::Motion *pMotion = m_pMotionData->GetMotion(m_nMotionType);
	// パーツ数を取得
	int nNumParts = m_pMotionData->GetNumParts();

	for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
	{
		// モデル情報を取得
		CMotionData::Model *pModel = m_pMotionData->GetModel(nCntParts);

		m_pModel[nCntParts] = CModel::Create(pModel->pos);
		m_pModel[nCntParts]->SetRot(pModel->rot);

		m_pModel[nCntParts]->SetMesh(pModel->pMesh);
		m_pModel[nCntParts]->SetBuffMat(pModel->pBuffMat);
		m_pModel[nCntParts]->SetNumMat(pModel->nNumMat);

		D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

		for (int nCntTex = 0; nCntTex < (int)pModel->nNumMat; nCntTex++)
		{
			m_pModel[nCntParts]->SetTexture(pModel->pTexture[nCntTex], nCntTex);
		}

		// 親子関係設定
		int nInxParent = pModel->nIdxParent;
		if (nInxParent != -1)
		{
			m_pModel[nCntParts]->SetParent(m_pModel[nInxParent]);
		}
		else
		{
			m_pModel[nCntParts]->SetParent(NULL);
		}

	}

	// モーションムーブクラスを生成
	m_pMotion = CMotionMove::Create();

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotion::Uninit(void)
{
	Release();

}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotion::Update(void)
{
	m_nMotionTypeOld = m_nMotionType;
	m_posOld = m_pos;

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotion::Draw(void)
{
	// デバイス取得処理
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

										// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// パーツの描画
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Draw(m_mtxWorld, m_pMotion->GetPos(nCntModel), m_pMotion->GetRot(nCntModel), *m_pMotionData->GetModel(nCntModel));
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションムーブのコンストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionMove::CMotionMove()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションムーブのデストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionMove::~CMotionMove()
{

}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションムーブの初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotionMove::Init(void)
{
	m_nKeyCnt = 0;
	m_nFrameCounter = 0;
	m_nAllFrameCounter = 0;
	m_bBlend = false;

	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		m_pos[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_posAdd[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rotAdd[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションムーブの終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionMove::Uninit(void)
{


}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションムーブのモーション更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionMove::MotionUpdate(CMotionData *motionData, int nMotionType, int nMotionTypeOld)
{
	int nFrame = 0; // モーションの分割フレームを記録

	// モーション情報を取得
	CMotionData::Motion *pMotion = motionData->GetMotion(nMotionType);
	// パーツ数を取得
	int nNumParts = motionData->GetNumParts();

	if (nMotionType != nMotionTypeOld)
	{ // 現在のモーションと前回のモーションが違った時
		m_nFrameCounter = 0;	// フレームをリセット
		m_nKeyCnt = 0;			// キーをリセット
		m_nAllFrameCounter = 0;	// 全てのフレームをリセット
		m_bBlend = true;		// ブレンドをする
		nFrame = BLEND_FRAME;	// ブレンドのフレーム数
	}
	else
	{ // 通常時のモーションの分割フレーム
		nFrame = pMotion->aKey[m_nKeyCnt].nFrame;
	}

	if (m_nFrameCounter == 0)
	{ // 1フレームごとの追加量を計算
		for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
		{
			m_posAdd[nCntPartsKey] =
				(pMotion->aKey[m_nKeyCnt].aPos[nCntPartsKey] - m_pos[nCntPartsKey]) / float(nFrame);

			// 差分を計算
			D3DXVECTOR3 DiffRot = pMotion->aKey[m_nKeyCnt].aRot[nCntPartsKey] - m_rot[nCntPartsKey];

			// 円周率超過確認
			DiffRot = CCalculation::PiCheck(DiffRot);

			// 1フレームの追加量を計算
			m_rotAdd[nCntPartsKey] = DiffRot / float(nFrame);

			// 円周率超過確認
			m_rotAdd[nCntPartsKey] = CCalculation::PiCheck(m_rotAdd[nCntPartsKey]);

		}
	}

	// モーション加算
	for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
	{
		m_pos[nCntPartsKey] += m_posAdd[nCntPartsKey];
		m_rot[nCntPartsKey] += m_rotAdd[nCntPartsKey];

		// 円周率超過確認
		m_rot[nCntPartsKey] = CCalculation::PiCheck(m_rot[nCntPartsKey]);
	}


}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションムーブのフレームカウンター増加処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionMove::FrameUpdate(CMotionData *motionData, int nMotionType)
{
	// モーション情報を取得
	CMotionData::Motion *pMotion = motionData->GetMotion(nMotionType);

	// フレームカウンター増加
	m_nFrameCounter++;
	m_nAllFrameCounter++;

	if (m_bBlend == true && m_nFrameCounter == BLEND_FRAME)
	{ // ブレンド時リセット
		m_nFrameCounter = 0;
		m_bBlend = false;
		m_nKeyCnt = 1;
	}
	else if (m_nFrameCounter == pMotion->aKey[m_nKeyCnt].nFrame)
	{ // フレームカウンターリセット
		m_nFrameCounter = 0;
		if (m_nKeyCnt == pMotion->nNumKey - 1
			&& pMotion->bLoop == false)
		{ //ループせずかつ最大キー数を超える

		}
		else
		{
			m_nKeyCnt = (m_nKeyCnt + 1) % pMotion->nNumKey;
		}
	}

	if (m_nAllFrameCounter == pMotion->nAllFrame)
	{
		m_nAllFrameCounter = 0;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションムーブの生成処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionMove *CMotionMove::Create(void)
{
	CMotionMove *pMotionMove = NULL;

	pMotionMove = new CMotionMove;	// CMotionMove型のインスタンス生成
	if (pMotionMove != NULL)
	{
		// 初期化処理
		pMotionMove->Init();
	}
	else
	{ // メモリ確保失敗
		MessageBox(NULL, "モーションのメモリ確保に失敗しました", "エラー", (MB_OK));
	}

	return pMotionMove;
}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションデータのコンストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionData::CMotionData()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションデータのデストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionData::~CMotionData()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションデータの初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotionData::Init(void)
{
	// 初期化
	// モデル情報
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		m_aModel[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aModel[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aModel[nCntParts].pMesh = NULL;
		m_aModel[nCntParts].pBuffMat = NULL;
		m_aModel[nCntParts].nNumMat = 0;
		for (int nCntTex = 0; nCntTex < MAX_MOTION_TEX; nCntTex++)
		{
			m_aModel[nCntParts].pTexture[nCntTex] = NULL;
		}
		m_aModel[nCntParts].nIdxParent = 0;
	}

	// モーション情報
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		m_aMotion[nCntMotion].bLoop = false;
		m_aMotion[nCntMotion].nNumKey = 0;
		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			m_aMotion[nCntMotion].aKey[nCntKey].nFrame = 0;
		}
		m_aMotion[nCntMotion].nAllFrame = 0;
	}

	// パーツ数
	m_nNumParts = 0;


	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションtxtデータの読み込み
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotionData::Load(char *cTextName)
{

	// ローカル変数宣言
	FILE *pFile;
	char cLine[MAX_LOAD_LINE];			// 1行の一時的読み込み
	char cData[MAX_LOAD_LINE];			// 一時的読み込み
										//char cData[MAX_LOAD_LINE];			// 一時的読み込み

										// デバイス取得
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	int nCntMotion = 0;			// モーション数
	int nCntModel = 0;

	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ

	pFile = fopen(cTextName, "r"); // ファイルを開く

	if (pFile != NULL) // pFileがNULL(空白）ではないときを真
	{ // ファイルが開けた際の処理ss
		while (fgets(cLine, MAX_LOAD_LINE, pFile) != NULL)
		{
			sscanf(cLine, "%s", cData);
			if (strcmp(cData, "NUM_MODEL") == 0)
			{
				sscanf(cLine, "%s %s %d", cData, cData, &m_nNumParts);
			}
			else if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				sscanf(cLine, "%s %s %s", cData, cData, cData);

				D3DXLoadMeshFromX(cData,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_aModel[nCntModel].pBuffMat,
					NULL,
					&m_aModel[nCntModel].nNumMat,
					&m_aModel[nCntModel].pMesh);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)m_aModel[nCntModel].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_aModel[nCntModel].nNumMat; nCntMat++)
				{
					if (pMat[nCntMat].pTextureFilename != NULL)
					{
						D3DXCreateTextureFromFile(pDevice,
							pMat[nCntMat].pTextureFilename,
							&m_aModel[nCntModel].pTexture[nCntMat]);
					}
				}

				nCntModel++;
			}
			else if (strcmp(cData, "CHARACTERSET") == 0)
			{
				while (strcmp(cData, "END_CHARACTERSET") != 0)
				{
					fgets(cLine, MAX_LOAD_LINE, pFile);

					sscanf(cLine, "%s", cData);

					if (strcmp(cData, "PARTSSET") == 0)
					{
						int nIndex = 0;
						while (strcmp(cData, "END_PARTSSET") != 0)
						{
							fgets(cLine, MAX_LOAD_LINE, pFile);
							sscanf(cLine, "%s", cData);

							if (strcmp(cData, "INDEX") == 0)
							{
								sscanf(cLine, "%s %s %d", cData, cData, &nIndex);
							}
							else if (strcmp(cData, "PARENT") == 0)
							{
								sscanf(cLine, "%s %s %d", cData, cData, &m_aModel[nIndex].nIdxParent);
							}
							else if (strcmp(cData, "POS") == 0)
							{
								sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aModel[nIndex].pos.x,
									&m_aModel[nIndex].pos.y,
									&m_aModel[nIndex].pos.z);

								int nDAta = 0;
							}
							else if (strcmp(cData, "ROT") == 0)
							{
								sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aModel[nIndex].rot.x,
									&m_aModel[nIndex].rot.y,
									&m_aModel[nIndex].rot.z);
							}
						}
					}
				}
			}
			else if (strcmp(cData, "MOTIONSET") == 0)
			{
				int nCntKey = 0;
				while (strcmp(cData, "END_MOTIONSET") != 0)
				{
					fgets(cLine, MAX_LOAD_LINE, pFile);
					sscanf(cLine, "%s", cData);

					if (strcmp(cData, "LOOP") == 0)
					{
						int nLoop;
						sscanf(cLine, "%s %s %d", cData, cData, &nLoop);

						if (nLoop == 0)
						{
							m_aMotion[nCntMotion].bLoop = false;
						}
						else
						{
							m_aMotion[nCntMotion].bLoop = true;
						}

					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(cLine, "%s %s %d", cData, cData, &m_aMotion[nCntMotion].nNumKey);
					}
					else if (strcmp(cData, "KEYSET") == 0)
					{
						int nCntParts = 0;
						while (strcmp(cData, "END_KEYSET") != 0)
						{
							fgets(cLine, MAX_LOAD_LINE, pFile);
							sscanf(cLine, "%s", cData);

							if (strcmp(cData, "FRAME") == 0)
							{
								sscanf(cLine, "%s %s %d", cData, cData, &m_aMotion[nCntMotion].aKey[nCntKey].nFrame);

								// モーション全体にかかるフレーム数を計算するためにキーごとのフレームを足す
								m_aMotion[nCntMotion].nAllFrame += m_aMotion[nCntMotion].aKey[nCntKey].nFrame;
							}
							else if (strcmp(cData, "KEY") == 0)
							{
								while (strcmp(cData, "END_KEY") != 0)
								{
									fgets(cLine, MAX_LOAD_LINE, pFile);
									sscanf(cLine, "%s", cData);
									if (strcmp(cData, "POS") == 0)
									{
										sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts].x,
											&m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts].y,
											&m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts].z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts].x,
											&m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts].y,
											&m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts].z);
									}
								}
								nCntParts++;
							}
						}
						nCntKey++;
					}
				}
				nCntMotion++;
			}
		}
		fclose(pFile); // ファイルを閉じる
	}
	else
	{ // ファイルが開けなかった際の処理
	}


	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションtxtデータの開放処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionData::UnLoad(void)
{
	//for (int nCntModel = 0; nCntModel < sizeof m_aModel / sizeof(Model); nCntModel++)
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		// テクスチャの開放
		for (int nCntTex = 0; nCntTex < MAX_MOTION_TEX; nCntTex++)
		{
			if (m_aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				m_aModel[nCntModel].pTexture[nCntTex]->Release();
				m_aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		// メッシュの開放
		if (m_aModel[nCntModel].pMesh != NULL)
		{
			m_aModel[nCntModel].pMesh->Release();
			m_aModel[nCntModel].pMesh = NULL;
		}
		// マテリアルの開放
		if (m_aModel[nCntModel].pBuffMat != NULL)
		{
			m_aModel[nCntModel].pBuffMat->Release();
			m_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションデータの生成処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionData *CMotionData::Create(void)
{
	CMotionData *pMotionData = NULL;

	pMotionData = new CMotionData;	// CMotion型のインスタンス生成
	if (pMotionData != NULL)
	{
		// 初期化処理
		pMotionData->Init();
	}
	else
	{ // メモリ確保失敗
		MessageBox(NULL, "モーションのメモリ確保に失敗しました", "エラー", (MB_OK));
	}

	return pMotionData;
}