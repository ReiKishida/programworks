//=============================================================================
//
// プレイヤー処理[モデル] [player.cpp]
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
#include "player.h"
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
#include "ui_gauge.h"
#include "enemy.h"
#include "enemy_pow.h"
#include "enemy_trc.h"
#include "collision.h"
#include "combogauge.h"
#include "boss.h"
#include "effect.h"
#include "cutin.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//=====================================
// プレイヤー情報
//=====================================
#define PLAYER_MOVE (0.5f)						//移動量
//#define PLAYER_MOVE (1.0f)						//移動量

#define PLAYER_MOVE_DEFAULT		(1.0f)			// プレイヤーが動いてる基準の移動量
#define MAX_CHAR (256)							//文字の最大数
#define GRAVITY_NUM (0.8f)						//重力の値
#define PLAYER_MOTION_BLEND_FRAME	(5)			// モーションブレンドのフレーム数
#define JUMP_POWER (25.0f)						//ジャンプ力
#define PLAYER_COLLISION_RADIUS		(20.0f)		// プレイヤーの当たり判定の半径

#define SCREEN_LIMIT_MAX_X (5000.0f )			//画面端(右端)
#define SCREEN_LIMIT_MIN_X (-120.0f)			//画面(左端)

#define SCREEN_LIMIT_MIN_Z (-370.0f)			//画面(下端)

#define COLLISION_RADIUS_S (25.0f)	//当たり判定の半径
#define COLLISION_RADIUS_M (30.0f)	//当たり判定の半径
#define COLLISION_LIFE (1)

#define PARTICLE_FIRE_NUM (10)
#define PARTICLE_BLACKFIRE_NUM (5)
#define PARTICLE_FIRE_LIFE (15)
#define PARTICLE_BLACKFIRE_LIFE (8)

//=====================================
// ライフ
//=====================================
#define PLAYER_LIFE (500.0f)
#define PLAYER_LIFE_FLAME (570.0f)

//=====================================
// パーティクルの色
//=====================================
#define PARTICLE_FIRE (D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f))
#define PARTICLE_FIRE_A (D3DXCOLOR(0.8f, 0.2f, 0.0f, 0.2f))

#define PARTICLE_DEST (D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f))
#define PARTICLE_DEST_A (D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.2f))

//=====================================
// ダメージ量
//=====================================
#define LIGHT_ATACK (9.0f)		//弱攻撃
#define LARGE_ATACK (15.0f)		//強攻撃


//=====================================
// 優先順位、敵の数
//=====================================
#define ENEMY_PRIORITY (1)						//敵の優先順位
#define COLLISION_PRIORITY (6)					//当たり判定の優先順位
#define WALL_PRIORITY (5)						//壁の優先順位
#define OBJECT_PRIORITY (4)						//オブジェクトの優先順位
#define CYLINDER_PRIORITY (3)					//シリンダーの優先順位
#define MAXENEMY_NUM (30)

//=====================================
// 読み込むテキストファイル
//=====================================
#define MODEL_FILENAME ("data/TEXT/motion_player.txt")			//読み込むファイル名

//**************************************************************************************************
// 静的メンバ変数
//**************************************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;							//共有テクスチャのポインタ
char			   CPlayer::m_acFileName[MAX_PLAYER_PARTS][256] = {};	// 読み込む
LPD3DXMESH		   CPlayer::m_apMesh[MAX_PLAYER_PARTS] = {};			//メッシュ情報へのポインタ
LPD3DXBUFFER	   CPlayer::m_apBuffMat[MAX_PLAYER_PARTS] = {};			//マテリアル情報へのポインタ
DWORD			   CPlayer::m_aNumMat[MAX_PLAYER_PARTS] = {};			//マテリアルの情報の数
D3DXVECTOR3		   CPlayer::m_Partspos[MAX_PLAYER_PARTS] = {};			//パーツの位置
D3DXVECTOR3		   CPlayer::m_Partsrot[MAX_PLAYER_PARTS] = {};			//パーツの位置
D3DXVECTOR3		   CPlayer::m_PartsDefaultpos[MAX_PLAYER_PARTS] = {};	//デフォルトのパーツ位置
D3DXVECTOR3		   CPlayer::m_PartsDefaultrot[MAX_PLAYER_PARTS] = {};	//デフォルトのパーツ角度
D3DXVECTOR3		   CPlayer::m_Motionpos[MAX_PLAYER_PARTS] = {};			//モーション差分代入用位置
D3DXVECTOR3		   CPlayer::m_Motionrot[MAX_PLAYER_PARTS] = {};			//モーション差分代入用角度
int				   CPlayer::m_aParent[MAX_PLAYER_PARTS] = {};			//パーツの親
int				   CPlayer::m_aIndex[MAX_PLAYER_PARTS] = {};			//パーツ番号
CMeshOrbit		  *CPlayer::m_pMeshOrbit = NULL;						//軌跡のポインタ
CUiGauge		  *CPlayer::m_apUi[MAX_PLAYER_UI] = {};					//UIのポインタ
CCollision		  *CPlayer::m_pCollisionAttack = {};	// 攻撃時の当たり判定のポインタ
float			   CPlayer::m_fLife = PLAYER_LIFE;								//プレイヤーのライフ
float			   CPlayer::m_fSkill = 0.0f;								//スタン
float			   CPlayer::m_fDamage = 0.0f;								//ダメージ
//========================================================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//========================================================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_bUseDraw = true;
}

//============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//============================================================================
CPlayer::~CPlayer()
{
}


//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/character/player.png", &m_pTexture);



	FILE *pFile = fopen(MODEL_FILENAME, "r");		// 読み込むテキストファイルのアドレス

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
void CPlayer::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//マテリアルの数の破棄
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
	{
		if (m_aNumMat[nCntModel] != NULL)
		{
			m_aNumMat[nCntModel] = NULL;

		}
	}

	//メッシュ情報の破棄
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
	{
		if (m_apMesh[nCntModel] != NULL)
		{
			m_apMesh[nCntModel]->Release();
			m_apMesh[nCntModel] = NULL;
		}
	}

	//マテリアル情報の破棄
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
	{
		if (m_apBuffMat[nCntModel] != NULL)
		{
			m_apBuffMat[nCntModel]->Release();
			m_apBuffMat[nCntModel] = NULL;
		}
	}
}


//=============================================================================
// 関数名：プレイヤーシーンの生成
// 関数の概要：プレイヤーシーンを生成する
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer  *pPlayer;
	pPlayer = new CPlayer;//シーンクラスの生成
	if (pPlayer != NULL)
	{
		pPlayer->m_pos = pos;
		pPlayer->m_rot.y = -D3DX_PI * 0.5f;
		pPlayer->m_posold = pPlayer->m_pos;
		pPlayer->m_bAction[PLAYERBOOLTYPE_JUMP] = false;
		pPlayer->m_bJump = false;
		pPlayer->Init();
	}
	return pPlayer;
}

