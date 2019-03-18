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

#include "meshField.h"
#include "input.h"
#include "manager.h"
#include "line.h"
#include "renderer.h"
#include "DebugProc.h"
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
LPDIRECT3DTEXTURE9	*CMeshField::m_pTextureThis = NULL;
int CMeshField::m_nNumFile = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshField::CMeshField(int nPriority):CScene(PRIORITY_MESHFIELD)
{
	m_pTexture = NULL;								// テクスチャへのポインタ
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pNor = NULL;	// 法線
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_fWidth = 0;							// 幅
	m_fHeight = 0;							// 高さ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転量
	m_bCullMode = false;					// カリングモード
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CMeshField::Load(void)
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
			if (strcmp(cData, "NUM_TEXTURE") == 0)		// モデルの数
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %hd", &cData[0], &cData[0], &nNumTexture);
				m_pTextureThis = new LPDIRECT3DTEXTURE9[nNumTexture]();
			}
			// モデルのファイル読み込み
			if (strcmp(cData, "TEXTURE_FILENAME") == 0 && nNumTexture > nCntTexture)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				// テクスチャの生成
				D3DXCreateTextureFromFile(pDevice,
					&cData[0],
					&m_pTextureThis[nCntTexture]);
				nCntTexture++;
				m_nNumFile++;
			}

		}
		fclose(pFile);// 終了
	}

	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CMeshField::UnLoad(void)
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
void CMeshField::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
HRESULT CMeshField::Init(void)
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
	m_nBlock_Z = 0;										// ブロックZ
	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Z + 1);// 頂点数
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1) + 2;// インデックス数
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1);	// ポリゴン数
	m_bCullMode = false;						// カリングモード
	SetObjType(OBJTYPE_MESHFIELD);				// オブジェクトタイプ設定

	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CMeshField::Uninit(void)
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
void CMeshField::Update(void)
{
	//VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//// 頂点バッファをロックし頂点データへのポインタを取得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//CDebugProc::print("頂点数：%d\n", m_nNumVertex);

	//static float fLength = 0;

	//fLength += 10;
	//static float *pSpeed = new float[m_nNumVertex];
	//for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	//{
	//	float fPosLenght = (pVtx[nCnt].pos.x*pVtx[nCnt].pos.x) + (pVtx[nCnt].pos.z*pVtx[nCnt].pos.z);				// それぞれを2乗してすべて足して距離を出す
	//																					// 二つの当たり判定の半径の距離を計算
	//	float fCollisionRadius = (fLength);	// プレイヤーの体の当たり判定の半径+敵の攻撃の当たり判定の半径を足す
	//	float fRadiusLenght = fCollisionRadius * fCollisionRadius;	// 二つの半径を足した値を2乗して、半径の距離を計算を計算

	//	if (fRadiusLenght > fPosLenght &&
	//		-fRadiusLenght < fPosLenght)
	//	{// 座標の距離が半径の距離よりも小さかったら
	//		pSpeed[nCnt] += 0.1f;
	//		pVtx[nCnt].pos.y = cosf(pSpeed[nCnt])*100;

	//	}
	//}

	//// 頂点バッファをアンロックする
	//m_pVtxBuff->Unlock();
	//SetVtx();
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
	//{// データの更新
	//	CMeshField::SetScan();
	//}
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい

				// Zバッファへの書き込み
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if (m_bCullMode)
	{// カリングオフ
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
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
	// カリングモード戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化

}
//=============================================================================
// 生成
//=============================================================================
CMeshField *CMeshField::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const int nBlockX, const int nBlockZ,const int type,const bool bCull)
{
	CMeshField *pMeshField = NULL;
	if (!CScene::OverData())
	{// ２Dポリゴン生成
		pMeshField = new CMeshField;
	}
	if (pMeshField != NULL)
	{
		// 初期化処理
		if (FAILED(pMeshField->Init()))
		{
			return NULL;
		}
		pMeshField->SetPos(pos);
		pMeshField->SetSize(fWidth, fHeight, fDepth);
		pMeshField->SetBlock(nBlockX, nBlockZ);
		pMeshField->m_type = type;
		pMeshField->m_bCullMode = bCull;
		pMeshField->BindTexture(m_pTextureThis[type]);
		pMeshField->MakeVtx();
	}
	return pMeshField;
}
//==============================================================================================================================
// 座標設定
//==============================================================================================================================
void CMeshField::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// 大きさ設定
//==============================================================================================================================
void CMeshField::SetSize(const float fWidth, const float fHeight, const float fDepth)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
}
//==============================================================================================================================
// 回転設定
//==============================================================================================================================
void CMeshField::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// 分割数の設定
//==============================================================================================================================
void CMeshField::SetBlock(const int nBlockX, const int nBlockZ)
{
	m_nBlock_X = nBlockX;
	m_nBlock_Z = nBlockZ;
}
//==============================================================================================================================
// 色の設定
//==============================================================================================================================
void CMeshField::SetColor(const D3DXCOLOR col)
{
	if (m_col == col)return;
	m_col = col;
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点カラーの設定
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].col = m_col;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//==============================================================================================================================
// 頂点設定
//==============================================================================================================================
void CMeshField::SetVtx(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 各頂点の法線
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntZ = 0; nCntZ < m_nBlock_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntZ*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntZ) + nCntX;
			int nLDOWN = (nCntZ * (m_nBlock_X + 1)) + 1 + nCntX;
			// 左面
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// 右面
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
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
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// 右上
		 // 番号を二倍して１引く
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Z)
		{// 左下
		 // Z増加で「２増」X増加で「倍」
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Z - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// 左端< X < 右端
		{// 中の上端￣
			int nIdy = nCnt + nCnt - 1;	// 番号を二倍して１引く
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Z&&nCnt < m_nNumVertex - 1)
		{// 中の下端＿
			int nIdypolygon = (m_nBlock_Z - 1) * 2 * m_nBlock_X;		// 左端のポリゴン番号
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (左端の番号 - 番号 -1 )*3でポリゴンを進める
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Z&&nCnt % (m_nBlock_X + 1) == 0)
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
	for (int nCntV = 0; nCntV < m_nBlock_Z + 1; nCntV++)
	{// テクスチャの設定
		for (int nCntH = 0; nCntH < m_nBlock_X + 1; nCntH++)
		{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
		 // テクスチャの設定
			pVtx[nCntH + (m_nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / m_nBlock_X) / 100.0f, float(nCntV * 100 / m_nBlock_Z) / 100.0f);
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//==============================================================================================================================
// 頂点生成
//==============================================================================================================================
void CMeshField::MakeVtx(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Z + 1);// 頂点数
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1) + 2;// インデックス数
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1);	// ポリゴン数

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
		float fHeight = 0;
		//fHeight = rand()%2 * 100.0f;
		pVtx[nCntPos].pos = D3DXVECTOR3(float(-((m_nBlock_X * m_fWidth) / 2) + m_fWidth * (nCntPos % (m_nBlock_X + 1))),
			fHeight,
			float((m_nBlock_Z * m_fDepth) / 2 - m_fDepth * (nCntPos / (m_nBlock_X + 1))));
	}

	// 各頂点の法線
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntZ = 0; nCntZ < m_nBlock_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntZ*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntZ) + nCntX;
			int nLDOWN = (nCntZ * (m_nBlock_X + 1)) + 1 + nCntX;
			// 左面
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// 右面
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// 左
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // 右
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
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
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// 右上
		 // 番号を二倍して１引く
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Z)
		{// 左下
		 // Z増加で「２増」X増加で「倍」
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Z - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// 左端< X < 右端
		{// 中の上端￣
			int nIdy = nCnt + nCnt - 1;	// 番号を二倍して１引く
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Z&&nCnt < m_nNumVertex - 1)
		{// 中の下端＿
			int nIdypolygon = (m_nBlock_Z - 1) * 2 * m_nBlock_X;		// 左端のポリゴン番号
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (左端の番号 - 番号 -1 )*3でポリゴンを進める
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Z&&nCnt % (m_nBlock_X + 1) == 0)
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

	//for (int nCntV = 0; nCntV < m_nBlock_Z + 1; nCntV++)
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
float CMeshField::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *Vec)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fHeight = -200;
	D3DXVECTOR3 VecA[4];// メッシュのベクトル
	D3DXVECTOR3 VexC[4];// プレイヤーと頂点のベクトル
	float fVecLine[4];	// 頂点ベクトルをプレイヤーが超えたかどうか

	D3DXVECTOR3 VecAL[3];// メッシュのベクトル
	D3DXVECTOR3 VexCL[3];// プレイヤーと頂点のベクトル
	D3DXVECTOR3 VecAR[3];// メッシュのベクトル
	D3DXVECTOR3 VexCR[3];// プレイヤーと頂点のベクトル
	float fVecLineL[3];	// 頂点ベクトルをプレイヤーが超えたかどうか
	float fVecLineR[3];	// 頂点ベクトルをプレイヤーが超えたかどうか

	for (int nCntZ = 0; nCntZ < m_nBlock_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			D3DXVECTOR3 meshPos[4] =
			{
				pVtx[0 + nCntX + nCntZ * (m_nBlock_X+1)].pos + m_pos,		// 0
				pVtx[1 + nCntX + nCntZ * (m_nBlock_X + 1)].pos + m_pos,		// 1
				pVtx[3 + m_nBlock_X - 1 + nCntX + nCntZ * (m_nBlock_X + 1)].pos + m_pos,		// 3
				pVtx[2 + m_nBlock_X - 1 + nCntX + nCntZ * (m_nBlock_X + 1)].pos + m_pos,		// 2
			};
			VecA[0] = meshPos[0] - meshPos[1];
			VecA[1] = meshPos[1] - meshPos[2];
			VecA[2] = meshPos[2] - meshPos[3];
			VecA[3] = meshPos[3] - meshPos[0];
			int nIn = 0;
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{// プレイヤーとメッシュのベクトル
				VexC[nCnt] = (pos - meshPos[nCnt]);

				fVecLine[nCnt] = (VexC[nCnt].z * VecA[nCnt].x) - (VexC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] >= 0)
				{// 超えた
					nIn++;
				}
			}
			if (nIn == 4)
			{
				// 左右判定
				// 左
				VecAL[0] = meshPos[3] - meshPos[0];	// 三角形を描くようなベクトル①
				VecAL[1] = meshPos[2] - meshPos[3];	// 三角形を描くようなベクトル②
				VecAL[2] = meshPos[0] - meshPos[2];	// 三角形を描くようなベクトル③
				VexCL[0] = (pos - meshPos[3]);
				VexCL[1] = (pos - meshPos[2]);
				VexCL[2] = (pos - meshPos[0]);
				// 右
				VecAR[0] = meshPos[0] - meshPos[1];	// 三角形を描くようなベクトル①
				VecAR[1] = meshPos[1] - meshPos[2];	// 三角形を描くようなベクトル②
				VecAR[2] = meshPos[2] - meshPos[0];	// 三角形を描くようなベクトル③

				VexCR[0] = (pos - meshPos[0]);
				VexCR[1] = (pos - meshPos[1]);
				VexCR[2] = (pos - meshPos[2]);

				bool bIn = false;
				bool bInR = false;
				for (int nCnt = 0; nCnt < 3; nCnt++)
				{// プレイヤーとメッシュのベクトル

					fVecLineL[nCnt] = (VexCL[nCnt].z * VecAL[nCnt].x) - (VexCL[nCnt].x * VecAL[nCnt].z);
					fVecLineR[nCnt] = (VexCR[nCnt].z * VecAR[nCnt].x) - (VexCR[nCnt].x * VecAR[nCnt].z);
					if (fVecLineL[nCnt] < 0)
					{// 超えた
						bIn = true;
					}
					if (fVecLineR[nCnt] < 0)
					{// 超えた
						bInR = true;
					}
				}

				if (!bIn)
				{// 左
					D3DXVECTOR3 vv1, vv2, nor;
					vv1 = meshPos[3] - meshPos[2];
					vv2 = meshPos[3] - meshPos[0];
					D3DXVec3Cross(&nor, &vv1, &vv2);
					D3DXVec3Normalize(&nor, &nor);
					D3DXVECTOR3 VecA = pos - meshPos[3];
					fHeight = (-(nor.x * VecA.x) - (nor.z * VecA.z)) / nor.y + meshPos[3].y;
					*Vec = nor;
				}
				if (!bInR)
				{// 右
					D3DXVECTOR3 vv1, vv2, nor;
					vv1 = meshPos[1] - meshPos[0];
					vv2 = meshPos[1] - meshPos[2];
					D3DXVec3Cross(&nor, &vv1, &vv2);
					D3DXVec3Normalize(&nor, &nor);
					D3DXVECTOR3 VecA = pos - meshPos[1];
					fHeight = (-(nor.x * VecA.x) - (nor.z * VecA.z)) / nor.y + meshPos[1].y;
					*Vec = nor;
				}
			}
		}
	}

	return fHeight;
}

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CMeshField::SetScan()
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
			if (strcmp(cData, "SETFIELD") == 0)
			{// OBJECTSETだった場合
				D3DXVECTOR3 pos;		// 位置
				float sizeX = 0;		// 大きさ[ 横 ]
				float sizeZ = 0;		// 大きさ[ 奥 ]
				int Width =0;				// 横数
				int Depth=0;				// 奥数
				int type=0;				// 種類
				int nCull = 0;
				bool bCull = false;		// カリング
				while (strcmp(cData, "ENDSETFIELD") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);

					if (strcmp(cData, "FIELD") == 0)
					{// 位置の読み込み
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %f %d %d %d", &cData[0], &cData[0], &type, &pos.x, &pos.y, &pos.z, &sizeX, &sizeZ, &Width, &Depth,&nCull);
						if (nCull == 1)
						{
							bCull = true;
						}
						CMeshField::Create(pos, sizeX, 0.0f, sizeZ, Width, Depth,type, bCull);
					}
					if (strcmp(cData, "SETVERTEX") == 0)
					{// SET_FIELDだった場合
						D3DXVECTOR3 aMeshPos[100] = {};
						D3DXVECTOR3 start = D3DXVECTOR3(0, 0, 0);		// 位置
						int nBlock = 0;						//
						int nCntMesh = 0;
						int nCntBrock = -1;
						while (strcmp(cData, "END_SETVERTEX") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "TYPE") == 0)
							{// 種類の読み込み
								sscanf(&cLine[0], "%s %s %d ", &cData[0], &cData[0], &type);
							}
							else if (strcmp(cData, "BLOCK") == 0)
							{// ブロックの総数を数える
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nBlock);
							}
							else if (strcmp(cData, "CULL") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCull);
								if (nCull == 1)
								{
									bCull = true;
								}
							}
							else if (strcmp(cData, "SET_FIELDSET") != 0)
							{// SET_FIELDSET以外の時
								sscanf(&cLine[0], "%d %s %f %f %f", &nCntMesh, &cData[0], &start.x, &start.y, &start.z);
								aMeshPos[nCntMesh] = start;
								nCntMesh++;		// 頂点カウンターの加算
							}

						}
						CMeshField *pMesh;
						pMesh = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 1, nBlock, type, bCull);
						pMesh->SetVtxPos(&aMeshPos[0]);
					}
				}
			}
		}
		fclose(pFile);// 終了
	}
}

//=============================================================================
// 頂点位置の設定
//=============================================================================
void CMeshField::SetVtxPos(D3DXVECTOR3 *pPos)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// X軸・Z軸分割用
	//for (int nCnt = 0; nCnt < m_nNumVertex; nCnt += m_nBlock_X + 1)
	//{
	//	for (int nCntBlock = 0; nCntBlock < m_nBlock_X + 1; nCntBlock++)
	//	{
	//		pVtx[nCnt + nCntBlock].pos = pPos[nCnt+nCntBlock];
	//	}
	//}

	// Z軸だけ分割用
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].pos = pPos[nCnt];
	}
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	SetVtx();
}