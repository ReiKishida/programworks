//=============================================================================
//
// 植物の処理 [environment.cpp]
// Author : 木下 領太
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "environment.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "meshWall.h"
#include "camera.h"
//*********************************************************************************
// マクロ定義
//*********************************************************************************
#define FILE_SCAN  "data/object_scan.txt"		// ファイルの名前読み込み
#define MAX_CHAR (1024)

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9	*CEnvironmentBillboard::m_apTexture = NULL;
int					CEnvironmentBillboard::m_nNumTexture = 0;
CEnvironmentBillboard *CEnvironment::m_apEnviroment[MAX_ENVIROMENT] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CEnvironmentBillboard::CEnvironmentBillboard(int nPriority):CBillboard(PRIORITY_ENVIROMENT)
{
	m_type = 0;
	m_bUse = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnvironmentBillboard::~CEnvironmentBillboard()
{
}

//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CEnvironmentBillboard::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FILE_SCAN, "r");		// ファイルの名前の読み込み
	short int nCntTexture = 0;

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// 行の読み込み
			if (strcmp(cData, "NUM_BILLBOARD_TEXTURE") == 0)		// モデルの数
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_nNumTexture);
				m_apTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture]();
			}
			// モデルのファイル読み込み
			if (strcmp(cData, "BILLBOARD_FILENAME") == 0 && m_nNumTexture > nCntTexture)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				// テクスチャの生成
				D3DXCreateTextureFromFile(pDevice,
					&cData[0],
					&m_apTexture[nCntTexture]);
				nCntTexture++;
			}

		}
		fclose(pFile);// 終了
	}

	return S_OK;
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CEnvironmentBillboard::UnLoad(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnvironmentBillboard::Init()
{
	if (FAILED(CBillboard::Init()))
	{
		return E_FAIL;
	}

	// 情報の初期化
	m_type = 0;
	m_bUse = false;
	SetObjType(OBJTYPE_ENVIROMENT);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnvironmentBillboard::Uninit(void)
{
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnvironmentBillboard::Update(void)
{
	if (!m_bUse) return;

}

//=============================================================================
// 描画処理
//=============================================================================
void CEnvironmentBillboard::Draw(void)
{
	if (!m_bUse) return;

	CBillboard::Draw();
}

//=============================================================================
// 弾の設定
//=============================================================================
void CEnvironmentBillboard::Set(const int nIdy,const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int type)
{
	m_bUse = true;
	m_type = type;
	BindTexture(m_apTexture[m_type]);

	SetPos(pos);
	SetRot(0.0f);
	SetVtxType(CBillboard::VTXTYPE_BASE);
	SetSize(fWidth, fHeight);
	SetVtx();
}

//=============================================================================
// 使用中かの設定
//=============================================================================
void CEnvironmentBillboard::SetUse(const bool bUse)
{
	m_bUse = bUse;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CEnvironment::CEnvironment() : CScene(PRIORITY_ENVIROMENT)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnvironment::~CEnvironment()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CEnvironment *CEnvironment::Create(void)
{
	CEnvironment *pBullet = NULL;

	// クラスの生成
	pBullet = new CEnvironment;

	if (pBullet == NULL)
	{// 生成できなかった場合
		return NULL;
	}

	// 初期化処理
	if (FAILED(pBullet->Init()))
	{
		return NULL;
	}

	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnvironment::Init(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_ENVIROMENT; nCntBullet++)
	{
		if (m_apEnviroment[nCntBullet] != NULL) continue;

		m_apEnviroment[nCntBullet] = new CEnvironmentBillboard;

		if (m_apEnviroment[nCntBullet] == NULL)
		{// 生成できなかった場合
			return E_FAIL;
		}

		// 初期化処理
		if (FAILED(m_apEnviroment[nCntBullet]->Init()))
		{
			return E_FAIL;
		}
		m_apEnviroment[nCntBullet]->SetVtx();
	}

	SetObjType(OBJTYPE_ENVIROMENT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnvironment::Uninit(void)
{
	// クラスの終了処理
	for (int nCntBullet = 0; nCntBullet < MAX_ENVIROMENT; nCntBullet++)
	{
		if (m_apEnviroment[nCntBullet] == NULL) continue;

		m_apEnviroment[nCntBullet]->Uninit();
		m_apEnviroment[nCntBullet] = NULL;
	}

	// このクラスの開放処理
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnvironment::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnvironment::Draw(void)
{
}

//=============================================================================
// 弾の設定
//=============================================================================
int CEnvironment::Set(const int nIdy,const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int type)
{
	for (int nCntCall = 0; nCntCall < MAX_ENVIROMENT; nCntCall++)
	{
		if (m_apEnviroment[nCntCall] == NULL) continue;

		if (!m_apEnviroment[nCntCall]->GetUse())
		{// 未使用の場合
			m_apEnviroment[nCntCall]->Set(nIdy, pos, fWidth, fHeight, type);
			return nCntCall;
		}
	}
	return 0;
}
//=============================================================================
// 削除
//=============================================================================
void CEnvironment::Delete(const int nID)
{
	if (m_apEnviroment[nID] != NULL)
	{
		m_apEnviroment[nID]->SetUse(false);
	}
}
//=============================================================================
// 読み込み
//=============================================================================
void CEnvironment::SetScan(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FILE_SCAN, "r");		// ファイルの名前の読み込み
	int nNumAll = 0;
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// 行の読み込み

			if (strcmp(cData, "SETBILLBOARD") == 0)
			{// SET_FIELDだった場合
				int nType = 0;
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
				D3DXVECTOR2 size = D3DXVECTOR2(0.0f,0.0f);
				while (strcmp(cData, "END_SETBILLBOARD") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{// 種類の読み込み
						sscanf(&cLine[0], "%s %s %d ", &cData[0], &cData[0], &nType);
					}
					if (strcmp(cData, "POS") == 0)
					{// 種類の読み込み
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(cData, "SIZE") == 0)
					{// 種類の読み込み
						sscanf(&cLine[0], "%s %s %f %f ", &cData[0], &cData[0], &size.x, &size.y);
					}

				}
				CEnvironment::Set(nNumAll, pos, size.x, size.y, nType);
				nNumAll++;
			}
		}
		fclose(pFile);// 終了
	}
}