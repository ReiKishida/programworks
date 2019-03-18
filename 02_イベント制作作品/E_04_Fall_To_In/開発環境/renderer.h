#pragma once
//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"
class CFade;

//====================================
// �����_�����O�N���X
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
	static CFade *GetFade(void);					// �t�F�[�h

private:
	LPDIRECT3D9				m_pD3D;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)

	static CFade *m_pFade;						// �t�F�[�h

};
#endif