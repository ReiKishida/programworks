//=============================================================================
//
// メッシュウォール処理 [scene_meshfield.cpp]
// Author :岸田怜(Kishida Rei)
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
#include "scene_meshwall.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WALL_FILENAME ("data/TEXT/wall_data.txt")
#define WALL_FILENAME_TUTO ("data/TEXT/wall_data_Tuto.txt")
#define MAX_CHAR (125)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSceneMeshWall::m_pTexture = NULL; //共有テクスチャのポインタ

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：クリアな値を代入
//=============================================================================
CSceneMeshWall::CSceneMeshWall(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：処理の最後に呼び出される
//=============================================================================
CSceneMeshWall::~CSceneMeshWall()
{

}

//=============================================================================
// 関数名：テクスチャの読み込み
// 関数の概要：テクスチャを読み込む
//=============================================================================
HRESULT CSceneMeshWall::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field_wall/wall000.jpg", &m_pTexture);	//壁のテクスチャ
	return S_OK;
}
//=============================================================================
// 関数名：テクスチャの破棄
// 関数の概要：テクスチャを破棄する
//=============================================================================
void CSceneMeshWall::Unload(void)
{

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}

//=============================================================================
// 関数名：メッシュウォールの生成処理
// 関数の概要：メッシュフィールドを生成する
//=============================================================================
CSceneMeshWall *CSceneMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_X, int sprit_Y, float Width, float Height, WALLTYPE type)
{
	CSceneMeshWall  *pSceneMeshWall;
	pSceneMeshWall = new CSceneMeshWall;//シーンクラスの生成
	if (pSceneMeshWall != NULL)
	{
		pSceneMeshWall->m_pos = pos;
		pSceneMeshWall->m_rot = rot;
		pSceneMeshWall->m_nMesh_X = sprit_X;
		pSceneMeshWall->m_nMesh_Y = sprit_Y;
		pSceneMeshWall->m_fMeshWidth = Width;
		pSceneMeshWall->m_fMeshHeight = Height;
		pSceneMeshWall->m_type = type;
		pSceneMeshWall->Init();
	}
	return pSceneMeshWall;
}

//=============================================================================
// 関数名：メッシュウォールの初期化処理
// 関数の概要：頂点数とポリゴン数に注意
//=============================================================================
HRESULT CSceneMeshWall::Init(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点数
	m_nVertexNum = (m_nMesh_X + 1) * (m_nMesh_Y + 1);
	//インデックス数
	m_nIndexNum = (m_nMesh_X * m_nMesh_Y) * 2 + (4 * (m_nMesh_Y - 1)) + 2;
	//ポリゴン数
	m_nPolygonNum = (m_nMesh_X * m_nMesh_Y) * 2 + (4 * (m_nMesh_Y - 1));

	MakeVertex(pDevice);
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
// 関数名：メッシュウォールの終了処理
// 関数の概要：テクスチャ、頂点バッファの破棄
//=============================================================================
void CSceneMeshWall::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//自身の破棄
	Release();
}


//=============================================================================
// 関数名：メッシュウォールの更新処理
// 関数の概要：--
//=============================================================================
void CSceneMeshWall::Update(void)
{


}

//=============================================================================
// 関数名：メッシュウォールの描画
// 関数の概要：頂点数とポリゴン数に注意
//=============================================================================
void CSceneMeshWall::Draw(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//マトリックス情報

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


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	if (m_type == WALLTYPE_NORMAL)
	{//通常の壁ではないとき
	 // メッシュウォールの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPolygonNum);
	}
}

