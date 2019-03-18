//=============================================================================
//
// オブジェクトの配置の処理 [object.cpp]
// Author : 岸田怜(Kishida Rei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "sceneX.h"
#include "input.h"
#include "debugproc.h"
#include "scene_meshfield.h"
#include "game.h"
#include"tutorial.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "collision.h"
#include "particle.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_FILE_NAME			("data/TEXT/object_data.txt")		// Xファイルのアドレス
#define MODEL_FILE_NAME_TUTO			("data/TEXT/object_data_tuto.txt")		// Xファイルのアドレス

#define MAX_CHAR (256)	// 文字数の最大数

//=====================================
// 優先順位
//=====================================
#define ENEMY_PRIORITY (2)						//敵の優先順位
#define COLLISION_PRIORITY (6)					//当たり判定の優先順位
#define WALL_PRIORITY (5)						//壁の優先順位
#define OBJECT_PRIORITY (4)						//オブジェクトの優先順位
#define CYLINDER_PRIORITY (3)					//シリンダーの優先順位


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
char *CObject::m_pModelName[MAXMODEL_TYPE] = {};
int	 CObject::m_nMaxModelNum = 0;
LPDIRECT3DTEXTURE9 CObject::m_apTexture[MAX_TEXTURE_OBJ] = {}; //共有テクスチャのポインタ

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE type) : CSceneX(nPriority, type)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置の移動量の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向きの初期化
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目的の向きを初期化
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CObject::~CObject()
{

}


//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CObject::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/dram.png", &m_apTexture[0]);				// ドラム缶
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/dust.png", &m_apTexture[1]);				// ゴミ箱
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/keepout.png", &m_apTexture[2]);				// 停止線
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/telephone.png", &m_apTexture[3]);			// 電話
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/piller.png", &m_apTexture[4]);				// 柱
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/stair.png", &m_apTexture[5]);				// 階段
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/board01.png", &m_apTexture[6]);				// 看板01
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/board02.png", &m_apTexture[7]);				// 看板02
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/station_chair.png", &m_apTexture[8]);		// 椅子
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/ticket_gate.png", &m_apTexture[9]);			// 改札
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/train_chair.png", &m_apTexture[10]);		// 電車内椅子
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/tex.jpg", &m_apTexture[11]);				// 電車床
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/iron.jpg", &m_apTexture[12]);				// 鉄
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/train.png", &m_apTexture[13]);				// 電車
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/jihanki.png", &m_apTexture[14]);			// 自販機
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/denkou_000.png", &m_apTexture[15]);			// 電光掲示板

	return S_OK;

}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CObject::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE_OBJ; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}


//=============================================================================
// 関数名：モデルの生成
// 関数の概要：モデルを生成する
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type)
{
	CObject  *pModel;
	pModel = new CObject;//シーンクラスの生成
	if (pModel != NULL)
	{
		pModel->m_nTypeModel = type;
		pModel->m_pos = pos;
		pModel->m_rot = rot;
		pModel->m_fHeight = 100.0f;
		//初期化
		pModel->Init();
	}
	return pModel;
}

