//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "inputx.h"
#include "shadow.h"
#include "camera.h"
#include "model.h"
#include "game.h"
#include "Wall1.h"
#include "txt.h"
#include "motion.h"
#include "char_select.h"
#include "magic.h"
#include "sencer.h"
#include "magicsquare.h"
#include "sound.h"
#include "brazen.h"
#include "SencerCol.h"
#include "SencerAtk.h"
#include "enemy.h"
#include "SencerLock.h"
#include "SencerNextStage.h"
#include "life.h"
#include "Wall1.h"
#include "Wall2.h"
#include "WallBoss.h"
#include "message.h"
#include "boss.h"
#include "erosion.h"
#include "particle.h"
#include "enemy_arrow.h"
#include "enemy_axe.h"
#include "enemy_axe2.h"
#include "enemy0.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy4.h"
#include "enemy5.h"
#include "enemy6.h"
#include "enemy7.h"
#include "enemy8.h"

//=============================================================================
// マクロ定義
//============================================================================
#define MOVE_SPEED			(1.2f)						// 移動速度
//#define MOVE_SPEED			(3.5f)						// 移動速度(debug用)
#define STAGE_LIMIT_MIN_Z (-260.0f)

//=============================================================================
// グローバル変数
//=============================================================================
PLAYERSTATE		g_PlayerState = PLAYERSTATE_NEUTRAL;		// プレイヤーの状態
PLAYERSTATE		g_PlayerStateNow = PLAYERSTATE_NEUTRAL;
PLAYER			g_Player;
JoyState		g_JoyStatePlayer;							// ジョイパッドの状態

