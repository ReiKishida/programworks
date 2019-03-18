//=============================================================================
//
// �V�[������ [scene.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_POLYGON (2000)	//�I�u�W�F�N�g�̑���
#define PRIORITY_NUMBER (8)	//�I�u�W�F�N�g�̗D�揇��

class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,		//��̎��
		OBJTYPE_FADE,			//�t�F�[�h
		OBJTYPE_PLAYER,			//�v���C���[
		OBJTYPE_LOG,			//���O
		OBJTYPE_ENEMY,			//�G
		OBJTYPE_BOSS,			//�{�X
		OBJTYPE_CUTIN,			//�J�b�g�C��
		OBJTYPE_BULLET,			//�e
		OBJTYPE_EXPLOSION,		//����
		OBJTYPE_BG,				//�w�i
		OBJTYPE_UI,				//UI
		OBJTYPE_MENU,			//���j���[
		OBJTYPE_EFFECT,			//�G�t�F�N�g
		OBJTYPE_FORCE,			//�t�H�[�X
		OBJTYPE_TITLELOGO,		//�^�C�g�����S
		OBJTYPE_RANKINGLOOG,	//�����L���O���S
		OBJTYPE_TUTORIALLOGO,	//�����L���O���S
		OBJTYPE_FORCENUMBER,	//�X�R�A
		OBJTYPE_SCORE,			//�X�R�A
		OBJTYPE_RANKINGNUNBER,			//�����L���O����
		OBJTYPE_REMAINING,		//�c�@��
		OBJTYPE_MAX				//�I�u�W�F�N�g�̎�ނ̍ő吔
	}OBJTYPE;//�I�u�W�F�N�g�̎��
	CScene::CScene(int nPriority = 4);	//�R���X�g���N�^
	virtual ~CScene();	//�f�X�g���N�^
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;	//����������
	virtual void Uninit(void) = 0;	//�I������
	virtual void Update(void) = 0;	//�X�V����
	virtual void Draw(void) = 0;	//�`�揈��
	void SetObjType(OBJTYPE objtype);	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	OBJTYPE GetObjType(void);	//�I�u�W�F�N�g�̎�ނ̎擾
	static CScene *GetScene(int nPriority, int nIndex);	//�V�[���̎擾(�C���f�b�N�X)

	static void ReleaseSceneAll(void);		//�S�ẴV�[���̔j��
	static void UpdateAll(void);			//�S�ẴV�[���̍X�V
	static void DrawAll(void);				//�S�ẴV�[���̕`��
	static int GetNumAll(void);				//�V�[���̑����̎擾
	//static CScene **GetScene(void);			//�V�[���̎擾

protected:
	void Release(void);	//�I�u�W�F�N�g�̔j��
private:
	int m_nPriority;	//�D�揇�ʂ̔ԍ�
	static int m_nNumAll;	//�V�[���̑���
	static CScene *m_apScene[MAX_PRIORITY][MAX_POLYGON];	//�V�[���̃C���X�^���X����
	int m_nID;//�����̔ԍ�
	OBJTYPE m_Objtype;//�I�u�W�F�N�g�̎��
};

#endif