//=============================================================================
// 関数名：モデルの初期化処理
// 関数の概要：モデルの初期化
//=============================================================================
HRESULT CObject::Init(void)
{
	//位置情報の代入
	SetPos(m_pos);

	//角度情報の代入
	SetRot(m_rot);

	//ファイル名の割り当て
	BindFileName(m_pModelName[m_nTypeModel]);

	//初期化処理
	CSceneX::Init();

	//テクスチャの割り当て
	if (m_nTypeModel == 6 || m_nTypeModel == 7)
	{
		BindTexture(m_apTexture[6]);
	}
	else if (m_nTypeModel == 8 || m_nTypeModel == 9)
	{
		BindTexture(m_apTexture[7]);
	}
	else if (m_nTypeModel == 10)
	{
		BindTexture(m_apTexture[8]);
	}
	else if (m_nTypeModel == 11)
	{
		BindTexture(m_apTexture[9]);
	}
	else if (m_nTypeModel == 12 || m_nTypeModel == 13)
	{
		BindTexture(m_apTexture[10]);
	}
	else if (m_nTypeModel == 14 || m_nTypeModel == 15)
	{
		BindTexture(m_apTexture[11]);
	}
	else if (m_nTypeModel == 16)
	{
		BindTexture(m_apTexture[12]);
	}
	else if (m_nTypeModel == 17 || m_nTypeModel == 18 || m_nTypeModel == 19 || m_nTypeModel == 20 || m_nTypeModel == 21)
	{
		BindTexture(m_apTexture[13]);
	}
	else if (m_nTypeModel == 22)
	{
		BindTexture(m_apTexture[14]);
	}
	else if (m_nTypeModel == 23 || m_nTypeModel == 24)
	{
		BindTexture(NULL);
	}
	else if (m_nTypeModel == 25 || m_nTypeModel == 26)
	{
		BindTexture(m_apTexture[15]);
	}
	else
	{
		BindTexture(m_apTexture[m_nTypeModel]);
	}
	m_vtxMax = GetVtxMax();		//最大頂点座標の取得
	m_vtxMin = GetVtxMin();		//最小頂点座標の取得

	return S_OK;
}

//=============================================================================
// 関数名：モデルの終了処理
// 関数の概要：モデルの終了
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// 関数名：モデルの更新
// 関数の概要：--
//=============================================================================
void CObject::Update(void)
{
	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//プレイヤーの取得
	CPlayer *pPlayer = NULL;
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();
	switch (mode)
	{
	case CManager::MODE_GAME:
	pPlayer = CGame::GetPlayer();
		break;
	case CManager::MODE_TUTORIAL:
		pPlayer = CTutorial::GetPlayer();
		break;
	}


	D3DXVECTOR3 playerpos;
	if (mode == CManager::MODE_GAME)
	{
		playerpos = pPlayer->GetPos();
		if (playerpos.z > -35.0f)
		{
			if (m_nTypeModel == 14)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.3f));
			}
			if (m_nTypeModel == 25)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.3f));
			}
		}
		else
		{
			if (m_nTypeModel == 14)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			}
			if (m_nTypeModel == 25)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}

		if (playerpos.x <= 1400.0f && playerpos.x >= 1200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			if (m_nTypeModel == 23)
			{
				SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.3f));
			}
		}
		else if (playerpos.x <= 2400.0f && playerpos.x >= 2200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			if (m_nTypeModel == 23)
			{
				SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.3f));
			}
		}
		else
		{
			if (m_nTypeModel == 23)
			{
				SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
			}
		}
	}


	if (CScene::GetStop() == true)
	{//プレイヤー以外が止まっているとき
		if (m_nTypeModel == 8)
		{
			SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.3f));
		}
	}
	else
	{
		if (m_nTypeModel == 8)
		{
			SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
		}
	}

	CollisionAttack();

	//角度情報の代入
	SetRot(m_rot);

	//位置情報の代入
	SetPos(m_pos);
}

//=============================================================================
// 関数名：モデルの描画処理
// 関数の概要：階層構造で描画
//=============================================================================
void CObject::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//モデルの描画
	CSceneX::Draw();


}

//=============================================================================
// 関数名：攻撃の当たり判定処理
// 関数の概要：攻撃が当たった時の判定
//=============================================================================
void CObject::CollisionAttack(void)
{
	//当たり判定のポインタ
	CCollision *pCollision;



	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();




	pCollision = (CCollision*)GetSceneTop(COLLISION_PRIORITY);
	while (pCollision != NULL)//NULLになるまで
	{
		CCollision *pCollisionNext;
		pCollisionNext = (CCollision*)pCollision->GetSceneNext(COLLISION_PRIORITY);

		if (pCollision->CollisionAttack(&m_pos, &m_rot, m_fHeight) == true)
		{

			if (m_nTypeModel == 3)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SMALLHIT);

				pSound->PlaySound(CSound::SOUND_LABEL_SE_PHONE);
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}

			}
			if (m_nTypeModel == 0)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_HITDRAM);

				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
			}
			if (m_nTypeModel == 22)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SMALLHIT);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_JIHANKI);

				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
			}
		}

		pCollision = pCollisionNext;
	}
}




