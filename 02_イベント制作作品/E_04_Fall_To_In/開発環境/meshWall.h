////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッシュ壁処理 [meshWall.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include "scene.h"
//******************************************************************************************************************************
// メッシュ床のクラス
//******************************************************************************************************************************
class CMeshWall : public CScene
{
public:

	CMeshWall(int nPriority = CScene::PRIORITY_INIT);
	~CMeshWall();
	static HRESULT Load(void);	// テクスチャ生成
	static void UnLoad(void);	// テクスチャ破棄

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshWall *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const D3DXCOLOR col, const float fWidth, const float fHeight, const float fDepth,const int nBlockX,const int nBlockY,int type);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// 座標取得
	float GetWidth(void) { return m_fWidth; }			// 幅取得
	float GetHeight(void) { return m_fHeight; }			// 高さ取得
	float GetDepth(void) { return m_fDepth; }			// 奥行き取得
	D3DXVECTOR3 GetRot(void) { return m_rot; };			// 回転取得
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };	// マトリックス取得

	void SetPos(const D3DXVECTOR3 pos);											// 座標設定
	void SetSize(const float fWidth, const float fHeight, const float fDepth);	// 大きさ設定
	void SetBlock(const int nBlockX, const int nBlockY);						// 分割数設定
	void SetRot(const D3DXVECTOR3 fRot);										// 回転設定
	void SetColor(const D3DXCOLOR col);										// 色設定
	void SetVtx(void);															// 頂点設定
	void MakeVtx(void);														// 作成

	static void SetScan(void);		// Xファイル読み込み
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld,const float fWidth,const float fHeight);
private:
	static LPDIRECT3DTEXTURE9	*m_pTextureThis;		// テクスチャへのポインタ

	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 *m_pNor;		// 法線
	D3DXCOLOR	m_col;			// 色
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	float		m_fWidth;		// 幅
	float		m_fHeight;		// 高さ
	float		m_fDepth;		// 奥行き
	int         m_type;			// 種類
	int			m_nBlock_X;		// ブロックX
	int			m_nBlock_Y;		// ブロックZ
	int			m_nNumVertex;	// 頂点数
	int			m_nNumIndex;	// インデックス数
	int			m_nNumPolygon;	// ポリゴン数
	char		m_pFileName;					// ファイルの名前

	float		m_afAngle[4];
	float		m_afLength[4];

};
#endif
