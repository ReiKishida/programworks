////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// カメラ処理 [camera.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
#define MAX_CAMERA		(3)

//******************************************************************************************************************************
// カメラのクラス
//******************************************************************************************************************************
class CCamera
{
public:
	typedef enum
	{
		CAMERASTATE_NONE = 0,
		CAMERASTATE_EVENT ,
		CAMERASTATE_START,
		CAMERASTATE_MAX,
	}CAMERASTATE;

	typedef struct
	{
		D3DXVECTOR3	m_posV;			// 視点
		D3DXVECTOR3	m_posVOld;		// 前回の視点
		D3DXVECTOR3	m_posR;			// 注視点
		D3DXVECTOR3	m_vecU;			// 上方向ベクトル
		D3DXVECTOR3	m_rot;			// 回転量
		D3DXVECTOR3 m_ViewRot;		// ALTキーでの回転
		D3DXVECTOR3 m_Diffrot;		// 角度の差分
		D3DXVECTOR3 m_Destrot;		// 目的の角度
		D3DXVECTOR3 m_posVDiff;		// 場所の差分
		D3DXVECTOR3 m_posVDest;		// 目的の場所
		D3DXVECTOR3 m_posRDiff;		// 場所の差分
		D3DXVECTOR3 m_posRDest;		// 目的の場所
		CAMERASTATE m_nState;			// 状態
		int m_nStateCounter;			// 状態カウンター
		float m_fLength;				// V長さ
		float m_fHeight;				// V高さ
		float m_fLengthR;				// R長さ
		float m_fHeightR;				// R高さ
		D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX m_mtxView;			// ビューマトリックス
		D3DVIEWPORT9 m_viewpoart;		// ビューポート
	}Camera;

	CCamera();
	~CCamera();
	void SetPosV(const int nCamera, const D3DXVECTOR3 pos);
	void SetPosR(const int nCamera, const D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPosV(int nCamera) { return m_aCamera[nCamera].m_posV; }						// 視点の情報確保
	D3DXVECTOR3 GetPosR(int nCamera) { return m_aCamera[nCamera].m_posR; }						// 注視点の情報確保
	D3DXVECTOR3 GetRot(int nCamera) { return m_aCamera[nCamera].m_rot; };

	D3DXMATRIX GetMtxView(int nCamera) { return m_aCamera[nCamera].m_mtxView; };
	D3DXMATRIX GetMtxProjection(int nCamera) { return m_aCamera[nCamera].m_mtxProjection; };
	CAMERASTATE GetState(int nCamera) { return m_aCamera[nCamera].m_nState; };

	bool GetSlow(void) { return m_bSlow; };
	bool GetGrid(void) { return m_bGrid; };
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Set(int nCamera);
	void SetState(int nCamera, CAMERASTATE state);
	void SetSlow(bool slow);
	void SetGrid(bool grid);
private:
	Camera m_aCamera[MAX_CAMERA];

	static bool m_bPause;			// ポーズ中かどうか
	bool m_bReverse;
	D3DXVECTOR3	m_PauseOldposV;		// 視点
	D3DXVECTOR3	m_PauseOldposR;		// 注視点
	D3DXVECTOR3	m_PauseOldrot;		// 回転量
	float m_move;
	float m_Rotplus;
	int m_nCntPos;
	int m_nCntMove;					// 移動量のカウンター
	int m_nCntRot;
	float m_TitleRot;
	bool m_bPos;
	bool m_bRot;					// 回転するかどうか
	bool m_bSlow;					// スローモード
	bool m_bGrid;					// グリッドモード
};
#endif
