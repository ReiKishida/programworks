//=============================================================================
//
// 敵処理[モデル] [player.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "sceneX.h"
#include "scene2D.h"
#include "debugproc.h"
#include "boss.h"
#include "camera.h"
#include "model.h"
#include "object.h"
#include "scene_meshfield.h"
#include "scene_meshwall.h"
#include "scene_meshorbit.h"
#include "scene_meshcylinder.h"
#include "scene_gauge.h"
#include "game.h"
#include "sound.h"
#include "particle.h"
#include "collision.h"
#include "score.h"
#include "player.h"
#include "ui_gauge.h"
#include "combogauge.h"
#include "combo.h"
#include "effect.h"
#include "cutin.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//=====================================
// 敵情報
//=====================================
#define ENEMY_00_MOVE (0.5f)					//移動量

#define ENEMY_MOVE_DEFAULT		(1.0f)			// 敵が動いてる基準の移動量
#define MAX_CHAR (256)							//文字の最大数
#define GRAVITY_NUM (0.8f)						//重力の値
#define ENEMY_MOTION_BLEND_FRAME	(5)			// モーションブレンドのフレーム数
#define JUMP_POWER (25.0f)						//ジャンプ力
#define ENEMY_COLLISION_RADIUS		(20.0f)		// 敵の当たり判定の半径

#define SCREEN_LIMIT_MAX_X (5000.0f )			//画面端(右端)
#define SCREEN_LIMIT_MIN_X (-120.0f)			//画面(左端)

#define SCREEN_LIMIT_MIN_Z (-370.0f)			//画面(下端)
#define HITPARTICLE (12)
//=====================================
// パーティクルの色
//=====================================
#define PARTICLE_FIRE (D3DXCOLOR(0.7f, 0.2f, 0.9f, 1.0f))

//=====================================
// 優先順位
//=====================================
#define ENEMY_PRIORITY (2)						//敵の優先順位
#define COLLISION_PRIORITY (6)					//当たり判定の優先順位
#define WALL_PRIORITY (5)						//壁の優先順位
#define OBJECT_PRIORITY (4)						//オブジェクトの優先順位
#define CYLINDER_PRIORITY (3)					//シリンダーの優先順位

//=====================================
// 獲得スコア
//=====================================
#define LIGHT_ATTACK_SCORE (100)		//弱攻撃
#define MEDIUM_ATACK_SCORE (150)		//中攻撃
#define FINISH_ATACK_SCORE (500)	//最終段攻撃
#define LARGE_ATACK_SCORE (100)		//強攻撃
#define SKILL_ATACK_SCORE (2000)	//スキル攻撃



//=====================================
// ダメージ量
//=====================================
#define LIGHT_ATACK (12.0f)		//弱攻撃
#define MEDIUM_ATACK (15.0f)		//中攻撃
#define FINISH_ATACK (20.0f)	//最終段攻撃
#define LARGE_ATACK (13.0f)		//強攻撃
#define SKILL_ATACK (200.0f)		//強攻撃

//=====================================
// コンボゲージ増加量
//=====================================
#define LIGHT_ATACK_GAUGE (13.0f)		//弱攻撃
#define MEDIUM_ATACK_GAUGE (15.0f)		//中攻撃
#define FINISH_ATACK_GAUGE (50.0f)		//最終段攻撃
#define LARGE_ATACK_GAUGE (12.0f)		//強攻撃

//=====================================
// スキルゲージ増加量
//=====================================
#define LIGHT_ATACK_SKILL (8.0f)		//弱攻撃
#define MEDIUM_ATACK_SKILL (9.0f)		//中攻撃
#define FINISH_ATACK_SKILL (12.0f)		//最終段攻撃
#define LARGE_ATACK_SKILL (5.0f)		//強攻撃



//=====================================
// 読み込むテキストファイル
//=====================================
#define MODEL_FILENAME_00 ("data/TEXT/motion_boss_0.txt")			// ボスのテキスト


//**************************************************************************************************
// 静的メンバ変数
//**************************************************************************************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;							// 共有テクスチャのポインタ
char			   CBoss::m_acFileName[MAX_BOSS_PARTS][256] = {};		// 読み込むファイル名
LPD3DXMESH		   CBoss::m_apMesh[MAX_BOSS_PARTS] = {};				// メッシュ情報へのポインタ
LPD3DXBUFFER	   CBoss::m_apBuffMat[MAX_BOSS_PARTS] = {};				// マテリアル情報へのポインタ
DWORD			   CBoss::m_aNumMat[MAX_BOSS_PARTS] = {};				// マテリアルの情報の数
CMeshOrbit		  *CBoss::m_pMeshOrbit = NULL;							// 軌跡のポインタ
//========================================================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//========================================================================================================
CBoss::CBoss(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_bUseDraw = true;
}

//============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//============================================================================
CBoss::~CBoss()
{
}


//=============================================================================
// 関数名：使用するテクスチャとモデルの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CBoss::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/character/boss_ex.png", &m_pTexture);



	FILE *pFile = fopen(MODEL_FILENAME_00, "r");	// 読み込むテキストファイルのアドレス

													// 読み込みに必要な変数宣言
	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	int nCntModel = 0;								// 読み込むモデルの数
	int nCntTexture = 0;							// 読み込むテクスチャ数
	int nMaxModel = 0;								// モデルの最大数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{// ヌルチェック
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// "NUM_MODEL"の文字列があったら
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nMaxModel);	// モデルの数を取得
			}
			if (nCntModel < nMaxModel)
			{// モーションの総数以上は設定しない
				if (strcmp(acData, "MODEL_FILENAME") == 0)
				{// "MODEL_FILENAME"の文字列があったら
					sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &m_acFileName[nCntModel][0]);	// モデルの数を取得
																											// Xファイルの読み込み
					D3DXLoadMeshFromX(m_acFileName[nCntModel],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&m_apBuffMat[nCntModel],
						NULL,
						&m_aNumMat[nCntModel],
						&m_apMesh[nCntModel]);
					nCntModel++;		// モデル数を加算

				}
			}
		}
		fclose(pFile);	// 開いたファイルを閉じる
	}

	return S_OK;

}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CBoss::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//マテリアルの数の破棄
	for (int nCntModel = 0; nCntModel < MAX_BOSS_PARTS; nCntModel++)
	{
		if (m_aNumMat[nCntModel] != NULL)
		{
			m_aNumMat[nCntModel] = NULL;

		}
	}

	//メッシュ情報の破棄
	for (int nCntModel = 0; nCntModel < MAX_BOSS_PARTS; nCntModel++)
	{
		if (m_apMesh[nCntModel] != NULL)
		{
			m_apMesh[nCntModel]->Release();
			m_apMesh[nCntModel] = NULL;
		}
	}

	//マテリアル情報の破棄
	for (int nCntModel = 0; nCntModel < MAX_BOSS_PARTS; nCntModel++)
	{
		if (m_apBuffMat[nCntModel] != NULL)
		{
			m_apBuffMat[nCntModel]->Release();
			m_apBuffMat[nCntModel] = NULL;
		}
	}

}


//=============================================================================
// 関数名：敵シーンの生成
// 関数の概要：敵シーンを生成する
//=============================================================================
CBoss *CBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife)
{
	CBoss  *pBoss;
	pBoss = new CBoss;//シーンクラスの生成
	if (pBoss != NULL)
	{
		pBoss->m_pos = pos;
		//pBoss->m_rot.y = -D3DX_PI * 0.5f;
		pBoss->m_rot = rot;
		pBoss->m_fDestAngle = pBoss->m_rot.y;
		pBoss->m_fLife = fLife;
		pBoss->m_posold = pBoss->m_pos;
		pBoss->m_bAction[BOSSBOOLTYPE_JUMP] = false;
		pBoss->Init();
	}
	return pBoss;
}

