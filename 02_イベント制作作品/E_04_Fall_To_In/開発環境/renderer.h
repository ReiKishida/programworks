#pragma once
//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"
class CFade;

//====================================
// レンダリングクラス
//====================================
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void);
	static void SetAlphaBlend(bool bAlpha);
	static CFade *GetFade(void);					// フェード

private:
	LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)

	static CFade *m_pFade;						// フェード

};
#endif