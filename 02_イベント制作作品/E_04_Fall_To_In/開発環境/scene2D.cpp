//=============================================================================
//
// 2D�I�u�W�F�N�g���� [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
//*********************************************************************************
// �ÓI�����o�ϐ��錾
//*********************************************************************************
LPDIRECT3DTEXTURE9	CScene2D::m_pTextureThis = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority):CScene(nPriority)
{
	m_pTexture = NULL;								// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);		// �ʒu
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_fWidth = 0;							// ��
	m_fHeight = 0;							// ����
	m_fAngle[0] = 0;						// �p�x
	m_fLength[0] = 0;						// ����
	m_fAngle[1] = 0;						// �p�x
	m_fLength[1] = 0;						// ����
	m_fRot = 0;								// ��]��
	m_VtxType = VTXTYPE_CENTER;				// ���S�_
	m_bUse = true;							// �g�p����Ă��邩�ǂ���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CScene2D::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/CatFox.png",
		&m_pTextureThis);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CScene2D::UnLoad(void)
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
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CScene2D::Init(void)
{

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �|���S���̈ʒu��ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_fWidth = 0;
	m_fHeight = 0;
	m_fAngle[0] = 0;						// �p�x
	m_fLength[0] = 0;						// ����
	m_fAngle[1] = 0;						// �p�x
	m_fLength[1] = 0;						// ����
	m_fRot = 0;
	m_VtxType = VTXTYPE_CENTER;				// ���S�_
	m_bUse = true;							// �g�p����Ă��邩�ǂ���

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-(m_fWidth / 2), -(m_fHeight / 2), 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3( (m_fWidth / 2), -(m_fHeight / 2), 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-(m_fWidth / 2),  (m_fHeight / 2), 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3( (m_fWidth / 2),  (m_fHeight / 2), 0.0f) + m_pos;
	// rhw�̐ݒ�
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
	}
	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CScene2D::Uninit(void)
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

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	if (!m_bUse)return;
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}
//=============================================================================
// ����
//=============================================================================
CScene2D *CScene2D::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CScene2D *pScene2D = NULL;
	if (!CScene::OverData())
	{// �QD�|���S������
		pScene2D = new CScene2D;
	}
	if (pScene2D != NULL)
	{
		// ����������
		if (FAILED(pScene2D->Init()))
		{
			return NULL;
		}
		pScene2D->SetPos(pos);
		pScene2D->SetSize(fWidth, fHeight);
		pScene2D->BindTexture(m_pTextureThis);
		pScene2D->SetVtx();
	}
	return pScene2D;
}
//=============================================================================
// ���W�ݒ�
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_posOld = m_pos;
	m_pos = pos;
}
//=============================================================================
// �������W�ݒ�
//=============================================================================
void CScene2D::SetInitPos(D3DXVECTOR3 pos)
{
	m_Initpos = pos;
}
//=============================================================================
// �傫���ݒ�
//=============================================================================
void CScene2D::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;		// ��
	m_fHeight = fHeight;	// ����
	switch (m_VtxType)
	{// ���S�_�̈ʒu���ƂɌv�Z��ς���
	case VTXTYPE_CENTER:
		m_fAngle[0] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));
		m_fAngle[1] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[1] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));
		break;
	case VTXTYPE_BASE:
		// 01 ���_�̒�����]
		m_fAngle[0] = atan2f((m_fWidth / 2), (-m_fHeight));
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight)*(m_fHeight));
		// 23 ���_�̒�����]
		m_fAngle[1] = atan2f((m_fWidth / 2), (0));
		m_fLength[1] = m_fWidth / 2;
		break;
	case VTXTYPE_LEFT:
		break;
	case VTXTYPE_RIGHT:
		break;
	}
}
//=============================================================================
// ��]�ݒ�
//=============================================================================
void CScene2D::SetRot(float fRot)
{
  	m_fRot = fRot;

}
//=============================================================================
// �F�ݒ�
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// �e�N�X�`�����W�ݒ�[�p�^�[��]
//=============================================================================
void CScene2D::SetPattern(const D3DXVECTOR2 uv, const int nPatternX, const int nPatternY, const bool bTurn)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 1.0f : 0.0f)), uv.y * nPatternY);
	pVtx[1].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 0.0f : 1.0f)), uv.y * nPatternY);
	pVtx[2].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 1.0f : 0.0f)), uv.y * (nPatternY + 1));
	pVtx[3].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 0.0f : 1.0f)), uv.y * (nPatternY + 1));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �e�N�X�`�����W�ݒ�
//=============================================================================
void CScene2D::SetTex(D3DXVECTOR2 tex)
{
	m_tex = tex;
	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f) + m_tex;
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f) + m_tex;
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f) + m_tex;
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f) + m_tex;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();


}
//=============================================================================
// ���_���W
//=============================================================================
void CScene2D::SetVtx(void)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_VtxType)
	{// ���S�_�̈ʒu���ƂɌv�Z��ς���
	case VTXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;

		break;
	case VTXTYPE_BASE:
		pVtx[0].pos = D3DXVECTOR3(sinf( - m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(-m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		break;

	case VTXTYPE_LEFT:
		pVtx[0].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		break;
	case VTXTYPE_RIGHT:
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;

		break;
	}

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���_�ݒ�
//=============================================================================
void CScene2D::SetVtxType(VTXTYPE VtxType)
{
	m_VtxType = VtxType;
}
//=============================================================================
// �g�p����Ă��邩�ǂ���
//=============================================================================
void CScene2D::SetUse(const bool bUse)
{
	m_bUse = bUse;
}
void CScene2D::SetStamp(const float fWidth, const float fHeight)
{
	m_fWidth += (fWidth - m_fWidth)*0.3f;
	m_fHeight += (fHeight - m_fHeight)*0.3f;
	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	switch (m_VtxType)
	{// ���S�_�̈ʒu���ƂɌv�Z��ς���
	case VTXTYPE_CENTER:
		m_fAngle[0] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));
		m_fAngle[1] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[1] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));

		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;

		break;
	case VTXTYPE_BASE:
		// 01 ���_�̒�����]
		m_fAngle[0] = atan2f((m_fWidth / 2), (-m_fHeight));
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight)*(m_fHeight));
		// 23 ���_�̒�����]
		m_fAngle[1] = atan2f((m_fWidth / 2), (0));
		m_fLength[1] = m_fWidth / 2;

		pVtx[0].pos = D3DXVECTOR3(sinf(-m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(-m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		break;

	case VTXTYPE_LEFT:
		pVtx[0].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		break;
	case VTXTYPE_RIGHT:
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;

		break;
	}
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}