//=============================================================================
// 関数名：プレイヤーシーンの初期化処理
// 関数の概要：プレイヤー情報の設定
//=============================================================================
HRESULT CPlayer::Init(void)
{

	// プレイヤー情報の読み込み
	LoadPlayerText();

	 // モーション情報の初期化
	InitMotion();

	// プレイヤー情報の初期化
	InitPlayer();

	// ゲージ類の生成
	CreateGauge();

	// パーツの初期設定
	DefaultParts();


	// プレイヤーのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}
//=============================================================================
// 関数名：プレイヤーシーンの終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
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
// 関数名：プレイヤーシーンの更新処理
// 関数の概要：プレイヤーの移動、パーツのモーション更新
//=============================================================================
void CPlayer::Update(void)
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
	//プレイヤーの状態
	//==============================================
	PlayerState();

	//==============================================
	//プレイヤーの移動
	//==============================================
	PlayerMove();

	//==============================================
	//プレイヤーのアクション
	//==============================================
	PlayerAction();


	 //=============================================
	 // モーションの状態更新
	 //=============================================
	PlayerMotionState(m_bAction);
	PlayerMotion();

	//==============================================
	//位置情報を代入と慣性移動
	//==============================================
	PlayerSetMove_Interia();

	//=============================================
	// 当たり判定 (敵)
	//=============================================
	CollisionEnemy();

	//=============================================
	// 当たり判定 (攻撃)
	//=============================================
	CollisionAttack();

	//=============================================
	// 当たり判定 (オブジェクト)
	//=============================================
	CollisionObjectPlayer();

	//=============================================
	// 当たり判定(壁)
	//=============================================
	CollisionWallPlayer();

	//=============================================
	// 当たり判定 (シリンダー)
	//=============================================
	CollisionCylinderPlayer();

	//=============================================
	// 残り体力のチェック
	//=============================================
	PlayerLifeCheck();


	//=============================================
	// 残りスキルのチェック
	//=============================================
	PlayerSkillCheck();

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
	//プレイヤーの角度調整
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	//角度情報
	m_rot.y += m_fAngle * 0.35f;


	if (mode == CManager::MODE_TUTORIAL)
	{
		m_fSkill += 10.0f;
	}

	//==============================================
	// 落下と生成
	//==============================================
	PlayerFallen();


	//==============================================
	// ボスの生成
	//==============================================
	if ((m_pos.x >= 3700.0f && m_pos.x <= 4100.0f) && (m_pos.z <= -3450.0f && m_pos.z >= -3500.0f))
	{
		if (CGame::GetBool() == false)
		{
			CGame::ChangeBoolCreateBoss();
		}
	}



	//==============================================
	// デバッグログ関係
	//==============================================
#ifdef _DEBUG
	//PlayerDebugLog();
#endif


	//以前のモーションを代入
	m_MotionstateOld = m_Motionstate;


}

//=============================================================================
// 関数名：プレイヤーシーンの描画処理
// 関数の概要：プレイヤーのマトリックス設定、各パーツの描画
//=============================================================================
void CPlayer::Draw(void)
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
// 関数名：プレイヤーの影の描画
// 関数の概要：プレイヤーの影を描画する
//=============================================================================
void CPlayer::DrawShadow(void)
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


	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{
		//モデルの描画
		m_apModel[nCntParts]->Draw();
	}
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

//=============================================================================
// 関数名：プレイヤーのパーツの描画
// 関数の概要：プレイヤーのパーツを描画する
//=============================================================================
void CPlayer::DrawParts(void)
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
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{
			//モデルの描画
			m_apModel[nCntParts]->Draw();
		}
	}
}


