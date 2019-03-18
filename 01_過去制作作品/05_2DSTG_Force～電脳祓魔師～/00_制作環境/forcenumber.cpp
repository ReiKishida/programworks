//=============================================================================
//
// �X�R�A�̏���[2D�|���S��] [score.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "number.h"
#include "forcenumber.h"
#include "force.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CNumber *CForceNumber::m_apNumber[FORCE_NUMBER] = {};
int CForceNumber::m_nForce = 0;
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CForceNumber::CForceNumber() :CScene(7)
{//�N���A�Ȓl����
	m_nForce = 0;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CForceNumber::~CForceNumber()
{


}



//=============================================================================
//�X�R�A�̐���
//=============================================================================
CForceNumber *CForceNumber::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CForceNumber *pFoece;
	pFoece = new CForceNumber;

	if (pFoece != NULL)
	{
		//�X�R�A�̏���������
		pFoece->Init(pos);
	}

	return pFoece;



}


//=============================================================================
//����������
//=============================================================================
HRESULT CForceNumber::Init(D3DXVECTOR3 pos)
{
	for (int nCntNumber = 0; nCntNumber < FORCE_NUMBER; nCntNumber++)
	{
		//�����̐���
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{


			//����������
			m_apNumber[nCntNumber]->SetNumberWidth(30.0f);
			m_apNumber[nCntNumber]->SetNumberHeight(35.0f);
			m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x, pos.y + (60.0f * nCntNumber), pos.z));


			m_nForce = 0;

			//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_FORCENUMBER);
		}
	}

	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CForceNumber::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < FORCE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			//2D�V�[���̏I������
			m_apNumber[nCntNumber]->Uninit();
			m_apNumber[nCntNumber] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CForceNumber::Update(void)
{
	int nNumAttack = CForce::GetNumAttack();
	int nNumSpread = CForce::GetNumSpread();
	int nNumSpeed = CForce::GetNumSpeed();
	int nNumDefense = CForce::GetNumDefense();

	//�U�̃t�H�[�X
	m_apNumber[0]->SetNumber(nNumAttack);
	m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f,1.0f));

	//�g�̃t�H�[�X
	m_apNumber[1]->SetNumber(nNumSpread);
	m_apNumber[1]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	//���̃t�H�[�X
	m_apNumber[2]->SetNumber(nNumSpeed);
	m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//��
	m_apNumber[3]->SetNumber(nNumDefense);
	m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));

}

//=============================================================================
//�`�揈��
//=============================================================================
void CForceNumber::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < FORCE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
//�X�R�A�̐ݒ�
//=============================================================================
void CForceNumber::SetForce(int nFoece)
{
	m_nForce = nFoece;



}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CForceNumber::GetForce(void)
{
	return m_nForce;
}

//=============================================================================
//�X�R�A�̉��Z
//=============================================================================
void CForceNumber::AddForce(int nValue)
{


}

