//=============================================================================
//
// カメラ処理 [camera.h]
// Author :	岸田怜(Kishida Rei)
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラのクラス構造体
//*****************************************************************************
class CCamera
{
public:
	typedef enum
	{
		CAMERATYPE_TITLE = 0,	//タイトルのカメラ
		CAMERATYPE_TUTORIAL,	//チュートリアルのカメラ
		CAMERATYPE_GAME,		//ゲームのカメラ
		CAMERATYPE_RESULT,		//リザルトのカメラ
		CAMERATYPE_RANKING,		//ランキング
		CAMERATYPE_MAX			//種類の最大数
	}CAMERATYPE;				//カメラの種類
	CCamera();					//コンストラクタ
	~CCamera();					//デストラクタ
	HRESULT Init(void);			//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);			//更新処理
	void SetCamera(void);		//描画処理
	void OverView(void);		//角度調整
	D3DXMATRIX GetmtxView(void);//ビューマトリックスの取得
	D3DXVECTOR3 GetRot(void);	//カメラの角度情報の取得
	static void SetFree(void);			//
private:
	static CManager *m_pManager;//マネージャ
	D3DXVECTOR3 m_posVDest;		//目的の位置(視点)
	D3DXVECTOR3 m_posRDest;		//目的の位置(注視点)
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXVECTOR3 m_rot;			//角度情報
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	float m_eyepoint;			//視点
	CAMERATYPE m_type;
	static bool m_bFree;				//フリービュー

	//====================================================
	//角度情報
	//====================================================
	float m_fDestAngle;			//目的の角度
	float m_fAngle;				//角度

	//====================================================
	//視点情報
	//====================================================
	float m_fDestpos_V_x;		//目的の位置(視点-X)
	float m_fpos_V_x;			//現在の位置(視点-X)

	float m_fDestpos_V_y;		//目的の位置(視点-Y)
	float m_fpos_V_y;			//現在の位置(視点-Y)

	float m_fDestpos_V_z;		//目的の位置(視点-Z)
	float m_fpos_V_z;			//現在の位置(視点-Z)

	//====================================================
	//注視点情報
	//====================================================
	float m_fDestpos_R_x;		//目的の位置(注視点-X)
	float m_fpos_R_x;			//現在の位置(注視点-X)

	float m_fDestpos_R_y;		//目的の位置(注視点-Y)
	float m_fpos_R_y;			//現在の位置(注視点-Y)

	float m_fDestpos_R_z;		//目的の位置(注視点Z)
	float m_fpos_R_z;			//現在の位置(注視点-Z)

	//====================================================
	//距離の情報
	//====================================================
	float m_fLength;			//現在の長さ
	float m_fdestLength;		//目的の長さ

	//====================================================
	//高さの情報
	//====================================================
	float m_fHeight_V;			//現在の高さ(視点)
	float m_fdestHeight_V;		//目的の高さ(視点)

	float m_fHeight_R;			//現在の高さ(注視点)
	float m_fdestHeight_R;		//目的の高さ(注視点)

};

#endif
