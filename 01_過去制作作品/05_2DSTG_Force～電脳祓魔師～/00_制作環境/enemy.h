//=============================================================================
//
// 敵の処理[2D ポリゴン] [enemy.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYTYPE (5)			//敵の種類

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CManager;

class CEnemy : public CScene2D
{
public:
	typedef enum
	{
		ENEMYTYPE_NONE = 0,		//空白の敵
		ENEMYTYPE_NORMAL,		//通常の敵
		ENEMYTYPE_ATTACK,		//攻撃型の敵
		ENEMYTYPE_SPREAD,		//拡散型型の敵
		ENEMYTYPE_SPEED,		//弾速の速い型の敵
		ENEMYTYPE_DEFENSE,		//耐久型型の敵
		ENEMYTYPE_MAX			//敵の種類の最大数
	}ENEMYTYPE;//敵の種類
	typedef enum
	{
		ENEMYSTATE_NONE = 0,	//空白の状態
		ENEMYSTATE_NORMAL,		//通常状態
		ENEMYSTATE_COOLDOWN,	//クールダウン(次の弾発射までの間隔)
		ENEMYSTATE_MAX			//敵の状態の最大数
	}ENEMYSTATE;//敵の状態
	typedef enum
	{
		ENEMYMOVE_NONE = 0,			//空の敵の移動
		ENEMYMOVE_THROUGH_UPPER,	//敵の移動(下から上へ)
		ENEMYMOVE_THROUGH_UNDER,	//敵の移動(上から下へ)
		ENEMYMOVE_THROUGH_LEFT,		//敵の移動(右から左へ)
		ENEMYMOVE_THROUGH_RIGHT,	//敵の移動(左から右へ)
		ENEMYMOVE_FAN_R_LONG,		//敵の移動(扇形(距離長)：右へ)
		ENEMYMOVE_FAN_R_MIDIUM,		//敵の移動(扇形(距離中)：右へ)
		ENEMYMOVE_FAN_L_LONG,		//敵の移動(扇形(距離長)：左へ)
		ENEMYMOVE_FAN_L_MIDIUM,		//敵の移動(扇形(距離中)：左へ)
		ENEMYMOVE_STOP_R,			//敵の移動(画面中央から右下へはける)
		ENEMYMOVE_STOP_L,			//敵の移動(画面中央から左下へはける)
		ENEMYMOVE_STOP,				//敵の移動(止まる)
		ENEMYMOVE_MAX				//敵の移動の最大数
	}ENEMYMOVE;//敵の移動方法
	CEnemy::CEnemy();	//コンストラクタ
	CEnemy::~CEnemy();	//デストラクタ
	static CEnemy *Create(D3DXVECTOR3 pos, ENEMYTYPE type, ENEMYMOVE movetype);	//敵の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの破棄
	D3DXVECTOR3 GetPosEnemy(void);	//位置情報の取得
	void CheckLife(int nLife);
private:
	int m_nCntFrame;	//カウンタ用
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ENEMYTYPE]; //共有テクスチャのポインタ
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_pos;	//移動量
	D3DXVECTOR3 m_rot;//回転用角度
	int m_nLife;	//ライフ
	static int m_nNumEnemy;//敵の数
	ENEMYTYPE m_type;	//タイプ
	ENEMYMOVE m_movetype;//移動方法
	ENEMYSTATE m_state;//敵の状態
	int m_nCoolDown;//クールダウン
};

#endif