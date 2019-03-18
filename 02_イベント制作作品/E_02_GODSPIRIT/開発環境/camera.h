////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// カメラ処理 [camera.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
//******************************************************************************************************************************
// カメラの構造体
//******************************************************************************************************************************
typedef enum
{
	CAMERASTATE_NONE = 0,
	CAMERASTATE_CHANGESTAGE,
	CAMERASTATE_BOSS_MOVIE,
	CAMERASTATE_BOSS_MOVIE_START,
	CAMERASTATE_MAX,
}CAMERASTATE;
typedef struct
{
	D3DXVECTOR3	posV;			// 視点
	D3DXVECTOR3	posVOld;		// 前回の視点
	D3DXVECTOR3	posR;			// 注視点
	D3DXVECTOR3	vecU;			// 上方向ベクトル
	D3DXVECTOR3	rot;			// 回転量
	D3DXVECTOR3 Diffrot;		// 角度の差分
	D3DXVECTOR3 Destrot;		// 目的の角度
	D3DXVECTOR3 posVDiff;		// 場所の差分
	D3DXVECTOR3 posVDest;		// 目的の場所
	D3DXVECTOR3 posRDiff;		// 場所の差分
	D3DXVECTOR3 posRDest;		// 目的の場所
	D3DXVECTOR3 AttentionPos;	// 注目時のカメラ移動
	CAMERASTATE nState;			// 状態
	int nStateCounter;			// 状態カウンター
	float fLength;				// V長さ
	float fLengthAdd;			// 長さを加える
	float fHeight;				// V高さ
	float fLengthR;				// R長さ
	float fHeightR;				// R高さ
	float fHeightWall;			// 壁に当たったときの加算用
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	// ポーズ中に動かしたら元に戻す
	D3DXVECTOR3	PauseOldposV;			// 視点
	D3DXVECTOR3	PauseOldposR;			// 注視点
	D3DXVECTOR3	PauseOldrot;			// 回転量
	D3DVIEWPORT9 viewpoart;		// ビューポート

}Camera;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int nIdxCamera);
Camera *GetCamera(void);
#endif
