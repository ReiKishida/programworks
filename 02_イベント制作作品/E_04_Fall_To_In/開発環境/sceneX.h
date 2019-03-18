////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Xファイル処理 [sceneX.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"
class CLine;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_MODEL_TYPE	(30)		// モデルのタイプの最大数
#define MAX_VTX			(4)
#define MAX_VTX_LINE	(12)
//******************************************************************************************************************************
// Xファイルのクラス
//******************************************************************************************************************************
class CSceneX : public CScene
{
public:
	typedef struct
	{

		D3DXVECTOR3 m_ofset;					// オフセット
		float m_fWidth;							// 幅
		float m_fHeight;						// 高さ
		float m_fDepth;							// 奥行き
		D3DXVECTOR3 m_vtxMax;					// 最大頂点
		D3DXVECTOR3 m_vtxMin;					// 最小頂点

		float m_afAngle[MAX_VTX];				// ４頂点角度
		float m_afLength[MAX_VTX];				// ４頂点長さ
	}CollisionData;

	typedef struct
	{
		LPD3DXMESH m_pMesh;						// メッシュ情報へのポインタ
		LPD3DXBUFFER m_pBuffMat;				// マテリアル情報へのポインタ
		DWORD m_nNumMat;						// マテリアル情報の数
		LPDIRECT3DTEXTURE9 *m_pTextuer;			// テクスチャ
		char *m_pFileName;						// ファイル名
		int m_nNumCollision;					// 判定数
		CollisionData *m_pCollision;			// 判定
	}XData;


	CSceneX(int nPriority = CScene::PRIORITY_INIT);
	~CSceneX();
	static HRESULT Load(void);	// モデル生成
	static void UnLoad(void);	// モデル破棄

	void BindModel(const int type);
	static XData *GetModelData(const int type) { return &m_pModelData[type]; };
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneX *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type);
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetMove(void) { return m_move; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };
	D3DXCOLOR	GetColor(void) { return m_col; };
	int			GetType(void) { return m_type; };
	static int			GetNumFile(void) { return m_nNumFile; };

	void SetPos(const D3DXVECTOR3 pos);
	void SetMove(const D3DXVECTOR3 move);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtx(const D3DXMATRIX mtx);
	void SetColor(const D3DXCOLOR col);
	void SetType(const int type);
	void SetCameraID(const int nID);
	void SetShadowPos(const D3DXVECTOR3 pos);
	bool Collision(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move, const float fHeight);
	bool CollisionReflection(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move, const float fHeight);
	float GetHeight(D3DXVECTOR3 *pPos, bool *bIn);
	static void ScanCollision(void);

private:
	void SetCollision(void);
	void DeleteCollision(void);
	static XData m_pModelData[MAX_MODEL_TYPE];
	LPD3DXBUFFER m_pBuffMat;
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_posOld;				// 前回の位置
	D3DXVECTOR3 m_Initpos;				// 初期位置
	D3DXVECTOR3 m_move;					// 移動量
	D3DXCOLOR	m_col;					// カラー
	D3DXVECTOR3 m_Diffrot;				// 角度の差分
	D3DXVECTOR3 m_Destrot;				// 目的の角度
	D3DXVECTOR3 m_rot;					// 角度
	D3DXMATRIX  m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_ShadowPos;			// 影の位置
	int m_type;							// タイプ
	static int m_nNumFile;						// 読み込んだファイル数
	D3DXVECTOR3 **m_aMeshPos;			// ４頂点座標計算用ポインタ
	CLine ***m_apLine;					// 四角形生成用ポインタ
	int m_nCameraID;					// カメラ
};
#endif
