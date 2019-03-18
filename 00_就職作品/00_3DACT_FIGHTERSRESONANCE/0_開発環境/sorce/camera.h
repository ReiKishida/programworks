//=============================================================================
//
// �J�������� [camera.h]
// Author :	�ݓc��(Kishida Rei)
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �J�����̃N���X�\����
//*****************************************************************************
class CCamera
{
public:
	typedef enum
	{
		CAMERATYPE_TITLE = 0,	//�^�C�g���̃J����
		CAMERATYPE_TUTORIAL,	//�`���[�g���A���̃J����
		CAMERATYPE_GAME,		//�Q�[���̃J����
		CAMERATYPE_RESULT,		//���U���g�̃J����
		CAMERATYPE_RANKING,		//�����L���O
		CAMERATYPE_MAX			//��ނ̍ő吔
	}CAMERATYPE;				//�J�����̎��
	CCamera();					//�R���X�g���N�^
	~CCamera();					//�f�X�g���N�^
	HRESULT Init(void);			//����������
	void Uninit(void);			//�I������
	void Update(void);			//�X�V����
	void SetCamera(void);		//�`�揈��
	void OverView(void);		//�p�x����
	D3DXMATRIX GetmtxView(void);//�r���[�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetRot(void);	//�J�����̊p�x���̎擾
	static void SetFree(void);			//
private:
	static CManager *m_pManager;//�}�l�[�W��
	D3DXVECTOR3 m_posVDest;		//�ړI�̈ʒu(���_)
	D3DXVECTOR3 m_posRDest;		//�ړI�̈ʒu(�����_)
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	D3DXVECTOR3 m_rot;			//�p�x���
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
	float m_eyepoint;			//���_
	CAMERATYPE m_type;
	static bool m_bFree;				//�t���[�r���[

	//====================================================
	//�p�x���
	//====================================================
	float m_fDestAngle;			//�ړI�̊p�x
	float m_fAngle;				//�p�x

	//====================================================
	//���_���
	//====================================================
	float m_fDestpos_V_x;		//�ړI�̈ʒu(���_-X)
	float m_fpos_V_x;			//���݂̈ʒu(���_-X)

	float m_fDestpos_V_y;		//�ړI�̈ʒu(���_-Y)
	float m_fpos_V_y;			//���݂̈ʒu(���_-Y)

	float m_fDestpos_V_z;		//�ړI�̈ʒu(���_-Z)
	float m_fpos_V_z;			//���݂̈ʒu(���_-Z)

	//====================================================
	//�����_���
	//====================================================
	float m_fDestpos_R_x;		//�ړI�̈ʒu(�����_-X)
	float m_fpos_R_x;			//���݂̈ʒu(�����_-X)

	float m_fDestpos_R_y;		//�ړI�̈ʒu(�����_-Y)
	float m_fpos_R_y;			//���݂̈ʒu(�����_-Y)

	float m_fDestpos_R_z;		//�ړI�̈ʒu(�����_Z)
	float m_fpos_R_z;			//���݂̈ʒu(�����_-Z)

	//====================================================
	//�����̏��
	//====================================================
	float m_fLength;			//���݂̒���
	float m_fdestLength;		//�ړI�̒���

	//====================================================
	//�����̏��
	//====================================================
	float m_fHeight_V;			//���݂̍���(���_)
	float m_fdestHeight_V;		//�ړI�̍���(���_)

	float m_fHeight_R;			//���݂̍���(�����_)
	float m_fdestHeight_R;		//�ړI�̍���(�����_)

};

#endif
