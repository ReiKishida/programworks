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
#include "debugproc.h"
#include "input.h"
#include "manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CScene *CScene::m_pTop[PRIORITY_NUMBER] = {};	//先頭のオブジェクトのポインタ
CScene *CScene::m_pCur[PRIORITY_NUMBER] = {};	//現在のオブジェクトのポインタ
int CScene::m_nNumAll = 0;	//シーンの総数
bool CScene::m_bStop = false;	//止めるか
bool CScene::m_bBossStop = false;	//止めるか

//=============================================================================
// 関数名：コンストラクタ
// 関数の機能：生成されたときに呼び出される。オブジェクトのつなぎ合わせ。
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE type)
{
	m_bDeath = false;
	m_nPriority  = nPriority;	//優先順位の番号

		if (m_pTop[m_nPriority] == NULL)
		{//最初がなかったら
			m_pTop[m_nPriority] = this;//最初のポインタは生成したものに
		}

		if (m_pCur[m_nPriority] != NULL)
		{//今があったら(2個目以降のものが生成されたとき)
			m_pCur[m_nPriority]->m_pNext = this;//次のポインタは生成したもの
		}
		this->m_pPrev = m_pCur[m_nPriority]; // 前のポインタは今のポインタもの(最初の1個目のときはm_pCurにはなにも入ってないからNULL)
		this->m_pNext = NULL; // 次のポインタは空
		m_pCur[m_nPriority] = this; // 今のポインタは生成されたもの
		m_Objtype = type;// オブジェクトの種類の代入
		m_nNumAll++;// 総数を加算


}

//=============================================================================
// 関数名：デストラクタ
// 関数の機能：最後に呼び出される
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// 関数名：総数の取得
// 関数の機能：オブジェクトの総数を返す
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
// 関数名：停止の是非
// 関数の機能：――
//=============================================================================
void CScene::ChangeStop(void)
{
	m_bStop = m_bStop ? false : true;
}

void CScene::SetStop(bool bStop)
{
	m_bStop = bStop;
}

void CScene::SetBossStop(bool bStop)
{
	m_bBossStop = bStop;
}

//=============================================================================
// 関数名：停止の是非
// 関数の機能：――
//=============================================================================

void CScene::ChangeStopBoss(void)
{
	m_bBossStop = m_bBossStop ? false : true;
}

void CScene::SetStopBoss(bool bStop)
{
	m_bBossStop = bStop;
}



//=============================================================================
// 関数名：全てのオブジェクトの更新
// 関数の機能：全てのオブジェクトを更新する。更新後は死亡フラグが立ったものを破棄する
//=============================================================================
void CScene::UpdateAll(void)
{

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true)
	{
		ChangeStop();
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true)
	{
		ChangeStopBoss();
	}


	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{

		CScene	*pCur = m_pTop[nCntPriority];	//今実行するもの

		while (pCur != NULL)
		{//次に実行するものがあれば
			CScene	*pNext = pCur->m_pNext; //次実行するもの
			if (m_bStop == true)
			{
				if (pCur->m_Objtype == OBJTYPE_PLAYER || pCur->m_Objtype == OBJTYPE_COLLISION ||  pCur->m_Objtype == OBJTYPE_OBJECT || pCur->m_Objtype == OBJTYPE_PARTICLE || pCur->m_Objtype == OBJTYPE_EFFECT || pCur->m_Objtype == OBJTYPE_UIGAUGE || pCur->m_Objtype == OBJTYPE_PR || pCur->m_Objtype == OBJTYPE_CUTIN)
				{
					pCur->Update();//更新処理
				}
			}
			else if (m_bBossStop == true)
			{
				if (pCur->m_Objtype == OBJTYPE_BOSS || pCur->m_Objtype == OBJTYPE_COLLISION_ENEMY || pCur->m_Objtype == OBJTYPE_OBJECT || pCur->m_Objtype == OBJTYPE_PARTICLE || pCur->m_Objtype == OBJTYPE_EFFECT || pCur->m_Objtype == OBJTYPE_UIGAUGE || pCur->m_Objtype == OBJTYPE_PR || pCur->m_Objtype == OBJTYPE_CUTIN)
				{
					pCur->Update();//更新処理
				}
			}
			else
			{
				pCur->Update();//更新処理
			}

			pCur = pNext;//今実行するものに次実行するものを代入
		}
	}
	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		CScene	*pCur = m_pTop[nCntPriority];	//今実行するもの

		//===========================================================//
		//	Updateが終わった後死亡フラグが立てられた奴らを一瞬千撃	//
		//=========================================================//
		pCur = m_pTop[nCntPriority];	//今実行するもの
		while (pCur != NULL)
		{//次に実行するものがあれば
			CScene	*pNext = pCur->m_pNext; //次実行するもの(実行中に消えたら困るから)
			pCur->CheckDeath();//死亡フラグ調べ
			pCur = pNext;//今実行するものに次実行するものを代入
		}

	}



}

