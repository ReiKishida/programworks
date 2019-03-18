//=============================================================================
//
// 弾の処理[2D ポリゴン] [bullet.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CBullet : public CScene2D
{
public:
	typedef enum
	{
		BULLETTYPE_PLAYER = 0,		//プレイヤーの弾
		BULLETTYPE_ENEMY_NORMAL,	//敵の弾(通常)
		BULLETTYPE_ENEMY_HORMING,	//敵の弾(プレイヤー追尾)
		BULLETTYPE_ENEMY_BOMB,		//敵の弾(ボム)
		BULLETTYPE_ENEMY_LASER,		//レーザー(R)
		BULLETTYPE_ENEMY_LASER_L,		//レーザー(R)
		BULLETTYPE_ENEMY_LASER_R,		//レーザー(L)
		BULLETTYPE_MAX				//弾の種類の最大数
	}BULLETTYPE;
	CBullet::CBullet();	//コンストラクタ
	CBullet::~CBullet();	//デストラクタ
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife ,int nAttack);	//弾の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);
	static void Unload(void);
private:
	int m_nLife;	//ライフ
	int m_nAttack;	//攻撃力
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	D3DXVECTOR3 m_BulletMove;//弾の移動量
	D3DXVECTOR3 m_Move;//移動量
	BULLETTYPE m_type;	//弾の種類
	int m_nCntFrame;
};

#endif