//=============================================================================
// 関数名：敵シーンの初期化処理
// 関数の概要：敵情報の設定
//=============================================================================
HRESULT CBoss::Init(void)
{

	// 敵情報の読み込み
	LoadBossText();

	// モーション情報の初期化
	InitMotion();

	//ゲージの生成
	CreateGauge();


	// 敵情報の初期化
	InitBoss();

	// パーツの初期設定
	DefaultParts();


	// 2Dシーンのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_BOSS);

	return S_OK;
}
//=============================================================================
// 関数名：敵シーンの終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CBoss::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{
		//モデルの終了処理
		if (m_apModel[nCntParts] != NULL)
		{
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}
	}



	//オブジェクトの破棄
	Release();

}

//=============================================================================
// 関数名：敵シーンの更新処理
// 関数の概要：敵の移動、パーツのモーション更新
//=============================================================================
void CBoss::Update(void)
{
	//以前の位置に今の位置を代入
	m_posold = m_pos;


	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//==============================================
	//敵の状態
	//==============================================
	BossState();

	//==============================================
	//敵の移動
	//==============================================
	BossMove();

	//==============================================
	// 落下と生成
	//==============================================
	BossFallen();


	//==============================================
	//敵のアクション
	//==============================================
	BossAction();


	//=============================================
	// モーションの状態更新
	//=============================================
	BossMotionState(m_bAction);
	BossMotion();

	//==============================================
	//位置情報を代入と慣性移動
	//==============================================
	BossSetMove_Interia();

	//=============================================
	// 体力のチェック
	//=============================================
	BossLifeCheck();

	//=============================================
	// 当たり判定 (プレイヤー)
	//=============================================
	CollisionPlayer();

	//=============================================
	// 当たり判定 (攻撃)
	//=============================================
	CollisionAttack();

	//=============================================
	// 当たり判定 (オブジェクト)
	//=============================================
	CollisionObjectBoss();

	//=============================================
	// 当たり判定(壁)
	//=============================================
	CollisionWallBoss();

	//=============================================
	// 当たり判定 (シリンダー)
	//=============================================
	CollisionCylinderBoss();

	//=============================================
	// プレイヤーの探索
	//=============================================
	UpdateFind();



	//=============================================
	// コンボ終了フレームの更新
	//=============================================
	//UpdateCancelCombo();

	//目的の角度まで
	m_fAngle = m_fDestAngle - m_rot.y;
	//角度調整
	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2;
	}

	if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2;
	}
	//敵の角度調整
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	//角度情報
	m_rot.y += m_fAngle * 0.1f;

	if (m_move.y >= 32.0f)
	{
		m_move.y = 32.0f;
	}

	if (m_pos.y >= 220.0f)
	{
		m_pos.y = 220.0f;
	}

	m_nCooltime--;
	if (m_nCooltime <= 0)
	{
		m_nCooltime = 0;
	}


	m_fSkill += 0.7f;
	//m_fSkill += 100.0f;

	//==============================================
	// デバッグログ関係
	//==============================================
#ifdef _DEBUG
	//BossDebugLog();
#endif


	//以前のモーションを代入
	m_MotionstateOld = m_Motionstate;


}

//=============================================================================
// 関数名：敵シーンの描画処理
// 関数の概要：敵のマトリックス設定、各パーツの描画
//=============================================================================
void CBoss::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);




	//======================================================================
	//	影の設定
	//======================================================================
	DrawShadow();

	//======================================================================
	//	パーツの設定
	//======================================================================
	DrawParts();


}

//=============================================================================
// 関数名：敵の影の描画
// 関数の概要：敵の影を描画する
//=============================================================================
void CBoss::DrawShadow(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DXPLANE planeField;		// 投影する平面
	D3DXVECTOR4 vecLight;		// ライトのベクトル
	D3DXVECTOR3 pos, normal;	// 位置と法線


								//αブレンディングを減算合成にする
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライトのベクトルの設定
	vecLight = D3DXVECTOR4(-0.15f, 0.7f, -0.4f, 0.0f);


	pos = D3DXVECTOR3(0.0f, 0.5f, 0.0f);		// 平面の位置の設定
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 平面の法線の設定

												//影を投影する平面の作成
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	//平面に投影する影の設定
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//シャドウマトリックスとワールドマトリックスの掛け合わせ
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{
		//モデルの描画
		m_apModel[nCntParts]->Draw();
	}
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

//=============================================================================
// 関数名：敵のパーツの描画
// 関数の概要：敵のパーツを描画する
//=============================================================================
void CBoss::DrawParts(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);



	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_bUseDraw == true)
	{
		for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
		{
			//モデルの描画
			m_apModel[nCntParts]->Draw();
		}
	}
}


//=============================================================================
// 関数名：敵の移動処理
// 関数の概要：敵の移動関係
//=============================================================================
void CBoss::BossMove(void)
{

}

//=============================================================================
// 関数名：敵のアクション
// 関数の概要：攻撃系の処理
//=============================================================================
void CBoss::BossAction(void)
{

}

//=============================================================================
// 関数名：敵の落下
// 関数の概要：落下と地面への生成
//=============================================================================
void CBoss::BossFallen(void)
{
	//落下
	m_move.y -= GRAVITY_NUM;

	if (m_pos.y <= 0.0f)
	{//床の高さ以下だったら
		m_pos.y = 0.0f;//高さを地面の高さに
		m_move.y = 0.0f;
		if (m_state == BOSSSTATE_NORMAL || m_state == BOSSSTATE_STEEL)
		{
			if (m_bAction[BOSSBOOLTYPE_JUMP] == true)
			{
				if (m_move.x < 0.2f && m_move.x > -0.2f && m_move.z < 0.2f && m_move.z > -0.2f)
				{
					m_state = BOSSSTATE_STEEL;
					m_bAction[BOSSBOOLTYPE_LANDING] = true;//着地状態に
					m_bAction[BOSSBOOLTYPE_JUMP] = false;	//ジャンプ状態を解除
				}
				m_bAction[BOSSBOOLTYPE_JUMP] = false;//ジャンプできる状態に
			}
			//if (m_bAction[BOSSBOOLTYPE_SMALLDAMAGE] == true)
			//{// ダメージ中に着地したら
			//	m_StateCnt = 120;
			//	m_state = BOSSSTATE_INVISIBLE;
			//	m_bAction[BOSSBOOLTYPE_SMALLDAMAGE] = false;				//ダメージ状態を解除
			//}
		}
	}
	if (m_pos.y <= 0.0f && m_posold.y >= 1.0f)
	{//床の高さ以下だったら
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_RAND, 20);
		}
	}
}

