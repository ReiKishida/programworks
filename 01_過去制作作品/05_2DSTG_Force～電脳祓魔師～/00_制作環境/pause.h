//=============================================================================
//
// �|�[�Y����[�P��] [pause.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PAUSE_TEXTURE (7)
#define MAX_PAUSE (6)

class CPolygon;

class CPause
{
public:
	typedef enum
	{
		PAUSESTATE_NONE = 0,	//��̏��
		PAUSESTATE_NORMAL,		//�ʏ���
		PAUSESTATE_USE,			//�G���^�[�������ꂽ���
		PAUSESTATE_MAX			//��Ԃ̍ő吔
	}PAUSESTATE;
	typedef enum
	{
		PAUSETYPE_BACK = 0,		//�w�i
		PAUSETYPE_FLAME,		//�g
		PAUSETYPE_CONTINUE,		//�R���e�B�j���[
		PAUSETYPE_RETRY,			//���g���C
		PAUSETYPE_QUIT,				//�N�C�b�g
		PAUSETYPE_GUID,				//���ڐ���
		PAUSETYPE_MAX				//�ő吔
	}PAUSETYPE;
	typedef enum
	{

		PAUSEMENU_CONTINUE,		//�R���e�B�j���[
		PAUSEMENU_RETRY,			//���g���C
		PAUSEMENU_QUIT,				//�N�C�b�g
		PAUSEMENU_MAX				//�ő吔
	}PAUSEMENU;
	CPause();	//�R���X�g���N�^
	~CPause();	//�f�X�g���N�^
	 HRESULT Init(void);	//����������
	 void Uninit(void);	//�I������
	 void Update(void);	//�X�V����
	 void Draw(void);	//�`�揈��
	 static void SetSelect(void);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_PAUSE_TEXTURE]; //���L�e�N�X�`���̃|�C���^
	static CPolygon *m_apPolygon[MAX_PAUSE];//�V�[��2D
	static CPause *m_pPause;	//�|�[�Y
	static PAUSESTATE m_state[MAX_PAUSE];//���
	PAUSETYPE m_type;//���
	PAUSEMENU m_Menu;//�|�[�Y���j���[�̍���
	static int m_nSelectNum;	//���ݑI������Ă��鐔��
	static int m_nCntFrame;	//�t���[����
};


#endif