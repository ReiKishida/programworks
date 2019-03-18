//=============================================================================
//
// ボス処理[2D ポリゴン] [boss.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CManager;

class CBoss : public CScene2D
{
public:
	typedef enum
	{
		BOSSSTATE_NONE = 0,	//空の状態
		BOSSSTATE_APPEAR,	//出現状態
		BOSSSTATE_NORMAL,	//通常状態
		BOSSSTATE_DAMAGE,	//ダメージ状態
		BOSSSTATE_DEATH,	//死亡状態
		BOSSSTATE_HACK,		//ハック状態
		BOSSSTATE_MAX		//状態の最大数
	}BOSSSTATE;//ボスの状態
	typedef enum
	{
		BOSS_LOGIC_0 = 0,	//ロジックパターン0
		BOSS_LOGIC_1,		//ロジックパターン1
		BOSS_LOGIC_2,		//ロジックパターン2
	}BOSSLOGIC;//ロジックパターン
	typedef enum
	{
		BOSS_ATACKTYPE_NONE = 0,
		BOSS_ATACKTYPE_NORMAL,//通常弾
		BOSS_ATACKTYPE_BALKAN,//バルカン
		BOSS_ATACKTYPE_SPREAD,//拡散弾
		BOSS_ATACKTYPE_LASER,//レーザー
		BOSS_ATTACK_MAX//攻撃パターンの最大数
	}BOSSATTACK;
	CBoss::CBoss();	//コンストラクタ
	CBoss::~CBoss();	//デストラクタ
	static CBoss *Create(D3DXVECTOR3 pos);	//ボスの生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄
	void HitDamage(int nValue);//ダメージ処理
	static BOSSSTATE GetState(void);	//状態取得
	static void SetState(BOSSSTATE state);//状態設定
	void MovePattern0(void);//移動パターン(1:出現時)
	void MovePattern1(void);//移動パターン(2:左右移動)
	void MovePattern2(void);//移動パターン(3:三角移動)
	void AttackPattern0(void);//攻撃パターン(1:通常攻撃、レーザー)
	void AttackPattern1(void);//攻撃パターン(2:通常攻撃、拡散)
	void AttackPattern2(void);//攻撃パターン(3:全部)



	static void MoveInit(float dest_x, float dest_y, int state);//移動パターン(3:三角移動)

private:
	int m_nCntFlame;//フレーム数のカウント
	float m_fBossMove;//ボスの移動量
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	static D3DXVECTOR3 m_pos;	//座標
	static D3DXVECTOR3 m_posold;	//以前の座標
	static D3DXVECTOR3 m_Destpos;	//目的の座標
	static D3DXVECTOR3 m_move;	//移動量
	static D3DXVECTOR3 m_Moverot;	//移動量
	static BOSSSTATE m_state;	//ボスの状態
	int m_nLife;	//ライフ
	static bool m_bUse;	//生きているかどうか
	BOSSATTACK m_BossAttack;//ボスの攻撃パターン
	BOSSLOGIC m_Logic;//ボスのロジックパターン
	int m_nPatternAnim;	//アニメーションパターン
	int m_nCounterAnim;//アニメーションカウンター
	int m_nTex_Y;//テクスチャ移動用
	int m_nAttack;//攻撃力
	int m_nSpread;//拡散
	int m_nSpeed;//速度
	static int m_nCntWait;	//待機時間
	static int m_nCntCoolTime;	//クールタイム
	static int m_nHackTime;//ハックタイム
	static bool m_bWait;	//待機時間
	static bool m_bSwitch;	//待機時間
	static int m_nMoveTime;	//移動継続時間
	static float m_fAngle;	//アングル
	static int m_nMovePattern;//移動パターン
	static int m_MoveState;//移動
};

#endif