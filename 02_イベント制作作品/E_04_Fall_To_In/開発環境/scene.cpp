//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"
#include "game.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CScene::m_nNumAll = 0;					// 総数
CScene *CScene::m_apTop[MAX_PRIORITY_NUM] = {};				// 先頭のオブジェクトへのポインタ
CScene *CScene::m_apCur[MAX_PRIORITY_NUM] = {};				// 現在（最後）のオブジェクトへのポインタ
int CScene::m_nNumPriority[MAX_PRIORITY_NUM] = {};// 優先順位ごとの総数
//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	// 優先番号ごとに加算
	m_nNumPriority[nPriority]++;

	if (m_apTop[nPriority] == NULL)
	{// 先頭のオブジェクトを代入
		m_apTop[nPriority] = this;
	}

	if (m_apCur[nPriority] != NULL)
	{// 前回
	 // 前回のオブジェクトの次のオブジェクトを今回のオブジェクトにする
		m_apCur[nPriority]->m_apNext = this;
		// 前回のオブジェクトを代入
		m_apPrev = m_apCur[nPriority];
	}


	// 今回のオブジェクトを代入
	m_apCur[nPriority] = this;
	// オブジェクトタイプを設定
	m_objType = OBJTYPE_NONE;
	m_nPriority = nPriority;
	m_nID = m_nNumAll;
	m_bDeath = false;
	m_nNumAll++;

}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// 全終了処理
//=============================================================================
void CScene::ReleaseAll(void)
{

	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// 優先順位の最大数分回す
		CScene *pScene = m_apTop[nCntP];
		int nCntALL = 0;
		while (pScene != NULL)
		{
			CScene *pSceneNext = pScene->m_apNext;
			m_nNumPriority[pScene->m_nPriority]--;
			m_nNumAll--;
			// 終了処理
			pScene->Uninit();
			delete pScene;
			pScene = NULL;
			pScene = pSceneNext;
			nCntALL++;
		}
		m_apTop[nCntP] = NULL;
		m_apCur[nCntP] = NULL;
	}

}
//=============================================================================
// 全更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	if (!CGame::GetPause())
	{
		for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
		{// 優先順位の最大数分回す
			CScene *pScene = m_apTop[nCntP];

			while (pScene != NULL)
			{// アップデート
				CScene *pSceneNext = pScene->m_apNext;
				pScene->Update();
				pScene = pSceneNext;
			}
		}
	}
	// 死亡フラグ
	DeathFrag();
}
//=============================================================================
// 全描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// 優先順位の最大数分回す
		CScene *pScene = m_apTop[nCntP];
		while (pScene != NULL)
		{
			CScene *pSceneNext = pScene->m_apNext;
			pScene->Draw();
			pScene = pSceneNext;
		}
	}
}
//=============================================================================
// 一部描画
//=============================================================================
void CScene::DrawPart(int nIdy)
{
	CScene *pScene;
	CScene *pSceneNext;
	switch (nIdy)
	{
	case 0:// 通常カメラ
		for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
		{// 優先順位の最大数分回す
			CDebugProc::print("優先順位%d：%d\n", nCntP, m_nNumPriority[nCntP]);
			pScene = m_apTop[nCntP];
			while (pScene != NULL)
			{
				pSceneNext = pScene->m_apNext;
				pScene->Draw();

				pScene = pSceneNext;
			}
		}
		break;
	case 1:// ミニマップ

		break;
	case 2:

		break;
	case 3:		// リザルト
		break;
	}

}
//=============================================================================
// オブジェクトの破棄
//=============================================================================
void CScene::Release(void)
{
	if (this != NULL)
	{
		m_bDeath = true;	// 死亡フラグが立った
	}
}
//=============================================================================
// 死亡フラグたったオブジェクトの破棄
//=============================================================================
void CScene::DeathFrag(void)
{
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// 優先順位の最大数分回す
		CScene *pScene = m_apTop[nCntP];
		while (pScene != NULL)
		{// 死亡フラグチェック
			CScene *pSceneNext = pScene->m_apNext;
			if (pScene->m_bDeath)
			{// 死亡フラグが立った
				if (pScene->m_apPrev != NULL)
				{// 現在の次のオブジェクトを前のオブジェクトに教える
					pScene->m_apPrev->m_apNext = pScene->m_apNext;
				}
				if (pScene->m_apNext != NULL)
				{// 現在の前のオブジェクトを次のオブジェクトに教える
					pScene->m_apNext->m_apPrev = pScene->m_apPrev;
				}
				if (pScene->m_apNext == NULL)
				{// 最後尾を入れ替える
					m_apCur[nCntP] = pScene->m_apPrev;
				}
				if (pScene->m_apPrev == NULL)
				{// 先頭を入れ替える
					m_apTop[nCntP] = pScene->m_apNext;
				}
				m_nNumPriority[pScene->m_nPriority]--;
				m_nNumAll--;

				delete pScene;
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

}
//=============================================================================
// 総数の取得
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;
}
//=============================================================================
// オブジェクトの種類の設定
//=============================================================================
void CScene::SetObjType(CScene::OBJTYPE type)
{
	m_objType = type;
}
//=============================================================================
// オブジェクトの種類の取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}
//=============================================================================
// オブジェクトの最大数超える
//=============================================================================
bool CScene::OverData(void)
{
	bool bOver = false;
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// 優先順位の最大数分回す
		if (m_nNumPriority[nCntP] >= MAX_DATA)
		{// 最大数超えた
			bOver = true;
		}
	}
	return bOver;
}
//=============================================================================
// シーンの最初取得
//=============================================================================
CScene *CScene::GetScene(const int nPriority)
{
	return m_apTop[nPriority];
}