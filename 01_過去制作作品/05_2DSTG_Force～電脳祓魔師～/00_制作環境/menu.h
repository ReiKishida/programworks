//=============================================================================
//
// ���j���[��ʏ���[2D �|���S��] [menu.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CMenu
{
public:
	typedef enum
	{
		MENUSTATE_NONE = 0,	//��̏��
		MENUSTATE_NORMAL,		//�ʏ���
		MENUSTATE_USE,			//�G���^�[�������ꂽ���
		MENUSTATE_MAX			//��Ԃ̍ő吔
	}MENUSTATE;

	CMenu();	//�R���X�g���N�^
	~CMenu();	//�f�X�g���N�^
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	static D3DXCOLOR m_col; //�F���
	D3DXVECTOR3 m_MenuRot;//�p�x���
	D3DXVECTOR3 m_MenuPos;//�ʒu���
	int m_nPatternAnim;	//�A�j���[�V�����p�^�[��
	int m_nCounterAnim;//�A�j���[�V�����J�E���^�[
	int m_nTex_Y;//�e�N�X�`�����W
	int m_nCntFrame;//�t���[�����̃J�E���g
	int m_nSelect;	//�I�𒆂̔ԍ�
	float m_fWidth;//��
	float m_fHeight;//����
	float m_fDestAngle;//�ړI�̊p�x
	float m_fAngle;//�p�x
	static bool m_bRotate;	//��]���Ă邩
	MENUSTATE m_state;//���
};

#endif