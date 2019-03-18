#pragma once
//=============================================================================
//
// シェーダー処理 [Shader.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _SHADER_H_
#define _SHADER_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//====================================
// シーンクラス
//====================================
class CShader
{
public:
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄

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
	static LPD3DXEFFECT m_pShader;					// シェーダデータ
	static LPDIRECT3DVERTEXDECLARATION9 m_pDecl;	// シェーダー
	static LPDIRECT3DTEXTURE9 m_pTexture ;			// シェーダーテクスチャ
};

#endif