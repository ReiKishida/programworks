//=============================================================================
//
// �J�b�g�C������[2D�|���S��] [cutin.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "cutin.h"
#include "fade.h"
#include "bg.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOJI_WIDTH (80.0f)	//�|���S���̕�
#define MOJI_HEIGHT (85.0f)	//�|���S����Y�̍���
#define CUT_WIDTH (780.0f)	//�|���S���̕�
#define CUT_HEIGHT (250.0f)	//�|���S����Y�̍���

#define ENEMY_MOVE (1.5f)	//�J�b�g�C���̈ړ���

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CCutin::m_apTexture[MAX_TEXTURE_CUTIN] = {}; //���L�e�N�X�`���̃|�C���^
int CCutin::m_nPatternAnim = 0;	//�A�j���[�V�����p�^�[��
int CCutin::m_nCounterAnim = 0;//�A�j���[�V�����J�E���^�[
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CCutin::CCutin() : CScene2D(5)
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CCutin::~CCutin()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CCutin::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ryou.png", &m_apTexture[0]);//��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/iki.png", &m_apTexture[1]);//��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ten.png", &m_apTexture[2]);//�W
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/kai.png", &m_apTexture[3]);//�J
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cut_player.png", &m_apTexture[4]);//�v���C���[�̃J�b�g�C��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cut_enemy.png", &m_apTexture[5]);//�{�X�̃J�b�g�C��

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CCutin::Unload(void)
{
	for (int nCntCutinTex = 0; nCntCutinTex < MAX_TEXTURE_CUTIN; nCntCutinTex++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntCutinTex] != NULL)
		{
			m_apTexture[nCntCutinTex]->Release();
			m_apTexture[nCntCutinTex] = NULL;
		}
	}
}



//=============================================================================
//�J�b�g�C���̐���
//=============================================================================
CCutin *CCutin::Create(D3DXVECTOR3 pos, CUTINTYPE type)
{



	//�J�b�g�C���̃C���X�^���X����
	CCutin *pCutin;
	pCutin = new CCutin;
	if (pCutin != NULL)
	{
		//�J�b�g�C���̏�����
		pCutin->Init(pos, type);
	}
	return pCutin;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CCutin::Init(D3DXVECTOR3 pos, CUTINTYPE type)
{
	//�^�C�v�̊��蓖��
	m_Type = type;


	//�e�N�X�`���̊��蓖��
	switch (m_Type)
	{
	case CUTIN_TYPE_RYOU:
		BindTexture(m_apTexture[0]);
		//�J�b�g�C���̕��A�����ݒ�
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_RYOU;

		break;
	case CUTIN_TYPE_IKI:
		BindTexture(m_apTexture[1]);
		//�J�b�g�C���̕��A�����ݒ�
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_IKI;

		break;
	case CUTIN_TYPE_TEN:
		BindTexture(m_apTexture[2]);
		//�J�b�g�C���̕��A�����ݒ�
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_TEN;

		break;
	case CUTIN_TYPE_KAI:
		BindTexture(m_apTexture[3]);
		//�J�b�g�C���̕��A�����ݒ�
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_KAI;

		break;
	case CUTIN_TYPE_CUT_PLAYER:
		BindTexture(m_apTexture[4]);
		//�J�b�g�C���̕��A�����ݒ�
		SetScene2DWidth(CUT_WIDTH);
		SetScene2DHeight(CUT_HEIGHT);
		m_nLife = MAX_LIFE_CUTIN;
		m_nPatternAnim = 0;
		m_nCounterAnim = 0;
		break;
	case CUTIN_TYPE_CUT_ENEMY:
		BindTexture(m_apTexture[5]);
		//�J�b�g�C���̕��A�����ݒ�
		SetScene2DWidth(CUT_WIDTH);
		SetScene2DHeight(CUT_HEIGHT);
		m_nLife = MAX_LIFE_CUTIN;
		m_nPatternAnim = 0;
		m_nCounterAnim = 0;
		break;
	}


	//2D�V�[���̏���������
	CScene2D::Init(pos);

	if (m_Type == CUTIN_TYPE_CUT_PLAYER || m_Type == CUTIN_TYPE_CUT_ENEMY)
	{
		SetTexture(0.5f, 1.0f);
	}


	//�J�b�g�C���̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_CUTIN);



	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CCutin::Uninit(void)
{

	//�|���S���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CCutin::Update(void)
{

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	D3DXVECTOR3 moveCutin; //�J�b�g�C���̈ړ���
	D3DXVECTOR3 posCutin = GetPosScene2D();	//�J�b�g�C���̈ʒu���
	D3DXVECTOR3 rotCutin = GetRotScene2D();	//�J�b�g�C���̊p�x���
	D3DXVECTOR3 BulletMove;
	D3DXVECTOR3 playerPos;//�v���C���[�̈ʒu
	CPlayer *pPlayer;//�v���C���[
	pPlayer = CManager::GetPlayer();//�v���C���[�̎擾
	playerPos = pPlayer->GetPos();//�v���C���[�̈ʒu���擾


	if (m_Type == CUTIN_TYPE_CUT_PLAYER || m_Type == CUTIN_TYPE_CUT_ENEMY)
	{
		m_nCounterAnim++;//�A�j���[�V�����J�E���^�[�̉��Z

		if (m_nCounterAnim % 60 == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % 2;//�p�^�[��No.�X�V

			//�e�N�X�`���A�j���[�V����
			CScene2D::SetTextureAnim(0.5, 1.0f, m_nPatternAnim);
		}
	}
	//�ʒu���̐ݒ�
	SetPosScene2D(posCutin);

	//���C�t�̌���
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CCutin::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
//�`�揈��
//=============================================================================
void CCutin::NextAnim(void)
{
}

