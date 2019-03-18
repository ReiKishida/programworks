//=============================================================================
//
// オブジェクトの処理 [object.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "sceneX.h"
#include "renderer.h"
#include "manager.h"
#include "create.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "objCreate.h"
#include "object.h"
#include "meshField.h"
#include "DebugProc.h"
#include "line.h"
#include "meshWall.h"
#include "environment.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_OBJECT_PRINT "data/object_print.txt"		// ファイルの名前書き込み
#define FILE_FIELD_PRINT  "data/field_print.txt"		// ファイルの名前書き込み

#define MOVE (2.0f)								// 移動量
#define COLLAR   (0.0f)							// 透明度

//== オブジェクト ==//
#define MAX_OBJECT (19)							// オブジェクトの最大値
#define COLLISION  (100)						// 範囲

//== フィールド ==//
#define LINEMOVE (300)							// ラインの移動量

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//== コンストラクタ ==//
CObjCreate::CObjCreate()
{//クリア値にする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumObject = 0;
	m_bCamera = false;
}

//=== デストラクタ ===//
CObjCreate::~CObjCreate()
{

}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObjCreate::Init(void)
{
	CSceneX::Init();	// 初期化処理

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObjCreate::Uninit(void)
{
	CSceneX::Uninit();	// 終了処理
}

//============================================================================
// 更新処理
//=============================================================================
void CObjCreate::Update(void)
{
	CCreate::CREATEMODE mode = CCreate::GetCreateMode();		// モード取得の取得

																//== オブジェクトモード ==//
	if (mode == CCreate::CREATEMODE_OBJECT)
	{
		int nNum = CSceneX::GetNumFile();
		CCamera *pCamera = CCreate::GetCamera();	// カメラの情報取得
		m_pos = pCamera->GetPosR(0);	// カメラの注視点の取得
		bool Slow = pCamera->GetSlow();
		bool Grid = pCamera->GetGrid();
		int MaxObj = CSceneX::GetNumFile();

		if (m_bCamera == false)
		{// カメラモードがオフの状態
			m_rot = pCamera->GetRot(0);
		}

		CSceneX::SetPos(m_pos);		// 位置の更新
		CSceneX::SetRot(D3DXVECTOR3(0.0f, m_rot.y, m_rot.z));		// 角度の更新

		//== 種類の選択 ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_1))
		{// タイプ選択
			m_type--;
			if (m_type < 0)
			{
				m_type = MaxObj - 1;
			}
			BindModel(m_type);
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2))
		{// タイプ選択
			m_type++;
			if (m_type > MaxObj - 1)
			{
				m_type = 0;
			}
			BindModel(m_type);
		}


		if (CManager::GetInputKeyboard()->GetTrigger(DIK_K))
		{// オブジェクトの作成
			CObject::Create(m_pos, D3DXVECTOR3(0.0f, m_rot.y, m_rot.z), m_type, m_pos);
			m_nNumObject++;
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_L))
		{// オブジェクトの削除
			for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
			{// 優先順位の最大数分回す
				CScene	*pSceneTarget = CScene::GetScene(nCntP);
				while (pSceneTarget != NULL)
				{
					CScene *pSceneNext = pSceneTarget->GetNext();
					if (pSceneTarget != NULL)
					{
						if (pSceneTarget->GetObjType() == OBJTYPE_OBJECT)
						{// オブジェクトの取得
							CObject *pObject = ((CObject*)pSceneTarget);	// オブジェクトをギミック型に設定
							D3DXVECTOR3 ObjectPos = pObject->GetPos();	// 位置の取得
							if (m_pos.x > ObjectPos.x - COLLISION
								&& m_pos.x < ObjectPos.x + COLLISION
								&& m_pos.z > ObjectPos.z - COLLISION
								&& m_pos.z < ObjectPos.z + COLLISION)
							{
								pObject->Uninit();
								m_nNumObject--;
							}
						}
					}
					pSceneTarget = pSceneNext;
				}
			}
		}

		//== モード切替 ==//
		// カメラ切り替え
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4))
		{
			m_bCamera = m_bCamera ? false : true;
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F6))
		{//スローモードの切り替え
			Slow = Slow ? false : true;
			pCamera->SetSlow(Slow);
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7))
		{//グリッドモードの切り替え
			Grid = Grid ? false : true;
			pCamera->SetGrid(Grid);
		}


		//== データ処理 ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
		{// データの更新
			CObjCreate::SetScan();
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
		{// データの保存
			CObjCreate::SetPrint();
		}

		//== 操作の説明 ==//
		CDebugProc::print("\n");
		CDebugProc::print(" [[ 操作説明 ]]\n");
		CDebugProc::print(" 移動 [ W, S, A, D ][ R, F ]\n");
		CDebugProc::print(" 回転 [ Q, E ] \n");
		CDebugProc::print(" 種類 [ 1, 2 ] \n");
		CDebugProc::print(" 位置のリセット [ Z ]\n");
		CDebugProc::print(" 角度のリセット [ X ]\n");
		CDebugProc::print(" オブジェクト配置 [ K ] \n");
		CDebugProc::print(" オブジェクト削除 [ L ] \n");
		CDebugProc::print(" \n");

		if (m_bCamera == false)
		{// カメラモードがオフ
			CDebugProc::print(" カメラモード [ F4 ] [ オフ ]\n");
		}
		else
		{// カメラモードがオン
			CDebugProc::print(" カメラモード [ F4 ] [ オン ]\n");
		}
		CDebugProc::print(" ゲームモードに移行[ F5 ]\n");
		if (Slow == false)
		{// グリッドモードがオフ
			CDebugProc::print(" スローモード [ F6 ] [ オフ ]\n");
		}
		else
		{// グリッドモードがオン
			CDebugProc::print(" スローモード [ F6 ] [ オン ]\n");
		}
		if (Grid == false)
		{// グリッドモードがオフ
			CDebugProc::print(" グリッドモード [ F7 ] [ オフ ]\n");
		}
		else
		{// グリッドモードがオン
			CDebugProc::print(" グリッドモード [ F7 ] [ オン ]\n");
		}


		CDebugProc::print(" データの更新    [ F8 ]\n");
		CDebugProc::print(" データの保存    [ F9 ]\n");
		CDebugProc::print(" フィールドに切り替え [ F10 ]\n");
		CDebugProc::print(" \n");
		CDebugProc::print(" 位置 [ %.1f %.1f %.1f ]\n", m_pos.x, m_pos.y, m_pos.z);
		CDebugProc::print(" 角度 [ %.1f %.1f %.1f ]\n", 0.0f, m_rot.y, m_rot.z);
		CDebugProc::print(" 種類 [ %d ]\n", m_type);
		CDebugProc::print(" 配置数　[ %d ]\n", m_nNumObject);
		CDebugProc::print(" \n");

		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));		// 透明度を元に戻す
	}
	else
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, COLLAR));		// 見えない状態にする
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CObjCreate::Draw(void)
{
	CSceneX::Draw();	// 描画処理
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CObjCreate *CObjCreate::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type)
{
	CObjCreate *pObjCreate = NULL;
	if (!CObjCreate::OverData())
	{// ２Dポリゴン生成
		pObjCreate = new CObjCreate;
	}
	if (pObjCreate != NULL)
	{
		// 初期化処理
		if (FAILED(pObjCreate->Init()))
		{
			return NULL;
		}
		pObjCreate->SetPos(pos);
		pObjCreate->SetRot(rot);
		pObjCreate->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		pObjCreate->SetType(type);
		pObjCreate->BindModel(type);
	}
	return pObjCreate;
}

