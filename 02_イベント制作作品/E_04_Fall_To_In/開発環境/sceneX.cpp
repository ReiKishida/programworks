////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// モデル処理 [model.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "input.h"
#include "camera.h"
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "line.h"
#include "DebugProc.h"
#include "game.h"
#include "camera.h"
#include "Shader.h"
#include "sound.h"
#include "shadow.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_SCAN  "data/object_scan.txt"		// ファイルの名前読み込み
#define FILE_PRINT "data/object_print.txt"		// ファイルの名前書き込み

//******************************************************************************************************************************
// 静的メンバ変数宣言
//******************************************************************************************************************************
CSceneX::XData CSceneX::m_pModelData[MAX_MODEL_TYPE] = {};
int CSceneX::m_nNumFile = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPriority) :CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = 0;
	m_aMeshPos =NULL; //４頂点座標計算用ポインタ
	m_apLine= NULL;		// 四角形生成用ポインタ
	m_nCameraID = 0;// カメラ番号
	m_ShadowPos = D3DXVECTOR3(0, 0, 0);// 影の位置
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CSceneX::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FILE_SCAN, "r");
	short int nNumModel = 0;
	int nCntModel = 0;
	int nType = 0;
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{// 初期化
		m_pModelData[nCntMat].m_pMesh = NULL;
		m_pModelData[nCntMat].m_pBuffMat = NULL;
		m_pModelData[nCntMat].m_nNumMat = 0;
		m_pModelData[nCntMat].m_pTextuer = NULL;
		m_pModelData[nCntMat].m_pFileName = NULL;
		m_pModelData[nCntMat].m_nNumCollision = 0;
		m_pModelData[nCntMat].m_pCollision = NULL;
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "NUM_MODEL") == 0)		// モデルの数
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_nNumFile);
			}
			// モデルのファイル読み込み
			if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				m_pModelData[nCntModel].m_pFileName = &cData[0];

				// Xファイルの読み込み
				D3DXLoadMeshFromX(m_pModelData[nCntModel].m_pFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_pModelData[nCntModel].m_pBuffMat,
					NULL,
					&m_pModelData[nCntModel].m_nNumMat,
					&m_pModelData[nCntModel].m_pMesh);
				D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)m_pModelData[nCntModel].m_pBuffMat->GetBufferPointer();
				// テクスチャの配列設定
				m_pModelData[nCntModel].m_pTextuer = new LPDIRECT3DTEXTURE9[(int)m_pModelData[nCntModel].m_nNumMat];
				for (int nCntPlayerTex = 0; nCntPlayerTex < (int)m_pModelData[nCntModel].m_nNumMat; nCntPlayerTex++)
				{

					m_pModelData[nCntModel].m_pTextuer[nCntPlayerTex] = NULL;
					if (pMat[nCntPlayerTex].pTextureFilename != NULL)
					{
						// テクスチャの設定
						D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
							pMat[nCntPlayerTex].pTextureFilename,				// ファイルの名前
							&m_pModelData[nCntModel].m_pTextuer[nCntPlayerTex]);// テクスチャへのポインタ
					}
				}
				nCntModel++;

				//m_nNumFile++;		// 読み込んだファイル数の加算
			}
			if (strcmp(cData, "COLLISIONSET") == 0)
			{// 判定セット
				int nNumCube = 0;
				while (strcmp(cData, "END_COLLISIONSET") != 0)
				{// 判定終了までループ
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nType);
					}
					else if (strcmp(cData, "NUMCOLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_pModelData[nType].m_nNumCollision);
						m_pModelData[nType].m_pCollision = new CollisionData[m_pModelData[nType].m_nNumCollision]();
					}
					else if (strcmp(cData, "CUBE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f %f %f %f", &cData[0], &cData[0],
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.x,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.y,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.z,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fWidth,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fHeight,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fDepth);
						// 大きさから最大値最小値を計算
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax = D3DXVECTOR3(m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, m_pModelData[nType].m_pCollision[nNumCube].m_fHeight, m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin = D3DXVECTOR3(-m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, 0.0f, -m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;

						// 大きさから角度と距離を計算[ 左下　左奥　右奥　右下 ]
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[0] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[0] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[1] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[1] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[2] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[2] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[3] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[3] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));

						nNumCube++;
					}
				}

			}
		}
		fclose(pFile);// 終了
	}
	return S_OK;
}
//=============================================================================
// モデルの破棄
//=============================================================================
void CSceneX::UnLoad(void)
{
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{
		// メッシュの開放
		if (m_pModelData[nCntMat].m_pMesh != NULL)
		{
			m_pModelData[nCntMat].m_pMesh->Release();
			m_pModelData[nCntMat].m_pMesh = NULL;
		}
		// マテリアルの開放
		if (m_pModelData[nCntMat].m_pBuffMat != NULL)
		{
			m_pModelData[nCntMat].m_pBuffMat->Release();
			m_pModelData[nCntMat].m_pBuffMat = NULL;
		}
		// テクスチャの開放
		if (m_pModelData[nCntMat].m_pTextuer != NULL)
		{
			// テクスチャの開放
			for (int nCntMatTex = 0; nCntMatTex < int(m_pModelData[nCntMat].m_nNumMat); nCntMatTex++)
			{
				if (m_pModelData[nCntMat].m_pTextuer[nCntMatTex] != NULL)
				{
					m_pModelData[nCntMat].m_pTextuer[nCntMatTex]->Release();
					m_pModelData[nCntMat].m_pTextuer[nCntMatTex] = NULL;
				}
			}
			delete[] m_pModelData[nCntMat].m_pTextuer;
			m_pModelData[nCntMat].m_pTextuer = NULL;
		}
		for (int nCntCollision = 0; nCntCollision < m_pModelData[nCntMat].m_nNumCollision; nCntCollision++)
		{// 判定削除
			if (m_pModelData[nCntMat].m_pCollision != NULL)
			{
				delete[] m_pModelData[nCntMat].m_pCollision;
				m_pModelData[nCntMat].m_pCollision = NULL;
			}
		}

	}
}
//=============================================================================
// モデルの割り当て
//=============================================================================
void CSceneX::BindModel(const int type)
{
	m_pBuffMat = m_pModelData[type].m_pBuffMat;
	// 判定作成
	DeleteCollision();
	m_type = type;
	// 判定設定
	SetCollision();
}

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
HRESULT CSceneX::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ShadowPos = D3DXVECTOR3(0, 0, 0);// 影の位置
	m_aMeshPos = NULL;
	m_apLine = NULL;
	m_nCameraID = 0;// カメラ番号
	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CSceneX::Uninit(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{// 判定があるなら
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// 判定分
			if (m_apLine != NULL)
			{// ラインを削除
				if (m_apLine[nCntCol] != NULL)
				{// 判定の個数分削除
					for (int nLine = 0; nLine < MAX_VTX_LINE; nLine++)
					{// 立方体のラインを削除
						if (m_apLine[nCntCol][nLine] != NULL)
						{
							m_apLine[nCntCol][nLine]->Uninit();
							m_apLine[nCntCol][nLine] = NULL;
						}
					}
					delete[] m_apLine[nCntCol];
					m_apLine[nCntCol] = NULL;
				}
			}
			if (m_aMeshPos != NULL)
			{// 判定用座標を削除
				if (m_aMeshPos[nCntCol] != NULL)
				{// 判定の個数分削除
					delete[] m_aMeshPos[nCntCol];
				}
			}
		}
		if (m_apLine != NULL)
		{// ラインを削除
			delete m_apLine;
			m_apLine = NULL;
		}
		if (m_aMeshPos != NULL)
		{// 判定用座標を削除
			delete m_aMeshPos;
			m_aMeshPos = NULL;
		}
	}
	// オブジェクトの破棄
	Release();

}
//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void CSceneX::Update(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// 判定部分回す
			for (int meshCnt = 0; meshCnt < MAX_VTX; meshCnt++)
			{// ４頂点計算
				// 頂点座標を角度と距離で計算
				m_aMeshPos[nCntCol][meshCnt] = D3DXVECTOR3(sinf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt],
					0.0f,
					cosf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt]) + m_pos;
				if (m_apLine[nCntCol][meshCnt] != NULL)
				{// ライン座標設定「下」
					m_apLine[nCntCol][meshCnt]->SetPos(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f) ,
						m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f) );
				}
				if (m_apLine[nCntCol][4 + meshCnt] != NULL)
				{// ライン座標設定「上」
					m_apLine[nCntCol][4 + meshCnt]->SetPos(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f) ,
						m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f) );
				}
				if (m_apLine[nCntCol][8 + meshCnt] != NULL)
				{// ライン座標設定「横」
					m_apLine[nCntCol][8 + meshCnt]->SetPos(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f) ,
						m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f) );
				}
			}
		}
	}

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
	CCamera *pCamera = CManager::GetCameraMode();
	D3DXVECTOR3 rot = pCamera->GetRot(m_nCameraID);
	LPD3DXEFFECT pShader = CShader::GetEffect();
	LPDIRECT3DTEXTURE9 pTexture = CShader::GetTexture();

	// ライトの設定
	D3DXVECTOR3 lightrot = D3DXVECTOR3((0.7f)*cosf(rot.y) - (0.8f)*sinf(rot.y), -0.5f, (0.7f)*sinf(rot.y) + (0.8f)*cosf(rot.y));
	// 正規化
	D3DXVec3Normalize(&lightrot, &lightrot);

	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ライトON

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクニックの設定
	pShader->SetTechnique("StandardShader");
	// シェーダ開始
	pShader->Begin(0, 0);

	// ビューマトリックスを設定
	D3DXMATRIX matrix = m_mtxWorld *pCamera->GetMtxView(m_nCameraID) * pCamera->GetMtxProjection(m_nCameraID);
	// ライトの設定
	CShader::GetEffect()->SetVector("g_fLightDir", &D3DXVECTOR4(lightrot.x, lightrot.y, lightrot.z, 0)); // 回転行列
	pShader->SetMatrix("g_fMatrix", &matrix);
	// 色の設定
	pShader->SetVector("g_fColor", &D3DXVECTOR4(m_col.r, m_col.g, m_col.b, m_col.a));
	// パスの描画開始
	pShader->BeginPass(0);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat2 = 0; nCntMat2 < (int)m_pModelData[m_type].m_nNumMat; nCntMat2++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
		// テクスチャの設定
		pDevice->SetTexture(0, m_pModelData[m_type].m_pTextuer[nCntMat2]);
		// ランプテクスチャの設定(1番にランプテクスチャを設定)
		pDevice->SetTexture(1, pTexture);

		// モデル(パーツ)の描画
		m_pModelData[m_type].m_pMesh->DrawSubset(nCntMat2);
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
	// パスの描画終了
	pShader->EndPass();
	// シェーダ終了
	pShader->End();

	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ライトON
	CShadow::SetShadow(m_pModelData[m_type].m_pMesh, m_pModelData[m_type].m_nNumMat, m_mtxWorld, &m_ShadowPos);

}
//=============================================================================
// 生成
//=============================================================================
CSceneX *CSceneX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type)
{
	CSceneX *pSceneX = NULL;
	if (!CScene::OverData())
	{// ２Dポリゴン生成
		pSceneX = new CSceneX;
	}
	if (pSceneX != NULL)
	{
		// 初期化処理
		if (FAILED(pSceneX->Init()))
		{
			return NULL;
		}
		pSceneX->SetPos(pos);
		pSceneX->SetType(type);
		pSceneX->BindModel(type);

	}
	return pSceneX;
}
//==============================================================================================================================
// あたり判定
//==============================================================================================================================
bool CSceneX::Collision(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move,const float fHeight)
{
	bool bRand = false;
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{
			D3DXVECTOR3 VecA[4];// メッシュのベクトル
			D3DXVECTOR3 VecC[4];// プレイヤーと頂点のベクトル
			D3DXVECTOR3 VecCOld[4];// プレイヤーと頂点のベクトル
			float fVecLine[4], fVecLineOld[4];	// 頂点ベクトルをプレイヤーが超えたかどうか
			bool bIn = true;
			bool bInOld = true;

			VecA[0] = m_aMeshPos[nCntCol][0]- m_aMeshPos[nCntCol][1];
			VecA[1] = m_aMeshPos[nCntCol][1]- m_aMeshPos[nCntCol][2];
			VecA[2] = m_aMeshPos[nCntCol][2]- m_aMeshPos[nCntCol][3];
			VecA[3] = m_aMeshPos[nCntCol][3]- m_aMeshPos[nCntCol][0];

			int nCntLine = 0;
			float fLine = 10000;

			for (int nCnt = 0; nCnt < MAX_VTX; nCnt++)
			{// プレイヤーとメッシュのベクトル
				VecC[nCnt] = (*pPos  - m_aMeshPos[nCntCol][nCnt]);

				fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] < 0)
				{// どれかが範囲外
					bIn = false;
				}
				VecCOld[nCnt] = (posOld  - m_aMeshPos[nCntCol][nCnt]);

				fVecLineOld[nCnt] = (VecCOld[nCnt].z * VecA[nCnt].x) - (VecCOld[nCnt].x * VecA[nCnt].z);
				if (fVecLineOld[nCnt] < 0)
				{// どれかが範囲外
					bInOld = false;

				}
				float fabsLine = fabsf(fVecLineOld[nCnt]);
				if (fLine >= fabsLine)
				{// 一番近い
					fLine = fabsLine;
					nCntLine = nCnt;
				}
			}

			if (bIn)
			{// 入った
				if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y &&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y <= posOld.y )
				{// 上
					bRand = true;
					move->y = 0.0f;
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y ;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y < pPos->y + fHeight&&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y >= posOld.y + fHeight)
				{// 下
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y - fHeight;
					move->y = 0.0f;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y &&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y <= pPos->y + fHeight)
				{// 上下範囲内
					for (int meshCnt = 0; meshCnt < MAX_VTX; meshCnt++)
					{
						// 現在のベクトルの長さ＋過去のベクトルの長さ
						float All = fabsf(fVecLine[meshCnt]) + fabsf(fVecLineOld[meshCnt]);
						float Now = fabsf(fVecLine[meshCnt]) / All + 0.001f;// 0割防止のため加算
						D3DXVECTOR3 diff = (*pPos - posOld);// 座標と前回座標の差分を抽出
						pPos->x += -diff.x*Now;
						pPos->z += -diff.z*Now;
						move->x = 0;
						move->z = 0;

					}
					//CDebugProc::print("当たった面%d\n", nCntLine);
					//CDebugProc::print("当たった面%d\n", (nCntLine + 1) % MAX_VTX);
					//D3DXVECTOR3 EndPos = *pPos;
					//D3DXVECTOR3 AB = m_aMeshPos[nCntCol][nCntLine] - m_aMeshPos[nCntCol][(nCntLine + 1) % MAX_VTX];
					//D3DXVec3Normalize(&AB, &AB);

					//float fLength = AB.x * VecC[nCntLine].x + AB.z * VecC[nCntLine].z;

					//pPos->x = m_aMeshPos[nCntCol][nCntLine].x + AB.x*fLength;
					//pPos->z = m_aMeshPos[nCntCol][nCntLine].z + AB.z*fLength;
					break;
				}
			}
		}
	}
	return bRand;
}
//==============================================================================================================================
// 反射する
//==============================================================================================================================
bool CSceneX::CollisionReflection(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move, const float fHeight)
{
	bool bRand = false;
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{
			D3DXVECTOR3 VecA[4];// メッシュのベクトル
			D3DXVECTOR3 VecC[4];// プレイヤーと頂点のベクトル
			D3DXVECTOR3 VecCOld[4];// プレイヤーと頂点のベクトル
			float fVecLine[4], fVecLineOld[4];	// 頂点ベクトルをプレイヤーが超えたかどうか
			bool bIn = true;
			bool bInOld = true;

			VecA[0] = m_aMeshPos[nCntCol][0] - m_aMeshPos[nCntCol][1];
			VecA[1] = m_aMeshPos[nCntCol][1] - m_aMeshPos[nCntCol][2];
			VecA[2] = m_aMeshPos[nCntCol][2] - m_aMeshPos[nCntCol][3];
			VecA[3] = m_aMeshPos[nCntCol][3] - m_aMeshPos[nCntCol][0];

			int nCntLine = 0;

			for (int nCnt = 0; nCnt < MAX_VTX; nCnt++)
			{// プレイヤーとメッシュのベクトル
				VecC[nCnt] = (*pPos - m_aMeshPos[nCntCol][nCnt]);

				fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] < 0)
				{// どれかが範囲外
					bIn = false;
				}
				VecCOld[nCnt] = (posOld - m_aMeshPos[nCntCol][nCnt]);

				fVecLineOld[nCnt] = (VecCOld[nCnt].z * VecA[nCnt].x) - (VecCOld[nCnt].x * VecA[nCnt].z);
				if (fVecLineOld[nCnt] < 0)
				{// どれかが範囲外
					bInOld = false;
				}

			}

			if (bIn)
			{// 入った
				if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y &&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y <= posOld.y)
				{// 上
					bRand = true;
					move->y = 0.0f;
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y < pPos->y + fHeight&&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y >= posOld.y + fHeight)
				{// 下
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y - fHeight;
					move->y = 0.0f;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y + fHeight&&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y <= pPos->y + fHeight)
				{// 上下範囲内
						// 現在のベクトルの長さ＋過去のベクトルの長さ
					float All = fabsf(fVecLine[0]) + fabsf(fVecLineOld[0]);
					float Now = fabsf(fVecLine[0]) / All + 0.001f;// 0割防止のため加算
					D3DXVECTOR3 diff = (*pPos - posOld);// 座標と前回座標の差分を抽出
					pPos->x += -diff.x*Now;
					pPos->z += -diff.z*Now;
					move->x = -move->x;
					move->z = -move->z;
					break;
				}
			}
		}
	}
	return bRand;
}
//==============================================================================================================================
// 幅取得
//==============================================================================================================================

