//=============================================================================
//
// カメラ処理 [camera.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=============================================================================
// カメラの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 posV;			//現在の視点
	D3DXVECTOR3 posVOld;		//前回の視点
	D3DXVECTOR3 posVDest;		//目的の視点
	D3DXVECTOR3 posR;			//現在の注視点
	D3DXVECTOR3 posRDest;		//目的の注視点
	D3DXVECTOR3 rot;			//現在の向き
	D3DXVECTOR3 rotDest;		//目的の向き
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
	float fLength;				//視点の長さ
	int nIdxSencerLock;			
} CAMERA;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA *GetCamera(void);

#endif