//=============================================================================
// 読み込み関数
//=============================================================================
void CObjCreate::SetScan(void)
{
	//== オブジェクトを全て削除 ==//
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// 優先順位の最大数分回す
		CScene	*pSceneTarget = CScene::GetScene(nCntP);
		while (pSceneTarget != NULL)
		{
			CScene *pSceneNext = pSceneTarget->GetNext();
			if (pSceneTarget != NULL)
			{
				if (pSceneTarget->GetObjType() == OBJTYPE_OBJECT)
				{// オブジェクトの取得
					CObject *pObject = ((CObject*)pSceneTarget);	// オブジェクトをギミック型に設定
					pObject->Uninit();								// オブジェクトの削除
				}
				if (pSceneTarget->GetObjType() == OBJTYPE_MESHFIELD)
				{// オブジェクトの取得
					CMeshField *pMeshField = ((CMeshField*)pSceneTarget);	// オブジェクトを床型に設定
					pMeshField->Uninit();								// オブジェクトの削除
				}
				if (pSceneTarget->GetObjType() == OBJTYPE_MESHWALL)
				{// オブジェクトの取得
					CMeshWall *pMeshWall = ((CMeshWall*)pSceneTarget);	// オブジェクトを壁型に設定
					pMeshWall->Uninit();								// オブジェクトの削除
				}

				if (pSceneTarget->GetObjType() == OBJTYPE_ENVIROMENT)
				{// ビルボードの削除
					CEnvironment *pEnviroment = ((CEnvironment*)pSceneTarget);	// オブジェクトを植物型に設定
					pEnviroment->Uninit();							// オブジェクトの削除
				}
			}
			pSceneTarget = pSceneNext;
		}
	}
	CScene::DeathFrag();
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CEnvironment::Create();
		CEnvironment::SetScan();
	}
	CMeshWall::SetScan();
	CMeshField::SetScan();
	CObject::SetScan();		// 再びオブジェクトの配置
	CLine::SetDisp(false);
}