int g_nCntFootStep;
//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	Tex *pText = GetTex();							// ファイルデータの取得
	STAGEMODE g_Stage = GetStageMode();				//現在のステージ番号を取得

	// 位置・向きの初期設定
	if (g_Stage == STAGEMODE_STAGE1)
	{
		g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
		g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.rot = D3DXVECTOR3(0.0f, 1.0f, 0.0f) * D3DX_PI;
	}
	if (g_Stage == STAGEMODE_STAGE2)
	{
		g_Player.pos = D3DXVECTOR3(350.0f, 0.0f, -250.0f);
		g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 00.0f);
	}


	CHARACTER_TYPE pCharaType = GetCharaType();		// 選択キャラクター情報の取得

	//g_Player.rotDest = 1.0f * D3DX_PI;
	g_Player.rotDest = 0.0f;

	g_Player.fAngle = 0.0f;
	g_Player.vtxMinModel = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);				// 最小値の場合は多めに数値をいれる（比較のため)
	g_Player.vtxMaxModel = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);			// 最大値の場合は少なめに数値をいれる（比較のため)
	g_Player.MAX_PARTS = pText->MAX_PARTS;
	g_Player.status = PLAYERSTATUS_NORMAL;
	g_Player.nIdxShadow = SetShadow(g_Player.pos, g_Player.rot);
	g_Player.nIdxSencer = SetSencerCollision(g_Player.pos, g_Player.rot, SENCERCOLTYPE_PLAYER);
	g_Player.nIdxSencerLock = SetSencerLock(g_Player.pos, g_Player.rot);
	g_Player.nCntMotionFlame = 0;

	


	g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	g_nCntFootStep = 0;
	// 読み込むファイル名
	for (int nCntFile = 0; nCntFile < g_Player.MAX_PARTS; nCntFile++)
	{// テキストデータから読み込むファイル名を取得
		g_Player.aModel[nCntFile].FileName[0] = pText->aModelTex[nCntFile].FileName[0];
	}

	// パーツの親子関係
	for (int nCntIdx = 0; nCntIdx < g_Player.MAX_PARTS; nCntIdx++)
	{// テキストデータから親子関係を取得
		g_Player.aModel[nCntIdx].nIdxModelParent = pText->aModelTex[nCntIdx].Index;
	}

	// パーツの位置
	for (int nCntPos = 0; nCntPos < g_Player.MAX_PARTS; nCntPos++)
	{// テキストデータから位置情報を取得
		g_Player.aModel[nCntPos].pos = D3DXVECTOR3(pText->aModelTex[nCntPos].PosX, pText->aModelTex[nCntPos].PosY, pText->aModelTex[nCntPos].PosZ);
	}

	// パーツの回転	
	for (int nCntRot = 0; nCntRot < g_Player.MAX_PARTS; nCntRot++)
	{// テキストデータから回転情報を取得
		g_Player.aModel[nCntRot].rot = D3DXVECTOR3(pText->aModelTex[nCntRot].RotX, pText->aModelTex[nCntRot].RotY, pText->aModelTex[nCntRot].RotZ);
	}

	// ファイルの読み込み
	for (int nCntFile = 0; nCntFile < g_Player.MAX_PARTS; nCntFile++)
	{// パーツ数分のファイルの読み込み
		D3DXLoadMeshFromX(&pText->aModelTex[nCntFile].FileName[0], D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&g_Player.aModel[nCntFile].pBuffMatPlayerModel, NULL, &g_Player.aModel[nCntFile].nNumMatPlayerModel, &g_Player.aModel[nCntFile].pMeshPlayerModel);
	}


	//当たり判定の初期化
	switch (pCharaType)
	{
	case CHARACTER_TYPE_SOLDIER:
			g_Player.Collision.bUse = false;
			g_Player.Collision.nCntMotion = 0;
			g_Player.Collision.StartFlame = 15;
			g_Player.Collision.EndFlame = 0;
			g_Player.Collision.fRadius = 30.0f;
			g_Player.Collision.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
			g_Player.Collision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player.Collision.nIdxMotion = PLAYERSTATE_ATTACK;
			g_Player.Collision.nIdxParent = -1;
			g_Player.fMoveSpeed = 1.5f;
			g_Player.fSaveSpeed = 1.5f;
			SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f));
			SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f));
			SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f));
			SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f));
			SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f));
			SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43));

			break;
	case CHARACTER_TYPE_WARRIOR:
				g_Player.Collision.bUse = false;
				g_Player.Collision.nCntMotion = 0;
				g_Player.Collision.StartFlame = 30;
				g_Player.Collision.fRadius = 40.0f;
				g_Player.Collision.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
				g_Player.Collision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_Player.Collision.nIdxParent = 0;
				g_Player.fMoveSpeed = 1.1f;
				g_Player.fSaveSpeed = 1.1f;

				SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f));
				SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f));
				SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f));
				SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f));
				SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43));
				SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 + 10.0f));
				SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 + 20.0f));
				break;
	case CHARACTER_TYPE_SORCERER:
					g_Player.Collision.bUse = false;
					g_Player.Collision.nCntMotion = 0;
					g_Player.Collision.StartFlame = 7;
					g_Player.Collision.fRadius = 30.0f;
					g_Player.Collision.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
					g_Player.Collision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Player.Collision.nIdxParent = 0;
					g_Player.fMoveSpeed = 1.7f;
					g_Player.fSaveSpeed = 1.7f;


					SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f));
					SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f));
					SetSencerAtk(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43));

					break;
	}

	SetPlayerState(PLAYERSTATE_NEUTRAL);
	
	int nNumVtx = g_Player.aModel[0].pMeshPlayerModel->GetNumVertices();						// 頂点数を取得
	DWORD sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[0].pMeshPlayerModel->GetFVF());		// 頂点フォーマットのサイズを取得
	BYTE *pVtxBuff;																				// 頂点バッファへのポインタ

	g_Player.aModel[0].pMeshPlayerModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);	// 頂点バッファをロック

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		if (vtx.x < g_Player.vtxMinModel.x)
		{ // Xの最小値
			g_Player.vtxMinModel.x = vtx.x;
		}
		if (vtx.x > g_Player.vtxMaxModel.x)
		{ // Xの最大値
			g_Player.vtxMaxModel.x = vtx.x;
		}

		if (vtx.y < g_Player.vtxMinModel.y)
		{ // Yの最小値
			g_Player.vtxMinModel.y = vtx.y;
		}
		if (vtx.y > g_Player.vtxMaxModel.y)
		{ // Yの最大値
			g_Player.vtxMaxModel.y = vtx.y;
		}

		if (vtx.z < g_Player.vtxMinModel.z)
		{ // Zの最小値
			g_Player.vtxMinModel.z = vtx.z;
		}
		if (vtx.z > g_Player.vtxMaxModel.z)
		{ // Zの最大値
			g_Player.vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
	}
	g_PlayerStateNow = PLAYERSTATE_NEUTRAL;
	g_PlayerState = PLAYERSTATE_NEUTRAL;
	g_Player.nCntCharge = 0;
	g_Player.aModel[0].pMeshPlayerModel->UnlockVertexBuffer();	// 頂点バッファのアンロック
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// メッシュの破棄
	if (g_Player.aModel[0].pMeshPlayerModel != NULL)
	{
		g_Player.aModel[0].pMeshPlayerModel->Release();
		g_Player.aModel[0].pMeshPlayerModel = NULL;
	}
	// 頂点バッファの破棄
	if (g_Player.aModel[0].pBuffMatPlayerModel != NULL)
	{
		g_Player.aModel[0].pBuffMatPlayerModel->Release();
		g_Player.aModel[0].pBuffMatPlayerModel = NULL;
	}
	SetPlayerState(PLAYERSTATE_NEUTRAL);
	g_PlayerStateNow = PLAYERSTATE_NEUTRAL;
	g_PlayerState = PLAYERSTATE_NEUTRAL;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//コントローラー関係
	JoyState pJoystate = GetJoystate();
	XINPUT_STATE state;
	XInputGetState(0, &state);

	XINPUT_KEYSTROKE keystroke;
	XInputGetKeystroke(0, 0, &keystroke);

	STAGEMODE g_Stage = GetStageMode();				//現在のステージ番号を取得
	CHARACTER_TYPE pCharaType = GetCharaType();		// 選択キャラクター情報の取得
	CAMERA *pCamera = GetCamera();					//カメラの所得
	Motion *pMotion;
	pMotion = GetMotion();					//モーションデータの所得
	Tex *g_aTex = GetTex();							//ファイルデータの取得
	BOSS *pBoss;
	pBoss = GetBoss();

	int pCntErosion;
	pCntErosion = GetnCntErosion();
	g_Player.posOld = g_Player.pos;				//プレイヤーの旧位置を更新

	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	ENEMY_0 *pEnemy0;
	pEnemy0 = GetEnemy_0();

	ENEMY_1 *pEnemy1;
	pEnemy1 = GetEnemy_1();


	ENEMY_2 *pEnemy2;
	pEnemy2 = GetEnemy_2();


	ENEMY_3 *pEnemy3;
	pEnemy3 = GetEnemy_3();

	ENEMY_4 *pEnemy4;
	pEnemy4 = GetEnemy_4();


	ENEMY_5 *pEnemy5;
	pEnemy5 = GetEnemy_5();


	ENEMY_6 *pEnemy6;
	pEnemy6 = GetEnemy_6();


	ENEMYARROW *pEnemyArrow;
	pEnemyArrow = GetEnemyArrow();


	ENEMYAXE *pEnemyAxe;
	pEnemyAxe = GetEnemyAxe();

	ENEMYAXE2 *pEnemyAxe2;
	pEnemyAxe2 = GetEnemyAxe2();

	switch (g_Player.status)
	{
	case PLAYERSTATUS_NORMAL:

		break;

	case PLAYERSTATUS_DAMAGE:
		g_Player.nCntCoolDown--;
		if (g_Player.nCntCoolDown >= 0)
		{
			g_Player.status = PLAYERSTATUS_NORMAL;
		}

		break;

	}



	// キー入力の設定
	if (g_PlayerStateNow == PLAYERSTATE_NEUTRAL || g_PlayerStateNow == PLAYERSTATE_MOVE)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && pJoystate.bConnectionJoypad == true))
		{	// SPACEキーで攻撃モーション
			if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{//連打防止
				pMotion->nCntFlame = 0;
				SetPlayerState(PLAYERSTATE_ATTACK);

				if (pCharaType == CHARACTER_TYPE_SORCERER)
				{
					IncreaseErosion(5);

					SetMagic(g_Player.pos + D3DXVECTOR3(sinf(g_Player.rot.y + 20.0f), 55.0f, cosf(10.0f)), g_Player.rot, D3DXVECTOR3(sinf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 19.0f, 0.0f, cosf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 17.0f), 60, 0);
					SetMagic(g_Player.pos + D3DXVECTOR3(sinf(g_Player.rot.y + 20.0f), 55.0f, cosf(10.0f)), g_Player.rot, D3DXVECTOR3(sinf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 17.0f, 0.0f, cosf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 17.0f), 60, 0);
					SetMagic(g_Player.pos + D3DXVECTOR3(sinf(g_Player.rot.y + 20.0f), 55.0f, cosf(10.0f)), g_Player.rot, D3DXVECTOR3(sinf(g_Player.rot.y + (0.97f * D3DX_PI)) * 13.0f, 0.7f, cosf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 17.0f), 60, 0);

					PlaySound(SOUND_LABEL_SE_FIRE);

				}
			}
			g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
		if (GetKeyboardPress(DIK_RETURN) == true || state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && pJoystate.bConnectionJoypad == true)
		{	// SPACEキーで攻撃モーション
			if (pCharaType == CHARACTER_TYPE_SOLDIER || pCharaType == CHARACTER_TYPE_SORCERER)
			{
				if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
				{//	トリガー
					SetPlayerState(PLAYERSTATE_ORIGINAL);
					g_Player.move.x += 0.0f;
					g_Player.move.z += 0.0f;
					pMotion->nCntFlame = 0;
				}
				g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
			}
		}



		if (CollisionSencer(&g_Player.pos) == true)
		{//魔法陣のセンサーに当たった時
			CollisionSencerReturnCnt(&g_Player.pos);
			if (GetKeyboardTrigger(DIK_L) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
			{
				if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
				{//	トリガー
					SetPlayerState(PLAYERSTATE_MIASMA);
				}
				g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;

			}

		}
		// それぞれの条件で動く向きを変える（加速）
		if (GetKeyboardPress(DIK_D) == true || (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
		{
			if (GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
			{	// 右下移動
				g_Player.rotDest = (D3DX_PI * -0.25f + pCamera->rot.y) - g_Player.rot.y;
				g_Player.move.x += sinf(D3DX_PI * 0.75f + pCamera->rot.y) * g_Player.fMoveSpeed;
				g_Player.move.z += cosf(D3DX_PI * 0.75f + pCamera->rot.y) * g_Player.fMoveSpeed;

			}
			else if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
			{	// 右上移動
				g_Player.rotDest = (D3DX_PI * -0.75f + pCamera->rot.y) - g_Player.rot.y;
				g_Player.move.x += sinf(D3DX_PI * 0.25f + pCamera->rot.y) * g_Player.fMoveSpeed;
				g_Player.move.z += cosf(D3DX_PI * 0.25f + pCamera->rot.y) * g_Player.fMoveSpeed;

			}
			else
			{	// 右移動
				g_Player.rotDest = (D3DX_PI * -0.5f + pCamera->rot.y) - g_Player.rot.y;
				g_Player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fMoveSpeed;
				g_Player.move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fMoveSpeed;

			}

		}
		else if (GetKeyboardPress(DIK_A) == true || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
		{
			if (GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
			{	// 左下移動
				g_Player.rotDest = (D3DX_PI * 0.25f + pCamera->rot.y) - g_Player.rot.y;
				g_Player.move.x -= sinf(D3DX_PI * 0.25f + pCamera->rot.y) * g_Player.fMoveSpeed;
				g_Player.move.z -= cosf(D3DX_PI * 0.25f + pCamera->rot.y) * g_Player.fMoveSpeed;
			}
			else if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
			{	// 左上移動
				g_Player.rotDest = (D3DX_PI * 0.75f + pCamera->rot.y) - g_Player.rot.y;
				g_Player.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * g_Player.fMoveSpeed;
				g_Player.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * g_Player.fMoveSpeed;
			}
			else
			{	// 左移動
				g_Player.rotDest = (D3DX_PI * 0.5f + pCamera->rot.y) - g_Player.rot.y;
				g_Player.move.x -= sinf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fMoveSpeed;
				g_Player.move.z -= cosf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fMoveSpeed;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
		{	// 上に移動
			g_Player.rotDest = (D3DX_PI * 1.0f + pCamera->rot.y) - g_Player.rot.y;
			g_Player.move.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fMoveSpeed;
			g_Player.move.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fMoveSpeed;
		}
		else if (GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
		{	// 下に移動
			g_Player.rotDest = (D3DX_PI * 0.0f + pCamera->rot.y) - g_Player.rot.y;
			g_Player.move.x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fMoveSpeed;
			g_Player.move.z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fMoveSpeed;
		}

		if (keystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN && keystroke.VirtualKey == VK_PAD_RTRIGGER && pJoystate.bConnectionJoypad == true)
		{
			g_Player.nCntCharge++;
			IncreaseErosion(1);
			for (int nCntParticle = 0; nCntParticle < g_Player.nCntCharge; nCntParticle++)
			{
				SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 30.0f, g_Player.pos.z), 30, D3DXCOLOR(0.9f, 0.0f, 0.8f, 0.8f), PARTICLETYPE_EXPLOSION);
			}
			g_Player.fMoveSpeed = 0.0f;
			g_Player.fMoveSpeed = 0.0f;

		}
		if (keystroke.Flags & XINPUT_KEYSTROKE_KEYUP && keystroke.VirtualKey == VK_PAD_RTRIGGER && pJoystate.bConnectionJoypad == true)
		{
			pMotion->nCntFlame = 0;
			SetPlayerState(PLAYERSTATE_ATTACK2);
			g_Player.fMoveSpeed = g_Player.fSaveSpeed;
			g_Player.fMoveSpeed = g_Player.fSaveSpeed;
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				SetParticle(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43), 30, D3DXCOLOR(1.0f, 0.0f, 0.8f, 0.6f), PARTICLETYPE_EXPLOSION);
			}

			if (pCharaType == CHARACTER_TYPE_SORCERER)
			{
				IncreaseErosion(15);

				PlaySound(SOUND_LABEL_SE_FIRE);

				Set_L_Magic(g_Player.pos + D3DXVECTOR3(sinf(g_Player.rot.y + 20.0f), 55.0f, cosf(10.0f)), g_Player.rot, D3DXVECTOR3(sinf(g_Player.rot.y + (-1.0f  * D3DX_PI)) * 16.0f, 0.0f, cosf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 17.0f), 60, 1);
				Set_L_Magic(g_Player.pos + D3DXVECTOR3(sinf(g_Player.rot.y + 20.0f), 55.0f, cosf(10.0f)), g_Player.rot, D3DXVECTOR3(sinf(g_Player.rot.y + (-0.97f * D3DX_PI)) * 17.0f, 0.0f, cosf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 17.0f), 60, 1);
				Set_L_Magic(g_Player.pos + D3DXVECTOR3(sinf(g_Player.rot.y + 20.0f), 55.0f, cosf(10.0f)), g_Player.rot, D3DXVECTOR3(sinf(g_Player.rot.y + (0.97f * D3DX_PI)) * 15.0f, 0.7f, cosf(g_Player.rot.y + (-1.0f * D3DX_PI)) * 17.0f), 60, 1);

			}
		}
		//コントローラーの状態更新
		if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_PUSH)
		{
			g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NONE;
		}
		else if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NONE)
		{
			g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
		}
	}

	//=================================================--
	//通常攻撃
	//====================================================
	else if (g_PlayerStateNow == PLAYERSTATE_ATTACK)
	{
		if (pCharaType == CHARACTER_TYPE_SOLDIER)
		{
			if (pMotion->nCntFlame % g_Player.Collision.StartFlame == 0)
			{
				PlaySound(SOUND_LABEL_SE_SLASH);

			}
		}
		else if (pCharaType == CHARACTER_TYPE_WARRIOR)
		{
			if (pMotion->nCntFlame % g_Player.Collision.StartFlame == 0)
			{
				PlaySound(SOUND_LABEL_SE_SLASH2);

			}
		}

		if (pMotion->nCntFlame > g_Player.Collision.StartFlame)
		{
			if (pCharaType == CHARACTER_TYPE_SOLDIER || pCharaType == CHARACTER_TYPE_WARRIOR)
			{
				//エネミー当たり判定
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定0
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy0++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
#endif
				}


				//エネミー当たり判定1
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy1++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定2
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy2++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定3
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy3++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定4
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy4++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定5
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy5++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定6
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy6++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定(斧)
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyAxe++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
#endif
				}



				//エネミー当たり判定(斧2)
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyAxe2++)
				{
#if 1
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定(弓)
				for (int nCntEnemyArrow = 0; nCntEnemyArrow < MAX_ENEMY_ARROW; nCntEnemyArrow++, pEnemyArrow++)
				{
#if 1
					CollisionAttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					CollisionAttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					CollisionAttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
#endif
				}
				//ボスとの当たり判定
#if 1
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
#endif

			}
		}
	}




	//=================================================--
	//瘴撃
	//====================================================
	else if (g_PlayerStateNow == PLAYERSTATE_ATTACK2)
	{
		if (pCharaType == CHARACTER_TYPE_SOLDIER)
		{
			if (pMotion->nCntFlame % g_Player.Collision.StartFlame == 0)
			{
				PlaySound(SOUND_LABEL_SE_SLASH);

			}
		}
		else if (pCharaType == CHARACTER_TYPE_WARRIOR)
		{
			if (pMotion->nCntFlame % g_Player.Collision.StartFlame == 0)
			{
				PlaySound(SOUND_LABEL_SE_SLASH2);

			}
		}

		if (pMotion->nCntFlame > g_Player.Collision.StartFlame)
		{
			if (pCharaType == CHARACTER_TYPE_SOLDIER || pCharaType == CHARACTER_TYPE_WARRIOR)
			{
				//エネミー当たり判定(剣)
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
#if 1
					//エネミー当たり判定
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);



#endif
				}



				//エネミー当たり判定0
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy0++)
				{
#if 1
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
#endif
				}


				//エネミー当たり判定1
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy1++)
				{
#if 1
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定2
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy2++)
				{
#if 1
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定3
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy3++)
				{
#if 1
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定4
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy4++)
				{
#if 1
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定5
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy5++)
				{
#if 1
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定6
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy6++)
				{
#if 1
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
#endif
				}


				//エネミー当たり判定(斧)
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyAxe++)
				{
#if 1

					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);



