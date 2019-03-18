//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// モーション処理 [motion.h]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#ifndef _MOTION_H_
#define _MOTION_H_

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// インクルードファイル
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#include "main.h"
#include "scene.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 前方宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CModel;
class CMotion;
class CCollision;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_PARTS					(20)	// 最大パーツ数
#define MAX_MOTION_TEX				(20)	// テクスチャの最大数
#define MAX_KEY						(20)	// キーフレームの最大数
#define MAX_MOTION					(10)	// モーションの最大数
#define COLLISION_SPHERE_CREATE		(0)		// 当たり判定確認用の球体を生成するか	[0]しない/[1]する

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// クラス定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CMotionData
{ // モーションデータクラス
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// 構造体定義
	//+*++*++*++*++*++*++*++*++*++*++*
	typedef struct
	{ // モデル情報
		D3DXVECTOR3 pos;								// パーツの位置情報
		D3DXVECTOR3 rot;								// パーツの回転情報
		LPD3DXMESH pMesh;								// メッシュ（頂点の集まり）情報へのポインタ
		LPD3DXBUFFER pBuffMat;							// マテリアル情報へのポインタ
		DWORD nNumMat;									// マテリアル情報の数
		LPDIRECT3DTEXTURE9 pTexture[MAX_MOTION_TEX];	// モデル一つに対するテクスチャ
		int nIdxParent;									// 親パーツのインデックス
	}Model;

	typedef struct
	{ // キー情報
		int nFrame;								// キーフレーム
		D3DXVECTOR3 aPos[MAX_PARTS];			// キーごとの位置情報
		D3DXVECTOR3 aRot[MAX_PARTS];			// キーごとの回転情報
	}Key;

	typedef struct
	{ // モーション情報
		bool bLoop;								// ループするかどうか
		int nNumKey;							// キー数
		Key aKey[MAX_KEY];						// キー情報
		int nAllFrame;							// モーション全体のフレーム数
	}Motion;

	//+*++*++*++*++*++*++*++*++*++*++*
	// プロトタイプ宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	CMotionData::CMotionData();
	CMotionData::~CMotionData();
	HRESULT Init(void);
	HRESULT Load(char *cTextName);
	void UnLoad(void);

	// 各種値設定・取得関数
	Model *GetModel(const int nPartsId) { return &m_aModel[nPartsId]; }		// モデル情報取得
	Motion *GetMotion(const int nMotion) { return &m_aMotion[nMotion]; }	// モーション情報取得
	int GetNumParts(void) { return m_nNumParts; }							// パーツ数取得
	CCollision *GetCollisionData(void) { return m_pCollisionData; }			// 当たり判定情報取得

	// 静的メンバ関数
	static CMotionData *Create(void);

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// 変数宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	Model		m_aModel[MAX_PARTS];	// モデル情報
	Motion		m_aMotion[MAX_MOTION];	// モーション情報
	int			m_nNumParts;			// パーツ数
	CCollision	*m_pCollisionData;		// 当たり判定情報
};

class CMotionMove
{ // モーションムーブクラス
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// プロトタイプ宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	CMotionMove();
	~CMotionMove();
	HRESULT Init(void);
	void Uninit(void);
	void MotionUpdate(CMotionData *motionData, int nMotionType, int nMotionTypeOld);
	void FrameUpdate(CMotionData *motionData, int nMotionType);

	// 各種設定・取得関数
	D3DXVECTOR3 GetPos(int nParts) { return m_pos[nParts]; }	// モーション位置取得
	D3DXVECTOR3 GetRot(int nParts) { return m_rot[nParts]; }	// モーション向き取得
	int GetKeyCnt(void) { return m_nKeyCnt; }					// キーカウント取得
	int GetFrameCounter(void) { return m_nFrameCounter; }		// フレームカウンター取得

	// 静的メンバ関数
	static CMotionMove *Create(void);

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// 変数宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	int					m_nKeyCnt;				// キーカウント
	int					m_nFrameCounter;		// フレームカウンター
	int					m_nAllFrameCounter;		// 全体のフレームカウンター
	bool				m_bBlend;				// ブレンド中かどうか
	D3DXVECTOR3			m_pos[MAX_PARTS];		// モーション位置
	D3DXVECTOR3			m_rot[MAX_PARTS];		// モーション向き
	D3DXVECTOR3			m_posAdd[MAX_PARTS];	// 加算モーション位置
	D3DXVECTOR3			m_rotAdd[MAX_PARTS];	// 加算モーション向き
};

class CMotion : public CScene
{ // モーションクラス
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// プロトタイプ宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	CMotion(int nPriority = CScene::PRIORITY_INIT);
	virtual ~CMotion();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void CollisionUpdate(CCollision *pCollision);

	// 各種設定・取得関数
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }			// 位置設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }					// 位置取得
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			// 前回位置取得
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }		// 移動量設定
	D3DXVECTOR3 GetMove(void) { return m_move; }				// 移動量取得
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }			// 向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }					// 向き取得
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }	// 移動速度設定
	float GetSpeed(void) { return m_fSpeed; }					// 移動速度取得
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld;}
	void SetMatrix(const D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	void SetMotionType(const int nMotionType) { m_nMotionType = nMotionType; }	// モーションタイプ設定
	int GetMotionType(void) { return m_nMotionType; }							// モーションタイプ取得
	int GetMotionTypeOld(void) { return m_nMotionTypeOld; }						// 前回のモーションタイプ取得

	CModel *GetModel(const int nPartsId) { return m_pModel[nPartsId]; }			// モデル情報取得
	CMotionMove *GetMotion(void) { return m_pMotion; }							// モーション情報取得
	void BindMotion(CMotionData *pMotionData) { m_pMotionData = pMotionData; }	// モーションの設定

	CCollision *GetCollision(void) { return m_pCollision; }						// 当たり判定情報取得

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// 変数宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	D3DXVECTOR3			m_pos;					// 位置（中心座標）
	D3DXVECTOR3			m_posOld;				// 前回位置（中心座標）
	D3DXVECTOR3			m_move;					// 移動量
	D3DXVECTOR3			m_rot;					// 向き
	D3DXMATRIX			m_mtxWorld;				// ワールドマトリックス
	float				m_fSpeed;				// 移動速度
	CModel				*m_pModel[MAX_PARTS];	// モデルクラスのポインタ

	CMotionData			*m_pMotionData;			// モーションクラスのポインタ
	int					m_nMotionType;			// モーションの種類
	int					m_nMotionTypeOld;		// 前回のモーションの種類
	CMotionMove			*m_pMotion;				// モーションムーブクラスのポインタ

	CCollision			*m_pCollision;			// 当たり判定クラスのポインタ
};

#endif