//=============================================================================
// 書き込み関数
//=============================================================================
void CObjCreate::SetPrint(void)
{
	FILE *pFile;						// ファイルの作成
	pFile = fopen(FILE_OBJECT_PRINT, "w");		// ファイルの名前の書き込み

	if (pFile != NULL)
	{
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# 『オブジェクト』オブジェクトリスト [object.txt]");
		fprintf(pFile, "# Author : Okabe Kazuki\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# オブジェクトの種類・位置・角度\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
		{// 優先順位の最大数分回す
			CScene	*pSceneTarget = CScene::GetScene(nCntP);
			while (pSceneTarget != NULL)
			{
				CScene *pSceneNext = pSceneTarget->GetNext();
				if (pSceneTarget != NULL)
				{
					if (pSceneTarget->GetObjType() == OBJTYPE_OBJECT)
					{// オブジェクトの取得

						CObject *pObject = ((CObject*)pSceneTarget);	// オブジェクトをギミック型に設定
						D3DXVECTOR3 pos = pObject->GetPos();	// 位置の取得
						D3DXVECTOR3 rot = pObject->GetRot();	// 角度の取得
						int type = pObject->GetType();			// 種類の取得

						fprintf(pFile, "OBJECTSET\n");
						fprintf(pFile, "TYPE = %d			# [ 種類 ]\n", type);						// 種類の書き込み
						fprintf(pFile, "POS  = %.1f %.1f %.1f	# [ 位置 ]\n", pos.x, pos.y, pos.z);	// 位置の書き込み
						fprintf(pFile, "ROT  = %.1f %.1f %.1f	# [ 角度 ]\n", rot.x, rot.y, rot.z);	// 角度の書き込み
						fprintf(pFile, "END_OBJECTSET\n");
						fprintf(pFile, "\n");
					}
				}
				pSceneTarget = pSceneNext;
			}
		}
		fprintf(pFile, "END_SCRIPT		# この行は絶対消さないこと！\n");
		fclose(pFile);// 終了
	}
}

//=============================================================================
//
// フィールド作成処理 [object.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
D3DXVECTOR3 CFieldCreate::m_aMeshPos[MAX_FIELD][MAX_FIELD] = {};
int CFieldCreate::m_type = 0;
int CFieldCreate::m_nNumAll = 0;
int CFieldCreate::m_nNumBlock[MAX_FIELD] = {};
int CFieldCreate::m_nCntMesh = 0;

//== コンストラクタ ==//
CFieldCreate::CFieldCreate()
{//クリア値にする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = 0;
	m_fLength = 0.0f;
	m_nNumAll = 0;
	m_nCntMesh = 0;
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		m_nNumBlock[nCnt] = -1;
	}
}

