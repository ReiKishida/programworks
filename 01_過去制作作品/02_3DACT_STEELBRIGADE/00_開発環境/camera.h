//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;		//視点
	D3DXVECTOR3 posVDest;	//目的の視点
	D3DXVECTOR3 posR;		//注視点  
	D3DXVECTOR3 posRDest;	//目的の注視点  
	D3DXVECTOR3 vecU;		//上方向ベクトル
	D3DXVECTOR3 rot;		//回転
	D3DXVECTOR3 rotDest;		//目的の回転
	D3DXMATRIX mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス
	float fPosR;
	float fAngle;
	float fLength;
	float fRLength;
	D3DXVECTOR3 fDestAngle;
}CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

void OverView();

CAMERA *GetCamera(void);
#endif
