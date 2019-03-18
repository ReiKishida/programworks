//=============================================================================
//
// 当たり判定処理[概念] [collision.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CCollision : public CScene
{
public:

	typedef enum
	{
		COLLISIONTYPE_PLAYER_LIGHTPUNCH = 0,	//プレイヤーの攻撃(弱パンチ)
		COLLISIONTYPE_PLAYER_MEDIUMPUNCH,		//プレイヤーの攻撃(中パンチ)
		COLLISIONTYPE_PLAYER_SHORYUKEN,			//プレイヤーの攻撃(昇竜拳)
		COLLISIONTYPE_PLAYER_HEAVYPUNCH,		//プレイヤーの攻撃(強パンチ)
		COLLISIONTYPE_PLAYER_LIGHTKICK,			//プレイヤーの攻撃(弱キック)
		COLLISIONTYPE_PLAYER_MEDIUMKICK,		//プレイヤーの攻撃(中キック)
		COLLISIONTYPE_PLAYER_SPINKICK,			//プレイヤーの攻撃(回転蹴り)
		COLLISIONTYPE_PLAYER_HEAVYKICK,			//プレイヤーの攻撃(強キック)
		COLLISIONTYPE_PLAYER_SKILL,				//プレイヤーの攻撃(超必殺技)
		COLLSIONTYPE_ENEMY_LIGHTATTACK,			//敵の攻撃(弱攻撃)
		COLLSIONTYPE_ENEMY_HEAVYATTACK,			//敵の攻撃(強攻撃)
		COLLSIONTYPE_BOSS_LIGHTATTACK,			//ボスの攻撃(弱攻撃)
		COLLSIONTYPE_BOSS_HEAVYATTACK,			//ボスの攻撃(強攻撃)
		COLLSIONTYPE_BOSS_SKILLATTACK_S,			//ボスの攻撃(必殺技)
		COLLSIONTYPE_BOSS_SKILLATTACK_L,			//ボスの攻撃(必殺技大)
		COLLSIONTYPE_MAX						//攻撃最大数
	}COLLSIONTYPE;

	CCollision(int nPriority = 6, OBJTYPE type = OBJTYPE_COLLISION);	//コンストラクタ
	~CCollision();	//デストラクタ
	static CCollision *CreateAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nLife,  float fradius, COLLSIONTYPE type, D3DXVECTOR3 poscreater);		//当たり判定の
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	D3DXVECTOR3 GetPos(void);		//位置情報の取得
	void SetPos(D3DXVECTOR3 pos);	//位置情報の代入
	void SetRot(D3DXVECTOR3 rot);	//位置情報の代入
	void SetMtx(D3DXMATRIX mtx);	//角度情報の代入
	void SetLife(int nLife);		//ライフの設定
	D3DXMATRIX GetMatrix(void);				//マトリックスの取得
	void SetParent(CCollision *pModel);		//親子関係の設定
	bool CollisionAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float fHeight);	//攻撃の当たり判定
	int GetCollisionType(void);				//種類の取得
	float GetAngle(void);					//角度の取得

private:
	CCollision *m_pParent;				//親モデルのポインタ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_poscreater;			//作成者の位置情報
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_rot;					//角度
	float m_fRadius;					//半径
	float m_fAngle;
	int m_nLife;						//ライフ
	int m_nTypeCollision;				//当たったもの種類
	COLLSIONTYPE m_type;				//判定の種類


};




#endif