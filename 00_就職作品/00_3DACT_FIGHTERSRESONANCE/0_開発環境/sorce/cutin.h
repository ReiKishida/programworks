//=============================================================================
//
// �V�[��2D����[2D �|���S��] [scene2D.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _CUTIN_H_
#define _CUTIN_H_

#include "main.h"
#include "scene2D.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_CUTIN (9)
#define MAX_CUTIN (4)
//*****************************************************************************
// �V�[��2D�̃N���X�\����
//*****************************************************************************
class CCutin : public CScene
{
public:
	typedef enum
	{
		CUTINMODE_NONE = 0,
		CUTINMODE_PLAYER,
		CUTINMODE_ENEMY,
		CUTINMODE_ENEMYAPPEAR,
		CUTINMODE_MAX
	}CUTINMODE;
	typedef enum
	{
		CUTINTYPE_FRAME_PLAYER = 0,
		CUTINTYPE_FRAME_ENEMY,
		CUTINTYPE_FRAME2_PLAYER,
		CUTINTYPE_FRAME2_ENEMY,
		CUTINTYPE_SILHOUETTE_PLAYER,
		CUTINTYPE_SILHOUETTE_ENEMY,
		CUTINTYPE_SKILL_PLAYER,
		CUTINTYPE_SKILL_ENEMY,
		CUTINTYPE_NAME_ENEMY,
		CUTINTYPE_MAX
	}CUTINTYPE;

	typedef enum
	{
		CUTCOUNT_FRAME = 0,
		CUTCOUNT_FRAME2,
		CUTCOUNT_SILHOUETTE,
		CUTCOUNT_SKILL,
		CUTCOUNT_MAX
	}CUTCOUNT;

	CCutin::CCutin(int nPriority = 7, OBJTYPE type = OBJTYPE_CUTIN);	//�R���X�g���N�^
	CCutin::~CCutin();	//�f�X�g���N�^
	static HRESULT Load(void);
	static void Unload(void);

	static CCutin *Create(CUTINMODE mode);//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
protected:

private:
	static LPDIRECT3DTEXTURE9		m_apTexture[MAX_TEXTURE_CUTIN];				// �e�N�X�`���ւ̃|�C���^
	static CScene2D *m_apScene2D[MAX_CUTIN];
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;			// �|���S���̉�]
	float m_fWidth;	//�|���S���̕�
	float m_fHeight;	//�|���S���̍���
	int m_nCntflame;		//�J�E���g
	CUTINTYPE m_type;
	CUTCOUNT m_count;
	static CUTINMODE m_mode;
	//static int m_nCntFlame;
};

#endif