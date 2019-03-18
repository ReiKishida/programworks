//=============================================================================
//
// エフェクト処理[3D ポリゴン] [effect.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene_billboard.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAXPARTICLE_TEXTURE (3)
//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CSceneBillBoard;
//*****************************************************************************
// パーティクルのクラス構造体
//*****************************************************************************
class CParticle : public CSceneBillBoard
{

public:
	typedef enum
	{
		PARTICLEDRAWTYPE_ADD = 0,	//加算合成
		PARTICLEDRAWTYPE_DEST,		//減算合成
		PARTICLEDRAWTYPE_MAX		//描画タイプの最大数
	}PARTICLEDRAWTYPE;
	typedef enum
	{
		PARTICLETYPE_TEST = 0,			//テスト用パーティクル
		PARTICLETYPE_ATTACK_000,		//攻撃時のパーティクル000
		PARTICLETYPE_ATTACK_SHORYUKEN,	//昇竜拳のパーティクル
		PARTICLETYPE_ATTACK_SKILL,		//超必殺技のパーティクル
		PARTICLETYPE_ATTACK_SKILL_ENEMY,		//超必殺技のパーティクル
		PARTICLETYPE_ATTACK_SPINKICK,	//回転蹴りのパーティクル
		PARTICLETYPE_HIT_000,			//ヒット時のパーティクル000
		PARTICLETYPE_HIT_001,			//ヒット時のパーティクル001
		PARTICLETYPE_HIT_002,			//ヒット時のパーティクル002
		PARTICLETYPE_RAND,				//着地時のパーティクル
		PARTICLETYPE_SLIDING,			//スライディング時のパーティクル
		PARTICLETYPE_RUN,				//走っている時のパーティクル
		PARTICLETYPE_MAX				//パーティクルの種類の最大数
	}PARTICLETYPE;
	CParticle(int nPriority = 7, OBJTYPE type = OBJTYPE_PARTICLE);	//コンストラクタ
	~CParticle();	//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み
	static void Unload(void);									//テクスチャの破棄
	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLEDRAWTYPE drawtype, PARTICLETYPE type, int nLife);	//パーティクルの生成
	HRESULT Init(void);					//初期化処理
	void Uninit(void);					//終了処理
	void Update(void);					//更新処理
	void Draw(void);					//描画処理

private:
	static LPDIRECT3DTEXTURE9		m_apTexture[MAXPARTICLE_TEXTURE];		// テクスチャへのポインタ
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_move;			// ポリゴンの移動量
	D3DXVECTOR3				m_rot;			// ポリゴンの回転
	D3DXCOLOR				m_col;			//	色情報
	PARTICLEDRAWTYPE        m_Drawtype;		// 描画の種類
	PARTICLETYPE			m_Type;			// パーティクルの種類
	int						m_nLife;		// 寿命
	int						m_ParticleTextype;	//パーティクルのテクスチャ
	float					m_fRadius;		// 半径

};
#endif