//=============================================================================
// 関数名：プレイヤーの移動処理
// 関数の概要：プレイヤーの移動関係
//=============================================================================
void CPlayer::PlayerMove(void)
{
	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//カメラの角度取得
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();



	if (m_state == PLAYERSTATE_NORMAL )
	{//プレイヤーの状態で移動できるか
	 //=========================================//
	 //任意のキー(→キー)が押されたかどうか    //
	 //=======================================//
		if (pInputKeyboard->GetKeyboardPress(DIK_A) == true || pGamePadStick[0].aGamePad.lX < -GAMEPAD_DEADZONE)
		{//Aキーが押されたとき
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
			{//上キー(Wキー)が押された
				m_move.x += sinf(-D3DX_PI * 0.25f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(-D3DX_PI * 0.25f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * 0.75f) + CameraRot.y;


			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
			{//下キー(↓キー)が押された
				m_move.x += sinf(-D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(-D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * 0.25f) + CameraRot.y;


			}
			else
			{//左キー(←キー)のみ
				m_move.x += sinf((-D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf((-D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;

				m_fDestAngle = (D3DX_PI * 0.5f + CameraRot.y);
			}
		}
		//=========================================
		//任意のキー(Dキー)が押されたかどうか    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true || pGamePadStick[0].aGamePad.lX > GAMEPAD_DEADZONE)
		{
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
			{//上キー(↑キー)が押された
				m_move.x += sinf(D3DX_PI *  0.25f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(D3DX_PI *  0.25f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * -0.75f) + CameraRot.y;

			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
			{//下キー(↓キー)が押された
				m_move.x += sinf(D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * -0.25f) + CameraRot.y;
			}
			else
			{//右キー(→キー)のみ
				m_move.x += sinf((D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf((D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * -0.5f) + CameraRot.y;
			}
		}
		//=========================================
		//任意のキー(Wキー)が押されたかどうか    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
		{//↑キーが押されたとき
			m_move.x += sinf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
			m_move.z += cosf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
			m_fDestAngle = (D3DX_PI)+CameraRot.y;
		}
		//=========================================//
		//任意のキー(Sキー)が押されたかどうか	  //
		//=======================================//
		else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
		{//↓キーが押されたとき
			m_move.x -= sinf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
			m_move.z -= cosf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;

			m_fDestAngle = (D3DX_PI * 0.0f) + CameraRot.y;
		}


	}//(閉じ)プレイヤーの状態で移動できるか

	 //カメラの角度調整
	if (CameraRot.y > D3DX_PI)
	{
		CameraRot.y -= D3DX_PI * 2;
	}

	if (CameraRot.y < -D3DX_PI)
	{
		CameraRot.y += D3DX_PI * 2;
	}
}

//=============================================================================
// 関数名：プレイヤーのアクション
// 関数の概要：攻撃系の処理
//=============================================================================
void CPlayer::PlayerAction(void)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CComboGauge *pComboGauge;
	pComboGauge = CGame::GetComboGauge(1);

	//=======================================================//
	//任意のキー(Uキー)が押されたかどうか(弱パンチ)			//
	//=====================================================//

			if (pInputKeyboard->GetKeyboardTrigger(DIK_U) == true || pGamePad->GetGamePadTrigger(0, BUTTON_X) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
			{//Uキーが押されたとき

						if (m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_2] == false && m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_1] == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
						{
							m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_2] = true;
							m_bAction[PLAYERBOOLTYPE_SHORYUKEN] = true;
						}
				if (m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_1] == false && m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_0] == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
				{
					m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_1] = true;
					m_bAction[PLAYERBOOLTYPE_MIDDLEPUNCH] = true;
				}
				else if (m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_0] == false && m_state == PLAYERSTATE_NORMAL && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
				{
					m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_0] = true;
					m_bAction[PLAYERBOOLTYPE_LIGHTPUNCH] = true;
				}
			}

	//=======================================================//
	//任意のキー(Iキー)が押されたかどうか(強パンチ)			//
	//=====================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_I) == true || pGamePad->GetGamePadTrigger(0, BUTTON_Y) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false )
	{
		if (m_state == PLAYERSTATE_NORMAL)
		{
			for (int nCnt = 11; nCnt <= 12; nCnt++)
			{
				if (m_bAction[nCnt] == false)
				{
					m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_HEAVY] = true;
					m_bAction[PLAYERBOOLTYPE_HEAVYPUNCH] = true;
				}
			}
		}
	}




	//=======================================================//
	//任意のキー(Uキー)が押されたかどうか(弱キック)			//
	//=====================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_J) == true || pGamePad->GetGamePadTrigger(0, BUTTON_A) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
	{//エンターキーが押されたとき

		for (int nCnt = 11; nCnt <= 12; nCnt++)
		{
			if (m_bAction[nCnt] == false)
			{
				if (m_bAttackKick[ATTACKBOOLTYPE_KICK_2] == false && m_bAttackKick[ATTACKBOOLTYPE_KICK_1] == true)
				{
					m_bAttackKick[ATTACKBOOLTYPE_KICK_2] = true;
					m_bAction[PLAYERBOOLTYPE_SPINKICK] = true;
				}
			}
		}
		if (m_bAttackKick[ATTACKBOOLTYPE_KICK_1] == false && m_bAttackKick[ATTACKBOOLTYPE_KICK_0] == true)
		{
			m_bAttackKick[ATTACKBOOLTYPE_KICK_1] = true;
			m_bAction[PLAYERBOOLTYPE_MIDDLEKICK] = true;
		}
		else if (m_bAttackKick[ATTACKBOOLTYPE_KICK_0] == false && m_state == PLAYERSTATE_NORMAL)
		{
			m_bAttackKick[ATTACKBOOLTYPE_KICK_0] = true;
			m_bAction[PLAYERBOOLTYPE_LIGHTKICK] = true;
		}

	}
	//=======================================================//
	//任意のキー(Iキー)が押されたかどうか(強キック)			//
	//=====================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_K) == true || pGamePad->GetGamePadTrigger(0, BUTTON_B) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
	{
		if (m_state == PLAYERSTATE_NORMAL)
		{
			for (int nCnt = 11; nCnt <= 12; nCnt++)
			{
				if (m_bAction[nCnt] == false)
				{
					m_bAttackKick[ATTACKBOOLTYPE_KICK_HEAVY] = true;
					m_bAction[PLAYERBOOLTYPE_HEAVYKICK] = true;
				}
			}
		}
	}



	//===============================================================//
	//任意のキー(Oキー)が押されたかどうか(超必殺技)					//
	//=============================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_O) == true || pGamePad->GetGamePadTrigger(0, BUTTON_R1) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
	{//エンターキーが押されたとき
		if (m_fSkill >= PLAYER_LIFE)
		{
			for (int nCnt = 11; nCnt <= 12; nCnt++)
			{
				if (m_bAction[nCnt] == false)
				{
					if (m_state == PLAYERSTATE_NORMAL)
					{

							m_state = PLAYERSTATE_INVISIBLE;

							m_bAction[PLAYERBOOLTYPE_SKILL] = true;
							CCutin::Create(CCutin::CUTINMODE_PLAYER);
							m_fSkill = 0.0f;
							CScene::ChangeStop();

							m_StateCnt = 222;

					}
				}
			}
		}
	}


	//=============================================//
	//任意のキー(スペースキー)が押されたかどうか  //
	//===========================================//u
	if ((pInputKeyboard->GetKeyboardTrigger(DIK_L) == true || pGamePad->GetGamePadTrigger(0, BUTTON_R2) == true )&& m_state == PLAYERSTATE_NORMAL)
	{//スペースキーが押されたとき
		for (int nCnt = 11; nCnt <= 12; nCnt++)
		{
			if (m_bAction[nCnt] == false)
			{
				if (m_bAction[PLAYERBOOLTYPE_JUMP] == false)
				{
					m_bAction[PLAYERBOOLTYPE_JUMP] = true;
					m_bJump = true;
					m_move.y += JUMP_POWER;
				}
			}
		}
	}


	if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true)
	{
		m_fSkill += 100.0f;
	}
}

//=============================================================================
// 関数名：プレイヤーの落下
// 関数の概要：落下と地面への生成
//=============================================================================
void CPlayer::PlayerFallen(void)
{
	//落下
	m_move.y -= GRAVITY_NUM;

	if (m_pos.y <= 0.0f)
	{//床の高さ以下だったら
		m_pos.y = 0.0f;//高さを地面の高さに
		m_move.y = 0.0f;
		if (m_bAction[PLAYERBOOLTYPE_JUMP] == true)
		{
			if (m_move.x < 0.2f && m_move.x > -0.2f && m_move.z < 0.2f && m_move.z > -0.2f)
			{
				m_state = PLAYERSTATE_STEEL;
				m_bAction[PLAYERBOOLTYPE_LANDING] = true;//着地状態に
				m_bAction[PLAYERBOOLTYPE_JUMP] = false;	//ジャンプ状態を解除
				m_bJump = false;//ジャンプ状態を解除
			}
			m_bAction[PLAYERBOOLTYPE_JUMP] = false;//ジャンプできる状態に
			m_bJump = false;//ジャンプ状態を解除
		}

	}
	if (m_pos.y <= 0.0f && m_posold.y >= 1.0f)
	{//床の高さ以下だったら
		CreateParticleDust();
	}
}

//=============================================================================
// 関数名：プレイヤーのモーション
// 関数の概要：ブレンド設定や、モーション
//=============================================================================
void CPlayer::PlayerMotion(void)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//-----------------------------------------------------------------------
	// モーションブレンドの設定
	//-----------------------------------------------------------------------
	if (m_Motionstate != m_MotionstateOld)
	{// モーションのステートが変わったら
		m_nCounterFrame = 0;						// 現在のキーフレームのフレーム数を初期化
		m_nCounterKey = 0;							// キーフレームの初期化
		m_nCounterAllFrame = 0;						// モーションのフレーム数を初期化
		m_nFrame = PLAYER_MOTION_BLEND_FRAME;		// モーションブレンドのフレーム数を設定
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
				{// プレイヤーのモーションが

				case MOTION_SLIDING://スライディングの場合
					m_bAction[PLAYERBOOLTYPE_SLIDING] = false;
					m_nCounterKey--;

				case MOTION_JUMP: // ジャンプモーションだったら
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_LANDING: // 着地モーションだったら
					m_bAction[PLAYERBOOLTYPE_LANDING] = false;
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_LIGHTPUNCH:
					m_bAction[PLAYERBOOLTYPE_LIGHTPUNCH] = false;
					m_bAttackPunch[0] = false;

					m_nCounterKey--;
					break;
				case MOTION_MIDDLEPUNCH:
					m_bAction[PLAYERBOOLTYPE_MIDDLEPUNCH] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_SHORYUKEN:
					m_bAction[PLAYERBOOLTYPE_SHORYUKEN] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_HEAVYPUNCH:
					m_bAction[PLAYERBOOLTYPE_HEAVYPUNCH] = false;
					m_nCounterKey--;
					break;
				case MOTION_LIGHTKICK:
					m_bAction[PLAYERBOOLTYPE_LIGHTKICK] = false;
					m_bAttackKick[0] = false;
					m_nCounterKey--;
					break;
				case MOTION_MIDDLEKICK:
					m_bAction[PLAYERBOOLTYPE_MIDDLEKICK] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_SPINKICK:
					m_bAction[PLAYERBOOLTYPE_SPINKICK] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_HEAVYKICK:
					m_bAction[PLAYERBOOLTYPE_HEAVYKICK] = false;
					m_nCounterKey--;
					break;
				case MOTION_SMALLDAMAGE: // ダメージモーションだったら
					for (int nCntmotion = 0; nCntmotion <= 10; nCntmotion++)
					{
						m_bAction[nCntmotion] = false;
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = false;// ダメージ中じゃない状態にする
					m_bAction[PLAYERBOOLTYPE_MEDIUMDAMAGE] = false;// ダメージ中じゃない状態にする
					m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = false;// ダメージ中じゃない状態にする

					m_nCounterKey--; // キーフレームを戻す
					break;
				case MOTION_LARGEDAMAGE:
					for (int nCntmotion = 0; nCntmotion <= 10; nCntmotion++)
					{
						m_bAction[nCntmotion] = false;
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// 攻撃中だったら
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = false;// ダメージ中じゃない状態にする
					m_bAction[PLAYERBOOLTYPE_MEDIUMDAMAGE] = false;// ダメージ中じゃない状態にする
					m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = false;// ダメージ中じゃない状態にする

					m_nCounterKey--; // キーフレームを戻す
					break;
				case MOTION_SKILL:
					m_bAction[PLAYERBOOLTYPE_SKILL] = false;// ダメージ中じゃない状態にする
					CScene::SetStop(false);
					m_nCounterKey--; // キーフレームを戻す
					break;
				case MOTION_DEATH://死亡した場合
					break;
				}
				if (m_nCounterKey <= 0)
				{
					m_nCounterKey = 0;
				}
				m_state = PLAYERSTATE_NORMAL;
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
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{// パーツ数分ループ
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

	//----------------------------------------------
	// モーションの動作
	//----------------------------------------------
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{// パーツ数分ループ
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

	m_nCounterFrame++;		// 現在キーフレームのフレーム数を加算
	m_nCounterAllFrame++;	// モーションのフレーム数を加算

}

//=============================================================================
// 関数名：プレイヤーの状態変化
// 関数の概要：プレイヤーの状態変化
//=============================================================================
void CPlayer::PlayerState(void)
{
	switch (m_state)
	{
	case PLAYERSTATE_NORMAL:	//通常状態
		break;

	case PLAYERSTATE_DAMAGE:	//ダメージ状態
		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_StateCnt = 120;
			m_state = PLAYERSTATE_INVISIBLE;
		}
		break;

	case PLAYERSTATE_INVISIBLE:	//透明状態
		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_bUseDraw = true;
			m_state = PLAYERSTATE_NORMAL;
		}
		break;
	case PLAYERSTATE_APPEAR:	//出現時
		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_state = PLAYERSTATE_NORMAL;
		}
		break;
		case PLAYERSTATE_DEATH:	//出現時
			CGame::GAMESTATE pGameState;
			pGameState = CGame::GetGameState();
			if (pGameState == CGame::GAMESTATE_NORMAL)
			{
				CGame::SetGameState(CGame::GAMESTATE_CLEAR);
			}
			break;
	}

}

//=============================================================================
// 関数名：プレイヤーの移動量の代入と慣性移動
// 関数の概要：--
//=============================================================================
void CPlayer::PlayerSetMove_Interia(void)
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
// 関数名：プレイヤーのモーションステートの変更
// 関数の概要：プレイヤーの状態遷移
//=============================================================================
void CPlayer::PlayerMotionState(bool *bAction)
{
	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//カメラの角度取得
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//モデルのマトリックス情報
	D3DMATRIX ModelMtx;


	if (bAction[PLAYERBOOLTYPE_DEATH] == true)
	{// 死んだなら
		m_Motionstate = MOTION_DEATH; // 死亡モーションに設定
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_DEATH)
		{//1フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_DEATH)
		{//1フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SLIDING] == true)
	{// スライディング
		m_Motionstate = MOTION_SLIDING; // 攻撃モーションに設定
		if (m_nCounterAllFrame >= 18 && m_nCounterAllFrame <= 22 &&m_Motionstate == MOTION_SLIDING)
		{
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//パーティクルの生成
				CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
		}
		if (m_nCounterAllFrame == 20 && m_Motionstate == MOTION_SLIDING)
		{//20フレームかつスライディングモーションのとき
			m_state = PLAYERSTATE_STEEL;	//硬直状態に
			//移動量の加算
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 7.5f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 7.5f);
		}
	}
	else if(bAction[PLAYERBOOLTYPE_LIGHTPUNCH] == true)
	{
		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_LIGHTPUNCH;	//弱パンチモーションに設定
		if (m_nCounterAllFrame == 7 && m_Motionstate == MOTION_LIGHTPUNCH)
		{//7フレームかつ弱パンチモーションのとき
			//右手のマトリックス情報の取得
			ModelMtx = m_apModel[9]->GetMatrix();
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_0);	//プレイヤーの攻撃１
			//プレイヤーの移動
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.7f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.7f);
		}
		if (m_nCounterAllFrame == 13 &&m_Motionstate == MOTION_LIGHTPUNCH)
		{//7フレームかつ弱パンチモーションのとき
		 //判定の発生
			ModelMtx = m_apModel[9]->GetMatrix();
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 , ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_S, CCollision::COLLISIONTYPE_PLAYER_LIGHTPUNCH, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_MIDDLEPUNCH] == true)
	{//プレイヤーが中パンチをしたなら
		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_MIDDLEPUNCH;	//中パンチモーションに設定
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_MIDDLEPUNCH)
		{//8フレームかつ中パンチモーションのとき
			 //右手のマトリックス情報の取得
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//プレイヤーの攻撃１
			//プレイヤーの移動
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.2f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.2f);
		}
		if (m_nCounterAllFrame == 13 && m_Motionstate == MOTION_MIDDLEPUNCH)
		{//13フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[6]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 , ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_S, CCollision::COLLISIONTYPE_PLAYER_MEDIUMPUNCH, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SHORYUKEN] == true)
	{//プレイヤーが昇竜拳をしたなら
		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_SHORYUKEN;	//昇竜拳モーションに設定
		if (m_nCounterAllFrame == 5 && m_Motionstate == MOTION_SHORYUKEN)
		{//13フレームかつ昇竜拳のとき
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_2);	//プレイヤーの攻撃１

		}
		if (m_nCounterAllFrame == 13 && m_Motionstate == MOTION_SHORYUKEN)
		{//13フレームかつ昇竜拳のとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 9.0f);
			m_move.y += 22.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 9.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE2);//炎音

		}
		if (m_nCounterAllFrame > 19 && m_nCounterAllFrame < 28 && m_Motionstate == MOTION_SHORYUKEN)
		{//19フレーム以上28フレーム以下のとき、昇竜拳のとき
			ModelMtx = m_apModel[6]->GetMatrix();
			CreateParticleFlame_Shoryu(&ModelMtx);
		}
		if (m_nCounterAllFrame == 12  && m_Motionstate == MOTION_SHORYUKEN)
		{//12フレームのとき、昇竜拳のとき
			ModelMtx = m_apModel[6]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 , ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_M, CCollision::COLLISIONTYPE_PLAYER_SHORYUKEN, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_HEAVYPUNCH] == true)
	{//プレイヤーが強攻撃をしたなら
		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_HEAVYPUNCH;	//強パンチモーションに設定
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//6フレームかつ強パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//プレイヤーの攻撃１
		}
		if (m_nCounterAllFrame == 9 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//9フレームかつ強パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);//炎音
		}
		if (m_nCounterAllFrame == 13 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//13フレームかつ強パンチモーションのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 3.0f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 3.0f);

		}
		if (m_nCounterAllFrame > 15 && m_nCounterAllFrame <= 21 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//19フレーム以上のとき強パンチのとき
			ModelMtx = m_apModel[6]->GetMatrix();
			CreateParticleFlame_SpinKick(&ModelMtx);
		}
		if (m_nCounterAllFrame == 21 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//31フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[6]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 35.0f, CCollision::COLLISIONTYPE_PLAYER_HEAVYPUNCH, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_LIGHTKICK] == true)
	{//弱キック
		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_LIGHTKICK;	//弱キックモーションに設定
		if (m_nCounterAllFrame == 5 && m_Motionstate == MOTION_LIGHTKICK)
		{//7フレームかつ中パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_0);	//プレイヤーの攻撃１
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.2f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.2f);
		}
		if (m_nCounterAllFrame == 15 && m_Motionstate == MOTION_LIGHTKICK)
		{//12フレームのとき、昇竜拳のとき
			ModelMtx = m_apModel[12]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_M, CCollision::COLLISIONTYPE_PLAYER_LIGHTKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_MIDDLEKICK] == true)
	{//中キック
		m_state = PLAYERSTATE_STEEL;		//硬直状態に
		m_Motionstate = MOTION_MIDDLEKICK;	//中キックモーションに設定
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_MIDDLEKICK)
		{//7フレームかつ中パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//プレイヤーの攻撃１
			m_move.x += (sinf(m_rot.y - D3DX_PI)) * 1.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI)) * 1.0f;
		}
		if (m_nCounterAllFrame == 16 && m_Motionstate == MOTION_MIDDLEKICK)
		{//12フレームのとき、昇竜拳のとき
			ModelMtx = m_apModel[11]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 35.0f, CCollision::COLLISIONTYPE_PLAYER_MEDIUMKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SPINKICK] == true)
	{//回転キック

		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_SPINKICK;	//回転キックモーションに設定
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_SPINKICK)
		{//8フレームかつ中パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_2);	//プレイヤーの攻撃１
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE2);//炎音
		}

		if (m_nCounterAllFrame == 14 && m_Motionstate == MOTION_SPINKICK)
		{//14フレームかつ中パンチモーションのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 10.0f);
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 10.0f);
		}
		if (m_nCounterAllFrame > 15 && m_nCounterAllFrame < 27 && m_Motionstate == MOTION_SPINKICK)
		{//12フレーム以上のとき回転蹴りのとき
			ModelMtx = m_apModel[15]->GetMatrix();
			CreateParticleFlame_SpinKick(&ModelMtx);
		}
		if (m_nCounterAllFrame == 31 && m_Motionstate == MOTION_SPINKICK)
		{//31フレームかつ回転蹴りモーションのとき
			ModelMtx = m_apModel[15]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 40.0f, CCollision::COLLISIONTYPE_PLAYER_SPINKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_HEAVYKICK] == true)
	{//回転キック
		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_HEAVYKICK;	//強キックモーションに設定
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_HEAVYKICK)
		{//6フレームかつ中パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//プレイヤーの攻撃１
		}
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_HEAVYKICK)
		{//12フレームかつ中パンチモーションのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.4f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.4f);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE3);//炎音

		}
		if (m_nCounterAllFrame > 13 && m_nCounterAllFrame < 19 && m_Motionstate == MOTION_HEAVYKICK)
		{//8フレーム以上のとき強キックのとき
			ModelMtx = m_apModel[12]->GetMatrix();
			CreateParticleFlame_Shoryu(&ModelMtx);
		}
		if (m_nCounterAllFrame == 20 && m_Motionstate == MOTION_HEAVYKICK)
		{//20フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[12]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 35.0f, CCollision::COLLISIONTYPE_PLAYER_HEAVYKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SKILL] == true)
	{// ダメージ着地
		m_Motionstate = MOTION_SKILL; // 超必殺技モーションに設定
		ModelMtx = m_apModel[1]->GetMatrix();
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_SKILL)
		{//6フレーム以上のときスキルのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FLASH);	//光演出
		}
		if (m_nCounterAllFrame == 15 && m_Motionstate == MOTION_SKILL)
		{//15フレーム以上のときスキルのとき
			CEffect::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43 - 15.5f), 150.0f, 6, CEffect::EFFECTTYPE_SMALL, CEffect::EFFECTMODE_SHRINKING);
		}
		if (m_nCounterAllFrame == 60 && m_Motionstate == MOTION_SKILL)
		{//60フレーム以上のときスキルのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE3);//炎音

		}
		if (m_nCounterAllFrame >= 60 && m_nCounterAllFrame <= 222 && m_Motionstate == MOTION_SKILL)
		{//60フレーム以上のときスキルのとき
			ModelMtx = m_apModel[6]->GetMatrix();
			CreateParticleFlame_Skill(&ModelMtx);
		}
		if (m_nCounterAllFrame == 100 && m_Motionstate == MOTION_SKILL)
		{//60フレーム以上のときスキルのとき
			CScene::ChangeStop();

		}
		if (m_nCounterAllFrame == 170 && m_Motionstate == MOTION_SKILL)
		{//60フレーム以上のときスキルのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 6.0f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 6.0f);
		}
		if (m_nCounterAllFrame == 170 && m_Motionstate == MOTION_SKILL)
		{//6フレームかつ強パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_2);	//プレイヤーの攻撃１
		}
		if (m_nCounterAllFrame == 170 && m_Motionstate == MOTION_SKILL)
		{//9フレームかつ強パンチモーションのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);//炎音
		}
		if (m_nCounterAllFrame == 180 && m_Motionstate == MOTION_SKILL)
		{//20フレームかつ中パンチモーションのとき
			ModelMtx = m_apModel[6]->GetMatrix();
			//判定の発生
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 45.0f, CCollision::COLLISIONTYPE_PLAYER_SKILL, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_LANDING] == true)
	{// ダメージ着地
		m_state = PLAYERSTATE_STEEL;
		m_Motionstate = MOTION_LANDING; // 着地モーションに設定
	}
	else if (bAction[PLAYERBOOLTYPE_JUMP] == true)
	{// プレイヤーがジャンプ中なら
		m_Motionstate = MOTION_JUMP; // ジャンプモーションに設定
	}
	else if (bAction[PLAYERBOOLTYPE_SMALLDAMAGE])
	{
		m_Motionstate = MOTION_SMALLDAMAGE;
		if (m_nCounterAllFrame == 4 && m_Motionstate == MOTION_SMALLDAMAGE)
		{//2フレーム以上のとき小ダメージのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERHIT_2);//ダメージ１

		}

		m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.05f) * -1.0f;
		m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.05f) * -1.0f;
		m_state = PLAYERSTATE_STEEL;	//硬直状態に

	}
	else if (bAction[PLAYERBOOLTYPE_LARGEDAMAGE])
	{
		m_state = PLAYERSTATE_STEEL;	//硬直状態に
		m_Motionstate = MOTION_LARGEDAMAGE;
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_LARGEDAMAGE)
		{//1フレームのとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERHIT_1);//ダメージ１
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_LARGEDAMAGE)
		{//1フレームのとき
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;

			CreateParticleDust();

		}

	}
	else if (m_move.x > PLAYER_MOVE_DEFAULT ||
			 m_move.x < -PLAYER_MOVE_DEFAULT ||
			 m_move.z >  PLAYER_MOVE_DEFAULT ||
			 m_move.z < -PLAYER_MOVE_DEFAULT)
	{// プレイヤーが動いているとき
		m_Motionstate = MOTION_MOVE; // 移動状態にする

	}
	else
	{// プレイヤーが動いてないとき
		m_Motionstate = MOTION_NEUTRAL; // ニュートラル状態にする
	}
}


