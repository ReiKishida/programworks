//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// モデル処理 [model.h]
// Author:圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#ifndef _MODEL_H_
#define _MODEL_H_

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// インクルードファイル
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#include "main.h"
#include "scene.h"
#include "motion.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_MODEL_TEX		(20)		// テクスチャの最大数

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// クラス定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CModel
{ // モデルクラス
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// プロトタイプ宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	CModel();
	~CModel();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(D3DXMATRIX mtxParent);
	void Draw(D3DXMATRIX mtxParent,D3DXVECTOR3 posMotion,D3DXVECTOR3 rotMotion);
	void Draw(D3DXMATRIX mtxParent, D3DXVECTOR3 posMotion, D3DXVECTOR3 rotMotion, CMotionData::Model model);

	// 各種設定・取得関数
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }										// 位置設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }												// 位置取得
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }										// 回転設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }												// 回転取得
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }					// ワールドマトリックスの設定
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }										// ワールドマトリックスの取得
	void SetParent(CModel *parent) { m_pParent = parent; }									// 親モデルへのポインタの設定
	void SetMesh(const LPD3DXMESH mesh) { m_pMesh = mesh; }									// メッシュ設定
	void SetBuffMat(const LPD3DXBUFFER buffMat) { m_pBuffMat = buffMat; }					// マテリアル設定
	void SetNumMat(const DWORD numMat) { m_nNumMat = numMat; }								// マテリアル情報の数設定
	void SetTexture(const LPDIRECT3DTEXTURE9 tex, const int nId) { m_pTexture[nId] = tex; }	// テクスチャ設定
	void BindModel(const LPD3DXMESH mesh, const LPD3DXBUFFER buffMat, const DWORD numMat);

	// 静的メンバ関数
	static CModel *Create(const D3DXVECTOR3 pos);

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// 変数宣言
	//+*++*++*++*++*++*++*++*++*++*++*
	D3DXVECTOR3			m_pos;				// 位置（中心座標）
	D3DXVECTOR3			m_rot;				// 向き
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	CModel				*m_pParent;			// 親モデルへのポインタ
	LPD3DXMESH			m_pMesh;			// メッシュ（頂点の集まり）情報へのポインタ
	LPD3DXBUFFER		m_pBuffMat;			// マテリアル情報へのポインタ
	DWORD				m_nNumMat;			// マテリアル情報の数
	LPDIRECT3DTEXTURE9	m_pTexture[MAX_MODEL_TEX] = {};
};

#endif