#endif
				}


				//エネミー当たり判定(斧2)
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyAxe2++)
				{
#if 1
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					Collision_L_AttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定(弓)
				for (int nCntEnemyArrow = 0; nCntEnemyArrow < MAX_ENEMY_ARROW; nCntEnemyArrow++, pEnemyArrow++)

				{
#if 1
					Collision_L_AttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					Collision_L_AttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					Collision_L_AttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					Collision_L_AttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					Collision_L_AttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					Collision_L_AttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
#endif
				}

				//ボスとの当たり判定
#if 1
				Collision_L_AttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				Collision_L_AttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				Collision_L_AttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				Collision_L_AttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
#endif
			}
		}
	}
	else if (g_PlayerStateNow == PLAYERSTATE_ORIGINAL)
	{
		if (pCharaType == CHARACTER_TYPE_SORCERER)
		{

			if (pMotion->nCntFlame / g_Player.Collision.StartFlame == 0)
			{
				PlaySound(SOUND_LABEL_SE_SLASH);

			}

			if (pMotion->nCntFlame / g_Player.Collision.StartFlame == 0)
			{

				//エネミー当たり判定
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy->nIndexSencer
						, nCntEnemy);



#endif
				}

				//エネミー当たり判定0
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy0++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy0->nIndexSencer
						, nCntEnemy);