//=============================================================================
// 関数名：角度の修正
// 関数の概要：大きければ、引く、小さければ、足す
//=============================================================================
void CPlayer::PlayerRotFixes(D3DXVECTOR3 rot)
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
void CPlayer::PlayerRotFixes(float rot)
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
// 関数名：プレイヤー情報の読み込み
// 関数の概要：プレイヤー情報の読み込みとパーツの読み込み
//=============================================================================
void CPlayer::LoadPlayerText(void)
{
	FILE *pFile = fopen(MODEL_FILENAME, "r");		// 読み込むテキストファイルのアドレス

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
// 関数名：プレイヤーの初期化
// 関数の概要：--
//=============================================================================
void CPlayer::InitPlayer(void)
{
	//プレイヤー情報
	m_fRadius = PLAYER_COLLISION_RADIUS; // プレイヤーの当たり判定の半径
	m_StateCnt = 20;
	m_state = PLAYERSTATE_APPEAR;
	m_fDestAngle = -D3DX_PI * 0.5f;
	m_fLife = PLAYER_LIFE;
	m_fHeight = 120.0f;
	m_fSkill = 0.0f;
	m_fDamage = 0.0f;

	//軌跡の生成
	//m_apModel[12]->CreateOrbit();
}
//=============================================================================
// 関数名：プレイヤーのモーションの初期化
// 関数の概要：--
//=============================================================================
void CPlayer::InitMotion(void)
{
	m_nNumKey = 2;
	m_pKeyInfo = m_aKeyInfo;
	m_nKey = 0;
	m_nCountMotion = 0;
	m_Motionstate = MOTION_NEUTRAL;
}
//=============================================================================
// 関数名：ゲージ類の生成
// 関数の概要：ゲージ類を生成する
//=============================================================================
void CPlayer::CreateGauge()
{
	m_apUi[0] = CUiGauge::Create(D3DXVECTOR3(105.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f), 510.0f, 25.0f, PLAYER_LIFE_FLAME, PLAYER_LIFE_FLAME, CUiGauge::GAUGETYPE_UNDER);		//体力ゲージの下地生成
	m_apUi[1] = CUiGauge::Create(D3DXVECTOR3(138.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 450.0f, 15.0f, PLAYER_LIFE, PLAYER_LIFE, CUiGauge::GAUGETYPE_RED);		//仮体力ゲージ生成
	m_apUi[2] = CUiGauge::Create(D3DXVECTOR3(138.0f, 40.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 450.0f, 15.0f, PLAYER_LIFE, PLAYER_LIFE, CUiGauge::GAUGETYPE_GREEN);	//本体力ゲージ生成
	m_apUi[3] = CUiGauge::Create(D3DXVECTOR3(55.0f, 90.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 455.0f, 25.0f, PLAYER_LIFE, PLAYER_LIFE, CUiGauge::GAUGETYPE_UNDER);		//スキルゲージの下地生成
	m_apUi[4] = CUiGauge::Create(D3DXVECTOR3(85.0f, 90.0f, 0.0f), D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f), 400.0f, 15.0f, 0.0f, PLAYER_LIFE, CUiGauge::GAUGETYPE_SKILL);		//スキルゲージの生成
	m_apUi[5] = CUiGauge::Create(D3DXVECTOR3(0.0f, 55.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 120.0f, 60.0f, 50.0f, 50.0f, CUiGauge::GAUGETYPE_FACE_PLAYER);		//スキルゲージの生成

}
//=============================================================================
// 関数名：パーツの初期設定
// 関数の概要：パーツの初期設定を行う
//=============================================================================
void CPlayer::DefaultParts(void)
{
	for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
	{//総フレーム数の計算
		m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
	}

	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{// パーツ数分ループ
	 // モデルの位置をニュートラルモーションに初期化
		m_PartsDefaultpos[nCntParts] = m_Partspos[nCntParts] + m_aKeyInfo[m_Motionstate].aKey[0].pos[nCntParts];
		m_PartsDefaultrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[0].rot[nCntParts];
	}

}

//=============================================================================
// 関数名：攻撃との当たり判定
// 関数の概要：--
//=============================================================================
void CPlayer::CollisionAttack(void)
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

	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	//カメラの角度取得
	D3DXVECTOR3 CameraRot = pCamera->GetRot();


	pCollision = (CCollision*)GetSceneTop(COLLISION_PRIORITY);
	if (m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_STEEL)
	{
		while (pCollision != NULL)//NULLになるまで
		{
			CCollision *pCollisionNext;
			pCollisionNext = (CCollision*)pCollision->GetSceneNext(COLLISION_PRIORITY);
			if (pCollision->GetObjType() == CScene::OBJTYPE_COLLISION_ENEMY)
			{
				if (pCollision->CollisionAttack(&m_pos, &m_rot, m_fHeight) == true)
				{
					//m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

					int nTypeHitType = pCollision->GetCollisionType();
					switch (nTypeHitType)
					{
					case HITTYPE_LIGHTATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
							//ライフの減少
						m_fLife -= LIGHT_ATACK;
						//スキルの増加
						m_fSkill += 10.0f;
						m_fDamage += LIGHT_ATACK;
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//弱攻撃

						m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_HEAVYATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
						m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

						//	m_rot.y = pCollision->GetAngle();
							//ライフの減少
						m_fLife -= LARGE_ATACK;
						//スキルの増加
						m_fSkill += 15.0f;
						m_fDamage += LARGE_ATACK;
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//強攻撃


						m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_LIGHTATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
						//ライフの減少
						m_fLife -= 20.0f;
						//スキルの増加
						m_fSkill += 15.0f;
						m_fDamage += 20.0f;
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//弱攻撃

						m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_HEAVYATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
						m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

						//ライフの減少
						m_fLife -= 30.0f;
						//スキルの増加
						m_fSkill += 35.0f;
						m_fDamage += 30.0f;
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//強攻撃


						m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_SKILLATTACK_S:
						//m_rot.y = pCollision->GetAngle();

						//ライフの減少
						m_fLife -= 5.0f;
						//スキルの増加
						m_fSkill += 10.0f;
						m_fDamage += 5.0f;
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//弱攻撃

						m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_SKILLATTACK_L:
						//	m_rot.y = pCollision->GetAngle();
						m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

							//ライフの減少
						m_fLife -= 150.0f;
						//スキルの増加
						m_fSkill += 100.0f;
						m_fDamage += 150.0f;
						//弱攻撃音
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//強攻撃


						m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
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
// 関数名：オブジェクトとの当たり判定
// 関数の概要：プレイヤーとオブジェクトとの当たり判定
//=============================================================================
void CPlayer::CollisionObjectPlayer(void)
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
				m_bAction[PLAYERBOOLTYPE_JUMP] = false;
				m_bJump = false;
			}
		}
		pObjectTop = pObjectNext;
	}

}
//=============================================================================
// 関数名：壁の当たり判定
// 関数の概要：プレイヤーと壁との当たり判定
//=============================================================================
void CPlayer::CollisionWallPlayer(void)
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
// 関数名：プレイヤーとの当たり判定
// 関数の概要：半径外へ戻す
//=============================================================================
bool CPlayer::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius)
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

				pPos->z = m_pos.z - m_fRadius - fRadius;	// オブジェクトの位置を戻す

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
// 関数名：シリンダーの当たり判定
// 関数の概要：プレイヤーとシリンダーとの当たり判定
//=============================================================================
void CPlayer::CollisionCylinderPlayer(void)
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
// 関数名：敵との当たり判定
// 関数の概要：プレイヤーと敵との当たり判定
//=============================================================================
void CPlayer::CollisionEnemy(void)
{
	//=========================================================================
	//敵のポインタ(スピード型)
	//=========================================================================
	CEnemy *pEnemy;

	pEnemy = (CEnemy*)GetSceneTop(ENEMY_PRIORITY);
	while (pEnemy != NULL)//NULLになるまで
	{
		CEnemy *pEnemyNext;
		pEnemyNext = (CEnemy*)pEnemy->GetSceneNext(ENEMY_PRIORITY);
		if (pEnemy->GetObjType() == CScene::OBJTYPE_ENEMY)
		{
			pEnemy->CollisionEnemy(&m_pos, &m_posold, &m_move, &m_rot, 25.0f);

		}
		pEnemy = pEnemyNext;
	}


	//=========================================================================
	//敵のポインタ(パワー型)
	//=========================================================================
	CEnemyPower *pEnemyPower;

	pEnemyPower = (CEnemyPower*)GetSceneTop(ENEMY_PRIORITY);
	while (pEnemyPower != NULL)//NULLになるまで
	{
		CEnemyPower *pEnemyPowerNext;
		pEnemyPowerNext = (CEnemyPower*)pEnemyPower->GetSceneNext(ENEMY_PRIORITY);
		if (pEnemyPower->GetObjType() == CScene::OBJTYPE_ENEMY_POW)
		{
			pEnemyPower->CollisionEnemy(&m_pos, &m_posold, &m_move, &m_rot, 20.0f);

		}
		pEnemyPower = pEnemyPowerNext;
	}


	//=========================================================================
	//敵のポインタ(トリッキー型)
	//=========================================================================
	CEnemyTrick *pEnemyTrick;

	pEnemyTrick = (CEnemyTrick*)GetSceneTop(ENEMY_PRIORITY);
	while (pEnemyTrick != NULL)//NULLになるまで
	{
		CEnemyTrick *pEnemyTrickNext;
		pEnemyTrickNext = (CEnemyTrick*)pEnemyTrick->GetSceneNext(ENEMY_PRIORITY);
		if (pEnemyTrick->GetObjType() == CScene::OBJTYPE_ENEMY_TRC)
		{
			pEnemyTrick->CollisionEnemy(&m_pos, &m_posold, &m_move, &m_rot, 20.0f);

		}
		pEnemyTrick = pEnemyTrickNext;
	}



	//=========================================================================
	//ボスのポインタ
	//=========================================================================
	CBoss *pBoss;

	pBoss = (CBoss*)GetSceneTop(ENEMY_PRIORITY);
	while (pBoss != NULL)//NULLになるまで
	{
		CBoss *pBossNext;
		pBossNext = (CBoss*)pBoss->GetSceneNext(ENEMY_PRIORITY);
		if (pBoss->GetObjType() == CScene::OBJTYPE_BOSS)
		{
			pBoss->CollisionBoss(&m_pos, &m_posold, &m_move, &m_rot, 20.0f);

		}
		pBoss = pBossNext;
	}

}

//=============================================================================
// 関数名：プレイヤー関係のデバッグログ
// 関数の概要：--
//=============================================================================
void CPlayer::PlayerDebugLog(void)
{
	CDebugProc::Print("\n");
	CDebugProc::Print("プレイヤー位置(XYZ): %.1f, %.1f, %.1f：\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("プレイヤー角度(XYZ): %.1f, %.1f, %.1f：\n", m_rot.x, m_rot.y, m_rot.z);
	CDebugProc::Print("プレイヤー移動量(XYZ): %.1f, %.1f, %.1f：\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("プレイヤーLIFE: %.1f\n", m_fLife);
	CDebugProc::Print("プレイヤーSKILL: %.1f\n", m_fSkill);

	CDebugProc::Print("状態変化のカウンタ：%d\n", m_StateCnt);

	switch (m_state)
	{
	case PLAYERSTATE_NORMAL://通常状態のとき
		CDebugProc::Print("STATE：[0]NORMAL\n");
		break;
	case PLAYERSTATE_STEEL://硬直状態のとき
		CDebugProc::Print("STATE：[1]STEEL\n");
		break;
	case PLAYERSTATE_DAMAGE://ダメージ状態のとき
		CDebugProc::Print("STATE：[2]DAMAGE\n");
		break;
	case PLAYERSTATE_INVISIBLE://無敵のとき
		CDebugProc::Print("STATE：[3]INVISIBLE\n");
		break;
	case PLAYERSTATE_APPEAR://出現時のとき
		CDebugProc::Print("STATE：[4]PLAYERSTATE_APPEAR\n");
		break;
	case PLAYERSTATE_DEATH://出現時のとき
		CDebugProc::Print("STATE：[5]PLAYERSTATE_DEATH\n");
		break;
	}

	switch (m_Motionstate)
	{
	case MOTION_NEUTRAL://ニュートラルのとき
		CDebugProc::Print("MOTION：[0]NEUTRAL\n");
		break;
	case MOTION_MOVE://移動のとき
		CDebugProc::Print("MOTION：[1]MOVE\n");
		break;
	case MOTION_SLIDING://スライディングのとき
		CDebugProc::Print("MOTION：[2]SLIDING\n");
		break;
	case MOTION_JUMP://ジャンプのとき
		CDebugProc::Print("MOTION：[3]JUMP\n");
		break;
	case MOTION_LANDING://着地のとき
		CDebugProc::Print("MOTION：[4]LANDING\n");
		break;
	case MOTION_LIGHTPUNCH://スライディングのとき
		CDebugProc::Print("MOTION：[5]MOTION_LIGHTPUNCH\n");
		break;
	case MOTION_MIDDLEPUNCH://スライディングのとき
		CDebugProc::Print("MOTION：[5]MOTION_MIDDLEPUNCH\n");
		break;
	}
	switch (m_MotionstateOld)
	{
	case MOTION_NEUTRAL://ニュートラルのとき
		CDebugProc::Print("MOTION：[0]NEUTRAL\n");
		break;
	case MOTION_MOVE://移動のとき
		CDebugProc::Print("MOTION：[1]MOVE\n");
		break;
	case MOTION_SLIDING://スライディングのとき
		CDebugProc::Print("MOTION：[2]SLIDING\n");
		break;
	case MOTION_JUMP://ジャンプのとき
		CDebugProc::Print("MOTION：[3]JUMP\n");
		break;
	case MOTION_LANDING://着地のとき
		CDebugProc::Print("MOTION：[4]LANDING\n");
		break;
	case MOTION_LIGHTPUNCH://スライディングのとき
		CDebugProc::Print("MOTION：[5]MOTION_LIGHTPUNCH\n");
		break;
	case MOTION_MIDDLEPUNCH://スライディングのとき
		CDebugProc::Print("MOTION：[5]MOTION_MIDDLEPUNCH\n");
		break;
	}
	CDebugProc::Print("KEY：%d / %d\n", m_nCounterKey, m_aKeyInfo[m_Motionstate].nNumKey);
	CDebugProc::Print("FRAME：%d / %d (%d / %d)\n", m_nCounterFrame, m_nFrame, m_nCounterAllFrame, m_nAllFrame);
	CDebugProc::Print("AllFrame：%d\n", m_nCounterAllFrame);

	CDebugProc::Print("m_bJump：%d\n", m_bJump);



}

//=============================================================================
// 関数名：プレイヤーのライフのチェック
// 関数の概要：プレイヤーのライフの確認
//=============================================================================
void CPlayer::PlayerLifeCheck(void)
{
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();


	if (m_fLife <= 0.0f)
	{
		if (mode == CManager::MODE_GAME)
		{
			m_state = PLAYERSTATE_DEATH;
			for (int nCntDamage = 5; nCntDamage < 9; nCntDamage++)
			{
				m_bAction[nCntDamage] = false;
			}
			m_bAction[PLAYERBOOLTYPE_DEATH] = true;
			m_fLife = 0.0f;
		}
	}

	if (m_fDamage >= PLAYER_LIFE)
	{
		m_fDamage = PLAYER_LIFE;
	}

	m_apUi[1]->AddValueSlip(m_fLife);	//仮体力ゲージ
	m_apUi[2]->AddValue(m_fLife);	//本体力ゲージ

}

//=============================================================================
// 関数名：プレイヤーのスキルのチェック
// 関数の概要：プレイヤーのスキルの確認
//=============================================================================
void CPlayer::PlayerSkillCheck(void)
{

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();



	if (m_fSkill >= PLAYER_LIFE)
	{
		m_fSkill = PLAYER_LIFE;

	}
	if (m_fSkill <= 0.0f)
	{
		m_fSkill = 0.0f;
	}

	m_apUi[4]->AddValueSlip(m_fSkill);

}

//=============================================================================
// 関数名：炎パーティクルの生成
// 関数の概要：炎パーティクルの生成
//=============================================================================
void CPlayer::CreateParticleFlame_SpinKick(D3DMATRIX *mtx)
{
	D3DXMATRIX ModelMtx = *mtx;

	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE_A, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_FIRE_LIFE);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_FIRE_LIFE);
		}
	}
}

//=============================================================================
// 関数名：炎パーティクルの生成
// 関数の概要：炎パーティクルの生成
//=============================================================================
void CPlayer::CreateParticleFlame_Shoryu(D3DMATRIX *mtx)
{
	D3DXMATRIX ModelMtx = *mtx;

	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE_A, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_FIRE_LIFE);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_FIRE_LIFE);
		}
	}
}

