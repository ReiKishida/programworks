//=============================================================================
//
// ポーズ処理[単独] [pause.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PAUSE_TEXTURE (7)
#define MAX_PAUSE (6)

class CPolygon;

class CPause
{
public:
	typedef enum
	{
		PAUSESTATE_NONE = 0,	//空の状態
		PAUSESTATE_NORMAL,		//通常状態
		PAUSESTATE_USE,			//エンターが押された状態
		PAUSESTATE_MAX			//状態の最大数
	}PAUSESTATE;
	typedef enum
	{
		PAUSETYPE_BACK = 0,		//背景
		PAUSETYPE_FLAME,		//枠
		PAUSETYPE_CONTINUE,		//コンティニュー
		PAUSETYPE_RETRY,			//リトライ
		PAUSETYPE_QUIT,				//クイット
		PAUSETYPE_GUID,				//項目説明
		PAUSETYPE_MAX				//最大数
	}PAUSETYPE;
	typedef enum
	{

		PAUSEMENU_CONTINUE,		//コンティニュー
		PAUSEMENU_RETRY,			//リトライ
		PAUSEMENU_QUIT,				//クイット
		PAUSEMENU_MAX				//最大数
	}PAUSEMENU;
	CPause();	//コンストラクタ
	~CPause();	//デストラクタ
	 HRESULT Init(void);	//初期化処理
	 void Uninit(void);	//終了処理
	 void Update(void);	//更新処理
	 void Draw(void);	//描画処理
	 static void SetSelect(void);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_PAUSE_TEXTURE]; //共有テクスチャのポインタ
	static CPolygon *m_apPolygon[MAX_PAUSE];//シーン2D
	static CPause *m_pPause;	//ポーズ
	static PAUSESTATE m_state[MAX_PAUSE];//状態
	PAUSETYPE m_type;//種類
	PAUSEMENU m_Menu;//ポーズメニューの項目
	static int m_nSelectNum;	//現在選択されている数字
	static int m_nCntFrame;	//フレーム数
};


#endif