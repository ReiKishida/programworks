//***************************************************************************//
//
// メッセージ処理 [Message.h]
// Author : KishidaRei
//
//***************************************************************************//
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "main.h"
//***************************************************************************//
// 構造体宣言
//***************************************************************************//
typedef enum
{
	MESSAGETYPE_APPER = 0,
	MESSAGETYPE_NORMAL,
	MESSAGETYPE_MAX
}MESSAGETYPE;

typedef struct
{

	D3DXVECTOR3 pos;								// 位置
	D3DXVECTOR3 rot;								// 移動
	D3DXCOLOR Col;								//色
	bool bUse;									//使用しているか
	MESSAGETYPE type;								//タイプ	
	int nType;
}MESSAGE;
//***************************************************************************//
// プロトタイプ宣言
//***************************************************************************//
void InitMessage(void);
void UninitMessage(void);
void UpdateMessage(void);
void DrawMessage(void);
void DeleteMessage(void);

MESSAGE *GetMessageBox(void);

void SetMessageBox(D3DXVECTOR3 pos, int ntype);
#endif