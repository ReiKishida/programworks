//=============================================================================
//
// シーン2D処理[2D ポリゴン] [scene2D.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _CUTIN_H_
#define _CUTIN_H_

#include "main.h"
#include "scene2D.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_CUTIN (9)
#define MAX_CUTIN (4)
//*****************************************************************************
// シーン2Dのクラス構造体
//*****************************************************************************
class CCutin : public CScene
{
public:
	typedef enum
	{
		CUTINMODE_NONE = 0,
		CUTINMODE_PLAYER,
		CUTINMODE_ENEMY,
		CUTINMODE_ENEMYAPPEAR,
		CUTINMODE_MAX
	}CUTINMODE;
	typedef enum
	{
		CUTINTYPE_FRAME_PLAYER = 0,
		CUTINTYPE_FRAME_ENEMY,
		CUTINTYPE_FRAME2_PLAYER,
		CUTINTYPE_FRAME2_ENEMY,
		CUTINTYPE_SILHOUETTE_PLAYER,
		CUTINTYPE_SILHOUETTE_ENEMY,
		CUTINTYPE_SKILL_PLAYER,
		CUTINTYPE_SKILL_ENEMY,
		CUTINTYPE_NAME_ENEMY,
		CUTINTYPE_MAX
	}CUTINTYPE;

	typedef enum
	{
		CUTCOUNT_FRAME = 0,
		CUTCOUNT_FRAME2,
		CUTCOUNT_SILHOUETTE,
		CUTCOUNT_SKILL,
		CUTCOUNT_MAX
	}CUTCOUNT;

	CCutin::CCutin(int nPriority = 7, OBJTYPE type = OBJTYPE_CUTIN);	//コンストラクタ
	CCutin::~CCutin();	//デストラクタ
	static HRESULT Load(void);
	static void Unload(void);

	static CCutin *Create(CUTINMODE mode);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
protected:

private:
	static LPDIRECT3DTEXTURE9		m_apTexture[MAX_TEXTURE_CUTIN];				// テクスチャへのポインタ
	static CScene2D *m_apScene2D[MAX_CUTIN];
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_rot;			// ポリゴンの回転
	float m_fWidth;	//ポリゴンの幅
	float m_fHeight;	//ポリゴンの高さ
	int m_nCntflame;		//カウント
	CUTINTYPE m_type;
	CUTCOUNT m_count;
	static CUTINMODE m_mode;
	//static int m_nCntFlame;
};

#endif