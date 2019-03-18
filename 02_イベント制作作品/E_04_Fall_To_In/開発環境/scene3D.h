////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ３Dポリゴン処理 [scene3D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"
//******************************************************************************************************************************
// 3Dポリゴンのクラス
//******************************************************************************************************************************
class CScene3D : public CScene
{
public:

	CScene3D(int nPriority = CScene::PRIORITY_INIT);
	~CScene3D();
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene3D *Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// 座標取得
	D3DXVECTOR3 GetMove(void) { return m_move; };		// 移動量取得
	float GetWidth(void) { return m_fWidth; }			// 幅取得
	float GetHeight(void) { return m_fHeight; }			// 高さ取得
	float GetDepth(void) { return m_fDepth; }			// 奥行き取得
	D3DXVECTOR3 GetRot(void) { return m_rot; };			// 回転取得
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };	// マトリックス取得
	D3DXCOLOR GetColor(void) { return m_col; };
	void SetPos(const D3DXVECTOR3 pos);
	void SetMove(const D3DXVECTOR3 move);
	void SetSize(const float fWidth, const float fHeight, const float fDepth);	// 大きさ設定
	void SetRot(const D3DXVECTOR3 fRot);	// 回転設定
	void SetColor(const D3DXCOLOR col); // 色設定
	void SetVtx(void);
	void Collision(const D3DXVECTOR3 pos);

private:
	static LPDIRECT3DTEXTURE9	m_pTextureThis;		// テクスチャへのポインタ
	D3DXVECTOR3 m_aVtxPos[4];
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	// 向き
	D3DXCOLOR	m_col;	// 色
	D3DXMATRIX	m_mtxWorld;// ワールドマトリックス
	float		m_fWidth;		// 幅
	float		m_fHeight;		// 高さ
	float		m_fDepth;		// 奥行き
	float		m_fAngle;	// 角度
	float		m_fLength;	// 距離
};
#endif
