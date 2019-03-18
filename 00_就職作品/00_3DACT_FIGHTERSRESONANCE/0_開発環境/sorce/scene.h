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
#define PRIORITY_NUMBER (9)	//�I�u�W�F�N�g�̗D�揇��

//*****************************************************************************
// �V�[���S�̂̃N���X�\����
//*****************************************************************************
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,			//��̎��
		OBJTYPE_SCENE2D,			//2D�V�[��
		OBJTYPE_SCENE3D,			//3D�V�[��
		OBJTYPE_SCENEGAUGE,			//�Q�[�W�V�[��
		OBJTYPE_SCENEGAUGEBILLBOARD,//�Q�[�W�V�[��(�r���{�[�h)
		OBJTYPE_UIGAUGE,			//UI�Q�[�W
		OBJTYPE_UIGAUGEBILLBOARD,	//UI�Q�[�W
		OBJTYPE_COMBOGAUGE,			//�R���{�Q�[�W
		OBJTYPE_SCENEX,				//X�V�[��
		OBJTYPE_SCENEBILLBOARD,		//�r���{�[�h�V�[��
		OBJTYPE_SCENEBILLBOARDANIM,	//�r���{�[�h�V�[��(�A�j���[�V����)
		OBJTYPE_TITLELOGO,			//�^�C�g�����S
		OBJTYPE_TUTOLOGO,			//�`���[�g���A�����S
		OBJTYPE_RESULTLOGO,		//�����L���O���S
		OBJTYPE_RANKINGLOGO,		//�����L���O���S
		OBJTYPE_MESHFIELD,			//���b�V���t�B�[���h
		OBJTYPE_MESHWALL,			//���b�V���E�H�[��
		OBJTYPE_MESHCYLINDER,		//���b�V���V�����_�[
		OBJTYPE_RANKINGNUNBER,		//�����L���O�̐���
		OBJTYPE_SCENEORBIT,			//�O��
		OBJTYPE_COLLISION,			//�����蔻��
		OBJTYPE_COLLISION_ENEMY,	//�����蔻��
		OBJTYPE_PLAYER,				//�v���C���[
		OBJTYPE_ENEMY,				//�G
		OBJTYPE_ENEMY_POW,				//�G
		OBJTYPE_ENEMY_TRC,				//�G
		OBJTYPE_BOSS,				//�{�X
		OBJTYPE_COMBO,				//�R���{
		OBJTYPE_RANK,				//�ݸ
		OBJTYPE_UI,					//UI
		OBJTYPE_UIRESULT,			//UI���U���g
		OBJTYPE_UIRANKING,			//UI�����L���O
		OBJTYPE_ITEMCOUNTER,		//�A�C�e���J�E���^�[
		OBJTYPE_EFFECT,				//�G�t�F�N�g
		OBJTYPE_PARTICLE,			//�p�[�e�B�N��
		OBJTYPE_PARTICLE2D,			//�p�[�e�B�N��2D
		OBJTYPE_SCORE,				//�X�R�A
		OBJTYPE_FINALSCORE,			//�X�R�A
		OBJTYPE_COMBOSCORE,			//�X�R�A
		OBJTYPE_TIMER,				//�^�C�}�[
		OBJTYPE_OBJECT,				//�I�u�W�F�N�g
		OBJTYPE_PR,					//�L��
		OBJTYPE_CUTIN,
		OBJTYPE_MAX					//�I�u�W�F�N�g�̎�ނ̍ő吔
	}OBJTYPE;
	CScene(int nPriority = 3, OBJTYPE type = OBJTYPE_NONE);	//�R���X�g���N�^
	virtual ~CScene();	//�f�X�g���N�^
	virtual HRESULT Init(void) = 0;	//����������
	virtual void Uninit(void) = 0;	//�I������
	virtual void Update(void) = 0;	//�X�V����
	virtual void Draw(void) = 0;	//�`�揈��
	void SetObjType(OBJTYPE objtype);	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	OBJTYPE GetObjType(void);	//�I�u�W�F�N�g�̎�ނ̎擾
	int GetPriority(void);					//�D�揇�ʂ̎擾
	static void ReleaseSceneAll(void);		//�S�ẴV�[���̔j��
	static void UpdateAll(void);			//�S�ẴV�[���̍X�V
	static void DrawAll(void);				//�S�ẴV�[���̕`��
	static int GetNumAll(void);				//�V�[���̑����̎擾
	static void ChangeStop(void);			//�V�[���̒�~
	static void SetStop(bool bStop);				//��~�̐ݒ�
	static void SetBossStop(bool bStop);				//��~�̐ݒ�
	static void ChangeStopBoss(void);		//�V�[���̒�~
	static void SetStopBoss(bool bStop);				//��~��
	void CheckDeath(void);					//���S�t���O�𒲂ׂ�
	static bool GetStop(void);						//��~�̎擾
	static bool GetBossStop(void);						//��~�̎擾

	CScene *GetSceneTop(int nPriority);	//�V�[���̎擾(�C���f�b�N�X)
	CScene *GetSceneNext(int nPriority);	//�V�[���̎擾(�C���f�b�N�X)

protected:
	void Release(void);	//�I�u�W�F�N�g�̔j��

private:
	int m_nPriority;	//�D�揇�ʂ̔ԍ�
	static int m_nNumAll;	//�V�[���̑���
	static CScene *m_pTop[PRIORITY_NUMBER];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene *m_pCur[PRIORITY_NUMBER];	//���݂̃I�u�W�F�N�g�̃|�C���^
	CScene *m_pPrev;	//�O�̃|�C���^
	CScene *m_pNext;	//���̃|�C���^
	int m_nID;//�����̔ԍ�
	OBJTYPE m_Objtype;//�I�u�W�F�N�g�̎��
	bool m_bDeath;	//���S�t���O
	static bool m_bStop;//�v���C���[��
	static bool m_bBossStop;//�{�X��
};

#endif