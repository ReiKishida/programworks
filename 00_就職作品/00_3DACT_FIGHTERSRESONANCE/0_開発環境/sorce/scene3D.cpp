//=============================================================================
//
// �V�[��2D����[3D�|���S��] [scene3D.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_WIDTH (30) //�|���S����X�̑傫��
#define POLYGON_HEIGHT (30) //�|���S����Y�̑傫��
#define ANIMATION_SPEED (8)	//�A�j���[�V�����̃X�s�[�h
#define TEX_X (0.25f)	//�e�N�X�`�����W_X
#define TEX_Y (1.0f)	//�e�N�X�`�����W_Y

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager *CScene3D::m_pManager = NULL;
//=============================================================================
//3D�V�[���̐���
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CScene3D  *pScene3D;
	pScene3D = new CScene3D;//�V�[���N���X�̐���
	if (pScene3D != NULL)
	{
		pScene3D->m_posPolygon = pos;
		pScene3D->m_fWidth = fWidth;
		pScene3D->m_fHeight = fHeight;
		pScene3D->Init();

	}
	return pScene3D;
}

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CScene3D::CScene3D(int nPriority , OBJTYPE type) :CScene(nPriority, type)
{
	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
												 //m_apTexture = NULL;
												 //m_pVtxBuff = NULL;
	m_nNumScene2D++;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
//����������
//=============================================================================
HRESULT CScene3D::Init(void)
{

	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CScene3D::Uninit(void)
{

	//// �e�N�X�`���̔j��
	//if (m_pTexture != NULL)
	//{
	//	m_pTexture->Release();
	//	m_pTexture = NULL;
	//}


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
void CScene3D::Update(void)
{


}

//=============================================================================
//�`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();


	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//�}�g���b�N�X���

								//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldPolygon);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rotPolygon.y, m_rotPolygon.x, m_rotPolygon.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxrot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_posPolygon.x, m_posPolygon.y, m_posPolygon.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxtrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldPolygon);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
//3D�V�[���̑����̎擾
//=============================================================================
int CScene3D::GetNum2DSceneAll(void)
{
	return m_nNumScene2D;
}

//=============================================================================
//�|���S���̈ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CScene3D::GetPosPolygon(void)
{
	return m_posPolygon;
}

//=============================================================================
//�|���S���̈ʒu�̐ݒ�
//=============================================================================
void CScene3D::SetPosPolygon(D3DXVECTOR3 pos)
{
	// ���_���̍쐬
	VERTEX_3D *pVtx;

	//�ʒu������
	m_posPolygon = pos;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�|���S���̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CScene3D::GetRotPolygon(void)
{
	return m_rotPolygon;
}

//=============================================================================
//�|���S���̊p�x�̐ݒ�
//=============================================================================
void CScene3D::SetRotPolygon(D3DXVECTOR3 rot)
{


	m_rotPolygon = rot;


}

void CScene3D::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
}

void CScene3D::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//�e�N�X�`���̃X�N���[��
//=============================================================================
void CScene3D::ScrollPolygon(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3)
{

	// ���_����ݒ�
	VERTEX_3D *pVtx; // ���_���̍쐬


	 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = Scroll_0;
	pVtx[1].tex = Scroll_1;
	pVtx[2].tex = Scroll_2;
	pVtx[3].tex = Scroll_3;


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

void CScene3D::SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���_����ݒ�
	VERTEX_3D *pVtx; // ���_���̍쐬

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
//�e�N�X�`���̊��蓖��
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//============================================================================================
//�F�̐ݒ�
//============================================================================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx; // ���_���̍쐬


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
//�D�揇�ʂ̐ݒ�
//============================================================================================
void CScene3D::SetPriority(int nPriority)
{
	m_nPriority = nPriority;
}

