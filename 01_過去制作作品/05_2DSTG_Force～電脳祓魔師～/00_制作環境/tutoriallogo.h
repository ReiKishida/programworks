//=============================================================================
//
// チュートリアル用ポリゴン処理[2D ポリゴン] [tutoriallogo.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TUTORIALLOGO_H_
#define _TUTORIALLOGO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TUTORIAL_LOGO_TEXTURE (2)
#define MAX_TUTORIAL_LOGO (2)

class CManager;

class CTutorialLogo : public CScene
{
public:

	CTutorialLogo();	//コンストラクタ
	~CTutorialLogo();	//デストラクタ
	static CTutorialLogo *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TUTORIAL_LOGO_TEXTURE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_TUTORIAL_LOGO];	//数字

};

#endif