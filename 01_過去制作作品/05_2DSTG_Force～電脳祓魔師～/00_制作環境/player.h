//=============================================================================
//
// プレイヤー処理[2D ポリゴン] [player.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CManager;

class CPlayer : public CScene2D
{
public:
	typedef enum
	{
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_APPEAR,
		PLAYERSTATE_NORMAL,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_DEATH,
		PLAYERSTATE_HACK,
		PLAYERSTATE_MAX
	}PLAYERSTATE;
	CPlayer::CPlayer();	//コンストラクタ
	CPlayer::~CPlayer();	//デストラクタ
	static CPlayer *Create(D3DXVECTOR3 pos);	//プレイヤーの生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);
	static void Unload(void);
	void HitDamage(int nValue);
	static bool GetbUse(void);
	static PLAYERSTATE GetState(void);
	static void SetState(PLAYERSTATE state);
	static D3DXVECTOR3 GetPos(void);
private:
	int m_nCntFrame;//フレーム数のカウント
	float m_fPlayerMove;//プレイヤーの移動量
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	static D3DXVECTOR3 m_pos;	//移動量
	D3DXVECTOR3 m_move;	//移動量
	static PLAYERSTATE m_Pstate;	//プレイヤーの状態
	static int m_nLife;	//ライフ
	static bool m_bUse;	//生きているかどうか
	int m_nPatternAnim;	//アニメーションパターン
	int m_nCounterAnim;//アニメーションカウンター
	int m_nTex_Y;//テクスチャ移動用
	int m_nAttack;//攻撃力
	int m_nSpread;//拡散力
	int m_nSpeed;//スピード
	static int m_nRespawn;
	static D3DXCOLOR m_PlayerCol;
};

#endif