void CPlayer::CreateParticleFlame_Skill(D3DMATRIX * mtx)
{
	D3DXMATRIX ModelMtx = *mtx;

	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE_A, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//パーティクルの生成
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
	}
}

//=============================================================================
// 関数名：土煙パーティクルの生成
// 関数の概要：土煙パーティクルの生成
//=============================================================================
void CPlayer::CreateParticleDust(void)
{
	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_RAND, 20);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_RAND, 20);
		}
	}
}

void CPlayer::CreateParticleDust_Shoryu(void)
{
	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, 9);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, 12);
		}
	}
}


//=============================================================================
// 関数名：プレイヤーのスキルのチェック
// 関数の概要：プレイヤーのスキルの確認
//=============================================================================
float CPlayer::GetDamage(void)
{
	return m_fDamage;
}

//=============================================================================
// 関数名：位置情報の取得
// 関数の概要：プレイヤーの位置情報を返す
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 関数名：角度情報の取得
// 関数の概要：プレイヤーの角度情報を返す
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// 関数名：移動量情報の取得
// 関数の概要：プレイヤーの移動量を返す
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// 関数名：UIゲージの取得
// 関数の概要：UIゲージを返す
//=============================================================================
CUiGauge *CPlayer::GetUiGauge(int nIndex)
{
	return m_apUi[nIndex];
}


//=============================================================================
// 関数名：スキルゲージの取得
// 関数の概要：スキルゲージを返す
//=============================================================================
void CPlayer::SetSkill(float fSkill)
{
	m_fSkill += fSkill;
}


