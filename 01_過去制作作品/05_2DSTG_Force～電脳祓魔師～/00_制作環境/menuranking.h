//=============================================================================
//
// �����L���O����[2D �|���S��] [ranking.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _MENURANKING_H_
#define _MENURANKING_H_

#include "main.h"

class CMenuRanking
{
public:
	CMenuRanking();	//�R���X�g���N�^
	~CMenuRanking();	//�f�X�g���N�^
	static HRESULT Init(void);	//����������
	static void Uninit(void);	//�I������
	static void Update(void);	//�X�V����
	static void Draw(void);	//�`�揈��
private:


};

#endif