//=============================================================================
// 関数名：頂点情報の作成
// 関数の概要：とりあえずは頂点を置くだけ
//=============================================================================
void CSceneMeshWall::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall000.jpg", &m_pTexture);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);



	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntMesh_Y = 0; nCntMesh_Y <= m_nMesh_Y; nCntMesh_Y++)
	{
		for (int nCntMesh_X = 0; nCntMesh_X <= m_nMesh_X; nCntMesh_X++)
		{
			// 頂点情報の設定

			pVtx[0].pos = D3DXVECTOR3(-((m_fMeshWidth * m_nMesh_X) / 2) + (m_fMeshWidth * nCntMesh_X),
				(m_nMesh_Y * m_fMeshHeight) / 2 - (m_fMeshHeight * nCntMesh_Y),
				0.0f);

			//法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntMesh_X * 1.0f, nCntMesh_Y * 1.0f);

			pVtx += 1;
		}
	}


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();



}
//=============================================================================
// 関数名：インデックス情報の作成
// 関数の概要：インデックスでポリゴンを生成する
//=============================================================================
void CSceneMeshWall::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	WORD *pIdx;	// インデックスデータへのポインタ

				// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD)*m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスバッファをロックし頂点データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);




	int nCntIndex = 0;	//インデックスのカウント

	for (int nCntIndex_Z = 0; nCntIndex_Z < m_nMesh_Y; nCntIndex_Z++)
	{
		for (int nCntIndex_X = 0; nCntIndex_X <= m_nMesh_X; nCntIndex_X++, nCntIndex++)
		{
			pIdx[0] = (m_nMesh_X + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex_X == m_nMesh_X && nCntIndex_Z < m_nMesh_Y - 1)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (m_nMesh_X + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	// 頂点バッファをアンロックする
	m_pIdxBuff->Unlock();



}

//=============================================================================
// 関数名：手前面の壁の当たり判定
// 関数の概要：手前面から来たものの当たり判定を行う
//=============================================================================
void CSceneMeshWall::CollisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posold, float fRadius)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	D3DXVECTOR3 VecA;			//壁のベクトル
	D3DXVECTOR3 VecC;			//壁と(プレイヤーまたは敵の)位置のベクトル
	D3DXVECTOR3 Vtxpos_Start_X;	//始点
	D3DXVECTOR3 Vtxpos_End_X;	//終点
	D3DXVECTOR3 Vtxpos_Start_Z;	//始点
	D3DXVECTOR3 Vtxpos_End_Z;	//終点

	float VecLine;				//判定用の境界線

								// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//横幅の算出
	Vtxpos_Start_X = pVtx[0].pos + m_pos;
	Vtxpos_End_X = pVtx[m_nVertexNum - 1].pos + m_pos;

	//縦幅の算出
	Vtxpos_Start_Z.x = m_pos.x;
	Vtxpos_Start_Z.y = m_pos.x;
	Vtxpos_Start_Z.z = m_pos.z + pVtx[m_nVertexNum - 1].pos.x;
	Vtxpos_End_Z.x = m_pos.x;
	Vtxpos_End_Z.y = m_pos.x;
	Vtxpos_End_Z.z = m_pos.z + pVtx[0].pos.x;


	if (m_rot.y == 0.0f)
	{//手前壁
		if (pos->x >= Vtxpos_Start_X.x && pos->x <= Vtxpos_End_X.x)
		{//壁の範囲
			if (posold->z <= m_pos.z)
			{

				//VecAの計算
				VecA = Vtxpos_End_X - Vtxpos_Start_X;
				D3DXVec3Normalize(&VecA, &VecA);//norの正規化
				//VecCの計算
				VecC = *pos - Vtxpos_Start_X;
				D3DXVec3Normalize(&VecC, &VecC);//norの正規化


				//ベクトル計算
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->z = posold->z;
				}
			}
		}
	}
	else if (m_rot.y == D3DX_PI)
	{//手前壁
		if (pos->x >= Vtxpos_Start_X.x && pos->x <= Vtxpos_End_X.x)
		{//壁の範囲
			if (posold->z >= m_pos.z)
			{

				//VecAの計算
				VecA = Vtxpos_Start_X - Vtxpos_End_X;
				D3DXVec3Normalize(&VecA, &VecA);//norの正規化
												//VecCの計算
				VecC = *pos - Vtxpos_End_X;
				D3DXVec3Normalize(&VecC, &VecC);//norの正規化

												//ベクトル計算
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->z = posold->z;
				}
			}
		}

	}
	else if (m_rot.y == D3DX_PI * 0.5f)
	{//手前壁
		if (pos->z <= Vtxpos_Start_Z.z && pos->z >= Vtxpos_End_Z.z)
		{//壁の範囲
			if (posold->x <= m_pos.x)
			{
				//VecAの計算
				VecA = Vtxpos_End_Z - Vtxpos_Start_Z;
				D3DXVec3Normalize(&VecA, &VecA);//norの正規化
				//VecCの計算
				VecC = *pos - Vtxpos_Start_Z;
				D3DXVec3Normalize(&VecC, &VecC);//norの正規化

												//ベクトル計算
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->x = posold->x;
				}
			}
		}

	}
	else if (m_rot.y == -D3DX_PI * 0.5f)
	{//手前壁
		if (pos->z <= Vtxpos_Start_Z.z && pos->z >= Vtxpos_End_Z.z)
		{//壁の範囲
			if (posold->x >= m_pos.x)
			{
				//VecAの計算
				VecA = Vtxpos_Start_Z - Vtxpos_End_Z;
				D3DXVec3Normalize(&VecA, &VecA);//norの正規化
				//VecCの計算
				VecC = *pos - Vtxpos_End_Z;
				D3DXVec3Normalize(&VecC, &VecC);//norの正規化

				//ベクトル計算
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->x =posold->x;
				}
			}
		}

	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