#endif
				}


				//エネミー当たり判定1
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy1++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy1->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定2
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy2++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy2->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定3
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy3++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy3->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定4
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy4++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy4->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定5
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy5++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy5->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定6
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy6++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemy6->nIndexSencer
						, nCntEnemy);
#endif
				}

				//エネミー当たり判定(斧)
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyAxe++)
				{
#if 1
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);
					CollisionAttackPlayer(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe->nIndexSencer
						, nCntEnemy);



#endif
				}


				//エネミー当たり判定(斧2)
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyAxe2++)
				{
#if 1
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
					CollisionAttackAxe(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyAxe2->nIndexSencer
						, nCntEnemy);
#endif
				}


				//エネミー当たり判定(弓)
				for (int nCntEnemyArrow = 0; nCntEnemyArrow < MAX_ENEMY_ARROW; nCntEnemyArrow++, pEnemyArrow++)
				{
#if 1
					CollisionAttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					CollisionAttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
					CollisionAttackPvA(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 10.0f)
						, g_Player.Collision.fRadius
						, pEnemyArrow->nIndexSencer
						, nCntEnemyArrow);
#endif
				}

				//ボスとの当たり判定
#if 1
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 50.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 40.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 30.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
				CollisionAttackPvB(D3DXVECTOR3(g_Player.Collision.mtxWorldCollision._41, g_Player.Collision.mtxWorldCollision._42, g_Player.Collision.mtxWorldCollision._43 - 20.0f)
					, g_Player.Collision.fRadius
					, pBoss->nIndexSencer
				);
