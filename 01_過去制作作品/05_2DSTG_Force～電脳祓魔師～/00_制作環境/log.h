//=============================================================================
//
// ログ処理[2D ポリゴン] [log.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _LOG_H_
#define _LOG_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LOGTEXTURE_NUM (5)
#define MAX_LOG_NUM (8)

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CManager;

class CLog : public CScene2D
{
public:
	typedef enum
	{
		LOGTYPE_NORMAL = 0,	//通常状態
		LOGTYPE_ATTACK,		//攻撃タイプ
		LOGTYPE_SPREAD,		//拡散型
		LOGTYPE_SPEED,		//スピード型
		LOGTYPE_DEFENSE,	//防御型
	}LOGTYPE;//ログのタイプ
	typedef enum
	{
		LOGNUM_ZERO = 0,	//0
		LOGNUM_FIRST,		//1
		LOGNUM_SECOND,		//2
		LOGNUM_THIRD,		//3
		LOGNUM_FORTH,		//4
		LOGNUM_FIFTH,		//5
		LOGNUM_SIXTH,		//6
		LOGNUM_SEVENTH,		//7

	}LOGNUM;//ログのタイプ
	CLog();	//コンストラクタ
	~CLog();	//デストラクタ
	static CLog *Create(D3DXVECTOR3 pos, LOGTYPE type);	//ログの生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄
	void MoveLog(void);	//ログの移動
	void MoveInit(float dest_x, float dest_y, int state);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_LOGTEXTURE_NUM]; //共有テクスチャのポインタ
	D3DXCOLOR m_col;		//色情報
	D3DXVECTOR3 m_pos;		//位置情報
	D3DXVECTOR3 m_Destpos;	//目的の位置
	D3DXVECTOR3 m_posold;	//目的の位置
	LOGTYPE m_type;			//ログのタイプ
	LOGNUM m_lognum;		//ログの番号
	static int m_nCntLog;	//ログの数
	static int m_nCntLogOld;	//過去のログの数
	int m_MoveState;	//移動情報
	float m_fAngle;	//角度情報
	//static bool m_bMove;

};

#endif