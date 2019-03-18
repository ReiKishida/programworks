#pragma once
//=============================================================================
//
// �f�o�b�O���� [DebugProc.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR_NAME (2048)
//====================================
// �f�o�b�O
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
	static 	LPD3DXFONT m_pFont;			// �t�H���g�ւ̃|�C���^
	static char m_aStr[MAX_CHAR_NAME];			// �����i�[
};
#endif