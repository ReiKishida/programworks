//=============================================================================
//
// モデル表示処理[3D ポリゴン] [model.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTS (29)
#define MAX_TEXTURE (2)
//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CMeshOrbit;
class CCollision;
class CModel
{
public:

	CModel();	//コンストラクタ
	~CModel();	//デストラクタ
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetPos(D3DXVECTOR3 pos);	//位置情報の代入
	void SetRot(D3DXVECTOR3 rot);	//角度情報の代入
	void SetMtx(D3DXMATRIX mtx);	//角度情報の代入
	void CreateOrbit(void);			//軌跡の生成
	void CreateAttackCollision(void);	//攻撃判定の生成
	D3DXMATRIX GetMatrix(void);				//マトリックスの取得
	void Load(void);						//使用するパーツの読み込み
	void Unload(void);						//使用するパーツの破棄
	void SetParent(CModel *pModel);			//親子関係の設定
	void BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat,  LPDIRECT3DTEXTURE9 tex);	//テクスチャの割り当て
	void SetColor(D3DXCOLOR col);	//色情報の代入


private:
	 LPDIRECT3DTEXTURE9			 m_pTexture;		// テクスチャへのポインタ
	 LPD3DXMESH					 m_pMesh;			//メッシュ情報へのポインタ
	 LPD3DXBUFFER				 m_pBuffMat;		//マテリアル情報へのポインタ
	 DWORD						 m_nNumMat;			//マテリアルの情報の数
	 D3DXMATERIAL				*m_pMat;			//マテリアルへのポインタ
	CModel						*m_pParent;			//親モデルのポインタ
	CCollision *m_pCollisionAttack;					// 攻撃時の当たり判定のポインタ
	int m_nNumParts;								//パーツ数
	int m_nMaxParts;								//パーツの最大数
	D3DXMATRIX m_mtxWorld;							//ワールドマトリックス
	D3DXVECTOR3 m_pos;								//位置
	D3DXVECTOR3 m_rot;								//向き
	static CMeshOrbit *m_pMeshOrbit;				//軌跡のポインタ
};

#endif