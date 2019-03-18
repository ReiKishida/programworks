//=============================================================================
//
// シーン処理 [scene.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_POLYGON (2000)	//オブジェクトの総数
#define PRIORITY_NUMBER (8)	//オブジェクトの優先順位

class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,		//空の種類
		OBJTYPE_FADE,			//フェード
		OBJTYPE_PLAYER,			//プレイヤー
		OBJTYPE_LOG,			//ログ
		OBJTYPE_ENEMY,			//敵
		OBJTYPE_BOSS,			//ボス
		OBJTYPE_CUTIN,			//カットイン
		OBJTYPE_BULLET,			//弾
		OBJTYPE_EXPLOSION,		//爆発
		OBJTYPE_BG,				//背景
		OBJTYPE_UI,				//UI
		OBJTYPE_MENU,			//メニュー
		OBJTYPE_EFFECT,			//エフェクト
		OBJTYPE_FORCE,			//フォース
		OBJTYPE_TITLELOGO,		//タイトルロゴ
		OBJTYPE_RANKINGLOOG,	//ランキングロゴ
		OBJTYPE_TUTORIALLOGO,	//ランキングロゴ
		OBJTYPE_FORCENUMBER,	//スコア
		OBJTYPE_SCORE,			//スコア
		OBJTYPE_RANKINGNUNBER,			//ランキング数字
		OBJTYPE_REMAINING,		//残機数
		OBJTYPE_MAX				//オブジェクトの種類の最大数
	}OBJTYPE;//オブジェクトの種類
	CScene::CScene(int nPriority = 4);	//コンストラクタ
	virtual ~CScene();	//デストラクタ
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;	//初期化処理
	virtual void Uninit(void) = 0;	//終了処理
	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理
	void SetObjType(OBJTYPE objtype);	//オブジェクトの種類の設定
	OBJTYPE GetObjType(void);	//オブジェクトの種類の取得
	static CScene *GetScene(int nPriority, int nIndex);	//シーンの取得(インデックス)

	static void ReleaseSceneAll(void);		//全てのシーンの破棄
	static void UpdateAll(void);			//全てのシーンの更新
	static void DrawAll(void);				//全てのシーンの描画
	static int GetNumAll(void);				//シーンの総数の取得
	//static CScene **GetScene(void);			//シーンの取得

protected:
	void Release(void);	//オブジェクトの破棄
private:
	int m_nPriority;	//優先順位の番号
	static int m_nNumAll;	//シーンの総数
	static CScene *m_apScene[MAX_PRIORITY][MAX_POLYGON];	//シーンのインスタンス生成
	int m_nID;//自分の番号
	OBJTYPE m_Objtype;//オブジェクトの種類
};

#endif