//=============================================================================
//
// フォース処理[2D ポリゴン] [force.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _FORCE_H_
#define _FORCE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FORCE (4)

class CManager;

class CForce : public CScene
{
public:
	typedef enum
	{
		FORCETYPE_ATTACK = 0,	//攻のフォース
		FORCETYPE_SPREAD,		//拡のフォース
		FORCETYPE_SPEED,		//速のフォース
		FORCETYPE_DEFENSE,		//護のフォース
		FORCETYPE_NONE,			//空のフォース
		FORCETYPE_MAX			//フォースの最大数
	}FORCETYPE;


	CForce();	//コンストラクタ
	~CForce();	//デストラクタ
	static CForce *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetForce(int nCntForce,FORCETYPE type);	//フォースの設定

	static int GetNumAttack(void);//攻のフォース数の取得
	static int GetNumSpread(void);//拡のフォース数の取得
	static int GetNumSpeed(void);//速のフォース数の取得
	static int GetNumDefense(void);//護のフォース数の取得

	static void CheckDamage(int nDamage);//DPSチェック

	static int GetnCnt(void);//DPS取得

	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FORCE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_FORCE];	//数字
	static int  m_aForce[MAX_FORCE];	//フォース代入用

	FORCETYPE m_type[MAX_FORCE];	//タイプ
	int m_nCnt;	//フォースのカウント

	static int m_nNumAttack;	//攻の数
	static int m_nNumSpread;	//速の数
	static int m_nNumSpeed;		//拡の数
	static int m_nNumDefense;	//護の数

	static int m_nCntDPS;//DPS計算用
	static int  m_nDPS;//DPS計算用
	static int m_nDamage;//DPS計算用


};

#endif