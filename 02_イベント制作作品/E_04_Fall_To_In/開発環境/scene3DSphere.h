//=============================================================================
//
// 3Dメッシュスフィアの処理 [scene3DSphere.h]
// Author : 亀田 憲尚
//
//=============================================================================
#ifndef _SCENE3DSPHERE_H_
#define _SCENE3DSPHERE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene3DSphere : public CScene
{// 3Dメッシュスフィアクラス(CSceneの派生クラス)
public:
	CScene3DSphere(int nPriority = CScene::PRIORITY_INIT);
	~CScene3DSphere();
	static CScene3DSphere *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection = 0);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void MakeVertex(void);
	void SetAll(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection);
	void SetPosition(const D3DXVECTOR3 pos);
	void SetTexMove(const D3DXVECTOR2 tex);
	D3DXVECTOR3 GetPosition(void) { return m_pos; };
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; };
	D3DXVECTOR2 GetTexMove(void) { return m_texMove; };
private:
	LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffLid;	// 頂点バッファへのポインタ[蓋]
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;		// インデックスバッファへのポインタ
	D3DXMATRIX					m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3					m_pos;			// 位置
	D3DXVECTOR3					m_rot;			// 向き
	D3DXCOLOR					m_col;			// カラー
	D3DXVECTOR2					m_texMove;		// テクスチャ移動
	float						m_fRadius;		// 半径
	float						m_fHeight;		// 高さ
	int							m_nCutH;		// 水平の分割数
	int							m_nCutV;		// 垂直の分割数
	int							m_nDirection;	// 向き[ 表 = 0, 裏 = 1 ]
	int							m_nNumVtx;		// 頂点数
	int							m_nNumPolygon;	// ポリゴン数
	int							m_nNumIdx;		// インデックス数
};

#endif
