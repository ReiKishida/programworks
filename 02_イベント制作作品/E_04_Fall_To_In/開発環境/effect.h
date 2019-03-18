//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 亀田 憲尚
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EFFECT (1024)	// エフェクトの最大数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CSceneEffect
{// エフェクトポリゴンクラス
public:
	typedef enum
	{// エフェクトの種類
		TYPE_CIRCLE = 0,	// 円
		TYPE_SHINY,			// キラキラ
		TYPE_EXPLOSION,		// 爆発
		TYPE_INK,		// インク
		TYPE_MAX			// 種類の総数
	} TYPE;

	typedef enum
	{// エフェクトのサイズ変化の種類
		SCALETYPE_NONE = 0,	// 変わらない
		SCALETYPE_UP,		// 拡大
		SCALETYPE_DOWN,		// 縮小
		SCALETYPE_UPDOWN,	// 拡縮
		SCALETYPE_MAX		// 種類の総数
	} SCALETYPE;
	CSceneEffect();
	~CSceneEffect();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const TYPE type, const SCALETYPE scaleType, const float fTrans);
	bool GetUse(void) { return m_bUse; }
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[TYPE_MAX];	// 共有テクスチャへのポインタ配列
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// 頂点バッファへのポインタ
	D3DXMATRIX					m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_move;					// 移動量
	float						m_fRadius;				// 半径
	float						m_fRadiusInit;			// 初期半径
	D3DXCOLOR					m_col;					// 色
	int							m_nLife;				// 表示時間
	float						m_fRot;					// 回転軸
	float						m_fAngle;				// 角度
	float						m_fLength;				// 対角線の長さ
	TYPE						m_type;					// 種類
	SCALETYPE					m_scaleType;			// サイズ変化の種類
	float						m_fTrans;				// 拡縮度
	float						m_fAlpha;				// アルファ減少値
	int							m_nCounter;				// カウンター
	bool						m_bUse;					// 使用されているどうか
};

class CEffect : public CScene
{// エフェクトクラス(CSceneの派生クラス)
public:
	CEffect();
	~CEffect();
	static CEffect *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const CSceneEffect::TYPE type, const CSceneEffect::SCALETYPE scaleType, const float fTrans);
	static void SetExplosion(const D3DXVECTOR3 pos, const int nPlayerID);
private:
	static CSceneEffect *m_apEffect[MAX_EFFECT];	// エフェクトクラスのポインタ配列
};

#endif