//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// SPゲージ処理 [spGage.h]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#ifndef _SPGAGE_H_
#define _SPGAGE_H_

#include "main.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef enum
{
	SPGAGE_TYPE_1P_FRAME = 0,	// 1P枠
	SPGAGE_TYPE_1P_SPBLACK,		// 1PSP黒
	SPGAGE_TYPE_1P_SPRED,		// 1PSP赤
	SPGAGE_TYPE_1P_SP,			// 1PSP
	SPGAGE_TYPE_2P_FRAME,		// 2P枠
	SPGAGE_TYPE_2P_SPBLACK,		// 2PSP黒
	SPGAGE_TYPE_2P_SPRED,		// 2PSP赤
	SPGAGE_TYPE_2P_SP,			// 2PSP
	SPGAGE_TYPE_MAX				// 総数
}SPGAGE_TYPE;

typedef enum
{
	LEG_TYPE_LEFT = 0,
	LEG_TYPE_RIGHT,
	LEG_TYPE_MAX
}LEG_TYPE;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void InitSpGage(void);
void UninitSpGage(void);
void UpdateSpGage(void);
void DrawSpGage(void);
void SetSpGage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, SPGAGE_TYPE type);
void SetSpAsset(D3DXVECTOR3 pos, LEG_TYPE legType);
void SetSpGageDestSkillPoint(int nSkillPoint, LEG_TYPE legType);
void SetSpGageSkillPoint(int nSkillPoint, LEG_TYPE legType);
void AddSkillPoint(int nPlayerID, int nSkillPoint);
#endif
