////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �RD�|���S������ [scene3D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "line.h"
//*********************************************************************************
// �ÓI�����o�ϐ��錾
//*********************************************************************************
LPDIRECT3DTEXTURE9	CScene3D::m_pTextureThis = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene3D::CScene3D(int nPriority ):CScene(nPriority)
{
	m_pTexture = NULL;								// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// �F
	m_fWidth = 0;							// ��
	m_fHeight = 0;							// ����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]��
	m_fAngle = 0;	// �p�x
	m_fLength=0;	// ����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CScene3D::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg.jpg",
		&m_pTextureThis);


	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CScene3D::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTextureThis != NULL)
	{
		m_pTextureThis->Release();
		m_pTextureThis = NULL;
	}
}
//=============================================================================
// �e�N�X�`�����蓖��
//=============================================================================
void CScene3D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==============================================================================================================================
// ����������
//==============================================================================================================================
HRESULT CScene3D::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// �F
	m_fWidth = 0;								// ��
	m_fHeight = 0;								// ����
	m_fDepth = 0;								// ���s��
	m_fAngle = 0;								// �p�x
	m_fLength = 0;								// ����
	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// �I�u�W�F�N�g�̔j��
	Release();

}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void CScene3D::Update(void)
{

}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void CScene3D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

												// �A���t�@�e�X�g�̏���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//==============================================================================================================================
// ����
//==============================================================================================================================
void CScene3D::Collision(D3DXVECTOR3 pos)
{

	D3DXVECTOR3 VecA[4];// ���b�V���̃x�N�g��
	D3DXVECTOR3 VexC[4];// �v���C���[�ƒ��_�̃x�N�g��
	float fVecLine[4];	// ���_�x�N�g�����v���C���[�����������ǂ���
	D3DXVECTOR3 meshPos[4] =
	{
		m_aVtxPos[0],
		m_aVtxPos[1],
		m_aVtxPos[3],
		m_aVtxPos[2],
	};
	VecA[0] = meshPos[0] - meshPos[1];
	VecA[1] = meshPos[1] - meshPos[2];
	VecA[2] = meshPos[2] - meshPos[3];
	VecA[3] = meshPos[3] - meshPos[0];
	int nIn = 0;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{// �v���C���[�ƃ��b�V���̃x�N�g��
		VexC[nCnt] = (pos - meshPos[nCnt]);

		fVecLine[nCnt] = (VexC[nCnt].z * VecA[nCnt].x) - (VexC[nCnt].x * VecA[nCnt].z);
		if (fVecLine[nCnt] > 0)
		{// ������
			nIn++;
		}
	}
	if (nIn == 4)
	{
		// ���_�J���[�̐ݒ�
		SetVtx();
	}

}
//=============================================================================
// ����
//=============================================================================
CScene3D *CScene3D::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth)
{
	CScene3D *pScene3D = NULL;
	if (!CScene::OverData())
	{// �QD�|���S������
		pScene3D = new CScene3D;
	}
	if (pScene3D != NULL)
	{
		// ����������
		if (FAILED(pScene3D->Init()))
		{
			return NULL;
		}
		pScene3D->SetPos(pos);
		pScene3D->SetSize(fWidth, fHeight, fDepth);
	//	pScene3D->BindTexture(m_pTextureThis);
		pScene3D->SetVtx();
	}
	return pScene3D;
}
//==============================================================================================================================
// ���W�ݒ�
//==============================================================================================================================
void CScene3D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// �ړ���
//==============================================================================================================================
void CScene3D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}
//==============================================================================================================================
// �傫���ݒ�
//==============================================================================================================================
void CScene3D::SetSize(const float fWidth, const float fHeight, const float fDepth)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	m_fAngle = atan2f(m_fWidth / 2, m_fDepth / 2);
	m_fLength = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fDepth / 2)* (m_fDepth / 2));
}
//==============================================================================================================================
// ��]�ݒ�
//==============================================================================================================================
void CScene3D::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// �F�ݒ�
//==============================================================================================================================
void CScene3D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}
//==============================================================================================================================
// ���_�ݒ�
//==============================================================================================================================
void CScene3D::SetVtx(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ), cosf(+m_fAngle + m_rot.y)*m_fLength) ;
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI - m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ) , cosf(-m_fAngle + m_rot.y)*m_fLength) ;
	pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ) , cosf(D3DX_PI - m_fAngle + m_rot.y)*m_fLength) ;
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ), cosf(D3DX_PI + m_fAngle + m_rot.y)*m_fLength) ;
	m_aVtxPos[0] = pVtx[0].pos + m_pos;
	m_aVtxPos[1] = pVtx[1].pos + m_pos;
	m_aVtxPos[2] = pVtx[2].pos + m_pos;
	m_aVtxPos[3] = pVtx[3].pos + m_pos;
	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
