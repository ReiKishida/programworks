//=============================================================================
//
// �T�E���h���� [sound.h]
// Author :Kishida Rei
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		// BGM(TITLE)
		SOUND_LABEL_BGM_TUTORIAL,		// BGM(TUTORIAL)
		SOUND_LABEL_BGM_GAME,			// BGM(GAME)
		SOUND_LABEL_BGM_BOSS,			// BGM(BOSS)
		SOUND_LABEL_BGM_RESULT,			// BGM(RESULT)
		SOUND_LABEL_BGM_RANKING,		// BGM(RESULT
		SOUND_LABEL_SE_PICKUP,			// �擾(SE)
		SOUND_LABEL_SE_HIT,				// �q�b�g(SE)
		SOUND_LABEL_SE_DECIDE,			// ����(SE)
		SOUND_LABEL_SE_RUN,				// RUN(SE)
		SOUND_LABEL_SE_FIRE,			// FIRE(SE)
		SOUND_LABEL_SE_FIRE2,			// FIRE2(SE)
		SOUND_LABEL_SE_FIRE3,			// FIRE3(SE)
		SOUND_LABEL_SE_PLAYERATTACK_0,	// �v���C���[�̍U���P
		SOUND_LABEL_SE_PLAYERATTACK_1,	// �v���C���[�̍U���Q
		SOUND_LABEL_SE_PLAYERATTACK_2,	// �v���C���[�̍U���R
		SOUND_LABEL_SE_PLAYERHIT_1,		// �v���C���[�̔�_���[�W�P
		SOUND_LABEL_SE_PLAYERHIT_2,		// �v���C���[�̔�_���[�W�Q
		SOUND_LABEL_SE_HIT_LIGHT,		// �q�b�g���i��j
		SOUND_LABEL_SE_HIT_MEDIUM,		// �q�b�g���i���j
		SOUND_LABEL_SE_HIT_LARGE,		// �q�b�g���i��j
		SOUND_LABEL_SE_HIT_HIGH,		// �q�b�g���i�R��グ�j
		SOUND_LABEL_SE_FLASH,			//�@��
		SOUND_LABEL_SE_PHONE,			//�@�d�b
		SOUND_LABEL_SE_SMALLHIT,		//�@�����q�b�g��
		SOUND_LABEL_SE_HITDRAM,		//�@�����q�b�g
		SOUND_LABEL_SE_JIHANKI,		//�@���̋@
		SOUND_LABEL_MAX
	} SOUND_LABEL;
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;


	CSound();	//�R���X�g���N�^
	~CSound();	//�f�X�g���N�^
	HRESULT InitSound(HWND hWnd);	//������
	void UninitSound(void);			//�I������
	HRESULT PlaySound(SOUND_LABEL label);//�T�E���h�̍Đ�
	void StopSound(SOUND_LABEL label);//�T�E���h�̒�~
	void StopSound(void);//�T�E���h�̒�~
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },		// �^�C�g��
		{ "data/BGM/tutorial.wav", -1 },	// �`���[�g���A��
		{ "data/BGM/game.wav", -1 },		// �Q�[��
		{ "data/BGM/boss.wav", -1 },		// �{�X
		{ "data/BGM/result.wav", 0 },		// ���U���g
		{ "data/BGM/ranking.wav",-1 },		// �����L���O
		{ "data/SE/item_pickup.wav", 0 },	// �擾
		{ "data/SE/hit.wav", 0 },			// ��_���[�W(����)
		{ "data/SE/decide.wav", 0 },		// ����
		{ "data/SE/run.wav", 0 },			// ����
		{ "data/SE/fire/fire.wav", 0 },		//���P
		{ "data/SE/fire/fire2.wav", 0 },		//���Q
		{ "data/SE/fire/fire3.wav", 0 },		//��
		{ "data/SE/0_voice_player/player_Attack_0.wav", 0 },		//�v���C���[�̍U��1
		{ "data/SE/0_voice_player/player_Attack_1.wav", 0 },		//�v���C���[�̍U��2
		{ "data/SE/0_voice_player/player_Attack_2.wav", 0 },		//�v���C���[�̍U��3
		{ "data/SE/0_voice_player/player_hit1.wav", 0 },			//�v���C���[�̃_���[�W�P
		{ "data/SE/0_voice_player/player_hit2.wav", 0 },				//�v���C���[�̃_���[�W�Q
		{ "data/SE/hit/light.wav", 0 },				//��U��
		{ "data/SE/hit/medium.wav", 0 },				//���U��
		{ "data/SE/hit/large.wav", 0 }	,			//��U��
		{ "data/SE/hit/high.wav", 0 },			//�R��グ
		{ "data/SE/flash.wav", 0 },			//��
		{ "data/SE/phone.wav", 0 },			//�d�b
		{ "data/SE/hit.wav", 0 },			//�q�b�g
		{ "data/SE/goin.wav", 0 },			//�K�C�[��
		{ "data/SE/jihanki.wav", 0 }			//���̋@

	};


};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
