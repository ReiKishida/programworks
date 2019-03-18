//=============================================================================
//
// �V�[������[2D�|���S��] [scene2D.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "explosion.h"
#include "score.h"
#include "timer.h"
#include "sound.h"
#include "game.h"
#include "force.h"
#include "enemy.h"
#include "bg.h"
#include "particle.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_WIDTH (30) //�|���S����X�̑傫��
#define POLYGON_HEIGHT (30) //�|���S����Y�̑傫��
#define ANIMATION_SPEED (8)	//�A�j���[�V�����̃X�s�[�h
#define TEX_X (0.25f)	//�e�N�X�`�����W_X
#define TEX_Y (1.0f)	//�e�N�X�`�����W_Y

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
bool g_bDelete = false;
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager *CScene2D::m_pManager = NULL;
int CScene2D::m_nDamage = 0;

//=============================================================================
//2D�V�[���̐���
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos)
{
	CScene2D  *pScene2D;
	pScene2D = new CScene2D;//�V�[���N���X�̐���
	if (pScene2D != NULL)
	{
		pScene2D->Init(pos);

	}
	return pScene2D;
}

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority) :CScene(nPriority)
{
	m_rotScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	m_posScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F
	m_fScene2DWidth = 0.0f;	//�|���S���̕�
	m_fScene2DHeight = 0.0f;	//�|���S���̍���
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_nNumScene2D++;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//����������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	//�t���[�����̃J�E���g�̏�����
	m_nCntframe = 0;
	//DPS������
	m_nDPS = 0;
	if (m_pManager == NULL)
	{
		m_pManager = new CManager;
	}
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	//�ʒu�A��]��񏉊���
	m_rotScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�F���̏�����
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_posScene2D = pos;



	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	m_fLengthScene2D = sqrtf((m_fScene2DWidth / 2 + m_fScene2DWidth / 2) * (m_fScene2DHeight / 2 + m_fScene2DHeight / 2));

	m_fAngleScene2D = atan2f(m_fScene2DWidth / 2, m_fScene2DHeight / 2);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	pVtx[1].pos = D3DXVECTOR3(m_posScene2D.x + sinf(D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	pVtx[2].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	pVtx[3].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);


	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();



	return S_OK;


	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CScene2D::Uninit(void)
{


	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();

}

//=============================================================================
//�X�V����
//=============================================================================
void CScene2D::Update(void)
{

	int m_nCntframe = CForce::GetnCnt();
	if (m_nCntframe % 60 == 0)
	{
		CheckDamage();
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	if (m_pManager == NULL)
	{
		m_pManager = new CManager;
	}
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// �f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ă������I)
	pDevice->SetTexture(0, m_pTexture);


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);



}
//=============================================================================
//�G�t�F�N�g�`�揈��
//=============================================================================
void CScene2D::DrawEffect(void)
{
	if (m_pManager == NULL)
	{
		m_pManager = new CManager;
	}
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// �f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ă������I)
	pDevice->SetTexture(0, m_pTexture);


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
//2D�V�[���̑����̎擾
//=============================================================================
int CScene2D::GetNum2DSceneAll(void)
{
	return m_nNumScene2D;
}

//=============================================================================
//�|���S���̈ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosScene2D(void)
{
	return m_posScene2D;
}

//=============================================================================
//�|���S���̈ʒu�̐ݒ�
//=============================================================================
void CScene2D::SetPosScene2D(D3DXVECTOR3 pos)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	//�ʒu������
	m_posScene2D = pos;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posScene2D.x + sinf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posScene2D.x + sinf(m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�|���S���̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CScene2D::GetRotScene2D(void)
{
	return m_rotScene2D;
}

//=============================================================================
//�|���S���̊p�x�̐ݒ�
//=============================================================================
void CScene2D::SetRotScene2D(D3DXVECTOR3 rot)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	m_rotScene2D = rot;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posScene2D.x + sinf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posScene2D.x + sinf(m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�|���S���̒����̎擾
//=============================================================================
float CScene2D::GetLengthScene2D(void)
{
	return m_fLengthScene2D;
}

//=============================================================================
//�|���S���̒����̐ݒ�
//=============================================================================
void CScene2D::SetLengthScene2D(float fLength)
{
	m_fLengthScene2D = fLength;
}

//=============================================================================
//�|���S���̒����̐ݒ�
//=============================================================================
void CScene2D::SetScene2DWidth(float fWidth)
{
	m_fScene2DWidth = fWidth;
}
//=============================================================================
//�|���S���̒����̐ݒ�
//=============================================================================
void CScene2D::SetScene2DHeight(float fHeight)
{
	m_fScene2DHeight = fHeight;

}
//=============================================================================
//�e�N�X�`���̊��蓖��
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}


//=============================================================================
//�e�N�X�`���̐ݒ�
//=============================================================================
void CScene2D::SetTexture(float fTex_X, float fTex_Y)
{
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}

//=============================================================================
//�e�N�X�`���̐ݒ�(4���_��)
//=============================================================================
void CScene2D::SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(fTex_X_0, fTex_Y_0);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, fTex_Y_0);
	pVtx[2].tex = D3DXVECTOR2(fTex_X_0, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================
//�e�N�X�`���A�j���[�V�����̐ݒ�
//=============================================================================
void CScene2D::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================
//�e�N�X�`���A�j���[�V�����̐ݒ�
//=============================================================================
void CScene2D::SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================================================
//�w�i�X�N���[��
//=============================================================================================================
void CScene2D::ScrollBg(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = Scroll_0;
	pVtx[1].tex = Scroll_1;
	pVtx[2].tex = Scroll_2;
	pVtx[3].tex = Scroll_3;


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//============================================================================================
//�F�̐ݒ�
//============================================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	//�F������
	m_color = col;

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================================
//�F�̎擾
//============================================================================================
D3DXCOLOR CScene2D::GetColor(void)
{
	return m_color;
}

//============================================================================================
//�D�揇�ʂ̐ݒ�
//============================================================================================
void CScene2D::SetPriority(int nPriority)
{
	m_nPriority = nPriority;
}
//============================================================================================
//���̎擾
//============================================================================================
float CScene2D::GetWidthScene2D(void)
{
	return m_fScene2DWidth;
}
//============================================================================================
//���̎擾
//============================================================================================
float CScene2D::GetHeightScene2D(void)
{
	return m_fScene2DHeight;

}

//============================================================================================
//���C�t�̎擾
//============================================================================================
int CScene2D::GetLife(void)
{
	return m_nLife;
}

//============================================================================================
//���C�t�̐ݒ�
//============================================================================================
void CScene2D::SetLife(int nLife)
{
	m_nLife = nLife;
}


//============================================================================================
// DPS�`�F�b�N
//============================================================================================
void  CScene2D::CheckDamage()
{
	CForce::CheckDamage(m_nDamage);
	m_nDamage = 0;
}


//============================================================================================
// �_���[�W�ʂ̎擾
//============================================================================================
int CScene2D::GetDamage(void)
{
	return m_nDamage;
}


//============================================================================================
//�q�b�g���C�t
//============================================================================================
void CScene2D::HitLife(int nDamage)
{

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//DPS�Z�o�p
	m_nDamage += nDamage;


	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE::MODE_GAME)
	{
		//���C�t����
		m_nLife -= nDamage;
	}

	//�p�[�e�B�N���̐���
	for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
	{
		CParticle::Create(m_posScene2D, D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f), CParticle::PARITCLETYPE_HIT, 35);
	}
	for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
	{
		CParticle::Create(m_posScene2D, D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f), CParticle::PARITCLETYPE_HIT_R, 35);
	}

}


//============================================================================================
//�t�H�[�X�̃e�N�X�`���ݒ�
//============================================================================================
void CScene2D::SetForceTexture(int nType)
{
	m_nType = nType;

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 1.0f);
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================================
//�t�H�[�X�̐ݒ�
//============================================================================================
void CScene2D::SetForce(int nType)
{
	m_nType = nType;


}


//============================================================================================
//�t�H�[�X�̎擾
//============================================================================================
int CScene2D::GetForce()
{
	return m_nType;
}

