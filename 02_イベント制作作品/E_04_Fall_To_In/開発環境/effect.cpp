//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : �T�c ����
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	EFFECT_TEX_NAME1	"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C���� 1
#define	EFFECT_TEX_NAME2	"data/TEXTURE/effect001.png"	// �ǂݍ��ރe�N�X�`���t�@�C���� 2
#define	EFFECT_TEX_NAME3	"data/TEXTURE/Explosion000.png"	// �ǂݍ��ރe�N�X�`���t�@�C���� 2
#define	EFFECT_TEX_NAME4	"data/TEXTURE/ink.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C���� 2

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSceneEffect::m_apTexture[CSceneEffect::TYPE_MAX] = {};
CSceneEffect *CEffect::m_apEffect[MAX_EFFECT] = {};

//=============================================================================
// �e�N�X�`���̐�������
//=============================================================================
HRESULT CSceneEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (pDevice == NULL)
	{// �擾���s�����ꍇ
		return E_FAIL;

	}

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME1, &m_apTexture[TYPE_CIRCLE]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME2, &m_apTexture[TYPE_SHINY]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME3, &m_apTexture[TYPE_EXPLOSION]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME4, &m_apTexture[TYPE_INK]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̊J������
//=============================================================================
void CSceneEffect::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneEffect::CSceneEffect()
{
	m_pVtxBuff = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0.0f;
	m_fRadiusInit = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nLife = 0;
	m_fRot = 0.0f;
	m_fAlpha = 0.0f;
	m_fLength = 0.0f;
	m_type = TYPE_CIRCLE;
	m_scaleType = SCALETYPE_NONE;
	m_fTrans = 0.0f;
	m_fAlpha = 0.0f;
	m_nCounter = 0;
	m_bUse = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneEffect::~CSceneEffect()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneEffect::Init(void)
{
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (pDevice == NULL)
	{// �擾���s�����ꍇ
		return E_FAIL;
	}

	// ���̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0.0f;
	m_fRadiusInit = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nLife = 0;
	m_fRot = 0.0f;
	m_fAlpha = 0.0f;
	m_fLength = 0.0f;
	m_type = TYPE_CIRCLE;
	m_scaleType = SCALETYPE_NONE;
	m_fTrans = 0.0f;
	m_fAlpha = 0.0f;
	m_nCounter = 0;
	m_bUse = false;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_3D
		, D3DPOOL_MANAGED
		, &m_pVtxBuff
		, NULL);

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.0f);

	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