//=============================================================================
// 関数名：モデルの情報の読み込み処理
// 関数の概要：テキストファイルからモデル情報を読み込む
//=============================================================================
void CObject::LoadModelText(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	FILE *pFile = fopen(MODEL_FILE_NAME, "r");		// 読み込むテキストファイルのアドレス

	// 読み込むための変数宣言
	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	char cFileName[MAXMODEL_TYPE][MAX_CHAR];		// 読み込む
	int nCntModel = 0;								// 読み込むモデルの数
	int nCntTexture = 0;							// 読み込むテクスチャ数
	int nTypeModel;									// 使用するモデルのタイプ
	int nMaxModel = 0;								// モデルの最大数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{// ヌルチェック
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// "NUM_MODEL"の文字列があったら
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nMaxModel);	// モデルの数を取得
			}
			if (strcmp(acData, "MODEL_FILENAME") == 0)
			{// "MODEL_FILENAME"の文字列があったら
				sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &cFileName[nCntModel][0]);	// モデルの数を取得
				m_pModelName[nCntModel] = &cFileName[nCntModel][0];//ファイル名の代入
				nCntModel++;		// モデル数を加算
			}
			if (strcmp(acData, "MODELSET") == 0)
			{// "MODELSET"の文字列があったら

				while (strcmp(acData, "END_MODELSET") != 0)
				{// "END_MODELSET"がまで情報を取得し続ける
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー

					if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"の文字列があったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nTypeModel);					// 使用するモデルの番号を取得
					}
					else if (strcmp(acData, "POS") == 0)
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
				}
				//モデルの生成
				Create(pos, rot, nTypeModel);
			}
		}
		fclose(pFile);	// 開いたファイルを閉じる
	}
}

//=============================================================================
// 関数名：モデルの情報の読み込み処理(Tuto)
// 関数の概要：テキストファイルからモデル情報を読み込む
//=============================================================================
void CObject::LoadModelTextTuto(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	FILE *pFile = fopen(MODEL_FILE_NAME_TUTO, "r");		// 読み込むテキストファイルのアドレス

													// 読み込むための変数宣言
	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	char cFileName[MAXMODEL_TYPE][MAX_CHAR];		// 読み込む
	int nCntModel = 0;								// 読み込むモデルの数
	int nCntTexture = 0;							// 読み込むテクスチャ数
	int nTypeModel;									// 使用するモデルのタイプ
	int nMaxModel = 0;								// モデルの最大数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{// ヌルチェック
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// "NUM_MODEL"の文字列があったら
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nMaxModel);	// モデルの数を取得
			}
			if (strcmp(acData, "MODEL_FILENAME") == 0)
			{// "MODEL_FILENAME"の文字列があったら
				sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &cFileName[nCntModel][0]);	// モデルの数を取得
				m_pModelName[nCntModel] = &cFileName[nCntModel][0];//ファイル名の代入
				nCntModel++;		// モデル数を加算
			}
			if (strcmp(acData, "MODELSET") == 0)
			{// "MODELSET"の文字列があったら

				while (strcmp(acData, "END_MODELSET") != 0)
				{// "END_MODELSET"がまで情報を取得し続ける
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー

					if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"の文字列があったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nTypeModel);					// 使用するモデルの番号を取得
					}
					else if (strcmp(acData, "POS") == 0)
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
				}
				//モデルの生成
				Create(pos, rot, nTypeModel);
			}
		}
		fclose(pFile);	// 開いたファイルを閉じる
	}


}

