//=============================================================================
//
// ライト処理 [light.h]
// Author : 佐藤亮太
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef struct
{// ライトの構造体
	D3DLIGHT9 Light;				// ライトの情報
	bool bLight;					// ライトのオンオフ
	bool bUse;						// 使用されているかどうか
}Light;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
int SetPointLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, float Attenuation, float Range);		// ポイントライトの設置
int SetSpotLight(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR Diffuse);						// スポットライトの設置
void SetPositionLight(int nIdxLight, D3DXVECTOR3 pos, D3DXVECTOR3 rot);						// ライトの位置,回転,更新
void SwitchingLight(int nIdxLight);															// ライトのオンオフを切り替え


#endif
