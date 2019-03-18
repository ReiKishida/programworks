////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ボタンモーション表示処理 [buttonMotion.h]
// Author : 圷 和也
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _BUTTONMOTION_H_
#define _BUTTONMOTION_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"
//******************************************************************************************************************************
// 3Dポリゴンのクラス
//******************************************************************************************************************************
class CButtonMotion : public CScene3D
{
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// 構造体定義
	//+*++*++*++*++*++*++*++*++*++*++*
	typedef enum
	{ // モデル種類
		TEX_TYPE_A = 0,	// A
		TEX_TYPE_B,		// B
		TEX_TYPE_X,		// X
		TEX_TYPE_Y,		// X
		TEX_TYPE_MAX	// 最大数
	}TEX_TYPE;

	CButtonMotion(int nPriority = 2);
	~CButtonMotion();
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CButtonMotion *Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const TEX_TYPE texType);

private:
	TEX_TYPE	m_texType;
	int			m_nCounter;
	bool		m_bDisp;

	static LPDIRECT3DTEXTURE9	m_apTextureThis[TEX_TYPE_MAX];	// テクスチャへのポインタ
	static char					*m_aTexInfo[TEX_TYPE_MAX];	// テクスチャ名前情報
};
#endif