//=============================================================================
// 関数名：敵のモーション
// 関数の概要：ブレンド設定や、モーション
//=============================================================================
void CBoss::BossMotion(void)
{

	//-----------------------------------------------------------------------
	// モーションブレンドの設定
	//-----------------------------------------------------------------------
	if (m_Motionstate != m_MotionstateOld)
	{// モーションのステートが変わったら
		m_nCounterFrame = 0;						// 現在のキーフレームのフレーム数を初期化
		m_nCounterKey = 0;							// キーフレームの初期化
		m_nCounterAllFrame = 0;						// モーションのフレーム数を初期化
		m_nFrame = ENEMY_MOTION_BLEND_FRAME;		// モーションブレンドのフレーム数を設定
		m_nAllFrame = 0;							//全てのフレームのリセット
		for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
		{//総フレーム数の計算
			m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
		}
		m_bMotionBlend = true;		// モーションブレンドをtrueにする
	}

	//-----------------------------------------------------------------------
	// キーフレームの切り替わり
	//-----------------------------------------------------------------------
	if (m_nCounterFrame == m_nFrame)
	{// モーションブレンドの時、フレーム数が最後になったら
		m_nCounterFrame = 0; // 現在のキーフレームのフレーム数を初期化
		m_nFrame = m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].nFrame; // モーションのフレーム数を設定

		m_nCounterKey++; // キーフレームを進める

		if (m_nCounterKey == m_aKeyInfo[m_Motionstate].nNumKey)
		{// キーフレームが最後まで来たら
			if (m_aKeyInfo[m_Motionstate].nLoop == 0)
			{// ループしない場合
				switch (m_Motionstate)
				{// 敵のモーションが

				case MOTION_JUMP: // ジャンプモーションだったら
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_LANDING: // 着地モーションだったら
					m_bAction[BOSSBOOLTYPE_LANDING] = false;	//状態を戻す
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_LIGHTATTACK: //弱攻撃
					m_bAction[BOSSBOOLTYPE_LIGHTATTACK] = false;//状態を戻す
					m_nCounterKey--; // キーフレームを戻す
					break;
				case MOTION_HEAVYATTACK: //強攻撃
					m_bAction[BOSSBOOLTYPE_HEAVYATTACK] = false;//状態を戻す
					m_nCounterKey--;// キーフレームを戻す
					break;
				case MOTION_MOVEATTACK: //強攻撃
					m_bAction[BOSSBOOLTYPE_MOVEATTACK] = false;//状態を戻す
					m_nCounterKey--;// キーフレームを戻す
					break;
				case MOTION_SKILL:
					m_bAction[BOSSBOOLTYPE_SKILL] = false;// ダメージ中じゃない状態にする
					CScene::SetBossStop(false);
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_NORMALDAMAGE: //通常のダメージ
					m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = false; //状態を戻す
					for (int nCntDamage = 0; nCntDamage < 8; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
					}
					m_nCounterKey--;// キーフレームを戻す
					break;
				case MOTION_FLOATDAMAGE: //浮かしのダメージ
					m_bAction[BOSSBOOLTYPE_FLOATDAMAGE] = false; //状態を戻す
					for (int nCntDamage = 0; nCntDamage < 8; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
					}
					m_nCounterKey--;// キーフレームを戻す
					break;
				case MOTION_IMPACTDAMAGE: //吹っ飛ばしダメージ
					m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = false;
					for (int nCntDamage = 0; nCntDamage < 8; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
					}
					m_nCounterKey--;// キーフレームを戻す
					break;
				case MOTION_DEATH://死亡
					m_bAction[BOSSBOOLTYPE_DEATH] = false;
					m_nCounterKey--;// キーフレームを戻す
					break;
				}
				if (m_nCounterKey <= 0)
				{//キーが０以下になったら
					m_nCounterKey = 0;
				}
				m_state = BOSSSTATE_NORMAL; //通常状態へ
			}
			else
			{// ループする
				m_nCounterKey = m_nCounterKey % m_aKeyInfo[m_Motionstate].nNumKey; // キーフレームを進める
				m_nCounterAllFrame = 0; // モーションのフレーム数(ALL)を初期化
			}
		}
	}

	//-----------------------------------------------------------------------
	// モーションの差分を代入
	//-----------------------------------------------------------------------
	if (m_nCounterFrame == 0)
	{// フレーム数が最初の時
		for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
		{// パーツ数分ループ
			if (m_apModel[nCntParts] != NULL)
			{
				// モーションの位置差分を代入
				m_Motionpos[nCntParts] = (m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].pos[nCntParts] + m_Partspos[nCntParts]) - m_PartsDefaultpos[nCntParts];
				// モーションの回転差分を代入
				m_Motionrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].rot[nCntParts] - m_PartsDefaultrot[nCntParts];

				// 回転の差分を修正する
				// X軸の修正
				if (m_Motionrot[nCntParts].x > D3DX_PI)
				{// +3.14より、超えてしまったら
					m_Motionrot[nCntParts].x -= D3DX_PI * 2.0f;
				}
				else if (m_Motionrot[nCntParts].x < -D3DX_PI)
				{// -3.14より、下回ったら
					m_Motionrot[nCntParts].x += D3DX_PI * 2.0f;
				}
				// Y軸の修正
				if (m_Motionrot[nCntParts].y > D3DX_PI)
				{// +3.14より、超えてしまったら
					m_Motionrot[nCntParts].y -= D3DX_PI * 2.0f;
				}
				else if (m_Motionrot[nCntParts].y < -D3DX_PI)
				{// -3.14より、下回ったら
					m_Motionrot[nCntParts].y += D3DX_PI * 2.0f;
				}
				// Z軸の修正
				if (m_Motionrot[nCntParts].z > D3DX_PI)
				{// +3.14より、超えてしまったら
					m_Motionrot[nCntParts].z -= D3DX_PI * 2.0f;
				}
				else if (m_Motionrot[nCntParts].z < -D3DX_PI)
				{// -3.14より、下回ったら
					m_Motionrot[nCntParts].z += D3DX_PI * 2.0f;
				}
			}
		}
	}

	//----------------------------------------------
	// モーションの動作
	//----------------------------------------------
	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{// パーツ数分ループ
		if (m_apModel[nCntParts] != NULL)
		{
			// 位置差分をフレーム数で割った値を加算していく
			m_PartsDefaultpos[nCntParts].x += m_Motionpos[nCntParts].x / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
			m_PartsDefaultpos[nCntParts].y += m_Motionpos[nCntParts].y / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
			m_PartsDefaultpos[nCntParts].z += m_Motionpos[nCntParts].z / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
																						//回転値の代入
			m_apModel[nCntParts]->SetPos(m_PartsDefaultpos[nCntParts]);

			// 回転差分をフレーム数で割った値を加算していく
			m_PartsDefaultrot[nCntParts].x += m_Motionrot[nCntParts].x / m_nFrame;		// 回転の差分をフレーム数で割った値を加算していく
			m_PartsDefaultrot[nCntParts].y += m_Motionrot[nCntParts].y / m_nFrame;		// 回転の差分をフレーム数で割った値を加算していく
			m_PartsDefaultrot[nCntParts].z += m_Motionrot[nCntParts].z / m_nFrame;		// 回転の差分をフレーム数で割った値を加算していく
																						//回転値の代入
			m_apModel[nCntParts]->SetRot(m_PartsDefaultrot[nCntParts]);

			// モデルの回転を修正する
			// X軸の修正
			if (m_Partsrot[nCntParts].x > D3DX_PI)
			{// +3.14より、超えてしまったら
				m_Partsrot[nCntParts].x -= D3DX_PI * 2.0f;
			}
			else if (m_Partsrot[nCntParts].x < -D3DX_PI)
			{// -3.14より、下回ったら
				m_Partsrot[nCntParts].x += D3DX_PI * 2.0f;
			}
			// Y軸の修正
			if (m_Partsrot[nCntParts].y > D3DX_PI)
			{// +3.14より、超えてしまったら
				m_Partsrot[nCntParts].y -= D3DX_PI * 2.0f;
			}
			else if (m_Partsrot[nCntParts].y < -D3DX_PI)
			{// -3.14より、下回ったら
				m_Partsrot[nCntParts].y += D3DX_PI * 2.0f;
			}
			// Z軸の修正
			if (m_Partsrot[nCntParts].z > D3DX_PI)
			{// +3.14より、超えてしまったら
				m_Partsrot[nCntParts].z -= D3DX_PI * 2.0f;
			}
			else if (m_Partsrot[nCntParts].z < -D3DX_PI)
			{// -3.14より、下回ったら
				m_Partsrot[nCntParts].z += D3DX_PI * 2.0f;
			}

		}
	}

	m_nCounterFrame++;		// 現在キーフレームのフレーム数を加算
	m_nCounterAllFrame++;	// モーションのフレーム数を加算

}

//=============================================================================
// 関数名：敵の状態変化
// 関数の概要：敵の状態変化
//=============================================================================
void CBoss::BossState(void)
{
	switch (m_state)
	{
	case BOSSSTATE_NORMAL:	//通常状態
		break;


	case BOSSSTATE_APPEAR:	//出現時


		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_state = BOSSSTATE_NORMAL;
			CScene::ChangeStopBoss();
		}
		break;

	case BOSSSTATE_DEATH:
		CScore::AddScore(1000);
		Uninit();
		CGame::GAMESTATE pGameState;
		pGameState = CGame::GetGameState();
		if (pGameState == CGame::GAMESTATE_NORMAL)
		{
			CGame::SetGameState(CGame::GAMESTATE_CLEAR);
		}
		break;



		break;
	}
}

