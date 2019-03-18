//=============================================================================
//
// �u�[�X�g�Q�[�W���� [boost.cpp]
// Author :Kishida Rei
//
//=============================================================================
#include "boost.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "inputx.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOOST_VERTEX ((g_Boost_h + 1) * (g_Boost_v + 1))	//�u�[�X�g�Q�[�W���_��
#define BOOST_INDEX ( (g_Boost_h * g_Boost_v )* 2 + (4 * (g_Boost_v - 1)) + 2 )	//�u�[�X�g�Q�[�W�̃C���f�b�N�X��
#define BOOST_POLYGON ( (g_Boost_h * g_Boost_v) * 2 + (4 * (g_Boost_v - 1)) )//�u�[�X�g�Q�[�W�̃|���S����

#define BOOST_RADIUS (45.0f)	//���a�̑傫��
#define BOOST_RADIUS2 (7.0f)	//���a�̓���


#define BOOST_HEIGHT (5.0f)	//�u�[�X�g�Q�[�W�̍���

#define BOOST_H (100) //���b�V���̕�����(H)
#define BOOST_V (1) //���b�V���̕�����(V)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexBoost(LPDIRECT3DDEVICE9 pDevice);	//���_��񐶐�
void MakeIndexBoost(LPDIRECT3DDEVICE9 pDevice);		//�C���f�b�N�X��񐶐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBoost = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoost = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBoost = NULL; //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_posBoost;		//�ʒu(���S���W)
D3DXVECTOR3 g_rotBoost;		//����
D3DXMATRIX g_mtxWorldBoost;	//���[���h�}�g���b�N�X


int g_nBoost;	//�u�[�X�g��

int g_Boost_h, g_Boost_v;	//���̕�����,�c�̕�����
JoyState g_JoyStateBoost;	//�Q�[���p�b�h�̏��(�u�[�X�g�Q�[�W)

//=============================================================================
// ����������
//=============================================================================
void InitBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	g_Boost_h = BOOST_H;	//���̕�����
	g_Boost_v = BOOST_V;	//�c�̕�����

	int Polygon = (g_Boost_h * g_Boost_v) * 2 + (4 * (g_Boost_v - 1));	//�|���S�����̌v�Z
	g_nBoost = -200;//�u�[�X�g�c��

	MakeVertexBoost(pDevice);	//���_��񐶐�
	MakeIndexBoost(pDevice);	//�C���f�b�N�X��񐶐�

	//�v���C���[�̏��̎擾
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	//�u�[�X�g�̈ʒu���
	g_posBoost = pPlayer.pos;
	g_rotBoost = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	//�Q�[���p�b�h�̏��
	g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_NOTPUSH;



}

