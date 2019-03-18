//=============================================================================
//
// 当たり判定処理[概念] [Collision.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "collision.h"
#include "scene2D.h"
#include "debugproc.h"
#include "scene_meshorbit.h"
#include "particle.h"
#include "enemy.h"
#include "enemy_pow.h"
#include "enemy_trc.h"
#include "player.h"
#include "boss.h"
#include "game.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR (256)
#define GRAVITY_NUM (0.7f)

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CCollision::CCollision(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CCollision::~CCollision()
{
}



//=============================================================================
// 関数名：当たり判定の生成
// 関数の概要：当たり判定を生成する
//=============================================================================
CCollision * CCollision::CreateAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, float fradius, COLLSIONTYPE type, D3DXVECTOR3 poscreater)
{
	CCollision  *pCollision;
	pCollision = new CCollision;//コリジョンクラスの生成

	pCollision->m_pos = pos;
	pCollision->m_rot = rot;
	pCollision->m_fRadius = fradius;
	pCollision->m_nLife = nLife;
	pCollision->m_type = type;
	pCollision->m_poscreater = poscreater;
	pCollision->Init();
	return pCollision;
}

//=============================================================================
// 関数名：当たり判定の初期化処理
// 関数の概要：Xファイル情報の設定
//=============================================================================
HRESULT CCollision::Init(void)
{
	if (m_type == COLLISIONTYPE_PLAYER_LIGHTPUNCH || m_type == COLLISIONTYPE_PLAYER_MEDIUMPUNCH
		|| m_type == COLLISIONTYPE_PLAYER_HEAVYPUNCH || m_type == COLLISIONTYPE_PLAYER_SHORYUKEN
		|| m_type == COLLISIONTYPE_PLAYER_LIGHTKICK || m_type == COLLISIONTYPE_PLAYER_MEDIUMKICK
		|| m_type == COLLISIONTYPE_PLAYER_HEAVYKICK || m_type == COLLISIONTYPE_PLAYER_SPINKICK
		|| m_type == COLLISIONTYPE_PLAYER_SKILL)
	{

		SetObjType(OBJTYPE_COLLISION);
	}
	else
	{
		SetObjType(OBJTYPE_COLLISION_ENEMY);
	}


	return S_OK;
}

//=============================================================================
// 関数名：当たり判定の終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CCollision::Uninit(void)
{
	//自身の破棄
	Release();
}

//=============================================================================
// 関数名：当たり判定の更新処理
// 関数の概要：--
//=============================================================================
void CCollision::Update(void)
{



		m_nLife--;

		if (m_nLife <= 0)
		{
			m_nLife = 0;
			//終了処理
			Uninit();
		}

		//CDebugProc::Print("当たり判定発生\n");

}


//=============================================================================
// 関数名：当たり判定の描画処理
// 関数の概要：--
//=============================================================================
void CCollision::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//親のマトリックス
	D3DXMATRIX mtxParent;


	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	//親のマトリックスと掛け合わせる
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}
//=============================================================================
// 関数名：位置情報の取得
// 関数の概要：位置情報を取得する
//=============================================================================
D3DXVECTOR3 CCollision::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 関数名：位置情報の代入
// 関数の概要：位置情報を代入する
//=============================================================================
void CCollision::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//=============================================================================
// 関数名：角度情報の代入
// 関数の概要：角度情報を代入する
//=============================================================================
void CCollision::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 関数名：ワールドマトリックスの代入
// 関数の概要：ワールドマトリックス情報を代入する
//=============================================================================
void CCollision::SetMtx(D3DXMATRIX mtx)
{
	m_mtxWorld = m_mtxWorld;
}

//=============================================================================
// 関数名：ライフの代入
// 関数の概要：ライフを代入する
//=============================================================================
void CCollision::SetLife(int nLife)
{
	m_nLife = nLife;
}


//=============================================================================
// 関数名：ワールドマトリックスの取得
// 関数の概要：ワールドマトリックス情報を取得する
//=============================================================================
D3DXMATRIX CCollision::GetMatrix(void)
{
	return m_mtxWorld;
}

//=============================================================================
// 関数名：親のモデル
// 関数の概要：親のモデルの設定
//=============================================================================
void CCollision::SetParent(CCollision *pCollision)
{
	m_pParent = pCollision;
}



//=============================================================================
// 関数名：当たり判定（攻撃）
// 関数の概要：攻撃->体の当たり判定
//=============================================================================
bool CCollision::CollisionAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float fHeight)
{
	bool bCollision = false;

	D3DXVECTOR3 ColPos;
	float fLength = 0.0f;		//長さ
	float fRadius = 0.0f;		//半径


	//m_fAngle = atan2f(pos->x - m_poscreater.x, pos->z - m_poscreater.z);// 当たった角度を計算

	ColPos.x = (pos->x - m_pos.x) * (pos->x - m_pos.x);
	ColPos.y = (pos->y - m_pos.y) * (pos->y - m_pos.y);
	ColPos.z = (pos->z - m_pos.z) * (pos->z - m_pos.z);
	fLength = ColPos.x + ColPos.y + ColPos.z;
	fRadius = (m_fRadius + fRadius) * (m_fRadius + fRadius);

	if (m_pos.y <= pos->y + fHeight  && m_pos.y >= pos->y)
	{//半径内に入ったら

		if (ColPos.x + ColPos.z <= fRadius)
		{

			bCollision = true;

			switch (m_type)
			{
			case COLLISIONTYPE_PLAYER_LIGHTPUNCH:
				m_nTypeCollision = COLLISIONTYPE_PLAYER_LIGHTPUNCH;
				break;
			case COLLISIONTYPE_PLAYER_MEDIUMPUNCH:
				m_nTypeCollision = 1;
				break;
			case COLLISIONTYPE_PLAYER_SHORYUKEN:
				m_nTypeCollision = 2;
				break;
			case COLLISIONTYPE_PLAYER_HEAVYPUNCH:
				m_nTypeCollision = 3;
				break;
			case COLLISIONTYPE_PLAYER_LIGHTKICK:
				m_nTypeCollision = 4;
				break;
			case COLLISIONTYPE_PLAYER_MEDIUMKICK:
				m_nTypeCollision = 5;
				break;
			case COLLISIONTYPE_PLAYER_SPINKICK:
				m_nTypeCollision = 6;
				break;
			case COLLISIONTYPE_PLAYER_HEAVYKICK:
				m_nTypeCollision = 7;
				break;
			case COLLISIONTYPE_PLAYER_SKILL:
				m_nTypeCollision = 8;
				break;
			case COLLSIONTYPE_ENEMY_LIGHTATTACK:
				m_nTypeCollision = 9;
				break;
			case COLLSIONTYPE_ENEMY_HEAVYATTACK:
				m_nTypeCollision = 10;
				break;
			case COLLSIONTYPE_BOSS_LIGHTATTACK:
				m_nTypeCollision = 11;
				break;
			case COLLSIONTYPE_BOSS_HEAVYATTACK:
				m_nTypeCollision = 12;
				break;
			case COLLSIONTYPE_BOSS_SKILLATTACK_S:
				m_nTypeCollision = 13;
				break;
			case COLLSIONTYPE_BOSS_SKILLATTACK_L:
				m_nTypeCollision = 14;
				break;
			}
		}
	}
	return bCollision;
}

//=============================================================================
// 関数名：当たり判定の種類の取得
// 関数の概要：判定の種類を返す
//=============================================================================
int CCollision::GetCollisionType(void)
{
	return m_nTypeCollision;
}

float CCollision::GetAngle(void)
{
	return m_fAngle;
}