//=============================================================================
// 関数名：敵の移動量の代入と慣性移動
// 関数の概要：--
//=============================================================================
void CBoss::BossSetMove_Interia(void)
{
	//移動量に代入
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;
	//慣性移動
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;
	m_move.z += (0.0f - m_move.z) * 0.12f;
}

//=============================================================================
// 関数名：敵のモーションステートの変更
// 関数の概要：敵の状態遷移
//=============================================================================
void CBoss::BossMotionState(bool *bAction)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//カメラの角度取得
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//モデルのマトリックス情報
	D3DMATRIX ModelMtx;

	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerpos = pPlayer->GetPos();


	if (bAction[BOSSBOOLTYPE_LANDING] == true)
	{// ダメージ着地
		m_Motionstate = MOTION_LANDING; // 着地モーションに設定
	}
	else if (bAction[BOSSBOOLTYPE_JUMP] == true)
	{// 敵がジャンプ中なら
		m_Motionstate = MOTION_JUMP; // ジャンプモーションに設定
	}
	else if (bAction[BOSSBOOLTYPE_LIGHTATTACK] == true)
	{// 敵が弱攻撃をしたなら
		m_nCooltime = 130;
		if (m_nCounterAllFrame >= 2 && m_nCounterAllFrame <= 12 && m_Motionstate == MOTION_LIGHTATTACK)
		{//1フレームかつ弱攻撃モーションのとき
			m_state = BOSSSTATE_INVISIBLE;	//硬直状態に
		}
		if (m_nCounterAllFrame >= 11 && m_Motionstate == MOTION_LIGHTATTACK)
		{//8フレームかつ弱攻撃モーションのとき
			//m_state = BOSSSTATE_STEEL;	//硬直状態に
		}
		m_Motionstate = MOTION_LIGHTATTACK; // 弱攻撃モーションに設定
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_LIGHTATTACK)
		{//8フレームかつ弱攻撃モーションのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.2f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.2f);
		}

		if (m_nCounterAllFrame == 25 && m_Motionstate == MOTION_LIGHTATTACK)
		{//13フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[9]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 40.0f, CCollision::COLLSIONTYPE_BOSS_LIGHTATTACK, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_HEAVYATTACK] == true)
	{// 敵が強攻撃をしたなら
		m_nCooltime = 130;
		//m_state = BOSSSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_HEAVYATTACK; // 強攻撃モーションに設定
		if (m_nCounterAllFrame >= 3 && m_nCounterAllFrame <= 12 && m_Motionstate == MOTION_HEAVYATTACK)
		{//1フレームかつ弱攻撃モーションのとき
			m_state = BOSSSTATE_INVISIBLE;	//硬直状態に
		}
		if (m_nCounterAllFrame == 14 && m_Motionstate == MOTION_HEAVYATTACK)
		{//14フレームかつ強攻撃モーションのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.5f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.5f);
		}
		if (m_nCounterAllFrame > 13 && m_nCounterAllFrame < 25 && m_Motionstate == MOTION_HEAVYATTACK)
		{//12フレーム以上のとき回転蹴りのとき
			ModelMtx = m_apModel[15]->GetMatrix();
			for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 10);
			}
			for (int nCntParticle = 0; nCntParticle < 15; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 12);
			}
		}
		if (m_nCounterAllFrame == 36 && m_Motionstate == MOTION_HEAVYATTACK)
		{//13フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[9]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 50.0f, CCollision::COLLSIONTYPE_BOSS_HEAVYATTACK, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_MOVEATTACK] == true)
	{// 敵が強攻撃をしたなら
		m_nCooltime = 120;
		m_state = BOSSSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_MOVEATTACK; // 強攻撃モーションに設定
		if (m_nCounterAllFrame >= 7 && m_nCounterAllFrame <= 25 && m_Motionstate == MOTION_MOVEATTACK)
		{//14フレームかつ強攻撃モーションのとき
			ModelMtx = m_apModel[9]->GetMatrix();
			for (int nCntParticle = 0; nCntParticle < 5; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 7);
			}
			for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 9);
			}

			D3DXVECTOR3 DiffPos;
			//プレイヤーと敵のベクトルを算出
			DiffPos.x = m_pos.x - playerpos.x;
			DiffPos.y = m_pos.y - playerpos.y;
			DiffPos.z = m_pos.z - playerpos.z;
			D3DXVec3Normalize(&DiffPos, &DiffPos);

			//ベクトル方向に向かって移動
			m_move.x -= DiffPos.x * 2.0f;
			m_move.z -= DiffPos.z * 2.0f;

			//敵の角度設定
			m_fDestAngle = atan2f(m_pos.x - playerpos.x, m_pos.z - playerpos.z);
		}
		if (m_nCounterAllFrame == 29 && m_Motionstate == MOTION_MOVEATTACK)
		{//28フレームかつ移動パンチモーションのとき
			ModelMtx = m_apModel[9]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 55.0f, CCollision::COLLSIONTYPE_BOSS_HEAVYATTACK, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_NORMALDAMAGE])
	{
		m_Motionstate = MOTION_NORMALDAMAGE;
		m_state = BOSSSTATE_STEEL;	//硬直状態に
		if (m_nCounterAllFrame == 3 && m_Motionstate == MOTION_NORMALDAMAGE)
		{//14フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -0.91f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -0.91f;
		}
	}
	else if (bAction[BOSSBOOLTYPE_MEDIUMDAMAGE])
	{
		m_Motionstate = MOTION_NORMALDAMAGE;
		if (m_nCounterAllFrame == 3 && m_Motionstate == MOTION_NORMALDAMAGE)
		{//14フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI)) * -7.2f;
			m_move.z += (cosf(m_rot.y - D3DX_PI)) * -7.2f;

			m_state = BOSSSTATE_STEEL;	//硬直状態に
		}
	}
	else if (bAction[BOSSBOOLTYPE_FLOATDAMAGE])
	{
		m_state = BOSSSTATE_STEEL;	//硬直状態に

		m_Motionstate = MOTION_FLOATDAMAGE;
		if (m_nCounterAllFrame == 3 && m_Motionstate == MOTION_FLOATDAMAGE)
		{//3フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 33.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
		}
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_FLOATDAMAGE)
		{//3フレームのとき
			ModelMtx = m_apModel[1]->GetMatrix();

		}
	}
	else if (bAction[BOSSBOOLTYPE_IMPACTDAMAGE])
	{
		//サウンドの取得
		CSound *pSound;
		pSound = CManager::GetSound();

		m_Motionstate = MOTION_IMPACTDAMAGE;
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_IMPACTDAMAGE)
		{//1フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
			m_state = BOSSSTATE_STEEL;	//硬直状態に
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_IMPACTDAMAGE)
		{//1フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(m_pos, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
			m_state = BOSSSTATE_STEEL;	//硬直状態に
		}
	}
	else if (bAction[BOSSBOOLTYPE_SKILL] == true)
	{// ダメージ着地
		m_nCooltime = 80;
		m_Motionstate = MOTION_SKILL; // 超必殺技モーションに設定
		ModelMtx = m_apModel[1]->GetMatrix();
		if (m_nCounterAllFrame == 4 && m_Motionstate == MOTION_SKILL)
		{//6フレーム以上のときスキルのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FLASH);	//光演出
		}
		if (m_nCounterAllFrame == 5 && m_Motionstate == MOTION_SKILL)
		{//15フレーム以上のときスキルのとき
			CEffect::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43 - 15.5f), 150.0f, 6, CEffect::EFFECTTYPE_SMALL, CEffect::EFFECTMODE_SHRINKING);
		}
		if (m_nCounterAllFrame == 20 && m_Motionstate == MOTION_SKILL)
		{//60フレーム以上のときスキルのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE3);//炎音

		}
		if (m_nCounterAllFrame >= 20 && m_nCounterAllFrame <= 120 && m_Motionstate == MOTION_SKILL)
		{//60フレーム以上のときスキルのとき
			ModelMtx = m_apModel[6]->GetMatrix();

			for (int nCntParticle = 0; nCntParticle < 6; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SKILL_ENEMY, 5);
			}
			for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SKILL_ENEMY, 5);
			}
		}
		if (m_nCounterAllFrame == 40 && m_Motionstate == MOTION_SKILL)
		{//60フレーム以上のときスキルのとき

			CScene::ChangeStopBoss();

		}

		if (m_nCounterAllFrame == 75 && m_Motionstate == MOTION_SKILL)
		{//20フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[6]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 60.0f, CCollision::COLLSIONTYPE_BOSS_SKILLATTACK_S, m_pos);
		}
		if (m_nCounterAllFrame == 110 && m_Motionstate == MOTION_SKILL)
		{//20フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[6]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 40.0f, CCollision::COLLSIONTYPE_BOSS_SKILLATTACK_L, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_DEATH])
	{


		m_Motionstate = MOTION_DEATH;
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_DEATH)
		{//1フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
			m_state = BOSSSTATE_STEEL;	//硬直状態に
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_DEATH)
		{//28フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(m_pos, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
			m_state = BOSSSTATE_STEEL;	//硬直状態に
		}
		if (m_nCounterAllFrame == 78 && m_Motionstate == MOTION_DEATH)
		{//78フレームのとき
			m_state = BOSSSTATE_DEATH;
		}
	}
	else if (m_move.x > ENEMY_MOVE_DEFAULT ||
		m_move.x < -ENEMY_MOVE_DEFAULT ||
		m_move.z >  ENEMY_MOVE_DEFAULT ||
		m_move.z < -ENEMY_MOVE_DEFAULT)
	{// 敵が動いているとき
		m_Motionstate = MOTION_MOVE; // 移動状態にする
	}
	else
	{// 敵が動いてないとき
		m_Motionstate = MOTION_NEUTRAL; // ニュートラル状態にする
	}
}