//=============================================================================
// 関数名：全てのオブジェクトの描画
// 関数の機能：全てのオブジェクトを描画する
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		CScene	*pCur = m_pTop[nCntPriority];	//今実行するもの

		while (pCur != NULL)
		{//次に実行するものがあれば
			CScene	*pNext = pCur->m_pNext; //次実行するもの(実行中に消えたら困るから)
			pCur->Draw();//更新処理
			pCur = pNext;//今実行するものに次実行するものを代入
		}
	}

}


//=============================================================================
// 関数名：全てのオブジェクトの破棄
// 関数の機能：オブジェクトを全て破棄する。最初、最後のポインタを空にしてあげる
//=============================================================================
void CScene::ReleaseSceneAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		CScene	*pCur = m_pTop[nCntPriority];	//今実行するもの

		while (pCur != NULL)
		{//次に実行するものがあれば
			CScene	*pNext = pCur->m_pNext; //次実行するもの
			pCur->Uninit();//終了処理
			pCur = pNext;//今実行するものに次実行するものを代入
		}



		//===================================================================//
		//	ReleaseSceneAllが終わった後死亡フラグが立てられた奴らを一瞬千撃	//
		//=================================================================//
		pCur = m_pTop[nCntPriority];	//今実行するもの
		while (pCur != NULL)
		{//次に実行するものがあれば
			CScene	*pNext = pCur->m_pNext; //次実行するもの(実行中に消えたら困るから)
			pCur->CheckDeath();//死亡フラグ調べ
			pCur = pNext;//今実行するものに次実行するものを代入
		}

		m_pTop[nCntPriority] = NULL;//最初を空にする
		m_pCur[nCntPriority] = NULL;//今の情報を空にする

	}

	m_nNumAll = 0;	//総数をクリアに

}

//=============================================================================
// 関数名：オブジェクトの破棄
// 関数の機能：死亡フラグを立てる。立てたものはあとで消し去る
//=============================================================================
void CScene::Release(void)
{
	m_bDeath = true;
}

//=============================================================================
// 関数名：オブジェクトの種類の設定
// 関数の機能：オブジェクトの種類を設定する(種類はヘッダーに)
//=============================================================================
void CScene::SetObjType(OBJTYPE objtype)
{
	//オブジェクトの種類の代入
	m_Objtype = objtype;
}

//=============================================================================
// 関数名：オブジェクトの種類の取得
// 関数の機能：オブジェクトの種類を渡してあげる
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
 	return m_Objtype;
}

//=============================================================================
// 関数名：死亡フラグのチェック
// 関数の機能：死亡フラグが立てられているものを消す
//=============================================================================
void CScene::CheckDeath(void)
{

		if (this->m_bDeath == true)
		{//死亡フラグが立てられていたら

			if (m_pPrev != NULL)
			{//消すやつの前があるとき(つながりの間が消える時)
				this->m_pPrev->m_pNext = this->m_pNext;;//"消すやつの前" の "次 "　"は"　"消すやつ"の "次"
			}
			else
			{//消すやつの前がないとき("最初"から順に消えていっているとき)
				m_pTop[m_nPriority] = this->m_pNext;;//"最初"　"は"　"消すやつ" の "次"
			}
			if (this->m_pNext != NULL)
			{//消すやつの次があるとき(つながりの間が消える時)
				this->m_pNext->m_pPrev = this->m_pPrev;// "消すやつの次 " の "前"　"は"　"消すやつ" の "前"
			}
			else
			{//消すやつの後ろがないとき("最後"から順に消えていっているとき)
				m_pCur[m_nPriority] = this->m_pPrev;//"最後"　"は"　"消すやつ"の "前"
			}
			delete this;//"呼ばれたもの" を "消す"
			m_nNumAll--;//総数を減算

		}

}

//=============================================================================
// 関数名：停止の取得
// 関数の機能：停止を返す
//=============================================================================
bool CScene::GetStop(void)
{
	return m_bStop;
}

bool CScene::GetBossStop(void)
{
	return m_bBossStop;
}


//=============================================================================
// 関数名：優先順位の取得
// 関数の機能：優先順位を返す
//=============================================================================
int CScene::GetPriority(void)
{
	return m_nPriority;
}


//=============================================================================
// 関数名：オブジェクト先頭の取得
// 関数の機能：先頭シーンを返す
//=============================================================================
CScene *CScene::GetSceneTop(int nIndex)
{
	return m_pTop[nIndex];
}

//=============================================================================
// 関数名：オブジェクトの取得
// 関数の機能：次のシーンを返す
//=============================================================================
CScene *CScene::GetSceneNext(int nPriority)
{
	return this->m_pNext;
}