//=============================================================================
// 関数名：モデルとの当たり判定処理
// 関数の概要：めりこんだら戻す処理
//=============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	bool bRand = false;			// 着地したかどうか
	if (m_nTypeModel != 5)
	{
		//----------------------------------------------------------
		// 矩形での当たり判定
		//----------------------------------------------------------
		if (m_pos.x + m_vtxMin.x < pPos->x + fRadius &&
			m_pos.x + m_vtxMax.x > pPos->x - fRadius &&
			m_pos.z + m_vtxMin.z < pPos->z + fRadius &&
			m_pos.z + m_vtxMax.z > pPos->z - fRadius)
		{// オブジェクトの範囲内だったら
			if (m_pos.y + m_vtxMax.y <= pPosOld->y &&
				m_pos.y + m_vtxMax.y > pPos->y)
			{// 上からの判定
				pPos->y = m_pos.y + m_vtxMax.y;					// オブジェクトの位置を戻す
				pMove->y = 0.0f;								// 落下速度を0.0に戻す
				bRand = true;									// 着地した判定にする
			}
		}
		if (m_pos.y + m_vtxMax.y + 10.0f > pPos->y + fRadius &&
			m_pos.y + m_vtxMin.y <= pPos->y + fRadius)
		{// プレイヤーがオブジェクトの範囲内なら

		 // ブロックの当たり判定
			if (m_pos.x + m_vtxMin.x < pPos->x + fRadius &&
				m_pos.x + m_vtxMax.x > pPos->x - fRadius)
			{// オブジェクトがオブジェクトのZ軸の範囲内に入ったら
				if (m_pos.z + m_vtxMin.z >= pPosOld->z + fRadius &&
					m_pos.z + m_vtxMin.z < pPos->z + fRadius)
				{// 手前からの判定

					pPos->z = m_pos.z + m_vtxMin.z - fRadius;			// オブジェクトの位置を戻す

				}
				else if (m_pos.z + m_vtxMax.z <= pPosOld->z - fRadius &&
					m_pos.z + m_vtxMax.z > pPos->z - fRadius)
				{// 奥からの判定

					pPos->z = m_pos.z + m_vtxMax.z + fRadius;			// オブジェクトの位置を戻す

				}

			}
			if (m_pos.z + m_vtxMin.z < pPos->z + fRadius &&
				m_pos.z + m_vtxMax.z > pPos->z - fRadius)
			{// Z軸がオブジェクトの範囲内だったら
				if (m_pos.x + m_vtxMin.x >= pPosOld->x + fRadius &&
					m_pos.x + m_vtxMin.x < pPos->x + fRadius)
				{// 左からの判定

					pPos->x = m_pos.x + m_vtxMin.x - fRadius;			// オブジェクトの位置を戻す

				}
				else if (m_pos.x + m_vtxMax.x <= pPosOld->x - fRadius &&
					m_pos.x + m_vtxMax.x > pPos->x - fRadius)
				{// 右からの判定

					pPos->x = m_pos.x + m_vtxMax.x + fRadius;			// オブジェクトの位置を戻す

				}
			}
		}
	}
	return bRand;
}


//=============================================================================
// 関数名：ゴールとの当たり判定
// 関数の概要：プレイヤーが半径に入ってきたらtrueを返す
//=============================================================================
bool CObject::CollisionGoal(D3DXVECTOR3 *pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot)
{
	bool bCol = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//プレイヤーとの距離
	float fRadius = powf(30.0f, 1.0f);	//半径

	if (fLength < fRadius && pos->y <= m_pos.y + m_fHeight && pos->y >= m_pos.y)
	{//対象の頂点が範囲内だったら
		bCol = true;
		float fAttackAngle = atan2f(pos->x - m_pos.x,
		pos->z - m_pos.z);// 当たった角度を計算
		move->x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
		move->y = 20.0f;
		move->z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
	}
	else
	{
		bCol = false;

	}
	return bCol;
}