//=============================================================================
// 関数名：角度の修正
// 関数の概要：大きければ、引く、小さければ、足す
//=============================================================================
void CBoss::BossRotFixes(D3DXVECTOR3 rot)
{
	// X軸の修正
	if (rot.x > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{// -3.14より、下回ったら
		rot.x += D3DX_PI * 2.0f;
	}
	// Y軸の修正
	if (rot.y > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{// -3.14より、下回ったら
		rot.y += D3DX_PI * 2.0f;
	}
	// Z軸の修正
	if (rot.z > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{// -3.14より、下回ったら
		rot.z += D3DX_PI * 2.0f;
	}
}
//=============================================================================
// 関数名：角度の修正
// 関数の概要：大きければ、引く、小さければ、足す
//=============================================================================
void CBoss::BossRotFixes(float rot)
{
	if (rot > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot -= D3DX_PI * 2.0f;
	}
	else if (rot < -D3DX_PI)
	{// -3.14より、下回ったら
		rot += D3DX_PI * 2.0f;
	}

}
//=============================================================================
// 関数名：敵情報の読み込み
// 関数の概要：敵情報の読み込みとパーツの読み込み
//=============================================================================
void CBoss::LoadBossText(void)
{
	FILE *pFile = fopen(MODEL_FILENAME_00, "r");	// 読み込むテキストファイルのアドレス

	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	int nCntMotion = 0;								// モーションのカウンタ
	int nCntKey = 0;								// キーのカウンタ
	int nCntParts = 0;								// パーツのカウンタ
	int nCntModel = 0;								// 読み込むモデルのカウンタ
	int nCntTexture = 0;							// 読み込むテクスチャのカウンタ
	int nIndex = 0;									// 使用するモデルのタイプ
	int nParent = 0;								// 親の数字
	int nMaxParts = 0;								// パーツ数の最大数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (memcmp(acData, "CHARACTERSET", 12) == 0)
			{// "CHARACTERSET"の文字列があったら
				nCntModel = 0;
				while (strcmp(acData, "END_CHARACTERSET") != 0)
				{// "END_CHARACTERSET"がまで情報を取得し続ける
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー
					if (strcmp(acData, "PARTSSET") == 0)
					{// "MODELSET"の文字列があったら
						while (strcmp(acData, "END_PARTSSET") != 0)
						{// "END_PARTSSET"がまで情報を取得し続ける
							fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
							sscanf(&acLine[0], "%s", &acData);					// データをコピー
							if (strcmp(acData, "INDEX") == 0)
							{// "TYPE"の文字列があったら
								sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nIndex);						// 使用するモデルの番号を取得
								m_aIndex[nCntModel] = nIndex;
							}
							else if (strcmp(acData, "PARENT") == 0)
							{// "TYPE"の文字列があったら
								sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nParent);						// 使用するモデルの番号を取得
								m_aParent[nCntModel] = nParent;
							}
							else if (strcmp(acData, "POS") == 0)
							{// "POS"の文字列があったら
								sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// 位置を取得
								m_Partspos[nCntModel] = pos;
							}
							else if (strcmp(acData, "ROT") == 0)
							{// "ROT"の文字列があったら
								sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// 向きを取得
																														//テキストの角度情報通りに入れる計算
								rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
								rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
								rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
								m_Partsrot[nCntModel] = rot;
							}
						}
						m_apModel[nCntModel] = CModel::Create(m_Partspos[nCntModel], m_Partsrot[nCntModel]);		//パーツの生成
						if (nCntModel == 0)
						{//体のパーツのとき
							m_apModel[nCntModel]->SetParent(NULL);
						}
						else
						{//それ以外のパーツのとき
							m_apModel[nCntModel]->SetParent(m_apModel[m_aParent[nCntModel]]);
						}
						m_apModel[nCntModel]->BindModel(m_apMesh[nCntModel], m_apBuffMat[nCntModel], m_aNumMat[nCntModel], m_pTexture);
						nCntModel++;
					}//PARTSSET
				}//END_CHARACTERSET
			}//CHARACTERSET
			if (nCntMotion < MOTION_MAX)
			{// モーションの総数以上は設定しない
				if (strcmp(acData, "MOTIONSET") == 0)
				{// MOTIONSETの文字列があったら
					nCntKey = 0;
					while (strcmp(acData, "END_MOTIONSET") != 0)
					{// END_MOTIONSETが見つかるまでループする
						fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
						sscanf(&acLine[0], "%s", &acData);					// データをコピー

						if (strcmp(acData, "LOOP") == 0)
						{// LOOPがあったら
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].nLoop);	// ループするかどうかを取得

						}
						else if (strcmp(acData, "NUM_KEY") == 0)
						{// NUM_KEYがあったら
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].nNumKey);	// キー数を取得
						}
						else if (strcmp(acData, "KEYSET") == 0)
						{// KEYSETがあったら

							nCntParts = 0;// パーツのカウント変数を初期化

							while (strcmp(acData, "END_KEYSET") != 0)
							{// END_KEYSETがあるまでループ
								fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
								sscanf(&acLine[0], "%s", &acData);					// データをコピー

								if (strcmp(acData, "FRAME") == 0)
								{// FRAMEがあったら
									sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].aKey[nCntKey].nFrame);	// キーのフレーム数を取得
								}
								else if (strcmp(acData, "KEY") == 0)
								{// KEYがあったら
									while (strcmp(acData, "END_KEY") != 0)
									{// END_KEYがあるまでループ
										fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
										sscanf(&acLine[0], "%s", &acData);					// データをコピー
										if (strcmp(acData, "POS") == 0)
										{// POSがあったら
											sscanf(&acLine[0],
												"%s %s %f %f %f",
												&acData[0],
												&acData[0],
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].x,		// 各パーツのモーションの位置を取得
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].y,		// 各パーツのモーションの位置を取得
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].z);	// 各パーツのモーションの位置を取得
										}
										else if (strcmp(acData, "ROT") == 0)
										{// ROTがあったら
											sscanf(&acLine[0],
												"%s %s %f %f %f",
												&acData[0],
												&acData[0],
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].x,		// 各パーツのモーションの回転を取得
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].y,		// 各パーツのモーションの回転を取得
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].z);		// 各パーツのモーションの回転を取得
										}//ROT
									}// END_KEY
									nCntParts++;// パーツのカウントを進める
								}// KEY
							}// END_KEYSET
							nCntKey++;// キーフレームを進める
						}//KEYSET
					}// END_MOTIONSET
					nCntMotion++;// モーションを変える
				}// MOTIONSET
			}//モーションの総数以上は設定しない
		}//while
		fclose(pFile);// 開いたファイルを閉じる
	}//ファイルが開けてたら
}
//=============================================================================
// 関数名：敵の初期化
// 関数の概要：--
//=============================================================================
void CBoss::InitBoss(void)
{
	//敵情報
	m_fRadius = ENEMY_COLLISION_RADIUS; // 敵の当たり判定の半径
	m_StateCnt = 200;
	m_state = BOSSSTATE_APPEAR;
	m_fDestAngle = D3DX_PI * 0.5f;
	m_fHeight = 80.0f;
	CCutin::Create(CCutin::CUTINMODE_ENEMYAPPEAR);
	CScene::ChangeStopBoss();

	//軌跡の生成
	//m_apModel[12]->CreateOrbit();
}
//=============================================================================
// 関数名：敵のモーションの初期化
// 関数の概要：--
//=============================================================================
void CBoss::InitMotion(void)
{
	m_nNumKey = 2;
	m_pKeyInfo = m_aKeyInfo;
	m_nKey = 0;
	m_nCountMotion = 0;
	m_Motionstate = MOTION_NEUTRAL;
}
//=============================================================================
// 関数名：パーツの初期設定
// 関数の概要：パーツの初期設定を行う
//=============================================================================
void CBoss::DefaultParts(void)
{
	for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
	{//総フレーム数の計算
		m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
	}

	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{// パーツ数分ループ
	 // モデルの位置をニュートラルモーションに初期化
		m_PartsDefaultpos[nCntParts] = m_Partspos[nCntParts] + m_aKeyInfo[m_Motionstate].aKey[0].pos[nCntParts];
		m_PartsDefaultrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[0].rot[nCntParts];
	}

}

