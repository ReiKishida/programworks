//=============================================================================
//
// �Q�[�W����[2D�|���S��] [ui_gauge.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene_gauge.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "ui_gauge_bill.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUiGaugeBillBoard::m_pTexture = NULL;
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CUiGaugeBillBoard::CUiGaugeBillBoard(int nPriority, OBJTYPE type) : CSceneGaugeBillBoard(nPriority, type)
{
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CUiGaugeBillBoard::~CUiGaugeBillBoard()
{

}

//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CUiGaugeBillBoard::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/2_lifegauge_enemy.png", &m_pTexture);		// ���C�t�Q�[�W
	return S_OK;

}

//=============================================================================
// �֐����F�g�p�����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CUiGaugeBillBoard::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}


//=============================================================================
// �֐����F�Q�[�W�̐���
// �֐��̊T�v�F�Q�[�W�𐶐�����
//=============================================================================
CUiGaugeBillBoard *CUiGaugeBillBoard::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type)
{
	CUiGaugeBillBoard  *pSceneGauge;
	pSceneGauge = new CUiGaugeBillBoard;	//�V�[���N���X�̐���
	if (pSceneGauge != NULL)
	{
		pSceneGauge->m_pos = pos;						//�ʒu���̑��
		pSceneGauge->m_fWidth = fWidth;					//���̑��
		pSceneGauge->m_fHeight = fHeight;				//�����̑��
		pSceneGauge->m_fCurrentGauge = fCurrentGauge;	//���݂̒l�̑��
		pSceneGauge->m_fMaxGauge = fMaxGauge;			//�ő�l�̑��
		pSceneGauge->m_col = col;						//�Q�[�W�̐F���
		pSceneGauge->m_type = type;						//�Q�[�W�̎�ނ̑��
		pSceneGauge->Init();
	}
	return pSceneGauge;
}


//=============================================================================
// �֐����F�Q�[�W�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CUiGaugeBillBoard::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//�ʒu���̑��
	SetPosGauge(m_pos);

	//�Q�[�W�̕��̑��
	SetGaugeWidth(m_fWidth);
	//�Q�[�W�̍����̑��
	SetGaugeHeight(m_fHeight);

	//�ő�l�̑��
	SetMaxValue(m_fMaxGauge);

	if (m_type == GAUGETYPE_UNDER)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_pTexture);
	}

	//�Q�[�W�̏�����
	CSceneGaugeBillBoard::Init();
	//���݂̒l�̑��
	SetCurrentValue(m_fCurrentGauge);
	//�F���̑��
	SetColor(m_col);

	return S_OK;
}

//=============================================================================
// �֐����F�Q�[�W�̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�̔j���A���g�̔j��
//=============================================================================
void CUiGaugeBillBoard::Uninit(void)
{

	CSceneGaugeBillBoard::Uninit();
}

//=============================================================================
// �֐����F�Q�[�W�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CUiGaugeBillBoard::Update(void)
{


	if (m_type == GAUGETYPE_GREEN || m_type == GAUGETYPE_RED)
	{
		if (m_fCurrentGauge > m_fDestValue)
		{//�ړI�̃Q�[�W
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;
			}
		}
		SetCurrentValue(m_fCurrentGauge);

	}
	//�X�L���Q�[�W
	if (m_type == GAUGETYPE_SKILL)
	{
		if (m_fCurrentGauge < m_fDestValue)
		{//�ړI�̃Q�[�W
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;

			}
			SetCurrentValue(m_fCurrentGauge);
		}
	}

	//�̗͂̃Q�[�W�̍X�V
	//CSceneGaugeBillBoard::Update();



#ifdef _DEBUG
	//if (m_type == GAUGETYPE_GREEN)
	//{//�΃Q�[�W�̂Ƃ�
	//	CDebugProc::Print("GAUGETYPE_GREEN\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);

	//}
	//if (m_type == GAUGETYPE_RED)
	//{//�ԃQ�[�W�̂Ƃ�
	//	CDebugProc::Print("\n");
	//	CDebugProc::Print("GAUGETYPE_RED\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//}

	//if (m_type == GAUGETYPE_SKILL)
	//{//�ԃQ�[�W�̂Ƃ�
	//	CDebugProc::Print("\n");
	//	CDebugProc::Print("GAUGETYPE_SKILL\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//	CDebugProc::Print("\n");
	//}
#endif


}

//=============================================================================
// �֐����F�Q�[�W�̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CUiGaugeBillBoard::Draw(void)
{

	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//Z�o�b�t�@
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);


	//�Q�[�W�̕`��
	CSceneGaugeBillBoard::Draw();

	//// Z�o�b�t�@
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// �֐����F���l�̑��
// �֐��̊T�v�F�_���[�W��񕜂Ȃ�
//=============================================================================
void CUiGaugeBillBoard::AddValue(float fValue)
{
	m_fCurrentGauge = fValue;
	if (m_fCurrentGauge <= 0.0f)
	{
		m_fCurrentGauge = 0.0f;
	}
	if (m_fCurrentGauge >= m_fMaxGauge)
	{
		m_fCurrentGauge = m_fMaxGauge;
	}
	SetCurrentValue(m_fCurrentGauge);
}

//=============================================================================
// �֐����F���l�̑��
// �֐��̊T�v�F�_���[�W��񕜂Ȃ�
//=============================================================================
void CUiGaugeBillBoard::AddValueSlip(float fValue)
{
	m_fDestValue = fValue;
	if (m_fDestValue <= 0.0f)
	{
		m_fDestValue = 0.0f;
	}
	if (m_fDestValue >= m_fMaxGauge)
	{
		m_fDestValue = m_fMaxGauge;
	}
	m_fValue = (m_fDestValue - m_fCurrentGauge);
}

