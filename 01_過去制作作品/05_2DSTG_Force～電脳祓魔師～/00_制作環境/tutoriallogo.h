//=============================================================================
//
// �`���[�g���A���p�|���S������[2D �|���S��] [tutoriallogo.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _TUTORIALLOGO_H_
#define _TUTORIALLOGO_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TUTORIAL_LOGO_TEXTURE (2)
#define MAX_TUTORIAL_LOGO (2)

class CManager;

class CTutorialLogo : public CScene
{
public:

	CTutorialLogo();	//�R���X�g���N�^
	~CTutorialLogo();	//�f�X�g���N�^
	static CTutorialLogo *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TUTORIAL_LOGO_TEXTURE]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apScene2D[MAX_TUTORIAL_LOGO];	//����

};

#endif