float CSceneX::GetHeight(D3DXVECTOR3 *pPos, bool *bIn)
{
	float fHeight = -200;
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{
			D3DXVECTOR3 VecA[4];// メッシュのベクトル
			D3DXVECTOR3 VecC[4];// プレイヤーと頂点のベクトル
			float fVecLine[4];	// 頂点ベクトルをプレイヤーが超えたかどうか
			*bIn = true;

			VecA[0] = m_aMeshPos[nCntCol][0] - m_aMeshPos[nCntCol][1];
			VecA[1] = m_aMeshPos[nCntCol][1] - m_aMeshPos[nCntCol][2];
			VecA[2] = m_aMeshPos[nCntCol][2] - m_aMeshPos[nCntCol][3];
			VecA[3] = m_aMeshPos[nCntCol][3] - m_aMeshPos[nCntCol][0];

			for (int nCnt = 0; nCnt < MAX_VTX; nCnt++)
			{// プレイヤーとメッシュのベクトル
				VecC[nCnt] = (*pPos - m_aMeshPos[nCntCol][nCnt]);

				fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] < 0)
				{// どれかが範囲外
					*bIn = false;
				}
			}
			if (*bIn)
			{// 入った
				fHeight = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y;
				return fHeight;
			}
		}
	}
	return fHeight;
}
//==============================================================================================================================
// 判定設定
//==============================================================================================================================
void CSceneX::ScanCollision(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FILE_SCAN, "r");
	int nType = 0;

	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{// 判定を消す
		for (int nCntCollision = 0; nCntCollision < m_pModelData[nCntMat].m_nNumCollision; nCntCollision++)
		{// 判定削除
			if (m_pModelData[nCntMat].m_pCollision != NULL)
			{
				delete[] m_pModelData[nCntMat].m_pCollision;
				m_pModelData[nCntMat].m_pCollision = NULL;
			}
		}
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "COLLISIONSET") == 0)
			{// 判定セット
				int nNumCube = 0;
				while (strcmp(cData, "END_COLLISIONSET") != 0)
				{// 判定終了までループ
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nType);
					}
					else if (strcmp(cData, "NUMCOLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_pModelData[nType].m_nNumCollision);
						m_pModelData[nType].m_pCollision = new CollisionData[m_pModelData[nType].m_nNumCollision]();
					}
					else if (strcmp(cData, "CUBE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f %f %f %f", &cData[0], &cData[0],
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.x,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.y,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.z,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fWidth,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fHeight,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fDepth);
						// 大きさから最大値最小値を計算
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax = D3DXVECTOR3(m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, m_pModelData[nType].m_pCollision[nNumCube].m_fHeight, m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin = D3DXVECTOR3(-m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, 0.0f, -m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;

						// 大きさから角度と距離を計算[ 左下　左奥　右奥　右下 ]
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[0] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[0] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[1] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[1] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[2] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[2] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[3] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[3] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));

						nNumCube++;

					}
				}

			}
		}
		fclose(pFile);// 終了
	}
}
//==============================================================================================================================
// 判定生成
//==============================================================================================================================
void CSceneX::SetCollision(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{// 判定があるものなら
		if (m_aMeshPos == NULL)
		{// 「判定」個数分生成
			m_aMeshPos = new D3DXVECTOR3*[m_pModelData[m_type].m_nNumCollision]();
		}
		if (m_apLine == NULL)
		{// 「判定」個数分生成
			m_apLine = new CLine**[m_pModelData[m_type].m_nNumCollision]();
		}
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// 判定部分回す
			if (m_aMeshPos[nCntCol] == NULL)
			{// 「頂点」個数分生成
				m_aMeshPos[nCntCol] = new D3DXVECTOR3[MAX_VTX]();
			}
			if (m_apLine[nCntCol] == NULL)
			{// 「頂点」個数分生成
				m_apLine[nCntCol] = new CLine*[MAX_VTX_LINE]();
			}
			for (int meshCnt = 0; meshCnt < MAX_VTX; meshCnt++)
			{// ４頂点計算
			 // 頂点座標を角度と距離で計算
				m_aMeshPos[nCntCol][meshCnt] = D3DXVECTOR3(sinf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt],
					0.0f,
					cosf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt]) + m_pos;

				// ライン生成　立方体
				m_apLine[nCntCol][meshCnt] = CLine::Create(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f), m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f));
				m_apLine[nCntCol][4 + meshCnt] = CLine::Create(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f), m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f));
				m_apLine[nCntCol][8 + meshCnt] = CLine::Create(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f), m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f));
			}
		}
	}

}
//==============================================================================================================================
// 判定削除
//==============================================================================================================================
void CSceneX::DeleteCollision(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{// 判定があるものなら

		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// 判定分
			if (m_apLine != NULL)
			{// ラインを削除
				if (m_apLine[nCntCol] != NULL)
				{// 判定の個数分削除
					for (int nLine = 0; nLine < MAX_VTX_LINE; nLine++)
					{// 立方体のラインを削除
						if (m_apLine[nCntCol][nLine] != NULL)
						{
							m_apLine[nCntCol][nLine]->Uninit();
							m_apLine[nCntCol][nLine] = NULL;
						}
					}
					delete[] m_apLine[nCntCol];
					m_apLine[nCntCol] = NULL;
				}
			}
			if (m_aMeshPos != NULL)
			{// 判定用座標を削除
				if (m_aMeshPos[nCntCol] != NULL)
				{// 判定の個数分削除
					delete[] m_aMeshPos[nCntCol];
				}
			}
		}
		if (m_apLine != NULL)
		{// ラインを削除
			delete m_apLine;
			m_apLine = NULL;
		}
		if (m_aMeshPos != NULL)
		{// 判定用座標を削除
			delete m_aMeshPos;
			m_aMeshPos = NULL;
		}
	}
}
//==============================================================================================================================
// 位置設定
//==============================================================================================================================
void CSceneX::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// 移動量の設定
//==============================================================================================================================
void CSceneX::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}
//==============================================================================================================================
// 回転の設定
//==============================================================================================================================
void CSceneX::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// マトリックスの設定
//==============================================================================================================================
void CSceneX::SetMtx(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}
//==============================================================================================================================
// 色
//==============================================================================================================================
void CSceneX::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}
//==============================================================================================================================
// タイプ設定
//==============================================================================================================================
void CSceneX::SetType(const int type)
{
	m_type = type;
}
//==============================================================================================================================
// カメラ
//==============================================================================================================================
void CSceneX::SetCameraID(const int nID)
{
	m_nCameraID = nID;
}
//==============================================================================================================================
// 影
//==============================================================================================================================
void CSceneX::SetShadowPos(const D3DXVECTOR3 pos)
{
	m_ShadowPos = pos;
}

