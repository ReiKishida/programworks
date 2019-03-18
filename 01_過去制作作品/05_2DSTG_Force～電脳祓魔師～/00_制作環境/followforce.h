//=============================================================================
//
// 追従フォース処理[2D ポリゴン] [followforce.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _FOLLOWFORCE_H_
#define _FOLLOWFORCE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FOLLOWFORCE (4)

class CManager;

class CFollowForce : public CScene
{
public:
	typedef enum
	{
		FOLLOWFORCETYPE_ATTACK = 0,		//攻のフォース
		FOLLOWFORCETYPE_SPREAD,			//拡のフォース
		FOLLOWFORCETYPE_SPEED,			//速のフォース
		FOLLOWFORCETYPE_DEFENSE,		//護のフォース
		FOLLOWFORCETYPE_NONE,			//空のフォース
		FOLLOWFORCETYPE_MAX				//フォースの最大数
	}FOLLOWFORCETYPE;
	typedef struct
	{
		FOLLOWFORCETYPE type;
		D3DXCOLOR col;
	}FORCE;
	CFollowForce();	//コンストラクタ
	~CFollowForce();	//デストラクタ
	static CFollowForce *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetForce(int nCntForce, FOLLOWFORCETYPE type);	//フォースの設定
	static HRESULT Load(void);
	static void Unload(void);


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FOLLOWFORCE]; //共有テクスチャのポインタ
	static CScene2D *m_apPolygon[MAX_FOLLOWFORCE];	//数字
	static int  m_aForce[MAX_FOLLOWFORCE];	//フォース代入用
	FOLLOWFORCETYPE m_atype[MAX_FOLLOWFORCE];//タイプ
	FORCE m_aForceStruct[MAX_FOLLOWFORCE];//フォース構造体
	int m_nCnt;	//フォースのカウント
	D3DXVECTOR3 m_ForceRot;//回転用角度

	float fLengthForce[MAX_FOLLOWFORCE]; //プレイヤーとの長さ
	float fAngleForce; //プレイヤーとの長さ
	D3DXCOLOR m_ForceColor[MAX_FOLLOWFORCE];//フォースの色
	static int m_nNumAttack;	//攻の数
	static int m_nNumSpread;	//速の数
	static int m_nNumSpeed;		//拡の数
	static int m_nNumDefense;	//護の数
	static bool m_bUse;	//生きているかどうか



};

#endif