//=============================================================================
// �I������
//=============================================================================
void UninitBoost(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBoost != NULL)
	{
		g_pTextureBoost->Release();
		g_pTextureBoost = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBoost != NULL)
	{
		g_pVtxBuffBoost->Release();
		g_pVtxBuffBoost = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffBoost != NULL)
	{
		g_pIdxBuffBoost->Release();
		g_pIdxBuffBoost = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoost(void)
{
	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���͏��
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//�v���C���[�̏��̎擾
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	//�J�����̏��̎擾
	CAMERA pCamera;
	pCamera = *GetCamera();

	//�u�[�X�g�Q�[�W�̈ʒu�Ɗp�x
	g_posBoost = D3DXVECTOR3(pPlayer.pos.x, pPlayer.pos.y, pPlayer.pos.z);
	g_rotBoost = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f + pPlayer.rot.y, 0.0f);

	if (GetKeyboardPress(DIK_LSHIFT) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_LEFT_SHOULDER && pJoyState.bConnectionJoypad == true))
	{//�����̃V�t�g�L�[�������ꂽ�Ƃ��A�Q�[���p�b�h�̍��V�����_�[���������܂�ĂĂ���Ƃ�
		if (pPlayer.state == PLAYERSTATE_NORMAL)
		{//�v���C���[�̏�Ԃ��ʏ��Ԃ̂Ƃ�
			g_nBoost -= 2;
		}
	}

	if (GetKeyboardTrigger(DIK_RSHIFT) == true   || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true) )
	{//�E���̃V�t�g�L�[�������ꂽ�Ƃ��A�Q�[���p�b�h�̉E�V�����_�[���������܂�ĂĂ���Ƃ�
		if (pPlayer.state == PLAYERSTATE_NORMAL)
		{//�v���C���[�̏�Ԃ��ʏ��Ԃ̂Ƃ�
			if (g_JoyStateBoost.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				g_nBoost -= 60;
			}
			g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
	}
	if (g_nBoost > 0)
	{//�u�[�X�g�Q�[�W���[���̂Ƃ��̓[���ɂ���
		g_nBoost =  0;
	}


	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);
	if (pPlayer.state == PLAYERSTATE_NORMAL)
	{//�v���C���[�̏�Ԃ��ʏ��Ԃ̂Ƃ�
		for (int nCntBOOST_V = 0; nCntBOOST_V < g_Boost_v + 1; nCntBOOST_V++)
		{
			for (int nCntBOOST_H = 0; nCntBOOST_H < g_Boost_h + 1; nCntBOOST_H++)
			{
				if (nCntBOOST_V == 0)
				{
					//���_�J���[(�O��)
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				}
				if (nCntBOOST_V == 1)
				{
					//���_�J���[(����)
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
	}
	if (g_nBoost <= -100 || pPlayer.state == PLAYERSTATE_OVERHEAT)
	{//�u�[�X�g�c�ʂ����Ȃ��Ȃ��Ă����Ƃ�
		for (int nCntBOOST_V = 0; nCntBOOST_V < g_Boost_v + 1; nCntBOOST_V++)
		{
			for (int nCntBOOST_H = 0; nCntBOOST_H < g_Boost_h + 1; nCntBOOST_H++)
			{
				if (nCntBOOST_V == 0)
				{//�����̃J�E���g�O�̂Ƃ�
					//���_�J���[
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				if (nCntBOOST_V == 1)
				{//�����̃J�E���g�P�̂Ƃ�
					//���_�J���[
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBoost->Unlock();

	//�u�[�X�g�Q�[�W����
	g_nBoost++;

	//�Q�[���p�b�h�̉������ςȂ������
	if (g_JoyStateBoost.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateBoost.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxrot, mtxtrans;


	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBoost);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotBoost.y, g_rotBoost.x, g_rotBoost.z);

	D3DXMatrixMultiply(&g_mtxWorldBoost, &g_mtxWorldBoost, &mtxrot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		g_posBoost.x, g_posBoost.y, g_posBoost.z);

	D3DXMatrixMultiply(&g_mtxWorldBoost, &g_mtxWorldBoost, &mtxtrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBoost);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBoost, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffBoost);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, 0);


	// ���b�V���V�����_�[�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, BOOST_VERTEX, 0, BOOST_POLYGON + g_nBoost);



	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}


//=============================================================================
//�u�[�X�g�Q�[�W����
//=============================================================================
void UseBoost(int nDamage)
{
	g_nBoost += nDamage;
}

//=============================================================================
//�u�[�X�g�Q�[�W�擾
//=============================================================================

int GetBoost(void)
{
	return g_nBoost;
}


//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexBoost(LPDIRECT3DDEVICE9 pDevice)
{
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureBoost);


	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BOOST_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoost,
		NULL);



	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI / g_Boost_h;
	float fDestAngle;
	for (int nCntBOOST_V = 0; nCntBOOST_V < g_Boost_v + 1; nCntBOOST_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntBOOST_H = 0; nCntBOOST_H < g_Boost_h + 1; nCntBOOST_H++)
		{
			if (nCntBOOST_V == 0)
			{//�O���̐���
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.x = sinf(fDestAngle) * BOOST_RADIUS;
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.y = 1.0f;
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.z = cosf(fDestAngle) * BOOST_RADIUS;
				//�@���̐ݒ�
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


				//���_�J���[
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].tex = D3DXVECTOR2(nCntBOOST_H * (1.0f / (g_Boost_h / 2)), nCntBOOST_V * -1.0f);

				fDestAngle += fAngle;
			}
			if (nCntBOOST_V == 1)
			{//�����̐���
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.x = sinf(fDestAngle) * (BOOST_RADIUS - BOOST_RADIUS2);
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.y = 1.0f;
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.z = cosf(fDestAngle) * (BOOST_RADIUS - BOOST_RADIUS2);
				//�@���̐ݒ�
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

				//���_�J���[
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].tex = D3DXVECTOR2(nCntBOOST_H * (1.0f / (g_Boost_h / 2)), nCntBOOST_V * -1.0f);

				fDestAngle += fAngle;
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBoost->Unlock();



}

//=============================================================================
// �C���f�b�N�X���̍쐬
//=============================================================================
void MakeIndexBoost(LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * BOOST_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBoost,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//�C���f�b�N�X�̃J�E���g

						//�C���f�b�N�X�o�b�t�@�����b�N
	g_pIdxBuffBoost->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_Boost_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_Boost_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_Boost_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_Boost_h && nCntIndex_V <= g_Boost_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_Boost_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pIdxBuffBoost->Unlock();

}




