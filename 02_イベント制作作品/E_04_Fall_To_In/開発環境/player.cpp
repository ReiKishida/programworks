//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// プレイヤー処理 [player.cpp]
// Author:あくつ卍太郎
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "debugProc.h"
#include "model.h"
#include "motion.h"
#include "PanelHole.h"
#include "meshWall.h"
#include "buttonMotion.h"
#include "sound.h"
#include "result.h"
#include "fade.h"
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MOVE_SPEED			(1.0f)					// 通常時の走る速度
#define ORDER_SPEED			(0.4f)					// 提供時の走る速度
#define COOK_SPEED			(0.2f)					// 提供時の歩く速度
#define MOVE_INERTIA		(0.1f)					//

#define RANGE			(D3DXVECTOR3(100.0f,10.0f,100.0f))		// 壁との当たり判定
#define WALL_COLLISION	(D3DXVECTOR3(40.0f,40.0f,40.0f))		// 動物が逃げ出す範囲
#define CHARA_COLLISION	(D3DXVECTOR3(3.0f,3.0f,3.0f))		// プレイヤー自身と当たり判定する際の動物の半径
#define KUSHI_COLLISION	(100.0f)
#define MAX_LOAD_LINE (2560)
#define MAX_KISHI_TAKE (2)
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 静的メンバ変数宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
CMotionData *CPlayer::m_apMotion[MOTION_DATA_TYPE_MAX] = {};			// モーションへのポインタ
char *CPlayer::m_apMotionInfo[MOTION_DATA_TYPE_MAX] =
{
	// アドレス
	"data//TEXT//motion_takeru.txt",			// サンプル00
	"data//TEXT//motion_zozo_red.txt",			// サンプル00
	"data//TEXT//motion_zozo_blue.txt",			// サンプル00
};
CPlayer::MOTION_TYPE CPlayer::m_buttonMotionType[POSE_BUTTON_MAX] = {};
CButtonMotion *CPlayer::m_buttonMotionDisp = NULL;
bool CPlayer::m_bBottonCommand[POSE_BUTTON_MAX] = {};	// ボタン用ブール
bool CPlayer::m_bControl = false;
int	CPlayer::m_nControlLevel = 0;						// 操作Level
int CPlayer::m_n1PlayerMotion = -1;
int CPlayer::m_nNumAll = 0;
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// コンストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CPlayer::CPlayer() : CMotion(CScene::PRIORITY_PLAYER)
{
	m_nNumAll++;

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// デストラクタ
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CPlayer::~CPlayer()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CPlayer::Init(void)
{

	// モーションをプレイヤーに設定する
	if (m_nId == 0)
	{
		m_nType = (MOTION_DATA_TYPE)(rand() % MOTION_DATA_TYPE_MAX);

		m_n1PlayerMotion = m_nType;

		CResult::Set1P(m_nType);
	}
	else
	{
		bool bLoop = true;
		while (bLoop)
		{
			m_nType = (MOTION_DATA_TYPE)(rand() % MOTION_DATA_TYPE_MAX);

			if (m_nType != m_n1PlayerMotion)
			{
				bLoop = false;
				CResult::Set2P(m_nType);
			}
		}
	}


	// モーション設定
	CMotion::BindMotion(m_apMotion[m_nType]);


	// モーション初期化
	CMotion::Init();
	// オブジェクトタイプを設定
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);
	// モーションのBOOLを初期化する
	for (int nCntBool = 0; nCntBool < BOOL_MAX; nCntBool++)
	{
		m_bBool[nCntBool] = false;
		m_bBoolOld[nCntBool] = false;
	}
	// ボタン用のBOOLを初期化する
	for (int nCntBool = 0; nCntBool < BOOL_MAX; nCntBool++)
	{
		m_bBotton[nCntBool] = false;
	}

	// 半径情報を設定
	m_fRadius = 20.0f;
	m_fHeifht = 45.0f;

	// 移動速度設定
	CMotion::SetSpeed(MOVE_SPEED);

	CMotion::SetPos(D3DXVECTOR3(-100.0f + m_nId * 200.0f, 12000.0f, 0.0f));
	//CMotion::SetRot(D3DXVECTOR3(-D3DX_PI / 2.0f, 0.0f, D3DX_PI / 2.0f));


	// IDを初期化

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Uninit(void)
{
	if (m_nId == 0)
	{


		CResult::Set1P(m_nType);
	}
	else
	{

				CResult::Set2P(m_nType);

	}

	// モーション終了処理
	CMotion::Uninit();
	m_nNumAll--;
}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Update(void)
{
	// モーション更新処理
	CMotion::Update();

	// モーションのBOOLの過去のモーションを設定
	for (int nCntBool = 0; nCntBool < BOOL_MAX; nCntBool++)
	{
		m_bBoolOld[nCntBool] = m_bBool[nCntBool];
	}

	CManager::MODE pManager = CManager::GetMode();

	if (pManager == CManager::MODE::MODE_TITLE)
	{// タイトルモードの時

	 // モーション情報取得
		CMotionMove *pMotion = CMotion::GetMotion();

		// モーション種類取得
		int nMotionType = CMotion::GetMotionType();
		int nMotionTypeOld = CMotion::GetMotionTypeOld();

		if (pMotion != NULL)
		{
			// モーション更新処理
			pMotion->MotionUpdate(m_apMotion[m_nType], nMotionType, nMotionTypeOld);
		}


		// モーション情報を取得
		CMotionData::Motion *pMotionData = m_apMotion[m_nType]->GetMotion(nMotionType);

		// フレーム数が達した場合終了する
		if (pMotion->GetKeyCnt() == pMotionData->nNumKey - 1
			&& pMotionData->bLoop == false
			&& pMotionData->aKey[pMotionData->nNumKey - 1].nFrame == pMotion->GetFrameCounter() + 1)
		{ // モーション終了時にブールをfalseにする
		  /*m_bBool[MOTION_TYPE_ATTACK] = false;
		  m_bBool[MOTION_TYPE_RAND] = false;*/
		}

		// フレームカウンター増加処理
		pMotion->FrameUpdate(m_apMotion[m_nType], nMotionType);

		D3DXVECTOR3 pos = CMotion::GetPos();
		D3DXVECTOR3 move = CMotion::GetMove();

		// 位置更新
		pos += move;

		CMotion::SetPos(pos);
		CMotion::SetMove(move);
	}
	else if (pManager == CManager::MODE::MODE_GAME)
	{// ゲームモードの時
	 // コントローラー
		Controller();

		// モーション
		Motion();

		D3DXVECTOR3 rot = CMotion::GetRot();

		rot.x = -D3DX_PI / 2.0f + (rand() % 50) / 1000.0f;
		rot.z = D3DX_PI + (rand() % 50) / 1000.0f;

		CMotion::SetRot(rot);

		CManager::GetInputGamePad()->SetVibration(100, 1, m_nId);

		// ボタン表示
		ButtonDisp(CMotion::GetPos().y);
	}
	else if (pManager == CManager::MODE::MODE_RESULT)
	{// タイトルモードの時
	 // モーション情報取得
		CMotionMove *pMotion = CMotion::GetMotion();

		// モーション種類取得
		int nMotionType = CMotion::GetMotionType();
		int nMotionTypeOld = CMotion::GetMotionTypeOld();

		if (pMotion != NULL)
		{
			// モーション更新処理
			pMotion->MotionUpdate(m_apMotion[m_nType], nMotionType, nMotionTypeOld);
		}


		// モーション情報を取得
		CMotionData::Motion *pMotionData = m_apMotion[m_nType]->GetMotion(nMotionType);

		// フレーム数が達した場合終了する
		if (pMotion->GetKeyCnt() == pMotionData->nNumKey - 1
			&& pMotionData->bLoop == false
			&& pMotionData->aKey[pMotionData->nNumKey - 1].nFrame == pMotion->GetFrameCounter() + 1)
		{ // モーション終了時にブールをfalseにする
		  /*m_bBool[MOTION_TYPE_ATTACK] = false;
		  m_bBool[MOTION_TYPE_RAND] = false;*/
		}

		// フレームカウンター増加処理
		pMotion->FrameUpdate(m_apMotion[m_nType], nMotionType);
	}

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Draw(void)
{
	// デバイス取得処理
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	D3DXVECTOR3 pos = CMotion::GetPos();
	D3DXVECTOR3 rot = CMotion::GetRot();
	D3DXMATRIX mtx = CMotion::GetMatrix();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);
	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	// パーツの描画
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (CMotion::GetModel(nCntModel) != NULL)
		{
			CMotion::GetModel(nCntModel)->Draw(mtx, CMotion::GetMotion()->GetPos(nCntModel), CMotion::GetMotion()->GetRot(nCntModel),*m_apMotion[m_nType]->GetModel(nCntModel));
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーション処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Motion(void)
{
	// モーションタイプ更新
	MotionTypeUpdate();

	// モーション情報取得
	CMotionMove *pMotion = CMotion::GetMotion();

	// モーション種類取得
	int nMotionType = CMotion::GetMotionType();
	int nMotionTypeOld = CMotion::GetMotionTypeOld();

	if (pMotion != NULL)
	{
		// モーション更新処理
		pMotion->MotionUpdate(m_apMotion[m_nType], nMotionType, nMotionTypeOld);
	}


	// モーション情報を取得
	CMotionData::Motion *pMotionData = m_apMotion[m_nType]->GetMotion(nMotionType);

	// フレーム数が達した場合終了する
	if (pMotion->GetKeyCnt() == pMotionData->nNumKey - 1
		&& pMotionData->bLoop == false
		&& pMotionData->aKey[pMotionData->nNumKey - 1].nFrame == pMotion->GetFrameCounter() + 1)
	{ // モーション終了時にブールをfalseにする
		/*m_bBool[MOTION_TYPE_ATTACK] = false;
		m_bBool[MOTION_TYPE_RAND] = false;*/
	}

	// フレームカウンター増加処理
	pMotion->FrameUpdate(m_apMotion[m_nType], nMotionType);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// プレイヤーモーションの種類の設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::MotionTypeUpdate(void)
{
	int nMotionType = 0;		// モーションタイプ
								// 移動量取得
	D3DXVECTOR3 move = CMotion::GetMove();

	if (m_bBool[BOOL_JUMP] == true)
	{ // ジャンプ状態か
		//nMotionType = MOTION_TYPE_RAND;
	}
	else if (m_bBottonCommand[POSE_BOTTAN_A] == true)
	{ // 全体指示Aがある場合
		nMotionType = m_buttonMotionType[POSE_BOTTAN_A];
	}
	else if (m_bBottonCommand[POSE_BOTTAN_B] == true)
	{ // 全体指示Bがある場合
		nMotionType = m_buttonMotionType[POSE_BOTTAN_B];
	}
	else if (m_bBottonCommand[POSE_BOTTAN_X] == true)
	{ // 全体指示Xがある場合
		nMotionType = m_buttonMotionType[POSE_BOTTAN_X];
	}
	else if (m_bBottonCommand[POSE_BOTTAN_Y] == true)
	{ // 全体指示Yがある場合
		nMotionType = m_buttonMotionType[POSE_BOTTAN_Y];
	}
	else if (m_bControl == false)
	{ // 全体指示は無いが操作不可の場合
		nMotionType = MOTION_TYPE_NEUTRAL;
	}
	else
	{
		if (m_bBotton[POSE_BOTTAN_A])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_A];
		}
		else if (m_bBotton[POSE_BOTTAN_B])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_B];
		}
		else if (m_bBotton[POSE_BOTTAN_X])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_X];
		}
		else if (m_bBotton[POSE_BOTTAN_Y])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_Y];
		}
		else
		{
			// ニュートラル動作なのか
			nMotionType = MOTION_TYPE_NEUTRAL;
		}
	}
	// モーションタイプ設定
	CMotion::SetMotionType(nMotionType);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 操作処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Controller(void)
{
	// カメラを取得
	CCamera *pCamera = CManager::GetCameraMode();
	//D3DXVECTOR3 CameraRot = pCamera->GetRot();
	D3DXVECTOR3 CameraRot = pCamera->GetRot(0);
	// キーボード取得
	CInputKeyboard *pInput = CManager::GetInputKeyboard();
	// コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();

	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();

	// 位置・移動量取得
	D3DXVECTOR3 pos = CMotion::GetPos();
	D3DXVECTOR3 move = CMotion::GetMove();
	D3DXVECTOR3 rot = CMotion::GetRot();
	float fSpeed = CMotion::GetSpeed();

	float fDiffAngle = 0.0f;		// 差分
	float fDestAngle = 0.0f;		// 目的の角度
	float fMoveAngle = 0.0f;		// 現在の角度

	// 現在の角度を代入(Y)
	fMoveAngle = rot.y;

	if (m_bControl == true)
	{
		if (CManager::GetInputKeyboard()->GetPress(DIK_A))
		{ // 左移動
			if (CManager::GetInputKeyboard()->GetPress(DIK_W))
			{ // 左上移動
				move.x += sinf(-(D3DX_PI / 4) * 1) * (fSpeed);
				move.z += cosf(-(D3DX_PI / 4) * 1) * (fSpeed);
			}
			else if (CManager::GetInputKeyboard()->GetPress(DIK_S))
			{ // 左下移動
				move.x += sinf(-(D3DX_PI / 4) * 3) * (fSpeed);
				move.z += cosf(-(D3DX_PI / 4) * 3) * (fSpeed);
			}
			else
			{ // 左移動
				move.x += sinf(-(D3DX_PI / 4) * 2) * (fSpeed);
				move.z += cosf(-(D3DX_PI / 4) * 2) * (fSpeed);
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_D))
		{ // 右移動
			if (CManager::GetInputKeyboard()->GetPress(DIK_W))
			{ // 左上移動
				move.x += sinf((D3DX_PI / 4) * 1) * (fSpeed);
				move.z += cosf((D3DX_PI / 4) * 1) * (fSpeed);
			}
			else if (CManager::GetInputKeyboard()->GetPress(DIK_S))
			{ // 左下移動
				move.x += sinf((D3DX_PI / 4) * 3) * (fSpeed);
				move.z += cosf((D3DX_PI / 4) * 3) * (fSpeed);
			}
			else
			{ // 左移動
				move.x += sinf((D3DX_PI / 4) * 2) * (fSpeed);
				move.z += cosf((D3DX_PI / 4) * 2) * (fSpeed);
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_W))
		{ // 前移動
			move.x += sinf(0.0f) * (fSpeed);
			move.z += cosf(0.0f) * (fSpeed);
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S))
		{ // 後移動
			move.x += sinf(D3DX_PI) * (fSpeed);
			move.z += cosf(D3DX_PI) * (fSpeed);
		}


		if (pGamePadStick[m_nId].aGamePad.lX < -GAMEPAD_DEADZONE || pGamePadStick[m_nId].aGamePad.lX > GAMEPAD_DEADZONE || pGamePadStick[m_nId].aGamePad.lY < -GAMEPAD_DEADZONE || pGamePadStick[m_nId].aGamePad.lY> GAMEPAD_DEADZONE)
		{// ゲームパッド
			float fGamePagStickAngle = atan2f(float(pGamePadStick[m_nId].aGamePad.lX), float(-pGamePadStick[m_nId].aGamePad.lY));
			move.x += sinf(fGamePagStickAngle) * (fSpeed);
			move.z += cosf(fGamePagStickAngle) * (fSpeed);
		}

		// ボタンを離していたらfalseにする
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_A) == false)
		{
			m_bBotton[POSE_BOTTAN_A] = false;
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_B) == false)
		{
			m_bBotton[POSE_BOTTAN_B] = false;
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_X) == false)
		{
			m_bBotton[POSE_BOTTAN_X] = false;
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_Y) == false)
		{
			m_bBotton[POSE_BOTTAN_Y] = false;
		}
		if (pGamePad->GetGamePadTrigger(m_nId, BUTTON_Y) || pGamePad->GetGamePadTrigger(m_nId, BUTTON_X) ||
			pGamePad->GetGamePadTrigger(m_nId, BUTTON_B) || pGamePad->GetGamePadTrigger(m_nId, BUTTON_A))
		{
			CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_POSE);
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_A))
		{ // A入力
			if (m_bBotton[POSE_BOTTAN_B] == false && m_bBotton[POSE_BOTTAN_X] == false && m_bBotton[POSE_BOTTAN_Y] == false)
			{ // A以外の入力が無い場合
				m_bBotton[POSE_BOTTAN_A] = true;
			}
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_B))
		{ // B入力
			if (m_bBotton[POSE_BOTTAN_A] == false && m_bBotton[POSE_BOTTAN_X] == false && m_bBotton[POSE_BOTTAN_Y] == false)
			{ // B以外の入力が無い場合
				m_bBotton[POSE_BOTTAN_B] = true;
			}
		}
		if (m_nControlLevel >= 1)
		{ // 操作Levelが1以上の場合
			if (pGamePad->GetGamePadPress(m_nId, BUTTON_X))
			{ // X入力
				if (m_bBotton[POSE_BOTTAN_A] == false && m_bBotton[POSE_BOTTAN_B] == false && m_bBotton[POSE_BOTTAN_Y] == false)
				{ // B以外の入力が無い場合
					m_bBotton[POSE_BOTTAN_X] = true;
				}
			}
		}
		if (m_nControlLevel >= 2)
		{ // 操作Levelが1以上の場合
			if (pGamePad->GetGamePadPress(m_nId, BUTTON_Y))
			{ // Y入力
				if (m_bBotton[POSE_BOTTAN_A] == false && m_bBotton[POSE_BOTTAN_B] == false && m_bBotton[POSE_BOTTAN_X] == false)
				{ // B以外の入力が無い場合
					m_bBotton[POSE_BOTTAN_Y] = true;
				}
			}
		}
	}
	// 位置更新
	pos += move;
	pos.y -= 5.0f;
	if (CGame::GetStage() == CGame::STAGE_EARTH || CGame::GetStage() == CGame::STAGE_HELL)
	{
		pos.y -= 3.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_O))
	{
		pos.y -= 10;
	}

	// 慣性
	move.x += float(-move.x) * MOVE_INERTIA;
	move.y += float(-move.y) * MOVE_INERTIA;
	move.z += float(-move.z) * MOVE_INERTIA;


	// 差分を計算
	fDiffAngle = (fDestAngle - fMoveAngle);

	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle += -D3DX_PI * 2;
	}
	else if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2;
	}

	// 現在の角度に差分の何割かを代入
	fMoveAngle += fDiffAngle * 0.5f;

	if (fMoveAngle > D3DX_PI)
	{
		fMoveAngle += -D3DX_PI * 2;
	}
	else if (fMoveAngle < -D3DX_PI)
	{
		fMoveAngle += D3DX_PI * 2;
	}

	rot.y = fMoveAngle;

	// プレイヤー同士の押しあい
	//PlayerOverLap();
	CScene *m_pSceneTarget = CScene::GetScene(PRIORITY_PLAYER);
	while (m_pSceneTarget != NULL)
	{
		CScene *pSceneNext = m_pSceneTarget->GetNext();
		if (m_pSceneTarget != NULL)
		{
			if (m_pSceneTarget->GetObjType() == OBJTYPE_PLAYER)
			{// オブジェクト
				CPlayer *pPlayer = ((CPlayer*)m_pSceneTarget);
				if (m_nId != pPlayer->GetId())
				{
					D3DXVECTOR3 targetPos = pPlayer->GetPos();

					/*pos.z += 40.0f;
					targetPos.z += 40.0f;*/

					// 敵と当たるものの距離
					float fPosX = (pos.x - targetPos.x);
					float fPosY = (pos.y - targetPos.y);
					float fPosZ = (pos.z - targetPos.z);
					float fRadiusA = fPosX*fPosX + fPosY*fPosY + fPosZ*fPosZ;
					float fRadiusB = 0;

					fRadiusB = (m_fRadius + m_fRadius)* (pPlayer->GetRadius() + pPlayer->GetRadius());

					if (fRadiusB > fRadiusA&&
						-fRadiusB < fRadiusA)
					{// 人と人のあたり判定
						float fAngle2 = atan2f(pos.x - targetPos.x, pos.z - targetPos.z);

						pos.x = targetPos.x + sinf(fAngle2) * (m_fRadius + pPlayer->GetRadius());
						pos.z = targetPos.z + cosf(fAngle2) * (m_fRadius + pPlayer->GetRadius());

						//pos.z -= 40.0f;

						move.x *= 1.2f;
						move.z *= 1.2f;

						pPlayer->SetMove(move);

						move *= -1.0f;

						// 位置設定
						//CMotion::SetPos(pos);
					}
				}
			}
		}
		m_pSceneTarget = pSceneNext;
	}

	//
	int motion = GetMotionType();
	int nOk = 0;
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// 優先順位の最大数分回す
		CScene *m_pSceneTarget = CScene::GetScene(nCntP);
		while (m_pSceneTarget != NULL)
		{
			CScene *pSceneNext = m_pSceneTarget->GetNext();
			if (m_pSceneTarget != NULL)
			{
				if (m_pSceneTarget->GetObjType() == OBJTYPE_PANEL)
				{// オブジェクト

					if (nOk != 1)
					{
						CPanelHole *pObject = ((CPanelHole*)m_pSceneTarget);
						int nOkClone = pObject->Collision(pos + D3DXVECTOR3(0.0f, 0.0f, 40), GetPosOld(), m_fRadius, motion);
						if (nOkClone != 0)
						{
							nOk = nOkClone;
						}
						if (nOk == 1)
						{
							CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_SUCCES);

						}
					}
				}
				else if (m_pSceneTarget->GetObjType() == OBJTYPE_MESHWALL)
				{
					CMeshWall *pObject = ((CMeshWall*)m_pSceneTarget);
					if (pObject->Collision(&pos, GetPosOld(), m_fRadius, m_fHeifht))
					{

					}
				}
			}
			m_pSceneTarget = pSceneNext;
		}
	}

	if (nOk == 2)
	{
		if (m_nNumAll <= 1)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CResult::SetWin(CGame::m_WinId);
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
	}

	if (nOk == 2)
	{
		CGame::m_WinId = (m_nId + 1) % 2;
		CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_FAILED);

		Uninit();
		if (m_nNumAll <= 0)
		{
			CResult::WinReset();

			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);

			}
		}
		return;
	}

	// 各種反映
	CMotion::SetPos(pos);
	CMotion::SetMove(move);
	CMotion::SetRot(rot);
	CMotion::SetSpeed(MOVE_SPEED);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 操作状況取得
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
bool CPlayer::GetController(CONTROLLER Controller)
{
	bool bButton = false;
	CInputKeyboard *pInput = CManager::GetInputKeyboard();

	switch (Controller)
	{
	case CONTROLLER_LEFT_MOVE:	// 左移動
		if (pInput->GetPress(DIK_A) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_RIGHT_MOVE:	// 右移動
		if (pInput->GetPress(DIK_D) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_UP_MOVE:	// 上移動
		if (pInput->GetPress(DIK_W) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_LEFT_UP_MOVE:	// 左上移動
		if (pInput->GetPress(DIK_W) == true && pInput->GetPress(DIK_A) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_RIGHT_UP_MOVE: // 右上移動
		if (pInput->GetPress(DIK_W) == true && pInput->GetPress(DIK_D) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_LEFT_DOWN_MOVE:	// 左下移動
		if (pInput->GetPress(DIK_S) == true && pInput->GetPress(DIK_A) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_RIGHT_DOWN_MOVE: // 右下移動
		if (pInput->GetPress(DIK_S) == true && pInput->GetPress(DIK_D) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_DOWN_MOVE:	// 下移動
		if (pInput->GetPress(DIK_S) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_ATTACK:	// 攻撃
		if (pInput->GetPress(DIK_RETURN) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_JUMP:	// ジャンプ
		if (pInput->GetPress(DIK_SPACE) == true)
		{
			bButton = true;
		}
		break;
	}
	return bButton;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ポーズのランダム設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::RandomPose(void)
{
	// ダブりCheck
	int nCheckMotion[POSE_BUTTON_MAX];
	for (int nCnt = 0; nCnt < POSE_BUTTON_MAX; nCnt++)
	{
		nCheckMotion[nCnt] = -1;
	}

	int nMotion = -1;

	for (int nCnt = 0; nCnt < POSE_BUTTON_MAX; nCnt++)
	{
		bool bCheck = true;

		while (bCheck)
		{
			bCheck = false;

			nMotion = rand() % POSE_BUTTON_MAX + 1;
			for (int nCntCheck = 0; nCntCheck < POSE_BUTTON_MAX; nCntCheck++)
			{
				if (nMotion == nCheckMotion[nCntCheck])
				{// 今までのモーションと同じ場合
					bCheck = true;
				}
			}
		}

		// かぶりちぇっくのループを抜けた場合代入
		m_buttonMotionType[nCnt] = (MOTION_TYPE)nMotion;
		nCheckMotion[nCnt] = nMotion;
	}

	int nData = 0;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ボタン表示
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::ButtonDisp(const float fHeight)
{
	float fStartPos = 12000.0f;
	float fEarthStartPos = 2000.0f;
	float fHellStartPos = -7000.0f;

	CDebugProc::print("プレイヤー高さ: %f\n",fHeight);

	if (m_buttonMotionDisp != NULL)
	{
		D3DXVECTOR3 pos = m_buttonMotionDisp->GetPos();
		pos.y = fHeight - 200.0f;
		m_buttonMotionDisp->SetPos(pos);
		for (int nCnt = 0; nCnt < POSE_BUTTON_MAX; nCnt++)
		{
			m_bBottonCommand[nCnt] = false;
		}
	}

	if (fHeight < fHellStartPos - 800.0f)
	{//Yを消す
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
		// 操作可能にする
		m_bControl = true;
	}
	else if (fHeight < fHellStartPos - 500.0f)
	{ // Yモーションの指示
		m_bBottonCommand[POSE_BOTTAN_Y] = true;
	}
	else if (fHeight < fHellStartPos)
	{ // Y生成
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_Y);
		}
		// 操作不可にする
		m_bControl = false;
		// 操作Levelを1に
		m_nControlLevel = 2;
	}
	else if (fHeight < fEarthStartPos - 800.0f)
	{//Xを消す
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
		// 操作可能にする
		m_bControl = true;
	}
	else if (fHeight < fEarthStartPos - 500.0f)
	{ // Xモーションの指示
		m_bBottonCommand[POSE_BOTTAN_X] = true;
	}
	else if (fHeight < fEarthStartPos)
	{ // X生成
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_X);
		}
		// 操作不可にする
		m_bControl = false;

		// 操作Levelを1に
		m_nControlLevel = 1;
	}
	else if (fHeight < fStartPos - 1800.0f)
	{//Bを消す
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
		// 操作可能にする
		m_bControl = true;
	}
	else if (fHeight < fStartPos - 1500.0f)
	{ // Bモーションの指示
		m_bBottonCommand[POSE_BOTTAN_B] = true;
	}
	else if (fHeight < fStartPos - 1000.0f)
	{ // B生成
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_B);
		}
		// 操作不可にする
		m_bControl = false;
	}
	else if (fHeight < fStartPos - 800.0f)
	{//Aを消す
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
	}
	else if (fHeight < fStartPos - 500.0f)
	{ // Aモーションの指示
		m_bBottonCommand[POSE_BOTTAN_A] = true;
	}
	else if (fHeight < fStartPos)
	{ // A生成
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_A);
		}
		// 操作不可にする
		m_bControl = false;

		// 操作Levelを0に
		m_nControlLevel = 0;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// プレイヤーの押しあい
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::PlayerOverLap(void)
{
	CScene *m_pSceneTarget = CScene::GetScene(PRIORITY_PLAYER);
	while (m_pSceneTarget != NULL)
	{
		CScene *pSceneNext = m_pSceneTarget->GetNext();
		if (m_pSceneTarget != NULL)
		{
			if (m_pSceneTarget->GetObjType() == OBJTYPE_PLAYER)
			{// オブジェクト
				CPlayer *pPlayer = ((CPlayer*)m_pSceneTarget);
				if (m_nId != pPlayer->GetId())
				{
					D3DXVECTOR3 pos = CMotion::GetPos();
					D3DXVECTOR3 targetPos = pPlayer->GetPos();

					pos.z += 40.0f;
					targetPos.z += 40.0f;

					// 敵と当たるものの距離
					float fPosX = (pos.x - targetPos.x);
					float fPosY = (pos.y - targetPos.y);
					float fPosZ = (pos.z - targetPos.z);
					float fRadiusA = fPosX*fPosX + fPosY*fPosY + fPosZ*fPosZ;
					float fRadiusB = 0;

					fRadiusB = (m_fRadius + m_fRadius)* (pPlayer->GetRadius() + pPlayer->GetRadius());

					if (fRadiusB > fRadiusA&&
						-fRadiusB < fRadiusA)
					{// 人と人のあたり判定
						float fAngle2 = atan2f(pos.x - targetPos.x, pos.z - targetPos.z);

						pos.x = targetPos.x + sinf(fAngle2) * (m_fRadius + pPlayer->GetRadius());
						pos.z = targetPos.z + cosf(fAngle2) * (m_fRadius + pPlayer->GetRadius());

						pos.z -= 40.0f;

						// 位置設定
						CMotion::SetPos(pos);
					}
				}
			}
		}
		m_pSceneTarget = pSceneNext;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションtxtデータの読み込み
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CPlayer::Load(void)
{
	for (int nCntType = 0; nCntType < MOTION_DATA_TYPE_MAX; nCntType++)
	{
		m_apMotion[nCntType] = CMotionData::Create();

		if (m_apMotion[nCntType] != NULL)
		{
			m_apMotion[nCntType]->Load(m_apMotionInfo[nCntType]);
		}
	}
	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// モーションtxtデータの開放処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::UnLoad(void)
{
	for (int nCntType = 0; nCntType < MOTION_DATA_TYPE_MAX; nCntType++)
	{
		m_apMotion[nCntType]->UnLoad();
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 生成処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CPlayer *CPlayer::Create(int nId)
{
	CPlayer *pPlayer = NULL;

	pPlayer = new CPlayer;	// CPlayer型のインスタンス生成
	if (pPlayer != NULL)
	{
		pPlayer->m_nId = nId;
		// 初期化処理
		pPlayer->Init();
	}
	else
	{ // メモリ確保失敗
		MessageBox(NULL, "プレイヤーのメモリ確保に失敗しました", "エラー", (MB_OK));
	}
	return pPlayer;
}


void CPlayer::SetMotion(int nId)
{
	m_nType = (MOTION_DATA_TYPE)nId;
	BindMotion(m_apMotion[nId]);
}