// �I������
//=============================================================================
void CSceneEffect::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneEffect::Update(void)
{
	// �g�p����Ă��Ȃ��ꍇ���������Ȃ�
	if (!m_bUse) return;

	// �ʒu�X�V
	m_pos += m_move;

	switch (m_scaleType)
	{// �T�C�Y�ω��̎�ނ��Ƃ̏���
	case SCALETYPE_NONE:
		break;
	case SCALETYPE_UP:
		m_fRadius += m_fTrans / m_nLife;
		break;
	case SCALETYPE_DOWN:
		m_fRadius -= m_fTrans / m_nLife;
		break;
	case SCALETYPE_UPDOWN:
		m_nCounter++;
		m_fRadius = sinf(D3DX_PI / 15 * m_nCounter) * m_fTrans + m_fRadiusInit;
		break;
	default:
		break;
	}

	if (m_nLife > 0)
	{// ����������ꍇ
		m_col.a -= m_fAlpha;	// ���X�ɓ�����
		m_nLife--;
		if (m_nLife <= 0)
		{// �\�����Ԃ������Ȃ����ꍇ
			m_bUse = false;
		}
	}
	switch (m_type)
	{
	case TYPE_EXPLOSION:
		if (m_move.x + m_move.z <= 0)
		{
			m_fRot += -0.1f;
		}
		else
		{
			m_fRot += 0.1f;
		}
		m_move.x *= 0.97f;
		m_move.z *= 0.97f;
		m_move.y += 0.01f;
		break;
	}

	// �����Ă��Ȃ��ꍇ
	if (m_scaleType == SCALETYPE_NONE && m_nLife <= 0) return;

	VERTEX_3D *pVtx;
	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(sinf(m_fRot - m_fAngle) * m_fLength, cosf(m_fRot - m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fRot + m_fAngle) * m_fLength, cosf(m_fRot + m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(sinf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, cosf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, cosf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, 0.0f);

	// ���_�J���[�̍X�V
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneEffect::Draw(void)
{
	if (!m_bUse) return;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();
	D3DXMATRIX mtxView, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �f�o�C�X�̎擾
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �t�s���ݒ�
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

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
	pDevice->SetTexture(0, m_apTexture[m_type]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP	// �v���~�e�B�u�̎��
							, 0					// �J�n���钸�_�̃C���f�b�N�X
							, 2);				// �`�悷��v���~�e�B�u�̐�

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
void CSceneEffect::SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const TYPE type, const SCALETYPE scaleType, const float fTrans)
{
	m_pos = pos;
	m_move = move;
	m_fRadius = fRadius;
	m_fRadiusInit = fRadius;
	m_col = col;
	m_nLife = nLife;
	m_fRot = fRot;
	m_fAngle = atan2f(m_fRadius / 2.0f, m_fRadius / 2.0f);
	m_fLength = sqrtf((m_fRadius / 2.0f) * (m_fRadius / 2.0f) + (m_fRadius / 2.0f) * (m_fRadius / 2.0f));
	m_type = type;
	m_scaleType = scaleType;
	m_fTrans = fTrans;
	m_fAlpha = 0.0f;
	m_nCounter = 0;

	if (m_nLife != 0)
	{// �[�������
		m_fAlpha = m_col.a / m_nLife;
	}

	VERTEX_3D *pVtx;
	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(sinf(m_fRot - m_fAngle) * m_fLength, cosf(m_fRot - m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fRot + m_fAngle) * m_fLength, cosf(m_fRot + m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(sinf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, cosf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, cosf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	m_bUse = true;
}

//=============================================================================
// ��������
//=============================================================================
CEffect *CEffect::Create(void)
{
	CEffect *pEffect = NULL;

	// �N���X�̐���
	pEffect = new CEffect;

	if (pEffect == NULL)
	{// �����ł��Ȃ������ꍇ
		return NULL;
	}

	// ����������
	if (FAILED(pEffect->Init()))
	{
		return NULL;
	}

	return pEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect() : CScene(PRIORITY_EFFECT)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		m_apEffect[nCntEffect] =NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] != NULL) continue;

		m_apEffect[nCntEffect] = new CSceneEffect;

		if (m_apEffect[nCntEffect] == NULL)
		{// �����ł��Ȃ������ꍇ
			return E_FAIL;
		}

		// ����������
		if (FAILED(m_apEffect[nCntEffect]->Init()))
		{
			return E_FAIL;
		}
	}

	// ��ނ̐ݒ�
	SetObjType(OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	// �G�t�F�N�g�N���X�̏I������
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] != NULL)
		{
			m_apEffect[nCntEffect]->Uninit();
			delete m_apEffect[nCntEffect];
			m_apEffect[nCntEffect] = NULL;
		}
	}

	// ���̃N���X�̊J������
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	// �G�t�F�N�g�N���X�̍X�V����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] == NULL) continue;

		m_apEffect[nCntEffect]->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	// �G�t�F�N�g�N���X�̕`�揈��
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] == NULL) continue;

		m_apEffect[nCntEffect]->Draw();
	}
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
void CEffect::SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const CSceneEffect::TYPE type, const CSceneEffect::SCALETYPE scaleType, const float fTrans)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] == NULL) continue;

		if (!m_apEffect[nCntEffect]->GetUse())
		{// ���g�p�G�t�F�N�g�̏ꍇ
			m_apEffect[nCntEffect]->SetEffect(pos, move, fRadius, col, nLife, fRot, type, scaleType, fTrans);
			break;
		}
	}
}
//=============================================================================
// ����
//=============================================================================
void CEffect::SetExplosion(const D3DXVECTOR3 pos, const int nPlayerID)
{
	//	CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_EXOLOSION);

	for (int nCnt = 0; nCnt < 30; nCnt++)
	{
		float fMove = CManager::GetRandBetWeen(2, 8);
		float fRadius = CManager::GetRandBetWeen(30, 50);
		SetEffect(pos, D3DXVECTOR3(sinf(CManager::GetRandomPI()) * fMove, rand()%20/10.0f, cosf(CManager::GetRandomPI()) * fMove), fRadius,
			D3DXCOLOR(1.0f, rand()%100/100.0f, 0.0f, 1.0f),
			120, 0.0f, CSceneEffect::TYPE_EXPLOSION, CSceneEffect::SCALETYPE_UP, 10);
	}
}