//=============================================================================
//
// �w�i����[2D �|���S��] [bg.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_UI (12)	//�e�N�X�`����

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CUi : public CScene
{
public:
	typedef enum
	{
		UI_TYPE_BG_L = 0,		//���n(L)
		UI_TYPE_BG_R,		//���n(R)
		UI_TYPE_HIGHSCORE,	//�n�C�X�R�A
		UI_TYPE_SCORE,		//�X�R�A
		UI_TYPE_ATK,		//�U�̃t�H�[�X��
		UI_TYPE_SPR,		//�g�̃t�H�[�X��
		UI_TYPE_SPD,		//���̃t�H�[�X��
		UI_TYPE_DEF,		//��̃t�H�[�X��
		UI_TYPE_TIME,		//�^�C���̕���
		UI_TYPE_COLON,		//�R����
		UI_TYPE_DOT,		//�h�b�g
		UI_TYPE_LOG,		//���O�̕���
		UI_TYP_MAX			//UI�̍ő吔
	}UITYPE;//UI�̎��
	CUi(int nPriority = 6);	//�R���X�g���N�^
	~CUi();	//�f�X�g���N�^
	static CUi *Create(D3DXVECTOR3 pos, UITYPE type);	//�w�i�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_UI]; //���L�e�N�X�`���̃|�C���^
	D3DXCOLOR m_Color;
	int m_nCntColor;	//�J���[�J�E���g�p
	CScene2D *m_pScene2D;
	UITYPE m_Type;	//�w�i�̃^�C�v
};

#endif