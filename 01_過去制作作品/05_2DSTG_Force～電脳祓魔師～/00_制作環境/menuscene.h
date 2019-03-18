//=============================================================================
//
// ���j���[��ʏ���[2D �|���S��] [menu.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MENUTEXTURE_NUM (7)	//���j���[��ʂ̃e�N�X�`����
#define MAX_MENUPOLYGON_NUM (5)	//���j���[��ʂ̃e�N�X�`����

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CMenuScene : public CScene
{
public:
	typedef enum
	{
		MENUSTATE_NONE = 0,	//��̏��
		MENUSTATE_NORMAL,		//�ʏ���
		MENUSTATE_USE,			//�G���^�[�������ꂽ���
		MENUSTATE_MAX			//��Ԃ̍ő吔
	}MENUSTATE;
	typedef enum
	{
		MENUTYPE_GAMEMODE = 0,	//�Q�[�����[�h
		MENUTYPE_RANKING,		//�����L���O
		MENUTYPE_QUIT,			//�N�C�b�g
		MENUTYPE_HUMAN,			//�l
		MENUTYPE_WINDOW_GAME,		//�E�B���h�E
		MENUTYPE_WINDOW_RANKING,		//�E�B���h�E
		MENUTYPE_WINDOW_QUIT,		//�E�B���h�E
		MENUTYPE_MAX			//��Ԃ̍ő吔
	}MENUTYPE;
	CMenuScene();	//�R���X�g���N�^
	~CMenuScene();	//�f�X�g���N�^
	static CMenuScene *Create(D3DXVECTOR3 pos);	//�w�i�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_MENUTEXTURE_NUM]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apScene2D[MAX_MENUPOLYGON_NUM];//�V�[��2D�̃|�C���^
	D3DXVECTOR3 m_MenuRot;//�p�x���
	D3DXVECTOR3 m_MenuPos;//�ʒu���
	D3DXCOLOR m_col;	//�F���
	int m_nTex_Y;//�e�N�X�`�����W
	int m_nCntFrame;//�t���[�����̃J�E���g
	static int m_nSelectMenu;	//�I�𒆂̔ԍ�
	static int m_nSelectState;	//�I�𒆂̏��
	float m_fWidth;//��
	float m_fHeight;//����
	D3DXVECTOR3 m_fDestAngle;//�ړI�̊p�x
	D3DXVECTOR3 m_fAngle;//�p�x
	static bool m_bRotate;	//��]���Ă邩(+)
	MENUSTATE m_state;//���
	MENUTYPE m_type; //���
};

#endif