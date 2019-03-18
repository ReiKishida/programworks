////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッシュ床処理 [meshField.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "meshWall.h"
#include "input.h"
#include "manager.h"
#include "line.h"
#include "renderer.h"
#include "DebugProc.h"
#include "input.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_SCAN  "data/object_scan.txt"		// ファイルの名前読み込み

#define MAX_MESHFIELD		(100)
#define MAX_MESHFIELDTYPE	(24)
//*********************************************************************************
// 静的メンバ変数宣言
//*********************************************************************************
LPDIRECT3DTEXTURE9	*CMeshWall::m_pTextureThis = NULL;
//=============================================================================
// コンストラクタ
//=============================================================================
CMeshWall::CMeshWall(int nPriority):CScene(PRIORITY_MESHWALL)
{
	m_pTexture = NULL;								// テクスチャへのポインタ
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pNor = NULL;	// 法線
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_fWidth = 0;							// 幅
	m_fHeight = 0;							// 高さ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転量
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_afAngle[nCnt] = 0.0f;
		m_afLength[nCnt] = 0.0f;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshWall::~CMeshWall()
{

}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CMeshWall::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice= CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FILE_SCAN, "r");		// ファイルの名前の読み込み
	short int nNumTexture = 0;
	short int nCntTexture = 0;

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// 行の読み込み
			if (strcmp(cData, "NUM_WALL_TEXTURE") == 0)		// モデルの数
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %hd", &cData[0], &cData[0], &nNumTexture);
				m_pTextureThis = new LPDIRECT3DTEXTURE9[nNumTexture]();
			}
			// モデルのファイル読み込み
			if (strcmp(cData, "WALL_FILENAME") == 0 && nNumTexture > nCntTexture)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				// テクスチャの生成
				D3DXCreateTextureFromFile(pDevice,
					&cData[0],
					&m_pTextureThis[nCntTexture]);
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
void CMeshWall::UnLoad(void)
{
	// テクスチャの破棄
	if (m_pTextureThis != NULL)
	{
		delete[] m_pTextureThis;
		m_pTextureThis = NULL;
	}
}
//=============================================================================
// テクスチャ割り当て
//=============================================================================
void CMeshWall::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
HRESULT CMeshWall::Init(void)
{
	m_pVtxBuff = NULL;									// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;									// インデックスバッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色
	m_fWidth = 0;											// 幅
	m_fHeight = 0;											// 高さ
	m_fDepth = 0;											// 奥行き
	m_nBlock_X = 0;										// ブロックX
	m_nBlock_Y = 0;										// ブロックZ
	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Y + 1);// 頂点数
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1) + 2;// インデックス数
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1);	// ポリゴン数
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_afAngle[nCnt] = 0.0f;
		m_afLength[nCnt] = 0.0f;
	}
	SetObjType(OBJTYPE_MESHWALL);				// オブジェクトタイプ設定

	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CMeshWall::Uninit(void)
{

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// インデックスバッファバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	Release();
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void CMeshWall::Update(void)
{

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void CMeshWall::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい

	// Zバッファへの書き込み
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
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
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumIndex, 0, m_nNumPolygon);
	// Zバッファへの書き込み
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化

}
//=============================================================================
// 生成
//=============================================================================
CMeshWall *CMeshWall::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXCOLOR col,const float fWidth, const float fHeight, const float fDepth, const int nBlockX, const int nBlockY,int type)
{
	CMeshWall *pMeshWall = NULL;
	if (!CScene::OverData())
	{// 壁生成
		pMeshWall = new CMeshWall;
	}
	if (pMeshWall != NULL)
	{
		// 初期化処理
		if (FAILED(pMeshWall->Init()))
		{
			return NULL;
		}
		pMeshWall->SetPos(pos);
		pMeshWall->SetColor(col);
		pMeshWall->SetSize(fWidth, fHeight, fDepth);
		pMeshWall->SetBlock(nBlockX, nBlockY);
		pMeshWall->SetRot(rot);
		pMeshWall->m_type = type;
		pMeshWall->BindTexture(m_pTextureThis[type]);
		pMeshWall->MakeVtx();
	}
	return pMeshWall;
}
//==============================================================================================================================
// 座標設定
//==============================================================================================================================
void CMeshWall::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// 大きさ設定
//==============================================================================================================================
void CMeshWall::SetSize(const float fWidth, const float fHeight, const float fDepth)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;

	m_afAngle[0] = atan2f(-m_fWidth / 2, 0);
	m_afAngle[1] = atan2f(m_fWidth / 2, 0);
	m_afAngle[2] = atan2f(m_fWidth / 2, 50);
	m_afAngle[3] = atan2f(-m_fWidth / 2, 50);
	m_afLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2));
	m_afLength[1] = sqrtf((m_fWidth / 2)*(m_fWidth / 2));
	m_afLength[2] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + 50 * 50);
	m_afLength[3] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + 50 * 50);

}
//==============================================================================================================================
// 回転設定
//==============================================================================================================================
void CMeshWall::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// 分割数の設定
//==============================================================================================================================
void CMeshWall::SetBlock(const int nBlockX, const int nBlockY)
{
	m_nBlock_X = nBlockX;
	m_nBlock_Y = nBlockY;
}
//==============================================================================================================================
// 色の設定
//==============================================================================================================================
void CMeshWall::SetColor(const D3DXCOLOR col)
{
	if (m_col == col)return;
	m_col = col;
}
//==============================================================================================================================
// 頂点設定
//==============================================================================================================================
void CMeshWall::SetVtx(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 各頂点の法線
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntY = 0; nCntY < m_nBlock_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntY*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntY) + nCntX;
			int nLDOWN = (nCntY * (m_nBlock_X + 1)) + 1 + nCntX;
			// 左面
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// 右面
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
		}

	}

	int nCntCenter = 0;
	int nBlockCounter = 0;
	// 法線の設定
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		//	pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		if (nCnt == 0)
		{// 最初
			pVtx[nCnt].nor = (m_pNor[nCnt] + m_pNor[nCnt + 1]) / 2;
		}
		else if (nCnt == m_nNumVertex - 1)
		{// 最後
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// 右上
		 // 番号を二倍して１引く
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Y)
		{// 左下
		 // Z増加で「２増」X増加で「倍」
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Y - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// 左端< X < 右端
		{// 中の上端￣
			int nIdy = nCnt + nCnt - 1;	// 番号を二倍して１引く
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Y&&nCnt < m_nNumVertex - 1)
		{// 中の下端＿
			int nIdypolygon = (m_nBlock_Y - 1) * 2 * m_nBlock_X;		// 左端のポリゴン番号
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (左端の番号 - 番号 -1 )*3でポリゴンを進める
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Y&&nCnt % (m_nBlock_X + 1) == 0)
		{// 端の中の左	(横のブロック数+1 が最初の頂点であり、倍増で次の頂点。
		 // ポリゴンは(その頂点÷(ブロック数+1))の割合を出して利用する。
			int nBlock = nCnt / (m_nBlock_X + 1);	// 割合計算
			int nIdy = m_nBlock_X * 2;			// 増加量
			pVtx[nCnt].nor = (m_pNor[(nBlock - 1)*nIdy] + m_pNor[(nBlock)*nIdy] + m_pNor[(nBlock)*nIdy + 1]) / 3;
		}
		else if (nCnt != 0 && ((m_nNumVertex - 1) - nCnt) % (m_nBlock_X + 1) == 0)
		{// 端の中の右
			int nBlock = (nCnt - m_nBlock_X) / (m_nBlock_X + 1);	// 割合計算
			int nIdy = m_nBlock_X * 2;								// 増加量
			pVtx[nCnt].nor = (m_pNor[nIdy*nBlock - 1] + m_pNor[nIdy*nBlock - 2] + m_pNor[nIdy*(nBlock + 1) - 1]) / 3;
		}
		else
		{// 真ん中
			pVtx[nCnt].nor = (m_pNor[0 + nCntCenter] + m_pNor[1 + nCntCenter] + m_pNor[2 + nCntCenter]
				+ m_pNor[0 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[1 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[2 + m_nBlock_X * 2 + 1 + nCntCenter]) / 6;
			nCntCenter += 2;
			nBlockCounter++;
			if (nBlockCounter >= m_nBlock_X)
			{
				nBlockCounter = 0;
				nCntCenter += 2;
			}
		}
	}
	// 頂点カラーの設定
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].col = m_col;
	}
	// ブロックごとに一枚配置するテクスチャ
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		// テクスチャの設定
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt % (m_nBlock_X + 1)), 0.0f + (nCnt / (m_nBlock_X + 1)));
	}
	for (int nCntV = 0; nCntV < m_nBlock_Y + 1; nCntV++)
	{// テクスチャの設定
		for (int nCntH = 0; nCntH < m_nBlock_X + 1; nCntH++)
		{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
		 // テクスチャの設定
			pVtx[nCntH + (m_nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / m_nBlock_X) / 100.0f, float(nCntV * 100 / m_nBlock_Y) / 100.0f);
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//==============================================================================================================================
// 頂点生成
//==============================================================================================================================
void CMeshWall::MakeVtx(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Y + 1);// 頂点数
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1) + 2;// インデックス数
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1);	// ポリゴン数

	m_pNor = new D3DXVECTOR3[m_nNumPolygon];

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntPos = 0; nCntPos < m_nNumVertex; nCntPos++)
	{
		pVtx[nCntPos].pos = D3DXVECTOR3(float(-((m_nBlock_X * m_fWidth) / 2) + m_fWidth * (nCntPos % (m_nBlock_X + 1))),
			float((m_fHeight * m_nBlock_Y)) - (m_fHeight)*(nCntPos / (m_nBlock_X + 1)),
			0.0f);
	}



	// 各頂点の法線
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntY = 0; nCntY < m_nBlock_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntY*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntY) + nCntX;
			int nLDOWN = (nCntY * (m_nBlock_X + 1)) + 1 + nCntX;
			// 左面
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// 右面
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
		}

	}


	int nCntCenter = 0;
	int nBlockCounter = 0;
	// 法線の設定
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		//	pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		if (nCnt == 0)
		{// 最初
			pVtx[nCnt].nor = (m_pNor[nCnt] + m_pNor[nCnt + 1]) / 2;
		}
		else if (nCnt == m_nNumVertex - 1)
		{// 最後
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// 右上
		 // 番号を二倍して１引く
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Y)
		{// 左下
		 // Z増加で「２増」X増加で「倍」
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Y - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// 左端< X < 右端
		{// 中の上端￣
			int nIdy = nCnt + nCnt - 1;	// 番号を二倍して１引く
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Y&&nCnt < m_nNumVertex - 1)
		{// 中の下端＿
			int nIdypolygon = (m_nBlock_Y - 1) * 2 * m_nBlock_X;		// 左端のポリゴン番号
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (左端の番号 - 番号 -1 )*3でポリゴンを進める
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Y&&nCnt % (m_nBlock_X + 1) == 0)
		{// 端の中の左	(横のブロック数+1 が最初の頂点であり、倍増で次の頂点。
		 // ポリゴンは(その頂点÷(ブロック数+1))の割合を出して利用する。
			int nBlock = nCnt / (m_nBlock_X + 1);	// 割合計算
			int nIdy = m_nBlock_X * 2;			// 増加量
			pVtx[nCnt].nor = (m_pNor[(nBlock - 1)*nIdy] + m_pNor[(nBlock)*nIdy] + m_pNor[(nBlock)*nIdy + 1]) / 3;
		}
		else if (nCnt != 0 && ((m_nNumVertex - 1) - nCnt) % (m_nBlock_X + 1) == 0)
		{// 端の中の右
			int nBlock = (nCnt - m_nBlock_X) / (m_nBlock_X + 1);	// 割合計算
			int nIdy = m_nBlock_X * 2;								// 増加量
			pVtx[nCnt].nor = (m_pNor[nIdy*nBlock - 1] + m_pNor[nIdy*nBlock - 2] + m_pNor[nIdy*(nBlock + 1) - 1]) / 3;
		}
		else
		{// 真ん中
			pVtx[nCnt].nor = (m_pNor[0 + nCntCenter] + m_pNor[1 + nCntCenter] + m_pNor[2 + nCntCenter]
				+ m_pNor[0 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[1 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[2 + m_nBlock_X * 2 + 1 + nCntCenter]) / 6;
			nCntCenter += 2;
			nBlockCounter++;
			if (nBlockCounter >= m_nBlock_X)
			{
				nBlockCounter = 0;
				nCntCenter += 2;
			}
		}
	}
	// 頂点カラーの設定
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].col = m_col;
	}
	// ブロックごとに一枚配置するテクスチャ
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		// テクスチャの設定
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt % (m_nBlock_X + 1)), 0.0f + (nCnt / (m_nBlock_X + 1)));
	}

	//for (int nCntV = 0; nCntV < m_nBlock_Y + 1; nCntV++)
	//{// テクスチャの設定
	//	for (int nCntH = 0; nCntH < m_nBlock_X + 1; nCntH++)
	//	{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
	//	 // テクスチャの設定
	//		pVtx[nCntH + (m_nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / m_nBlock_X) / 100.0f, float(nCntV * 100 / m_nBlock_Z) / 100.0f);
	//	}
	//}
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


	WORD *pIdx;	// インデックスデータへのポインタ

				// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD)*m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスバッファをロックし頂点データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int nCntIdx = 0, nCntPos = 0; nCntIdx < m_nNumIndex; nCntIdx += 2)
	{
		if ((nCntIdx / 2) % (m_nBlock_X + 2) == m_nBlock_X + 1)
		{
			pIdx[nCntIdx] = nCntPos - 1;
			pIdx[nCntIdx + 1] = nCntPos + m_nBlock_X + 1;
		}
		else
		{
			pIdx[nCntIdx] = nCntPos + m_nBlock_X + 1;
			pIdx[nCntIdx + 1] = nCntPos;
			nCntPos++;
		}
	}
	// 頂点バッファをアンロックする
	m_pIdxBuff->Unlock();

}
//==============================================================================================================================
// 頂点生成
//==============================================================================================================================
bool CMeshWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, const float fWidth, const float fHeight)
{
	bool bHit = false;
	if (pPos->x == posOld.x && pPos->z == posOld.z)return false;

	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 VecA[4];// メッシュのベクトル
	D3DXVECTOR3 VecC[4];// プレイヤーと頂点のベクトル
	D3DXVECTOR3 VecCOld[4];// プレイヤーと頂点のベクトル
	float fVecLine[4];	// 頂点ベクトルをプレイヤーが超えたかどうか
	float fVecLineOld[4];	// 頂点ベクトルをプレイヤーが超えたかどうか
	D3DXVECTOR3 meshPos[4];

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		meshPos[nCnt] = D3DXVECTOR3(sinf(m_rot.y + m_afAngle[nCnt])*m_afLength[nCnt], pVtx[nCnt].pos.y, cosf(m_rot.y + m_afAngle[nCnt])*m_afLength[nCnt]) + m_pos;
	}

	VecA[0] = meshPos[0] - meshPos[1];
	VecA[1] = meshPos[1] - meshPos[2];
	VecA[2] = meshPos[2] - meshPos[3];
	VecA[3] = meshPos[3] - meshPos[0];

	bool nIn = true;
	bool bInOld = true;
	// ベクトルを垂直に回転させて、法線
	float fNor = atan2f(VecA[0].x, VecA[0].z) + D3DX_PI / 2;
	D3DXVECTOR3 Radius = D3DXVECTOR3(sinf(fNor)*fWidth, 0.0f, cosf(fNor)*fWidth);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// プレイヤーとメッシュのベクトル
		VecC[nCnt] = (*pPos + Radius - meshPos[nCnt]);
		fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
		if (fVecLine[nCnt] > 0)
		{
			nIn = false;
		}
		VecCOld[nCnt] = (posOld + Radius - meshPos[nCnt]);
		fVecLineOld[nCnt] = (VecCOld[nCnt].z * VecA[nCnt].x) - (VecCOld[nCnt].x * VecA[nCnt].z);
		if (fVecLineOld[nCnt] > 0)
		{
			bInOld = false;
		}
	}

	if (nIn&&fVecLine[0] <= 0)
	{
		if (m_fHeight + m_pos.y >= pPos->y &&
			m_pos.y <= pPos->y + fHeight)
		{// 高さ範囲内

			D3DXVECTOR3 EndPos = *pPos;
			D3DXVECTOR3 AB = meshPos[0] - meshPos[1];
			D3DXVec3Normalize(&AB, &AB);

			float fLength = AB.x * VecC[0].x + AB.z * VecC[0].z;

			pPos->x = meshPos[0].x + AB.x*fLength - Radius.x;
			pPos->z = meshPos[0].z + AB.z*fLength - Radius.z;

			CDebugProc::print("ベクトルAB：%.1f,%.1f\n", AB.x, AB.z);
			CDebugProc::print("長さ：%.1f\n", fLength);
			CDebugProc::print("座標：%.1f%.1f\n", meshPos[0].x + AB.x*fLength, meshPos[0].z + AB.z*fLength);

			//現在のベクトルの長さ＋過去のベクトルの長さ
			//float All = fabsf(fVecLine[0]) + fabsf(fVecLineOld[0]);
			//float Now = fabsf(fVecLine[0]) / All + 0.001f;// 0割防止のため加算
			//D3DXVECTOR3 diff = (*pPos - posOld);// 座標と前回座標の差分を抽出
			//pPos->x += -diff.x*Now;
			//pPos->z += -diff.z*Now;
			bHit = true;

		}
	}

	return bHit;
}

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CMeshWall::SetScan()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FILE_SCAN, "r");		// ファイルの名前の読み込み
	short int nNumModel = 0;

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// 行の読み込み
			if (strcmp(cData, "SETWALL") == 0)
			{// OBJECTSETだった場合
				D3DXVECTOR3 pos;		// 位置
				D3DXVECTOR3 rot;		// 角度
				D3DXCOLOR col=D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);			// 色
				float fWidth = 0;		// 大きさ[ 横 ]
				float fHeight = 0;		// 大きさ[ 縦 ]
				int nBlockX = 0;		// 横数
				int nBlockY = 0;		// 高さ
				int type = 0;			// 種類
				while (strcmp(cData, "ENDSETWALL") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{// タイプ読み込み
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &type);
					}
					else if (strcmp(cData, "POS") == 0)
					{// 座標読み込み
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pos.x, &pos.y, &pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{// 角度読み込み
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &rot.x, &rot.y, &rot.z);
						rot = D3DXVECTOR3(D3DXToRadian(rot.x), D3DXToRadian(rot.y), D3DXToRadian(rot.z));
					}
					else if (strcmp(cData, "COLOR" )== 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f %f", &cData[0], &cData[0], &col.r, &col.g, &col.b, &col.a);
					}
					else if (strcmp(cData, "SIZE") == 0)
					{// 大きさ読み込み
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &fWidth, &fHeight);
					}
					else if (strcmp(cData, "BLOCK") == 0)
					{// 大きさ読み込み
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &nBlockX, &nBlockY);
					}
				}
				CMeshWall::Create(pos, rot, col, fWidth, fHeight,0.0f, nBlockX, nBlockY, type);

			}
		}
		fclose(pFile);// 終了
	}
}
