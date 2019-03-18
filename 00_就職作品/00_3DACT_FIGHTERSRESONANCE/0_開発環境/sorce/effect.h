//=============================================================================
//
// エフェクト処理[3D ポリゴン] [effect.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

#include "main.h"
#include "scene_billboard_anim.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CSceneBillBoardAnim;
//*****************************************************************************
// エフェクトのクラス構造体
//*****************************************************************************
class CEffect : public CSceneBillBoardAnim
{
public:
	typedef enum
	{
		EFFECTMODE_SPREAD = 0,
		EFFECTMODE_SHRINKING,
		EFFECTMODE_MAX
	}EFFECTMODE;

	typedef enum
	{
		EFFECTTYPE_SMALL = 0,
		EFFECTTYPE_MEDIUM,
		EFFECTTYPE_LARGE,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect(int nPriority = 3, OBJTYPE type = OBJTYPE_EFFECT);	//コンストラクタ
	~CEffect();	//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み
	static void Unload(void);									//テクスチャの破棄
	static CEffect *Create(D3DXVECTOR3 pos, float fRadius, int nLife, EFFECTTYPE type, EFFECTMODE mode);//エフェクトの生成
	HRESULT Init(void);						//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[3]; //共有テクスチャのポインタ
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_rot;			// ポリゴンの回転
	float m_fRadius;	//幅
	int m_nLife;		//アニメーションカウンター
	EFFECTTYPE m_type;
	EFFECTMODE m_mode;
};

#endif