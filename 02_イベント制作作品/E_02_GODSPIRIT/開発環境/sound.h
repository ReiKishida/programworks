////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �T�E���h���� [sound.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//******************************************************************************************************************************
// �T�E���h�t�@�C��
//******************************************************************************************************************************
typedef enum
{
	SOUND_LABEL_TITLEBGM = 0,		// �^�C�g��
	SOUND_LABEL_TUTORIAL,			// �`���[�g���A��
	SOUND_LABEL_GAMEBGM1,			// �Q�[��
	SOUND_LABEL_BOSSBGM,			// �{�X
	SOUND_LABEL_GAMECLEARE,			// �N���A
	SOUND_LABEL_GAMEOVER,			// �Q�[���I�[�o�[
	SOUND_LABEL_RANKING,			// �����L���O
	SOUND_LABEL_DEATH,				// ���S
	SOUND_LABEL_SE_DECIDE,			// ����
	SOUND_LABEL_SE_SELECT,			// �I��
	SOUND_LABEL_SE_PAUSE,			// �|�[�Y
	SOUND_LABEL_SE_ITEMGET,			// �A�C�e���Q�b�g
	SOUND_LABEL_SE_HIT_0,			// �q�b�g
	SOUND_LABEL_SE_HIT_1,			// �q�b�g
	SOUND_LABEL_SE_LOCKON,			// ���b�N�I��
	SOUND_LABEL_SE_GRAUND,			// �n��
	SOUND_LABEL_SE_EXPLOSION,		// ����
	SOUND_LABEL_SE_ATTACKWOOD,		// �؍U��
	SOUND_LABEL_VOICE_ATTACK,		// �U���{�C�X
	SOUND_LABEL_VOICE_DAMAGE,		// �_���[�W�󂯂�
	SOUND_LABEL_VOICE_DEATH,		// ����
	SOUND_LABEL_VOICE_TUTORIALEND,	// �`���[�g���A���I���
	SOUND_LABEL_SE_GAMECLEARUI,		// �Q�[���N���A
	SOUND_LABEL_SE_GAMEOVERUI,		// �Q�[���I�[�o�[
	SOUND_LABEL_SE_TORI_MESSAGEWINDOW,		// message
	//--------------------------------�L�b�N�n-------------------------------------------------------------------//
	SOUND_LABEL_SE_KICK,											// �L�b�N��{
	SOUND_LABEL_SE_KICK_KICK,											// �L�b�N��{
	SOUND_LABEL_SE_KICK_KICK_KICK,									// �L�b�N���L�b�N���L�b�N(�T�}�[�\���g)
	SOUND_LABEL_SE_KICK_JUMP,										// �L�b�N���W�����v(�{���P�[�m�J�b�g)
	SOUND_LABEL_SE_KICK_JUMP_PUNCH,									// �L�b�N���W�����v���p���`(�@����)
	SOUND_LABEL_SE_KICK_JUMP_PUNCH2,									// �L�b�N���W�����v���p���`(�@����)
	SOUND_LABEL_SE_KICK_SPIN,										// �L�b�N���W�����v���p���`(�񂵏R��)
//--------------------------------�p���`�n------------------------------------------------------------------//
SOUND_LABEL_SE_PUNCH,											// �p���`��{
SOUND_LABEL_SE_PUNCH_PUNCH,											// �p���`���p���`
	SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH,								// �p���`���p���`���p���`(�E�X�g���[�g)
	SOUND_LABEL_SE_PUNCH_SPIN_JUMP,									// �p���`����]���W�����v(������)
	SOUND_LABEL_SE_PUNCH_SPIN,										// �p���`����](����)
	//--------------------------------�v���C���[�ėp���[�V����------------------------------------------------------//
	SOUND_LABEL_SE_COMBO_GENERIC,									// �R���{�U���ėp(�q�b�g��)
	//--------------------------------��]�n--------------------------------------------------------------------//
	SOUND_LABEL_SE_SPIN,											// ��]��{
	SOUND_LABEL_SE_SPIN_KICK,										// �t���C���_���X
	SOUND_LABEL_SE_SPIN_PUNCH,										// ���]
	SOUND_LABEL_SE_SPIN_KICK_SPIN,									// �u���C�N�_���X
	//--------------------------------���n�A�X�e�b�v-----------------------------------------------------//
	SOUND_LABEL_SE_RANDING,											// ���n��
	SOUND_LABEL_SE_STEP,											// �X�e�b�v
	//--------------------------------�W�����v-------------------------------------------------------------//
	SOUND_LABEL_SE_JUMP,											// �W�����v
	SOUND_LABEL_SE_JUMP_KICK,										// ��яR��
	SOUND_LABEL_SE_JUMP_PUNCH,										// �n���}�[�p���`
	SOUND_LABEL_SE_JUMP_SPIN,										// �R���g
	//--------------------------------�D�x�̕�--------------------------------------------------------------//
	SOUND_LABEL_SE_DATUYOU,											// �D�x�̕�
	//--------------------------------�Z�̓G-----------------------------------------------------------------//
	SOUND_LABEL_SE_ARMOR_ACTION,									// �Z�̓G�̍U��
	SOUND_LABEL_SE_ARMOR_WALK,										// �Z�̓G�̕��s
	//--------------------------------�{�����A�Ђ������Ȃ�----------------------------------------//
	SOUND_LABEL_SE_ENEMY_ATTACK_GENERIC,							// �G�U���ėp
	
	SOUND_LABEL_VOICE_DATUYOU,							// �D�x
	SOUND_LABEL_VOICE_JUMP_PUNCH,							// �D�x
	SOUND_LABEL_VOICE_KICK_JUMP_PUNCH,							// �D�x
	SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH,							// �D�x
	SOUND_LABEL_VOICE_SPIN_PUNCH_KICK,							// �D�x

	SOUND_LABEL_MAX
}SOUND_LABEL;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);	// �Đ�
void StopSound(SOUND_LABEL label);		// Uninit�œ������O�Ŏ~�߂�
void StopSound(void);
void ChangeVolume(SOUND_LABEL label, float fVolume);
void CheckPos(D3DXVECTOR3 pos);
#endif