//=== デストラクタ ===//
CFieldCreate::~CFieldCreate()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFieldCreate::Init(void)
{
	m_fLength = 150.0f;		// ラインの長さ

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFieldCreate::Uninit(void)
{
	//CSceneX::Uninit();	// 終了処理
}

//============================================================================
// 更新処理
//=============================================================================
void CFieldCreate::Update(void)
{
	CCreate::CREATEMODE mode = CCreate::GetCreateMode();		// モード取得の取得

	//== フィールドモード ==//
	if (mode == CCreate::CREATEMODE_FIELD)
	{
		CLine *pLine = CCreate::GetLine();				// ラインの情報取得
		CCamera *pCamera = CCreate::GetCamera();		// カメラの情報取得
		m_pos = pCamera->GetPosR(0);					// カメラの注視点の取得
		D3DXVECTOR3 LineStart = D3DXVECTOR3(sinf(-D3DX_PI / 2 + m_rot.y)*m_fLength, 0.0f, cosf(-D3DX_PI / 2 + m_rot.y)*m_fLength) + m_pos;
		D3DXVECTOR3 LineEnd = D3DXVECTOR3(sinf(D3DX_PI / 2 + m_rot.y) *m_fLength, 0.0f, cosf(D3DX_PI / 2 + m_rot.y)*m_fLength) + m_pos;
		bool Slow = pCamera->GetSlow();
		bool Grid = pCamera->GetGrid();
		int MaxField = CMeshField::GetNumFile();
		CMeshField *pMesh;
		if (m_bCamera == false)
		{
			m_rot = pCamera->GetRot(0);						// カメラの角度の取得
		}


		if (m_bCamera == false)
		{
			D3DXVECTOR3 rot = pCamera->GetRot(0);		// カメラの角度の取得
		}
		// 長さの大小を変える
		// あとでXのブロック数を変える

		//==種類変更 ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_1))
		{// タイプ選択
			m_type--;
			if (m_type < 0)
			{
				m_type = MaxField - 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2))
		{// タイプ選択
			m_type++;
			if (m_type > MaxField - 1)
			{
				m_type = 0;
			}
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_3))
		{// ラインの縮小
			if (m_fLength > 25)
			{
				m_fLength -= 25;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{// ラインのおっきく
			m_fLength += 25;
		}

		//== 地面の配置処理 ==//

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_K))
		{// ラインの配置
			CLine::Create(LineStart, LineEnd);
			m_aMeshPos[m_nNumAll][m_nCntMesh] = D3DXVECTOR3(LineStart);		// 頂点の始点取得
			m_aMeshPos[m_nNumAll][m_nCntMesh + 1] = D3DXVECTOR3(LineEnd);	// 頂点の終点取得
			m_nNumBlock[m_nNumAll]++;	// ブロック数の加算
			m_nCntMesh += 2;			// カウンターを頂点数分加算
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_L))
		{// 地面の配置
			if (m_nCntMesh != 0)
			{
				pMesh = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 1, m_nCntMesh / 2, m_type,false);
				pMesh->SetVtxPos(&m_aMeshPos[m_nNumAll][0]);
				m_nNumAll++;		// 次のブロックに移行
				m_nCntMesh = 0;		// メッシュの頂点をリセット
			}
		}

		//== モード切替 ==//
		// カメラ切り替え
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4))
		{
			m_bCamera = m_bCamera ? false : true;
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F6))
		{//スローモードの切り替え
			Slow = Slow ? false : true;
			pCamera->SetSlow(Slow);
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7))
		{//グリッドモードの切り替え
			Grid = Grid ? false : true;
			pCamera->SetGrid(Grid);
		}

		//== データ入出力 ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
		{// データの更新
			CFieldCreate::SetScan();
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
		{// データの保存
			CFieldCreate::SetPrint();
		}

		pLine->SetPos(LineStart, LineEnd);		// 位置の更新

		//== 操作の説明 ==//
		CDebugProc::print("\n");
		CDebugProc::print(" [[ 操作説明 ]]\n");
		CDebugProc::print(" 移動 [ W, S, A, D ][ R, F ]\n");
		CDebugProc::print(" 回転 [ Q, E ] \n");
		CDebugProc::print(" 種類 [ 1, 2 ]\n");
		CDebugProc::print(" 拡縮 [ 3, 4 ]\n");
		CDebugProc::print(" ライン配置 [ K ] \n");
		CDebugProc::print(" フィールドの配置 [ L ] \n");
		CDebugProc::print("\n");
		if (m_bCamera == false)
		{// グリッドモードがオフ
			CDebugProc::print(" カメラモード [ F4 ] [ オフ ]\n");
		}
		else
		{// グリッドモードがオン
			CDebugProc::print(" カメラモード [ F4 ] [ オン ]\n");
		}
		CDebugProc::print(" ゲームモードに移行[ F5 ]\n");
		if (Slow == false)
		{// グリッドモードがオフ
			CDebugProc::print(" スローモード [ F6 ] [ オフ ]\n");
		}
		else
		{// グリッドモードがオン
			CDebugProc::print(" スローモード [ F6 ] [ オン ]\n");
		}
		if (Grid == false)
		{// グリッドモードがオフ
			CDebugProc::print(" グリッドモード [ F7 ] [ オフ ]\n");
		}
		else
		{// グリッドモードがオン
			CDebugProc::print(" グリッドモード [ F7 ] [ オン ]\n");
		}

		CDebugProc::print(" データの更新 [ F8 ]\n");
		CDebugProc::print(" データの保存 [ F9 ]\n");
		CDebugProc::print(" オブジェクトに切り替え [ F10 ]\n");
		CDebugProc::print(" \n");
		CDebugProc::print(" 位置 [ %.1f %.1f %.1f ]\n", m_pos.x, m_pos.y, m_pos.z);
		CDebugProc::print(" 角度 [ %.1f %.1f %.1f ]\n", 0.0f, m_rot.y, m_rot.z);
		CDebugProc::print(" 長さ [ %.1f ]\n", m_fLength);
		CDebugProc::print("\n");
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFieldCreate::Draw(void)
{
}

//=============================================================================
// フィールドの生成処理
//=============================================================================
CFieldCreate *CFieldCreate::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type)
{
	CFieldCreate *pFieldCreate = NULL;

	if (!CFieldCreate::OverData())
	{// ２Dポリゴン生成
		pFieldCreate = new CFieldCreate;
	}
	if (pFieldCreate != NULL)
	{
		// 初期化処理
		if (FAILED(pFieldCreate->Init()))
		{
			return NULL;
		}
	}
	return pFieldCreate;
}

