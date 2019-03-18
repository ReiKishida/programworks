//=============================================================================
//
// �O�Ղ̏���[3D�|���S��] [scene_meshorbit.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "scene_meshorbit.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMeshOrbit::m_pTexture = NULL;
//=============================================================================
// �֐����F3D�V�[���̐���
// �֐��̊T�v�F3D�V�[���𐶐�����
//=============================================================================
CMeshOrbit *CMeshOrbit::Create()
{
	CMeshOrbit  *pOrbit;
	pOrbit = new CMeshOrbit;//�V�[���N���X�̐���
	if (pOrbit != NULL)
	{
		pOrbit->m_fWidth = 50.0f;
		pOrbit->m_fHeight = 40.0f;
		pOrbit->Init();

	}
	return pOrbit;
}

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CMeshOrbit::CMeshOrbit(int nPriority, OBJTYPE type) : CScene(3, type)
{
	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	m_nNumScene3D++;
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}

//=============================================================================
// �֐����F3D�V�[���̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{

	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gradation002.jpg", &m_pTexture);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{
		//���_���W
		pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


		//�@���̐ݒ�
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[
		pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * 0.01f, nCntVtx * 0.01f);
	}
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	m_aOffSet[0] = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	m_aOffSet[1] = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �֐����F3D�V�[���̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�A���g�̔j��
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

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
// �֐����F3D�V�[���̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CMeshOrbit::Update(void)
{


}

//=============================================================================
// �֐����F3D�V�[���̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CMeshOrbit::Draw(void)
{

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;


	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxrot, mtxtrans;	//�}�g���b�N�X���

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ���ʂ��J�����O

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int nCntVtx = 99; nCntVtx > 1; nCntVtx--)
	{
		m_aPosVertex[nCntVtx] = m_aPosVertex[nCntVtx - 2];
	}

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{
		D3DXVec3TransformCoord(&m_aPosVertex[nCntVtx], &m_aOffSet[nCntVtx], m_pMtxParent);
	}

	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{
		//���_�o�b�t�@�����b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[nCntVtx].pos = m_aPosVertex[nCntVtx];

		//���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

	}

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 98);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O


	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);





}

//=============================================================================
// �֐����F�ʒu���̎擾
// �֐��̊T�v�F�ʒu����Ԃ�
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetPosPolygon(void)
{
	return m_posPolygon;
}

//=============================================================================
// �֐����F�ʒu���̐ݒ�
// �֐��̊T�v�F�ʒu����ݒ肷��
//=============================================================================
void CMeshOrbit::SetPosPolygon(D3DXVECTOR3 pos)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	//�ʒu������
	m_posPolygon = pos;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �֐����F�p�x���̎擾
// �֐��̊T�v�F�ʒu����Ԃ�
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetRotPolygon(void)
{
	return m_rotPolygon;
}

//=============================================================================
// �֐����F�p�x���̐ݒ�
// �֐��̊T�v�F�p�x����ݒ肷��
//=============================================================================
void CMeshOrbit::SetRotPolygon(D3DXVECTOR3 rot)
{


	m_rotPolygon = rot;


}

//=============================================================================
// �֐����F�}�g���b�N�X���̐ݒ�
// �֐��̊T�v�F�ʒu����ݒ肷��
//=============================================================================
void CMeshOrbit::SetMtxParent(D3DXMATRIX *pMtx)
{
	m_pMtxParent = pMtx;
}

//=============================================================================
// �֐����F�e�N�X�`���̂�肠��
// �֐��̊T�v�Fm_pTexture�Ƀe�N�X�`�������蓖�Ă�
//=============================================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// �֐����F�F���̐ݒ�
// �֐��̊T�v�F���_���ɐF������
//=============================================================================
void CMeshOrbit::SetColor(D3DXCOLOR col)
{
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

void CMeshOrbit::CreateOrbit(void)
{
}



