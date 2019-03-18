//=============================================================================
//
// フェード処理 [fade.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"
//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CManager;

class CFade
{
public:
	CFade();
	~CFade();
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;

	HRESULT Init(CManager::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetFade(CManager::MODE modeNext);
	static FADE GetFade(void);
	void SetTexture(float fTex_X_0, float fTex_X_1, float fTex_Y_0, float fTex_Y_1);
	static void AddTex();
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9  m_pVtxBuff;	// 頂点バッファへのポインタ
	static FADE					m_fade;					// フェード状態
	static CManager::MODE			m_modeNext;				// 次のモード
	static D3DXCOLOR				m_color;			// フェード色
	static int m_nPatternAnim;	//アニメーションパターン
	static int m_nCounterAnim;//アニメーションカウンター
	static int m_nTex_Y;

};


#endif
