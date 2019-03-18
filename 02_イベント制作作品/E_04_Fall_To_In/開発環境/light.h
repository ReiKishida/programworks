//=============================================================================
//
// ライト処理 [light.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT			(10)		// ライトの最大数

//*****************************************************************************
// ライトのクラス
//*****************************************************************************
class CLight
{
public:
	typedef struct
	{// ライトの構造体
		D3DLIGHT9 m_Light;				// ライトの情報
		bool m_bLight;					// ライトのオンオフ
		bool m_bUse;						// 使用されているかどうか
	}Light;

	CLight();
	~CLight();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	int SetPoint(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, float Attenuation, float Range);		// ポイントライトの設置
	int SetSpot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR Diffuse);						// スポットライトの設置
	void SetPosition(int nIdxLight, D3DXVECTOR3 pos, D3DXVECTOR3 rot);						// ライトの位置,回転,更新
	void Switching(int nIdxLight);															// ライトのオンオフを切り替え

private:
	Light m_aLight[MAX_LIGHT];				// ライトの情報
};


#endif