//=============================================================================
// 読み込み関数
//=============================================================================
void CFieldCreate::SetScan(void)
{
	//== オブジェクトを全て削除 ==//
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// 優先順位の最大数分回す
		CScene	*pSceneTarget = CScene::GetScene(nCntP);
		while (pSceneTarget != NULL)
		{
			CScene *pSceneNext = pSceneTarget->GetNext();
			if (pSceneTarget != NULL)
			{
				if (pSceneTarget->GetObjType() == OBJTYPE_MESHFIELD)
				{// フィールドの取得
					CMeshField *pMeshField = ((CMeshField*)pSceneTarget);	// フィールドをギミック型に設定
					pMeshField->Uninit();									// フィールドの削除
				}
			}
			pSceneTarget = pSceneNext;
		}
	}
	CMeshField::SetScan();	// 再びフィールドの配置
}

//=============================================================================
// 書き込み関数
//=============================================================================
void CFieldCreate::SetPrint(void)
{
	FILE *pFile;								// ファイルの作成
	pFile = fopen(FILE_FIELD_PRINT, "w");		// ファイルの名前の書き込み

	if (pFile != NULL)
	{
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# 『フィールド』フィールドリスト [field.txt]");
		fprintf(pFile, "# Author : Okabe Kazuki\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# フィールドの種類・位置・角度\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		for (int nCnt1 = 0; nCnt1 < m_nNumAll; nCnt1++)
		{// 全てのブロック分回す
			fprintf(pFile, "SETFIELD\n");
			fprintf(pFile, "	SETVERTEX\n");
			fprintf(pFile, "		TYPE = %d\n", m_type);
			fprintf(pFile, "		BLOCK = %d\n", m_nNumBlock[nCnt1]);
			for (int nCnt2 = 0; nCnt2 < m_nNumBlock[nCnt1]+1; nCnt2++)
			{// ブロック分だけ回す
				fprintf(pFile, "    %d = %.1f %.1f %.1f\n", nCnt2 * 2, m_aMeshPos[nCnt1][nCnt2 * 2].x, m_aMeshPos[nCnt1][nCnt2 * 2].y, m_aMeshPos[nCnt1][nCnt2 * 2].z);	// 位置の書き込み
				fprintf(pFile, "    %d = %.1f %.1f %.1f\n", nCnt2 * 2 + 1, m_aMeshPos[nCnt1][(nCnt2 * 2) + 1].x, m_aMeshPos[nCnt1][(nCnt2 * 2) + 1].y, m_aMeshPos[nCnt1][(nCnt2 * 2) + 1].z);
			}
			fprintf(pFile, "	END_SETVERTEX\n");
			fprintf(pFile, "ENDSETFIELD\n");
		}
		fprintf(pFile, "END_SCRIPT		# この行は絶対消さないこと！\n");
		fclose(pFile);// 終了
	}
}
