//=============================================================================
//
// メッシュウォール処理 [meshwall.h]
// Author : 岸田怜(Kishida Rei)
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//*****************************************************************************
// メッシュフィールドクラスの構造体
//*****************************************************************************
class CSceneMeshWall : public CScene
{
public:
	typedef enum
	{
		WALLTYPE_NORMAL = 0,	//通常の壁
		WALLTYPE_INVISIBLE,		//透明の壁
		WALLTYPE_MAX			//壁の最大数
	}WALLTYPE;					//壁の種類
	CSceneMeshWall(int nPriority = 5, OBJTYPE type = OBJTYPE_MESHWALL);		//コンストラクタ
	~CSceneMeshWall();			//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み
	static void Unload(void);									//テクスチャの破棄
	static CSceneMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_X, int sprit_Y, float Width, float Height, WALLTYPE type);//壁の生成
	HRESULT Init(void);							//初期化処理
	void Uninit(void);							//終了処理
	void Update(void);							//更新処理
	void Draw(void);							//描画処理
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	//頂点情報の生成
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);	//インデックス情報の生成
	void CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, float fRadius);	//手前面の当たり判定
	static void LoadWallText(void);					//壁の情報の読み込み
	static void LoadWallText_Tuto(void);					//壁の情報の読み込み
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; //インデックスバッファへのポインタ
	//壁の情報
	D3DXVECTOR3 m_pos;		//位置(中心座標)
	D3DXVECTOR3 m_rot;		//向き
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	D3DXCOLOR m_col;		//色情報
	D3DXVECTOR2 m_TexPos;	//テクスチャ座標
	int m_nMesh_X;			//X軸の分割
	int m_nMesh_Y;			//Y軸の分割
	int m_nVertexNum;		//頂点数
	int m_nIndexNum;		//インデックス数
	int m_nPolygonNum;		//ポリゴン数
	float m_fMeshWidth;		//メッシュフィールドの幅
	float m_fMeshHeight;	//メッシュフィールドの高さ
	WALLTYPE m_type;		//壁の種類
};

#endif