// 関数名：壁の情報の読み込み処理
// 関数の概要：テキストファイルから壁の情報を読み込む
//=============================================================================
void CSceneMeshWall::LoadWallText(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	FILE *pFile = fopen(WALL_FILENAME, "r");		// 読み込むテキストファイルのアドレス

													// 読み込むための変数宣言
	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	int nCntWall = 0;								// 読み込むモデルの数
	int nSprit_X = 0;								//横の分割数
	int nSprit_Y = 0;								//縦の分割数
	float fWidth = 0.0f;							//横幅
	float fHeight = 0.0f;							//縦幅
	WALLTYPE TypeWall;								// 使用するモデルのタイプ
	int nMaxWall = 0;								// モデルの最大数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{// ヌルチェック
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー


			if (strcmp(acData, "WALLSET") == 0)
			{// "MODELSET"の文字列があったら

				while (strcmp(acData, "END_WALLSET") != 0)
				{// "END_MODELSET"がまで情報を取得し続ける
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー

					if (strcmp(acData, "POS") == 0)
					{// "POS"の文字列があったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// 位置を取得
					}
					else if (strcmp(acData, "ROT") == 0)
					{// "ROT"の文字列があったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// 向きを取得
																												//テキストの角度情報通りに入れる計算
						rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
						rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
						rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
					}
					else if (strcmp(acData, "SPRIT_X") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_X);	// 横の分割数の取得

					}
					else if (strcmp(acData, "SPRIT_Y") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_Y);	// 縦の分割数の取得

					}
					else if (strcmp(acData, "WALL_WIDTH") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fWidth);	// 横幅を取得

					}
					else if (strcmp(acData, "WALL_HEIGHT") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fHeight);	// 縦幅を取得

					}
					else if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"の文字列があったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &TypeWall);	// 使用するモデルの番号を取得
					}
				}
				//モデルの生成
				Create(pos, rot, nSprit_X, nSprit_Y, fWidth, fHeight, TypeWall);
			}
		}
		fclose(pFile);	// 開いたファイルを閉じる
	}
}

//=============================================================================
// 関数名：壁の情報の読み込み処理
// 関数の概要：テキストファイルから壁の情報を読み込む
//=============================================================================
void CSceneMeshWall::LoadWallText_Tuto(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	FILE *pFile = fopen(WALL_FILENAME_TUTO, "r");		// 読み込むテキストファイルのアドレス

													// 読み込むための変数宣言
	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	int nCntWall = 0;								// 読み込むモデルの数
	int nSprit_X = 0;								//横の分割数
	int nSprit_Y = 0;								//縦の分割数
	float fWidth = 0.0f;							//横幅
	float fHeight = 0.0f;							//縦幅
	WALLTYPE TypeWall;								// 使用するモデルのタイプ
	int nMaxWall = 0;								// モデルの最大数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{// ヌルチェック
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー


			if (strcmp(acData, "WALLSET") == 0)
			{// "MODELSET"の文字列があったら

				while (strcmp(acData, "END_WALLSET") != 0)
				{// "END_MODELSET"がまで情報を取得し続ける
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー

					if (strcmp(acData, "POS") == 0)
					{// "POS"の文字列があったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// 位置を取得
					}
					else if (strcmp(acData, "ROT") == 0)
					{// "ROT"の文字列があったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// 向きを取得
																												//テキストの角度情報通りに入れる計算
						rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
						rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
						rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
					}
					else if (strcmp(acData, "SPRIT_X") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_X);	// 横の分割数の取得

					}
					else if (strcmp(acData, "SPRIT_Y") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_Y);	// 縦の分割数の取得

					}
					else if (strcmp(acData, "WALL_WIDTH") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fWidth);	// 横幅を取得

					}
					else if (strcmp(acData, "WALL_HEIGHT") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fHeight);	// 縦幅を取得

					}
					else if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"の文字列があったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &TypeWall);	// 使用するモデルの番号を取得
					}
				}
				//モデルの生成
				Create(pos, rot, nSprit_X, nSprit_Y, fWidth, fHeight, TypeWall);
			}
		}
		fclose(pFile);	// 開いたファイルを閉じる
	}

}