#endif
			}
		}
	}

	if (g_PlayerStateNow == PLAYERSTATE_MOVE)
	{
		//pMotion->nCntFlame = 0;

		if (pMotion->nCntFlame % 30 == 0)
		{
			PlaySound(SOUND_LABEL_SE_FOOTSTEP);
		}

	}
	if (g_PlayerStateNow == PLAYERSTATE_NEUTRAL)
	{//ムーヴへの移行
		if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_D) == true
			|| GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_W) == true
			|| state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			|| state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			)
		{
			SetPlayerState(PLAYERSTATE_MOVE);
		}
	}

	// 減速
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.2f;
	g_Player.move.z += (0.0f - g_Player.move.z) * 0.2f;

	if (g_Player.rotDest < -D3DX_PI)
	{
		g_Player.rotDest += D3DX_PI * 2.0f;
	}
	if (g_Player.rotDest > D3DX_PI)
	{
		g_Player.rotDest -= D3DX_PI * 2.0f;
	}

	// 位置と方向の更新
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.z += g_Player.move.z;
	g_Player.rot.y += g_Player.rotDest * 0.1f;

	if (g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rot.y += D3DX_PI * 2.0f;
	}
	if (g_Player.rot.y > D3DX_PI)
	{
		g_Player.rot.y -= D3DX_PI * 2.0f;
	}

	g_Player.rotDest = 0.0f;

	switch (g_PlayerState)
	{
	case PLAYERSTATE_NEUTRAL:
		if (pMotion->KeyFlame == g_aTex->KEYFLAME[pMotion->nNumPlayermotion] - 1)
		{
			if (g_PlayerStateNow == PLAYERSTATE_NEUTRAL)
			{
				SetMotion(0);
			}
			else if (g_PlayerStateNow == PLAYERSTATE_ORIGINAL)
			{
				if (pCharaType == CHARACTER_TYPE_SOLDIER || pCharaType == CHARACTER_TYPE_SORCERER)
				{
					if (GetKeyboardPress(DIK_RETURN) == true
						|| state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && pJoystate.bConnectionJoypad == true)
					{
						if (pMotion->KeyFlame == g_aTex->KEYFLAME[pMotion->nNumPlayermotion] - 1)

						{// 最後のキーになったら0に戻す
							SetMotion_STATE(MOTIONSTATE_STOP);
						}
					}
					else
					{
						pMotion->KeyFlame = 0;
						SetMotion(0);
						g_PlayerStateNow = PLAYERSTATE_NEUTRAL;
						g_PlayerState = PLAYERSTATE_NEUTRAL;
						SetMotion_STATE(MOTIONSTATE_MOVE);
					}
				}
			}
			else if (g_PlayerStateNow == PLAYERSTATE_MIASMA)
			{
				if (GetKeyboardPress(DIK_L) == true
					|| state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true)
				{
					if (pMotion->KeyFlame == g_aTex->KEYFLAME[pMotion->nNumPlayermotion] - 1)

					{// 最後のキーになったら0に戻す
						SetMotion_STATE(MOTIONSTATE_STOP);
					}

				}
				else
				{
					pMotion->KeyFlame = 0;
					SetMotion(0);
					g_PlayerStateNow = PLAYERSTATE_NEUTRAL;
					g_PlayerState = PLAYERSTATE_NEUTRAL;
					SetMotion_STATE(MOTIONSTATE_MOVE);
				}
			}
			else if (g_PlayerStateNow == PLAYERSTATE_DEATH)
			{
				if (pMotion->KeyFlame == g_aTex->KEYFLAME[pMotion->nNumPlayermotion] - 1)
				{// 最後のキーになったら0に戻す
					SetMotion_STATE(MOTIONSTATE_STOP);
				}
			}
			else
			{
				pMotion->KeyFlame = 0;
				SetMotion(0);
				g_PlayerStateNow = PLAYERSTATE_NEUTRAL;
				g_PlayerState = PLAYERSTATE_NEUTRAL;
			}
		}
		break;
	case PLAYERSTATE_MOVE:
		g_PlayerStateNow = PLAYERSTATE_MOVE;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(1);
		break;
	case PLAYERSTATE_ATTACK:
		g_PlayerStateNow = PLAYERSTATE_ATTACK;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(2);
		break;
	case PLAYERSTATE_JAMP:
		g_PlayerStateNow = PLAYERSTATE_JAMP;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(3);
		break;
	case PLAYERSTATE_LANDING:
		g_PlayerStateNow = PLAYERSTATE_LANDING;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(4);
		break;
	case PLAYERSTATE_DAMAGE:
		g_PlayerStateNow = PLAYERSTATE_DAMAGE;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(5);
		break;
	case PLAYERSTATE_DEATH:
		g_PlayerStateNow = PLAYERSTATE_DEATH;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(6);
		break;
	case PLAYERSTATE_MIASMA:
		g_PlayerStateNow = PLAYERSTATE_MIASMA;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(7);
		break;
	case PLAYERSTATE_ORIGINAL:
		g_PlayerStateNow = PLAYERSTATE_ORIGINAL;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(8);
		break;
	case PLAYERSTATE_ATTACK2:
		g_PlayerStateNow = PLAYERSTATE_ATTACK2;
		g_PlayerState = PLAYERSTATE_NEUTRAL;
		SetMotion(2);
		break;
	}
	SetPositionShadow(g_Player.nIdxShadow, g_Player.pos);	//影の設定
	SetPositionSencerCollision(g_Player.nIdxSencer, g_Player.pos);//センサーの位置
	SetPositionSencerLock(g_Player.nIdxSencerLock, D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 50.0f, g_Player.pos.z));//センサーの位置
	CollisionSencer_Col(&g_Player.pos, &g_Player.posOld);
	if (CollisionSencerNext(&g_Player.pos) == true)
	{//モデル接触時の選択肢表示
		SetSelect(true);
	}

	if (CollisionSencer(&g_Player.pos) == true)
	{//魔法陣のセンサーに当たった時
		CollisionSencerReturnCnt(&g_Player.pos);
		SetMessageBox(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 550.0f, 0.0f), 0);
		if (GetKeyboardPress(DIK_L) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
		{
			DeleteMagicSquare(CollisionSencerReturnCnt(&g_Player.pos));
		}

	}
	else
	{
		DeleteMessage();
	}


	if (g_Stage == STAGEMODE_STAGE1)
	{//現在のステージがステージ1
	 //奥壁のあたり判定
		CollisionWall1_WIDTHBack_PL(&g_Player.pos, &g_Player.posOld);

		//手前壁のあたり判定
		CollisionWall1_WIDTHThisSide_PL(&g_Player.pos, &g_Player.posOld);

		//右壁のあたり判定
		CollisionWall1_RIGHT_PL(&g_Player.pos, &g_Player.posOld);

		//右壁のあたり判定
		CollisionWall1_LEFT_PL(&g_Player.pos, &g_Player.posOld);

		if (g_Player.pos.z < STAGE_LIMIT_MIN_Z)
		{
			g_Player.pos.z = STAGE_LIMIT_MIN_Z;
		}
	}
	else if (g_Stage == STAGEMODE_STAGE2)
	{//現在のステージがステージ2
	 //奥壁のあたり判定
		CollisionWall2_WIDTHBack_PL(&g_Player.pos, &g_Player.posOld);

		//手前壁のあたり判定
		CollisionWall2_WIDTHThisSide_PL(&g_Player.pos, &g_Player.posOld);

		//右壁のあたり判定
		CollisionWall2_RIGHT_PL(&g_Player.pos, &g_Player.posOld);

		//右壁のあたり判定
		CollisionWall2_LEFT_PL(&g_Player.pos, &g_Player.posOld);
	}
	else if (g_Stage == STAGEMODE_STAGE3)
	{//現在のステージがステージ2
	 //奥壁のあたり判定
		CollisionWallBoss_WIDTHBack_PL(&g_Player.pos, &g_Player.posOld);

		//手前壁のあたり判定
		CollisionWallBoss_WIDTHThisSide_PL(&g_Player.pos, &g_Player.posOld);

		//右壁のあたり判定
		CollisionWallBoss_RIGHT_PL(&g_Player.pos, &g_Player.posOld);

		//右壁のあたり判定
		CollisionWallBoss_LEFT_PL(&g_Player.pos, &g_Player.posOld);
	}

}

	

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						// 親モデルの情報

	//=============================================================================
	// プレイヤー
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorldPlayer);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorldPlayer, &g_Player.mtxWorldPlayer, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorldPlayer, &g_Player.mtxWorldPlayer, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorldPlayer);

	// 親モデル
	for (int nCntParts = 0; nCntParts < g_Player.MAX_PARTS; nCntParts++)
	{
		if (g_Player.aModel[nCntParts].nIdxModelParent == -1)
		{// 体の時、プレイヤーのマトリックスに入れる
			mtxParent = g_Player.mtxWorldPlayer;
		}
		else if (g_Player.aModel[nCntParts].nIdxModelParent == 0)
		{// 手(右)の時、腕(右)のマトリックスに入れる
			mtxParent = g_Player.aModel[0].mtxWorldPlayerModel;
		}
		else if (g_Player.aModel[nCntParts].nIdxModelParent == 1)
		{// 手(右)の時、腕(右)のマトリックスに入れる
			mtxParent = g_Player.aModel[1].mtxWorldPlayerModel;
		}
		else if (g_Player.aModel[nCntParts].nIdxModelParent == 8)
		{// 手(右)の時、腕(右)のマトリックスに入れる
			mtxParent = g_Player.aModel[8].mtxWorldPlayerModel;
		}
		else if (g_Player.aModel[nCntParts].nIdxModelParent == 5)
		{// 手(右)の時、腕(右)のマトリックスに入れる
			mtxParent = g_Player.aModel[5].mtxWorldPlayerModel;
		}

		else
		{// 頭、腕(左右)、脚(左右)の時、体のマトリックスに入れる
			mtxParent = g_Player.aModel[nCntParts - 1].mtxWorldPlayerModel;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.aModel[nCntParts].mtxWorldPlayerModel);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Player.aModel[nCntParts].rot.y, g_Player.aModel[nCntParts].rot.x, g_Player.aModel[nCntParts].rot.z);
		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorldPlayerModel, &g_Player.aModel[nCntParts].mtxWorldPlayerModel, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_Player.aModel[nCntParts].pos.x, g_Player.aModel[nCntParts].pos.y, g_Player.aModel[nCntParts].pos.z);
		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorldPlayerModel, &g_Player.aModel[nCntParts].mtxWorldPlayerModel, &mtxTrans);

		//親のマトリックス反映
		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorldPlayerModel, &g_Player.aModel[nCntParts].mtxWorldPlayerModel, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntParts].mtxWorldPlayerModel);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntParts].pBuffMatPlayerModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntParts].nNumMatPlayerModel; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			pDevice->SetTexture(0, NULL);
			//モデル(パーツ)の描画
			g_Player.aModel[nCntParts].pMeshPlayerModel->DrawSubset(nCntMat);
		}
		//マテリアルを返す
		pDevice->SetMaterial(&matDef);
	}

	/////////////////
	//当たり判定用//
	///////////////
	if (g_Player.Collision.nIdxParent == -1)
	{// 体の時、プレイヤーのマトリックスに入れる
		mtxParent = g_Player.aModel[16].mtxWorldPlayerModel;
	}
	if (g_Player.Collision.nIdxParent == 0)
	{// 体の時、プレイヤーのマトリックスに入れる
		mtxParent = g_Player.aModel[15].mtxWorldPlayerModel;
	}
	
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.Collision.mtxWorldCollision);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.Collision.rot.y, g_Player.Collision.rot.x, g_Player.Collision.rot.z);
	D3DXMatrixMultiply(&g_Player.Collision.mtxWorldCollision, &g_Player.Collision.mtxWorldCollision, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.Collision.pos.x, g_Player.Collision.pos.y, g_Player.Collision.pos.z);
	D3DXMatrixMultiply(&g_Player.Collision.mtxWorldCollision, &g_Player.Collision.mtxWorldCollision, &mtxTrans);

	//親のマトリックス反映
	D3DXMatrixMultiply(&g_Player.Collision.mtxWorldCollision, &g_Player.Collision.mtxWorldCollision, &mtxParent);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.Collision.mtxWorldCollision);
}

//------------------------------------------------------------------------
// プレイヤーの状態の設定												  |
//------------------------------------------------------------------------
void SetPlayerState(PLAYERSTATE state)
{
	g_PlayerState = state;
}




//------------------------------------------------------------------------
// プレイヤーの状態の取得												  |
//------------------------------------------------------------------------
PLAYERSTATE GetPlayerState(void)
{
	return g_PlayerState;
}

//------------------------------------------------------------------------
// プレイヤーの状態の取得(現状)											  |
//------------------------------------------------------------------------
PLAYERSTATE GetPlayerStateNow(void)
{
	return g_PlayerStateNow;
}

//------------------------------------------------------------------------
// プレイヤーデータの取得												  |
//------------------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_Player;
}


//XInputGetKeystroke
//XINPUT_KEYSTROKE keystroke;
//XInputGetKeystroke( 0, 0, &keystroke );
//if(keystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN && keystroke.VirtualKey == VK_PAD_A)
//	{
//    // プレイヤー1の A ボタンが押されたか、押しっぱなしになっていて入力が繰り返されている
//	}