//=============================================================================
// 関数名：オブジェクトとの当たり判定
// 関数の概要：敵とオブジェクトとの当たり判定
//=============================================================================
void CBoss::CollisionObjectBoss(void)
{
	//オブジェクトのポインタ
	CObject *pObjectTop;

	pObjectTop = (CObject*)GetSceneTop(OBJECT_PRIORITY);
	while (pObjectTop != NULL)//NULLになるまで
	{
		CObject *pObjectNext;
		pObjectNext = (CObject*)pObjectTop->GetSceneNext(OBJECT_PRIORITY);
		if (pObjectTop->GetObjType() == CScene::OBJTYPE_OBJECT)
		{
			if (pObjectTop->CollisionObject(&m_pos, &m_posold, &m_move, m_fRadius) == true)
			{
				m_bAction[BOSSBOOLTYPE_JUMP] = false;
			}
		}
		pObjectTop = pObjectNext;
	}

}
//=============================================================================
// 関数名：壁の当たり判定
// 関数の概要：敵と壁との当たり判定
//=============================================================================
void CBoss::CollisionWallBoss(void)
{
	//メッシュウォールのポインタ
	CSceneMeshWall *pCylinder;

	pCylinder = (CSceneMeshWall*)GetSceneTop(WALL_PRIORITY);
	while (pCylinder != NULL)//NULLになるまで
	{
		CSceneMeshWall *pWallNext;
		pWallNext = (CSceneMeshWall*)pCylinder->GetSceneNext(WALL_PRIORITY);
		if (pCylinder->GetObjType() == CScene::OBJTYPE_MESHWALL)
		{
			pCylinder->CollisionWall(&m_pos, &m_posold, 4.1f);

		}
		pCylinder = pWallNext;
	}

}

//=============================================================================
// 関数名：シリンダーの当たり判定
// 関数の概要：敵とシリンダーとの当たり判定
//=============================================================================
void CBoss::CollisionCylinderBoss(void)
{
	CGame::GAMESTATE gamestate = CGame::GetGameState();

	//メッシュウォールのポインタ
	CSceneMeshCylinder *pCylinder;

	pCylinder = (CSceneMeshCylinder*)GetSceneTop(CYLINDER_PRIORITY);
	while (pCylinder != NULL)//NULLになるまで
	{
		CSceneMeshCylinder *pWallNext;
		pWallNext = (CSceneMeshCylinder*)pCylinder->GetSceneNext(CYLINDER_PRIORITY);
		if (pCylinder->GetObjType() == CScene::OBJTYPE_MESHCYLINDER)
		{
			if (pCylinder->CollisionCylinder(&m_pos) == true)
			{
				if (gamestate == CGame::GAMESTATE_NORMAL && gamestate != CGame::GAMESTATE_CLEAR)
				{
					CGame::SetGameState(CGame::GAMESTATE_CLEAR);
				}
			}

		}
		pCylinder = pWallNext;
	}
}

