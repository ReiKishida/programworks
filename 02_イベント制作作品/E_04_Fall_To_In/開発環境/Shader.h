#pragma once
//=============================================================================
//
// �V�F�[�_�[���� [Shader.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _SHADER_H_
#define _SHADER_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//====================================
// �V�[���N���X
//====================================
class CShader
{
public:
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��

	CShader();
	~CShader();
	static CShader *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static LPD3DXEFFECT GetEffect(void) { return m_pShader; };
	static LPDIRECT3DVERTEXDECLARATION9 GetDecl(void) { return m_pDecl; };
	static LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; };
private:
	static LPD3DXEFFECT m_pShader;					// �V�F�[�_�f�[�^
	static LPDIRECT3DVERTEXDECLARATION9 m_pDecl;	// �V�F�[�_�[
	static LPDIRECT3DTEXTURE9 m_pTexture ;			// �V�F�[�_�[�e�N�X�`��
};

#endif