//=============================================================================
//
// シーン処理[2D ポリゴン] [scene2D.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CManager;

class CScene2D : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,
		STATE_NORMAL,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX
	}STATE;
	CScene2D::CScene2D(int nPriority = 3);	//コンストラクタ
	CScene2D::~CScene2D();	//デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void DrawEffect(void); //描画処理
	static CScene2D *Create(D3DXVECTOR3 pos);//敵の生成
	int GetNum2DSceneAll(void);	//2Dシーン数の取得
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void CheckDamage(void);
	static int GetDamage(void);
	void SetTexture(float fTex_X, float fTex_Y);//テクスチャの設定
	void SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y);//テクスチャの設定

	void SetColor(D3DXCOLOR col);	//色の設定
	D3DXCOLOR GetColor(void);	//色の設定

	void SetPriority(int nPriority);//優先順位
	void SetForceTexture(int nType);//フォーステクスチャの設定
	int GetForce();//フォースタイプの取得
	void SetForce(int nType);//フォースの設定

	//背景スクロール
	void ScrollBg(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3);

	//テクスチャアニメーション
	void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);
	void SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y);

	//ポリゴンの大きさの取得、設定
	void SetScene2DWidth(float fWidth);
	void SetScene2DHeight(float fWidth);

	//ポリゴンの位置情報の取得、設定
	D3DXVECTOR3 GetPosScene2D(void);
	void SetPosScene2D(D3DXVECTOR3 pos);

	//ポリゴンの角度情報の設定、設定
	D3DXVECTOR3 GetRotScene2D(void);
	void SetRotScene2D(D3DXVECTOR3 rot);

	//ポリゴンの長さの取得、設定
	float GetLengthScene2D(void);
	void SetLengthScene2D(float fLength);

	//ポリゴンの大きさの取得、設定
	float GetWidthScene2D(void);
	float GetHeightScene2D(void);

	//ライフの周り
	int GetLife(void);
	void SetLife(int nLife);
	void HitLife(int nDamage);


protected:
	STATE m_state;//状態

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_posScene2D;			// ポリゴンの位置
	D3DXVECTOR3				m_rotScene2D;			// ポリゴンの回転
	D3DXCOLOR				m_color;				//ポリゴンの色
	float m_fLengthScene2D;	//ポリゴンの長さ
	float m_fAngleScene2D;	//ポリゴンの角度
	float m_fScene2DWidth;	//ポリゴンの幅
	float m_fScene2DHeight;	//ポリゴンの高さ
	int m_nCntframe;		//カウント
	int m_nNumScene2D;	//2Dポリゴン数
	static CManager *m_pManager;//マネージャ
	int m_nLife;			//ライフ
	int m_nPriority;		//優先度
	int m_nType;			//タイプ
	static int m_nDamage;	//ダメージ
	int m_nDPS;				//DPS


};

#endif