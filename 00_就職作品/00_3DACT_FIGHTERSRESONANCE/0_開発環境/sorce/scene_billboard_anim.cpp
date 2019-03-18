//=============================================================================
//
// �r���{�[�h����[3D�|���S��] [scene_billboard.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "scene_billboard_anim.h"
#include "camera.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager *CSceneBillBoardAnim::m_pManager = NULL;
//=============================================================================
// �֐����F�r���{�[�h�̐���
// �֐��̊T�v�F�r���{�[�h�𐶐�����
//=============================================================================
CSceneBillBoardAnim *CSceneBillBoardAnim::Create(D3DXVECTOR3 pos)
{
	CSceneBillBoardAnim  *pSceneBillboard;
	pSceneBillboard = new CSceneBillBoardAnim;//�V�[���N���X�̐���
	if (pSceneBillboard != NULL)
	{
		pSceneBillboard->m_pos = pos;
		pSceneBillboard->m_fWidth = 5.0f;
		pSceneBillboard->m_fHeight = 10.0f;
		pSceneBillboard->m_fRadius = 10.0f;

		pSceneBillboard->Init();

	}
	return pSceneBillboard;
}

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CSceneBillBoardAnim::CSceneBillBoardAnim(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
										  //m_pTexture = NULL;
										  //m_pVtxBuff = NULL;
	m_nNumSceneBillBoard++;
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CSceneBillBoardAnim::~CSceneBillBoardAnim()
{

}

//=============================================================================
// �֐����F�r���{�[�h�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CSceneBillBoardAnim::Init(void)
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
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.0f);

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
// �֐����F�r���{�[�h�̏I������
// �֐��̊T�v�F���g��j������
//=============================================================================
void CSceneBillBoardAnim::Uninit(void)
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
// �֐����F�r���{�[�h�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CSceneBillBoardAnim::Update(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;


	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �֐����F�r���{�[�h�̕`�揈��
// �֐��̊T�v�F�s���ς��Ă����Ƃ�����������悤��
//=============================================================================
void CSceneBillBoardAnim::Draw(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	D3DXMATRIX pMtxView;
	pMtxView = pCamera->GetmtxView();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//�}�g���b�N�X���

	//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	pDevice->GetTransform(D3DTS_VIEW, &pMtxView);

	m_mtxWorld._11 = pMtxView._11;
	m_mtxWorld._12 = pMtxView._21;
	m_mtxWorld._13 = pMtxView._31;
	m_mtxWorld._21 = pMtxView._12;
	m_mtxWorld._22 = pMtxView._22;
	m_mtxWorld._23 = pMtxView._32;
	m_mtxWorld._31 = pMtxView._13;
	m_mtxWorld._32 = pMtxView._23;
	m_mtxWorld._33 = pMtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, m_pTexture);


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//=============================================================================
// �֐����F�G�t�F�N�g�`�揈��
// �֐��̊T�v�F�A���t�@�u�����h�̐ݒ�
//=============================================================================
void CSceneBillBoardAnim::DrawEffect(void)
{

	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//�}�g���b�N�X���

								//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	D3DXMATRIX pMtxView;
	pMtxView = pCamera->GetmtxView();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);




	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	pDevice->GetTransform(D3DTS_VIEW, &pMtxView);

	m_mtxWorld._11 = pMtxView._11;
	m_mtxWorld._12 = pMtxView._21;
	m_mtxWorld._13 = pMtxView._31;
	m_mtxWorld._21 = pMtxView._12;
	m_mtxWorld._22 = pMtxView._22;
	m_mtxWorld._23 = pMtxView._32;
	m_mtxWorld._31 = pMtxView._13;
	m_mtxWorld._32 = pMtxView._23;
	m_mtxWorld._33 = pMtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, m_pTexture);


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// �֐����F�r���{�[�h�̑������擾
// �֐��̊T�v�F�r���{�[�h�̑�����Ԃ�
//=============================================================================
int CSceneBillBoardAnim::GetNumSceneNillBoardAll(void)
{
	return m_nNumSceneBillBoard;
}

//=============================================================================
// �֐����F�r���{�[�h�̈ʒu���擾
// �֐��̊T�v�F�r���{�[�h�̈ʒu��Ԃ�
//=============================================================================
D3DXVECTOR3 CSceneBillBoardAnim::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �֐����F�r���{�[�h�̈ʒu��ݒ�
// �֐��̊T�v�F�r���{�[�h�̈ʒu��ݒ肷��
//=============================================================================
void CSceneBillBoardAnim::SetPos(D3DXVECTOR3 pos)
{


	//�ʒu������
	m_pos = pos;

}

//=============================================================================
// �֐����F�r���{�[�h�̊p�x���擾
// �֐��̊T�v�F�r���{�[�h�̊p�x��Ԃ�
//=============================================================================
D3DXVECTOR3 CSceneBillBoardAnim::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �֐����F�r���{�[�h�̊p�x��ݒ�
// �֐��̊T�v�F�r���{�[�h�̊p�x��ݒ肷��
//=============================================================================
void CSceneBillBoardAnim::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;


}

void CSceneBillBoardAnim::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
// �֐����F�r���{�[�h�Ƀe�N�X�`�������蓖��
// �֐��̊T�v�Fm_pTexture�Ƀe�N�X�`����ݒ肷��
//=============================================================================
void CSceneBillBoardAnim::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// �֐����F�r���{�[�h�̐F��ݒ�
// �֐��̊T�v�F�r���{�[�h�̐F��ݒ肷��
//=============================================================================
void CSceneBillBoardAnim::SetColor(D3DXCOLOR col)
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

//=============================================================================
// �֐����F�r���{�[�h�̃e�N�X�`���ݒ�
// �֐��̊T�v�F�e�N�X�`�����W��ݒ肷��
//=============================================================================
void CSceneBillBoardAnim::SetTexture(float fTex_X, float fTex_Y)
{
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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}
//=============================================================================
// �֐����F�r���{�[�h�̃e�N�X�`���ݒ�
// �֐��̊T�v�F�e�N�X�`�����W��ݒ肷��
//=============================================================================
void CSceneBillBoardAnim::SetTextureAnim(float fTex_X, int nPatternAnim)
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
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();


}