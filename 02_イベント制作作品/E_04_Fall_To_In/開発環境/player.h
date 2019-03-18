//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// プレイヤー処理 [player.h]
// Author:圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// インクルードファイル
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#include "main.h"
#include "scene.h"
#include "motion.h"
#include "model.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 前方宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CModel;
class CCollision;
class CAnimal;
class CFish;
class CVegetable;
class CProvideDisp;
class CButtonMotion;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_KUSHI		(4)
#define MAX_KUSHI_NUM	(3)

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// クラス定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CPlayer : public CMotion
{ // プレイヤークラス
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// 構造体定義
	//+*++*++*++*++*++*++*++*++*++*++*
	typedef enum
	{ // 操作の種類
		CONTROLLER_LEFT_MOVE = 0,	// 左移動
		CONTROLLER_RIGHT_MOVE,		// 右移動
		CONTROLLER_UP_MOVE,			// 上移動
		CONTROLLER_LEFT_UP_MOVE,	// 左上移動
		CONTROLLER_RIGHT_UP_MOVE,	// 右上移動
		CONTROLLER_LEFT_DOWN_MOVE,	// 左下移動
		CONTROLLER_RIGHT_DOWN_MOVE,	// 右下移動
		CONTROLLER_DOWN_MOVE,		// 下移動
		CONTROLLER_ATTACK,			// 攻撃
		CONTROLLER_JUMP,			// ジャンプ
		CONTROLLER_MAX,				// 総数
	}CONTROLLER;

	typedef enum
	{ // 種類
		MOTION_DATA_TYPE_TAKERU = 0,	// ＴＡＫＥＲＵ
		MOTION_DATA_TYPE_ZOZO_RED,			// ZOZO
		MOTION_DATA_TYPE_ZOZO_BLUE,			// ZOZO
		MOTION_DATA_TYPE_MAX,			// 最大数
	}MOTION_DATA_TYPE;

	typedef enum
	{ // モーションの種類
		MOTION_TYPE_NEUTRAL = 0,	// [0]	ニュートラル
		MOTION_TYPE_POSE_00,		// [1]	ポーズ00
		MOTION_TYPE_POSE_01,		// [2]	ポーズ01
		MOTION_TYPE_POSE_02,		// [3]	ポーズ02
		MOTION_TYPE_POSE_03,		// [4]	ポーズ03
		MOTION_TYPE_JUMP,			// [5]  ジャンプ
		MOTION_TYPE_LOOP,			// [6]  ループ
		MOTION_TYPE_WIN,			// [7]  kati
		MOTION_TYPE_LOOSE,			// [8]  make
		MOTION_TYPE_MAX,			// 総数
	}MOTION_TYPE;

	typedef enum
	{ // bool変数の種類
		BOOL_JUMP = 0,		// ジャンプ
		BOOL_MAX
	}BOOL;

	typedef enum
	{ // bool変数の種類
		POSE_BOTTAN_A = 0,		// ボタンA
		POSE_BOTTAN_B,			// ボタンB
		POSE_BOTTAN_X,			// ボタンX
		POSE_BOTTAN_Y,			// ボタンY
		POSE_BUTTON_MAX
	}POSE_BUTTON;

	//+*++*++*++*++*++*++*++*++*++*++*
	// プロトタイプ宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	CPlayer();
	virtual ~CPlayer();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void PlayerOverLap(void);

	// 各種設定取得関数
	void SetId(const int nId) { m_nId = nId; }	// ID設定
	int GetId(void) { return m_nId; }
	float GetRadius(void) { return m_fRadius; }	// 半径取得
	void SetMotion(int nId);

	// 静的メンバ関数
	static CPlayer *Create(int nId);
	static HRESULT Load(void);
	static void UnLoad(void);
	static void RandomPose(void);
	static void ButtonDisp(const float fHeight);

	static MOTION_TYPE GetButtonMotion(POSE_BUTTON button) { return m_buttonMotionType[button]; }

private:
	void Collision(void);
	void Controller(void);
	void Motion(void);
	void MotionTypeUpdate(void);
	bool GetController(CONTROLLER Controller);




	//+*++*++*++*++*++*++*++*++*++*++*
	// 変数宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	float				m_fRadius;					// 当たり判定用半径
	float				m_fHeifht;					// 当たり判定用高さ
	MOTION_DATA_TYPE	m_nType;					// モーションデータの種類
	bool				m_bBool[BOOL_MAX];			// モーション用ブール
	bool				m_bBoolOld[BOOL_MAX];		// モーション用ブール
	bool				m_bBotton[POSE_BUTTON_MAX];	// ボタン用ブール
	int					m_nId;						// もろもろの判定用のID

	// 静的メンバ変数
	static 	CMotionData *m_apMotion[MOTION_DATA_TYPE_MAX];			// モーションデータへのポインタ
	static char			*m_apMotionInfo[MOTION_DATA_TYPE_MAX];		// モーションテキスト名

	static MOTION_TYPE		m_buttonMotionType[POSE_BUTTON_MAX];			// ボタンに割り当てるモーション

	static CButtonMotion	*m_buttonMotionDisp;					// ボタン表示クラス
	static bool				m_bBottonCommand[POSE_BUTTON_MAX];		// ボタン用ブール
	static bool				m_bControl;								// Controlできるか否か
	static int				m_nControlLevel;						// 操作Level

	static int				m_n1PlayerMotion;						// 1Pのモーション
	static int				m_nNumAll;						// 1Pのモーション
};

#endif