//=============================================================================
// 関数名：攻撃との当たり判定
// 関数の概要：--
//=============================================================================
void CBoss::CollisionAttack(void)
{
	//当たり判定のポインタ
	CCollision *pCollision;

	//モデルのマトリックス情報
	D3DMATRIX ModelMtx;

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//コンボゲージのポインタ
	CComboGauge *pComboGauge;
	pComboGauge = CGame::GetComboGauge(1);

	//プレイヤーのポインタ
	CPlayer *pPlayer;
	pPlayer = (CPlayer*)GetSceneTop(1);



	pCollision = (CCollision*)GetSceneTop(COLLISION_PRIORITY);
	if (m_state == BOSSSTATE_NORMAL || m_state == BOSSSTATE_STEEL)
	{
		while (pCollision != NULL)//NULLになるまで
		{
			CCollision *pCollisionNext;
			pCollisionNext = (CCollision*)pCollision->GetSceneNext(COLLISION_PRIORITY);
			if (pCollision->GetObjType() == CScene::OBJTYPE_COLLISION)
			{
				if (pCollision->CollisionAttack(&m_pos, &m_rot, m_fHeight) == true)
				{
					for (int nCntDamage = 0; nCntDamage < 9; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
						m_state = BOSSSTATE_NORMAL; //通常状態へ

					}
					//コンボ終了フレームの設定
					m_nCancelFlame = 300;

					int nTypeHitType = pCollision->GetCollisionType();
					switch (nTypeHitType)
					{
					case HITTYPE_LIGHTPUNCH:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//ライフの減少
						m_fLife -= LIGHT_ATACK;
						//コンボゲージの増加
						pComboGauge->AddValueSlip(LIGHT_ATACK_GAUGE);

						//スキルゲージの増加
						pPlayer->SetSkill(LIGHT_ATACK_SKILL);

						//スコアの加算
						CScore::AddScore(LIGHT_ATTACK_SCORE);
						//CComboGauge::AddValueSlip(11.0f);
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//弱攻撃

						//m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_MEDIUMPUNCH:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//ライフの減少
						m_fLife -= MEDIUM_ATACK;
						//コンボゲージの増加
						pComboGauge->AddValueSlip(MEDIUM_ATACK_GAUGE);

						//スキルゲージの増加
						pPlayer->SetSkill(MEDIUM_ATACK_SKILL);

						//スコアの加算
						CScore::AddScore(MEDIUM_ATACK_SCORE);
						//中攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_MEDIUM);	//中攻撃

						//m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_SHORYUKEN:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//ライフの減少
						m_fLife -= FINISH_ATACK;
						//スコアの加算
						CScore::AddScore(FINISH_ATACK_SCORE);
						//コンボゲージの増加
						pComboGauge->AddValueSlip(FINISH_ATACK_GAUGE);

						//スキルゲージの増加
						pPlayer->SetSkill(FINISH_ATACK_SKILL);

						//大攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//大攻撃

						m_bAction[BOSSBOOLTYPE_FLOATDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 10);
						}
						break;
					case HITTYPE_HEAVYPUNCH:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//ライフの減少
						m_fLife -= LARGE_ATACK;
						//コンボゲージの増加
						pComboGauge->AddValueSlip(LARGE_ATACK_GAUGE);

						//スキルゲージの増加
						pPlayer->SetSkill(LARGE_ATACK_SKILL);

						//スコアの加算
						CScore::AddScore(LARGE_ATACK_SCORE);
						//大攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//大攻撃
						//m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_001, 10);
						}
						break;
					case HITTYPE_SKILL:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//ライフの減少
						m_fLife -= 250.0f;
						//コンボゲージの増加
						pComboGauge->AddValueSlip(80.0f);
						//スコアの加算
						CScore::AddScore(SKILL_ATACK_SCORE);
						//大攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//大攻撃
						m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_001, 60);
						}

						break;
					case HITTYPE_LIGHTKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//コンボゲージの増加
						pComboGauge->AddValueSlip(LIGHT_ATACK_GAUGE);

						//スキルゲージの増加
						pPlayer->SetSkill(LIGHT_ATACK_SKILL);

						//ライフの減少
						m_fLife -= LIGHT_ATACK;
						//スコアの加算
						CScore::AddScore(LIGHT_ATTACK_SCORE);
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//弱攻撃
						//m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_MEDIUMKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//コンボゲージの増加
						pComboGauge->AddValueSlip(MEDIUM_ATACK_GAUGE);
						//スキルゲージの増加
						pPlayer->SetSkill(MEDIUM_ATACK_SKILL);
						//ライフの減少
						m_fLife -= MEDIUM_ATACK;
						//スコアの加算
						CScore::AddScore(MEDIUM_ATACK_SCORE);
						//中攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_MEDIUM);	//中攻撃
						//m_bAction[BOSSBOOLTYPE_MEDIUMDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_SPINKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//コンボゲージの増加
						pComboGauge->AddValueSlip(FINISH_ATACK_GAUGE);
						//スキルゲージの増加
						pPlayer->SetSkill(FINISH_ATACK_SKILL);
						//ライフの減少
						m_fLife -= FINISH_ATACK;
						//スコアの加算
						CScore::AddScore(FINISH_ATACK_SCORE);
						//大攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//大攻撃
						m_bAction[BOSSBOOLTYPE_FLOATDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_HEAVYKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//コンボゲージの増加
						pComboGauge->AddValueSlip(LARGE_ATACK_GAUGE);
						//コンボゲージの増加
						pComboGauge->AddValueSlip(LARGE_ATACK_SKILL);
						//ライフの減少
						m_fLife -= LARGE_ATACK;
						//スコアの加算
						CScore::AddScore(LARGE_ATACK_SCORE);
						//大攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//大攻撃
						//m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					}
				}
			}
			pCollision = pCollisionNext;
		}
	}
}
//=============================================================================
// 関数名：プレイヤーとの当たり判定
// 関数の概要：半径外へ戻す
//=============================================================================
void CBoss::CollisionPlayer(void)
{
	//敵のポインタ
	CPlayer *pPlayer;

	pPlayer = (CPlayer*)GetSceneTop(1);
	while (pPlayer != NULL)//NULLになるまで
	{
		CPlayer *pPlayerNext;
		pPlayerNext = (CPlayer*)pPlayer->GetSceneNext(1);
		if (pPlayer->GetObjType() == CScene::OBJTYPE_PLAYER)
		{
			pPlayer->CollisionPlayer(&m_pos, &m_posold, &m_move, &m_rot, 25.0f);

		}
		pPlayer = pPlayerNext;
	}



}

//=============================================================================
// 関数名：ゲージの生成
// 関数の概要：体力ゲージの生成
//=============================================================================
void CBoss::CreateGauge(void)
{
	m_apLifeGauge[0] = CUiGauge::Create(D3DXVECTOR3(SCREEN_WIDTH  /2 - 300.0f, SCREEN_HEIGHT /2 + 300.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 800.0f, 30.0f, m_fLife, m_fLife, CUiGauge::GAUGETYPE_UNDER);	//ゲージの下地
	m_apLifeGauge[1] = CUiGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 245.0f, SCREEN_HEIGHT /2 + 300.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 700.0f, 20.0f, m_fLife, m_fLife, CUiGauge::GAUGETYPE_RED);	//体力ゲージの下地
	m_apLifeGauge[2] = CUiGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 245.0f, SCREEN_HEIGHT /2 + 300.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 700.0f, 20.0f, m_fLife, m_fLife, CUiGauge::GAUGETYPE_GREEN);	//体力ゲージ

}


//=============================================================================
// 関数名：コンボ終了フレームの更新
// 関数の概要：0になったらコンボ終了
//=============================================================================
void CBoss::UpdateCancelCombo(void)
{
	//コンボゲージのポインタ
	CComboGauge *pComboGauge;
	pComboGauge = CGame::GetComboGauge(1);

	//キャンセルフレームの減算
	m_nCancelFlame--;
	if (m_nCancelFlame <= 0)
	{
		m_nCancelFlame = 0;
		pComboGauge->SetComboGauge(0.0f);
		pComboGauge->SetCurrentGauge(0.0f);

	}

}
//=============================================================================
// 関数名：プレイヤーの探索
// 関数の概要：プレイヤーが半径に入ってきたらtrueを返す
//=============================================================================
bool CBoss::FindPlayer(D3DXVECTOR3 *pos, float fRange)
{
	bool bfind = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//プレイヤーとの距離
	float fFind = powf(fRange, 2.0f);	//半径

	if (fLength < fFind)
	{//対象の頂点が範囲内だったら
		bfind = true;
	}
	else
	{
		bfind = false;
	}


	return bfind;
}

//=============================================================================
// 関数名：プレイヤーへの攻撃
// 関数の概要：プレイヤーが半径に入ってきたらtrueを返す
//=============================================================================
bool CBoss::AttackPlayer(D3DXVECTOR3 * pos, float fRange)
{
	bool bfind = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//プレイヤーとの距離
	float fFind = powf(fRange, 2.0f);	//半径

	if (fLength < fFind)
	{//対象の頂点が範囲内だったら
		bfind = true;
	}
	else
	{
		bfind = false;
	}


	return bfind;
}
//=============================================================================
// 関数名：敵との当たり判定
// 関数の概要：半径外へ戻す
//=============================================================================
bool CBoss::CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius)
{
	bool bRand = false;			// 着地したかどうか
	//----------------------------------------------------------
	// 矩形での当たり判定
	//----------------------------------------------------------
	if (m_pos.x - m_fRadius < pPos->x + fRadius &&
		m_pos.x + m_fRadius > pPos->x - fRadius &&
		m_pos.z - m_fRadius < pPos->z + fRadius &&
		m_pos.z + m_fRadius > pPos->z - fRadius)
	{// オブジェクトの範囲内だったら
		if (m_pos.y + m_fHeight <= pPosOld->y && m_pos.y + m_fHeight > pPos->y)
		{// 上からの判定
			pPos->x += (sinf(pRot->y - D3DX_PI) * 1.0f) * 30.0f;
			pPos->z += (cosf(pRot->y - D3DX_PI) * 1.0f) * 30.0f;
		}
	}
	if (m_pos.y + m_fHeight + 10.0f > pPos->y + fRadius &&
		m_pos.y + -5.0f <= pPos->y + fRadius)
	{// プレイヤーがオブジェクトの範囲内なら
		if (m_pos.x - m_fRadius < pPos->x + fRadius && m_pos.x + m_fRadius > pPos->x - fRadius)
		{// オブジェクトがオブジェクトのZ軸の範囲内に入ったら
			if (m_pos.z - m_fRadius >= pPosOld->z + fRadius &&
				m_pos.z - m_fRadius < pPos->z + fRadius)
			{// 手前からの判定

				pPos->z = (m_pos.z - m_fRadius - fRadius);	// オブジェクトの位置を戻す

			}
			else if (m_pos.z + m_fRadius <= pPosOld->z - fRadius &&
				m_pos.z + m_fRadius > pPos->z - fRadius)
			{// 奥からの判定

				pPos->z = m_pos.z + m_fRadius + fRadius;	// オブジェクトの位置を戻す

			}

		}
		if (m_pos.z - m_fRadius < pPos->z + fRadius && m_pos.z + m_fRadius > pPos->z - fRadius)
		{// Z軸がオブジェクトの範囲内だったら
			if (m_pos.x - m_fRadius >= pPosOld->x + fRadius &&
				m_pos.x - m_fRadius < pPos->x + fRadius)
			{// 左からの判定

				pPos->x = m_pos.x - m_fRadius - fRadius;	// オブジェクトの位置を戻す

			}
			else if (m_pos.x + m_fRadius <= pPosOld->x - fRadius &&
				m_pos.x + m_fRadius > pPos->x - fRadius)
			{// 右からの判定

				pPos->x = m_pos.x + m_fRadius + fRadius;			// オブジェクトの位置を戻す

			}
		}
	}

	return bRand;
}

