#pragma once
//=============================================================================
//
// デバッグ処理 [DebugProc.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR_NAME (2048)
//====================================
// デバッグ
//====================================
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();
	HRESULT Init(void);
	void Uninit(void);
	static void print(char *fmt, ...);
	static void Draw(void);
protected:
private:
	static 	LPD3DXFONT m_pFont;			// フォントへのポインタ
	static char m_aStr[MAX_CHAR_NAME];			// 文字格納
};
#endif