////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ぱねるの処理 [sky.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PanelHole.h"
#include "manager.h"
#include "input.h"
#include "manager.h"
#include "player.h"
#include "game.h"
#include "renderer.h"
#include "sound.h"
#include "model.h"
#include "camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
int CPanelHole::m_nBreakId = -1;
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPanelHole::m_pTexture[CPanelHole::TYPE_MAX] = {};

CPlayer::MOTION_TYPE CPanelHole::m_aMotionType[CPanelHole::TYPE_MAX] =
{
	CPlayer::MOTION_TYPE_MAX,
	CPlayer::MOTION_TYPE_POSE_00,
	CPlayer::MOTION_TYPE_POSE_01,
	CPlayer::MOTION_TYPE_POSE_02,
	CPlayer::MOTION_TYPE_POSE_03,

	CPlayer::MOTION_TYPE_MAX,
	CPlayer::MOTION_TYPE_POSE_00,
	CPlayer::MOTION_TYPE_POSE_01,
	CPlayer::MOTION_TYPE_POSE_02,
	CPlayer::MOTION_TYPE_POSE_03,

	CPlayer::MOTION_TYPE_MAX,
	CPlayer::MOTION_TYPE_POSE_00,
	CPlayer::MOTION_TYPE_POSE_01,
	CPlayer::MOTION_TYPE_POSE_02,
	CPlayer::MOTION_TYPE_POSE_03,
};
//=============================================================================
// コンストラクタ
//=============================================================================
CPanelHole::CPanelHole() :CScene3D(PRIORITY_PANEL)
{
	m_bBreak = false;
	m_move = D3DXVECTOR3(0, 0, 0);
	m_nCounter = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CPanelHole::~CPanelHole()
{
}

//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CPanelHole::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	char *pTexture[TYPE_MAX]=
	{
		"data/TEXTURE/none/space.png",
		"data/TEXTURE/pose1/pose_1_space.png",
		"data/TEXTURE/pose2/pose_2_space.png",
		"data/TEXTURE/pose3/pose_3_space.png",
		"data/TEXTURE/pose4/pose_4_space.png",

		"data/TEXTURE/none/earth.png",
		"data/TEXTURE/pose1/pose_1_earth.png",
		"data/TEXTURE/pose2/pose_2_earth.png",
		"data/TEXTURE/pose3/pose_3_earth.png",
		"data/TEXTURE/pose4/pose_4_earth.png",

		"data/TEXTURE/none/under.png",
		"data/TEXTURE/pose1/pose_1_under.png",
		"data/TEXTURE/pose2/pose_2_under.png",
		"data/TEXTURE/pose3/pose_3_under.png",
		"data/TEXTURE/pose4/pose_4_under.png",

	};
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice,
			pTexture[nCnt],
			&m_pTexture[nCnt]);
	}
	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CPanelHole::UnLoad(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPanelHole::Init(void)
{
	m_bBreak = false;;
	m_nCounter = 0;
	CScene3D::Init();
	SetObjType(CScene::OBJTYPE_PANEL);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPanelHole::Uninit(void)
{
	CScene3D::Uninit();

}

//=============================================================================
// 更新処理
//=============================================================================
void CPanelHole::Update(void)
{
	if (m_nBreakId != -1)
	{
		CScene *m_pSceneTarget = CScene::GetScene(PRIORITY_PANEL);
		while (m_pSceneTarget != NULL)
		{
			CScene *pSceneNext = m_pSceneTarget->GetNext();
			if (m_pSceneTarget != NULL)
			{
				if (m_pSceneTarget->GetObjType() == OBJTYPE_PANEL)
				{// オブジェクト
					CPanelHole *pObject = ((CPanelHole*)m_pSceneTarget);
					if (pObject->m_nNumber == m_nBreakId)
					{
						pObject->m_bBreak = true;
						pObject->m_move = D3DXVECTOR3(sinf(CManager::GetRandomPI()) * 10, rand() % 20 / 10.0f, cosf(CManager::GetRandomPI()) * 10);
					}
				}

			}
			m_pSceneTarget = pSceneNext;
		}
		m_nBreakId = -1;
		m_nCounter = 0;
	}
	if (m_bBreak)
	{
		m_nCounter++;
		if (m_nCounter >= 30)
		{
			D3DXVECTOR3 pos = GetPos();
			pos += m_move;
			m_move.y -= 0.4f;
			SetPos(pos);
			if (pos.y <= -20000)
			{
				Uninit();
				return;
			}
		}
	}
	CScene3D::Update();

}
//=============================================================================
// 描画処理
//=============================================================================
void CPanelHole::Draw(void)
{
	CScene3D::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CPanelHole *CPanelHole::Create(D3DXVECTOR3 pos,float fWidth,float fDepth,TYPE type,int nNum)
{
	CPanelHole *pPowerCut = NULL;

	if (pPowerCut == NULL)
	{// イベントが発生生成
		pPowerCut = new CPanelHole;
	}
	if (pPowerCut != NULL)
	{
		if (FAILED(pPowerCut->Init()))
		{// 初期化処理
			return NULL;
		}
		pPowerCut->m_type = type;
		pPowerCut->SetPos(pos);
		pPowerCut->SetSize(fWidth,0.0f,fDepth);
		pPowerCut->m_nNumber = nNum;
		pPowerCut->BindTexture(m_pTexture[type]);
		pPowerCut->SetVtx();
	}

	return pPowerCut;
}

int CPanelHole::Collision(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const float fWidth, int nMotion)
{
	if (m_bBreak) { return 0; }
	int nOk = 0;
	D3DXVECTOR3 MeshPos = GetPos();
	float fMeshWidth = GetWidth();
	float fMeshDepth = GetDepth();
	if (pos.y <= MeshPos.y&&posOld.y > MeshPos.y)
	{// 前回入ってないで今回はいった
		if (pos.x  >= MeshPos.x - fMeshWidth&&
			pos.x  <= MeshPos.x + fMeshWidth&&
			pos.z  <= MeshPos.z + fMeshDepth&&
			pos.z  >= MeshPos.z - fMeshDepth)
		{
			if (nMotion == m_aMotionType[m_type])
			{// モーション一緒
				nOk = 1;
				m_nBreakId = m_nNumber;
			}
			else
			{

				nOk = 2;
			}
			return nOk;
		}
	}
	return nOk;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ぱねるの処理 [sky.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CPanel::m_nNumAll = 0;
CPanel::CPanel()
{
	 m_nNumAll = 0;
}
CPanel::~CPanel()
{

}

void CPanel::SetPanel(const D3DXVECTOR3 pos, const int nHole, const STAGE stage)
{// -330 330 -130 230
	// 660 360

	CPanelHole::m_nBreakId = -1;

	int aHole[3][6] = {
		{ 0,0,0,0,0,0},
		{ 0,0,0,0,0,0},
		{ 0,0,0,0,0,0},
	};
	int nX = 0;
	int nY = 0;
	for (int nCnt = 0; nCnt < nHole; nCnt++)
	{
		nX = CManager::GetIntRandBetWeen(0, 5);
		nY = CManager::GetIntRandBetWeen(0, 2);
		aHole[nY][nX] = 1;
	}
	for (int nCntX = 0; nCntX < 6; nCntX++)
	{
		for (int nCntY = 0; nCntY < 3; nCntY++)
		{
			switch (stage)
			{
			case STAGE_SPACE:

				if (aHole[nCntY][nCntX] == 1)
				{
					int nType = 0;
					int nType2 = 0;
					for (int nCnt = CPanelHole::TYPE_SPACE_1; nCnt < CPanelHole::TYPE_SPACE_4 + 1; nCnt++)
					{
						if (CPanelHole::m_aMotionType[nCnt] == CPlayer::GetButtonMotion(CPlayer::POSE_BOTTAN_A))
						{
							nType = nCnt;
						}
						if (CPanelHole::m_aMotionType[nCnt] == CPlayer::GetButtonMotion(CPlayer::POSE_BOTTAN_B))
						{
							nType2 = nCnt;
						}
					}
					if (CManager::GetIntRandBetWeen(0, 1) == 0)
					{
						CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
							, 120.0f, 120.0f, CPanelHole::TYPE(nType), m_nNumAll);
					}
					else
					{
						CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
							, 120.0f, 120.0f, CPanelHole::TYPE(nType2), m_nNumAll);

					}
				}
				else
				{
					CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
						, 120.0f, 120.0f, CPanelHole::TYPE_SPACE_NONE, m_nNumAll);
				}

				break;
			case STAGE_EARTH:

				if (aHole[nCntY][nCntX] == 1)
				{
					int nType = 0;
					int nType2 = 0;
					int nType3 = 0;
					for (int nCnt = CPanelHole::TYPE_EARTH_1; nCnt < CPanelHole::TYPE_EARTH_4 + 1; nCnt++)
					{
						if (CPanelHole::m_aMotionType[nCnt] == CPlayer::GetButtonMotion(CPlayer::POSE_BOTTAN_A))
						{
							nType = nCnt;
						}
						if (CPanelHole::m_aMotionType[nCnt] == CPlayer::GetButtonMotion(CPlayer::POSE_BOTTAN_B))
						{
							nType2 = nCnt;
						}
						if (CPanelHole::m_aMotionType[nCnt] == CPlayer::GetButtonMotion(CPlayer::POSE_BOTTAN_X))
						{
							nType3 = nCnt;

						}

					}
					int aaaa = CManager::GetIntRandBetWeen(0, 2);
					if (aaaa == 0)
					{
						CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
							, 120.0f, 120.0f, CPanelHole::TYPE(nType), m_nNumAll);
					}
					else if (aaaa = 2)
					{
						CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
							, 120.0f, 120.0f, CPanelHole::TYPE(nType2), m_nNumAll);

					}
					else
					{
						CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
							, 120.0f, 120.0f, CPanelHole::TYPE(nType3), m_nNumAll);

					}

				}
				else
				{
					CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
						, 120.0f, 120.0f, CPanelHole::TYPE_EARTH_NONE, m_nNumAll);
				}

				break;
			case STAGE_HELL:

				if (aHole[nCntY][nCntX] == 1)
				{
					CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
						, 120.0f, 120.0f, CPanelHole::TYPE(CManager::GetIntRandBetWeen(CPanelHole::TYPE_HELL_1, CPanelHole::TYPE_HELL_4)), m_nNumAll);
				}
				else
				{
					CPanelHole::Create(D3DXVECTOR3(-330 + 120.0f*nCntX, pos.y, 130 - 120.0f*nCntY)
						, 120.0f, 120.0f, CPanelHole::TYPE_HELL_NONE, m_nNumAll);
				}

				break;
			}
		}
	}
	m_nNumAll++;
}