//=============================================================================
// 関数名：敵関係のデバッグログ
// 関数の概要：--
//=============================================================================
void CBoss::BossDebugLog(void)
{
	CDebugProc::Print("\n");
	CDebugProc::Print("敵位置(XYZ): %.1f, %.1f, %.1f：\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("敵角度(XYZ): %.1f, %.1f, %.1f：\n", m_rot.x, m_rot.y, m_rot.z);
	//CDebugProc::Print("敵移動量(XYZ): %.1f, %.1f, %.1f：\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("m_fSkill: %.1f\n", m_fSkill);

	//CDebugProc::Print("状態変化のカウンタ：%d\n", m_StateCnt);

	//switch (m_state)
	//{
	//case BOSSSTATE_NORMAL://通常状態のとき
	//	CDebugProc::Print("STATE：[0]NORMAL\n");
	//	break;
	//case BOSSSTATE_STEEL://硬直状態のとき
	//	CDebugProc::Print("STATE：[1]STEEL\n");
	//	break;
	//case BOSSSTATE_APPEAR://出現時のとき
	//	CDebugProc::Print("STATE：[4]BOSSSTATE_APPEAR\n");
	//	break;
	//}

	//switch (m_Motionstate)
	//{
	//case MOTION_NEUTRAL://ニュートラルのとき
	//	CDebugProc::Print("MOTION：[0]NEUTRAL\n");
	//	break;
	//case MOTION_MOVE://移動のとき
	//	CDebugProc::Print("MOTION：[1]MOVE\n");
	//	break;
	//case MOTION_JUMP://ジャンプのとき
	//	CDebugProc::Print("MOTION：[3]JUMP\n");
	//	break;
	//case MOTION_LANDING://着地のとき
	//	CDebugProc::Print("MOTION：[4]LANDING\n");
	//	break;
	//case MOTION_SKILL://着地のとき
	//	CDebugProc::Print("MOTION：[6]MOTION_SKILL\n");
	//	break;
	//}
	//switch (m_MotionstateOld)
	//{
	//case MOTION_NEUTRAL://ニュートラルのとき
	//	CDebugProc::Print("MOTION：[0]NEUTRAL\n");
	//	break;
	//case MOTION_MOVE://移動のとき
	//	CDebugProc::Print("MOTION：[1]MOVE\n");
	//	break;
	//case MOTION_JUMP://ジャンプのとき
	//	CDebugProc::Print("MOTION：[3]JUMP\n");
	//	break;
	//case MOTION_LANDING://着地のとき
	//	CDebugProc::Print("MOTION：[4]LANDING\n");
	//	break;
	//}
	//CDebugProc::Print("KEY：%d / %d\n", m_nCounterKey, m_aKeyInfo[m_Motionstate].nNumKey);
	//CDebugProc::Print("FRAME：%d / %d (%d / %d)\n", m_nCounterFrame, m_nFrame, m_nCounterAllFrame, m_nAllFrame);
	//CDebugProc::Print("AllFrame：%d\n", m_nCounterAllFrame);

	//CDebugProc::Print("m_nCooltime：%d\n", m_nCooltime);


}

//=============================================================================
// 関数名：プレイヤーの探索
// 関数の概要：--
//=============================================================================
void CBoss::UpdateFind(void)
{
	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerpos = pPlayer->GetPos();

	if (FindPlayer(&playerpos, 70.0f) == true)
	{//プレイヤーが範囲内に入ったら
		if (m_state == BOSSSTATE_NORMAL)
		{
			D3DXVECTOR3 DiffPos;
			//プレイヤーと敵のベクトルを算出
			DiffPos.x = m_pos.x - playerpos.x;
			DiffPos.y = m_pos.y - playerpos.y;
			DiffPos.z = m_pos.z - playerpos.z;
			D3DXVec3Normalize(&DiffPos, &DiffPos);

			//ベクトル方向に向かって移動
			m_move.x -= DiffPos.x * 0.35f;
			m_move.z -= DiffPos.z * 0.35f;

			//敵の角度設定
			m_fDestAngle = atan2f(m_pos.x - playerpos.x, m_pos.z - playerpos.z);


		}

		if (AttackPlayer(&playerpos, 7.0f) == true)
		{
			if (m_state == BOSSSTATE_NORMAL)
			{
				m_move.x = 0.0f;
				m_move.z = 0.0f;


				for (int nCntAction = 6; nCntAction < 10; nCntAction++)
				{
					if (m_bAction[nCntAction] == false)
					{
						if (m_bAction[BOSSBOOLTYPE_SKILL] == false)
						{
							if (rand() % 60 == 0 && m_nCooltime == 0)
							{
								m_bAction[BOSSBOOLTYPE_LIGHTATTACK] = true;

							}
							else if (rand() % 80 == 0 && m_nCooltime == 0)
							{
								m_bAction[BOSSBOOLTYPE_HEAVYATTACK] = true;

							}
							else if (rand() % 90 == 0 && m_nCooltime == 0 && m_fSkill >= 480.0f)
							{
								m_state = BOSSSTATE_INVISIBLE;	//硬直状態に

								if (m_bAction[BOSSBOOLTYPE_LIGHTATTACK] == false && m_bAction[BOSSBOOLTYPE_HEAVYATTACK] == false)
								{
									CGame::GAMESTATE pGameState;
									pGameState = CGame::GetGameState();
									if (pGameState == CGame::GAMESTATE_NORMAL)
									{
										m_fSkill = 0.0f;
										m_bAction[BOSSBOOLTYPE_SKILL] = true;
										CCutin::Create(CCutin::CUTINMODE_ENEMY);
										CScene::ChangeStopBoss();
									}
								}
							}
						}

					}
				}
			}
		}//半径7以内で
		else
		{
			if (m_state == BOSSSTATE_NORMAL)
			{
				if (rand() % 60 == 0 && m_nCooltime == 0)
				{
					m_bAction[BOSSBOOLTYPE_MOVEATTACK] = true;

				}
			}

		}
	}//半径70以内で
}

//=============================================================================
// 関数名：位置情報の取得
// 関数の概要：敵の位置情報を返す
//=============================================================================
D3DXVECTOR3 CBoss::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 関数名：角度情報の取得
// 関数の概要：敵の角度情報を返す
//=============================================================================
D3DXVECTOR3 CBoss::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// 関数名：移動量情報の取得
// 関数の概要：敵の移動量を返す
//=============================================================================
D3DXVECTOR3 CBoss::GetMove(void)
{
	return m_move;
}

//=============================================================================
// 関数名：プレイヤーのライフのチェック
// 関数の概要：プレイヤーのライフの確認
//=============================================================================
void CBoss::BossLifeCheck(void)
{

	if (m_fLife <= 0.0f)
	{
		for (int nCntDamage = 5; nCntDamage < 9; nCntDamage++)
		{
			m_bAction[nCntDamage] = false;
		}
		m_bAction[BOSSBOOLTYPE_DEATH] = true;
		m_fLife = 0.0f;
	}

	if (m_fSkill >= 500.0f)
	{
		m_fSkill = 500.0f;
	}


	m_apLifeGauge[1]->AddValueSlip(m_fLife);
	m_apLifeGauge[2]->AddValue(m_fLife);




}

