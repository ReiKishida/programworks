//=============================================================================
//
// シーン処理 [scene.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_WIDTH (110) //ポリゴンのXの大きさ
#define POLYGON_HEIGHT (110) //ポリゴンのYの大きさ

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CScene *CScene::m_apScene[MAX_PRIORITY][MAX_POLYGON] = {};	//シーンのインスタンス生成
int CScene::m_nNumAll = 0;	//シーンの総数

//=============================================================================
//コンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{

	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		m_nPriority = nPriority;	//優先順位の番号を代入
		if (m_apScene[m_nPriority][nCntScene] == NULL)
		{
			m_apScene[m_nPriority][nCntScene] = this;//代入
			m_nID = nCntScene;	//自分自身の番号を取得
			m_Objtype = OBJTYPE_NONE;
			m_nNumAll++;
			break;
		}
	}

}

//=============================================================================
//デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
//総数の取得
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
//全てのオブジェクトの更新
//=============================================================================
void CScene::UpdateAll(void)
{
	int nIndex;
	nIndex = m_nNumAll;
	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
//全てのオブジェクトの描画
//=============================================================================
void CScene::DrawAll(void)
{
	int nIndex;
	nIndex = m_nNumAll;
	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
//オブジェクトの取得
//=============================================================================
//CScene **CScene::GetScene(void)
//{
//	return m_apScene;
//}

//=============================================================================
// 全てのオブジェクトの破棄
//=============================================================================
void CScene::ReleaseSceneAll(void)
{
	int nIndex;
	nIndex = m_nNumAll;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				delete m_apScene[nCntPriority][nCntScene];
				m_apScene[nCntPriority][nCntScene] = NULL;
			}
		}
	}
}

//=============================================================================
// オブジェクトの破棄
//=============================================================================
void CScene::Release(void)
{

		if (m_apScene[m_nPriority][m_nID] != NULL)
		{
			int nPriority;//自分の優先順位
			nPriority = m_nPriority;
			int nID;	//自分の数字
			nID = m_nID;
			delete m_apScene[nPriority][nID];
			m_apScene[nPriority][nID] = NULL;
			m_nNumAll--;
		}

}

//=============================================================================
// オブジェクトの種類の設定
//=============================================================================
void CScene::SetObjType(OBJTYPE objtype)
{
	//オブジェクトの種類の代入
	m_Objtype = objtype;
}

//=============================================================================
// オブジェクトの種類の取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
 	return m_Objtype;
}


//=============================================================================
// そのオブジェクトの取得
//=============================================================================
CScene *CScene::GetScene(int nPriority, int nIndex)
{

		return m_apScene[nPriority][nIndex];

}
