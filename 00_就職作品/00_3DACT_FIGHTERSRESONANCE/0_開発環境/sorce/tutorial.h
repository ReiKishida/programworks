//=============================================================================
//
// �`���[�g���A������[�V�[��] [tutorial.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY (50)
#define MAX_TUTORIAL_UI (12)

#include "main.h"
class CTutoLogo;
class CComboGauge;

class CTutorial
{
public:

	CTutorial();	//�R���X�g���N�^
	~CTutorial();	//�f�X�g���N�^
	static HRESULT Init(void);	//����������
	static void Uninit(void);	//�I������
	static void Update(void);	//�X�V����
	static void Draw(void);	//�`�揈��
	static CPlayer *GetPlayer(void);				//�v���C���[�̎擾
	static CComboGauge *GetComboGauge(int nCnt);	//�R���{�Q�[�W�̎擾


private:
	static CTutoLogo *m_pTutoLogo;
	static CComboGauge *m_apComboGauge[2];  //�R���{�Q�[�W
	static CPlayer *m_pPlayer;				//�v���C���[

};

#endif