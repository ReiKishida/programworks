//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "sound.h"
//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	D3DXVECTOR3 pos;	// �T�E���h�̈ʒu
} SOUNDPARAM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X�ւ̃|�C���^
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X�ւ̃|�C���^
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

															// �e���f�ނ̃p�����[�^
SOUNDPARAM g_aSoundParam[SOUND_LABEL_MAX] =
{
	{ "data/BGM/TitleBGM000.wav", -1 },		// �^�C�g��
	{ "data/BGM/TutorialBGM000.wav",-1 },	// �`���[�g���A��
	{ "data/BGM/GameBGM000.wav", -1 },		// �Q�[��
	{ "data/BGM/GameBossBGM.wav", -1 },		// �Q�[��
	{ "data/BGM/GameClearBGM.wav", -1 },			// �N���A
	{ "data/BGM/GameOverBGM.wav", -1 },			// �Q�[���I�[�o�[
	{ "data/BGM/RankingBGM.wav", -1 },			// �Q�[���I�[�o�[
	{ "data/BGM/DeathBGM.wav", -1 },			// �Q�[���I�[�o�[
	{ "data/SE/Decide.wav", 0 },				// ����
	{ "data/SE/Select.wav", 0 },				// �I��
	{ "data/SE/Pause001.wav", 0 },				// �|�[�Y
	{ "data/SE/ItemGet001.wav", 0 },			// �A�C�e���Q�b�g
	{ "data/SE/Hit001.wav", 0 },			// �q�b�g
	{ "data/SE/Hit002.wav", 0 },			// �q�b�g
	{ "data/SE/Lockon.wav", 0 },			// ���b�N�I��
	{ "data/SE/WalkGrand.wav", -1 },				// �n��
	{ "data/SE/Explosion.wav", 0 },				// ����
	{ "data/SE/AtackWood.wav", 0 },			// �؂��U��
	{ "data/SE/PlayerAttack.wav", 0 },			// �U��
	{ "data/SE/VOICE/V_DAMAGE.wav", 0 },			// �_���[�W
	{ "data/SE/VOICE/V_DEATH.wav", 0 },			// ����
	{ "data/SE/VOICE/V_OK.wav", 0 },			// �`���[�g���A���I���
	{ "data/SE/GameClearUI.wav", 0 },				// �N���A
	{ "data/SE/GameOverUI.wav", 0 },				// �Q�[���I�[�o
	{ "data/SE/MessageWindow.wav", 0 },				// ���b�Z�[�W�E�B���h�E
//------------------------------------�L�b�N�n----------------------------------------------//
	{ "data/SE/KickBasic.wav", 0 },													// �L�b�N��{
	{ "data/SE/KickKick.wav", 0 },													// �L�b�N���L�b�N
	{ "data/SE/Samaso.wav", 0 },												// �L�b�N���L�b�N���L�b�N(�T�}�[�\���g)
	{ "data/SE/Volcano_cut.wav", 0 },												// �L�b�N���W�����v(�{���P�[�m�J�b�g)
	{ "data/SE/Slams.wav", 0 },														// �L�b�N���W�����v���p���`(�@����)
	{ "data/SE/Ultimate.wav", 0 },														// �L�b�N���W�����v���p���`(�@����2)
	{ "data/SE/SpinKick.wav", 0 },													// �L�b�N���W�����v���p���`(�񂵏R��)
//------------------------------------�p���`�n----------------------------------------------//
	{ "data/SE/PunchBasic.wav", 0 },												// �p���`��{
	{ "data/SE/PunchPunch.wav", 0 },												// �p���`���p���`
	{ "data/SE/RightStraight2.wav", 0 },												// �p���`���p���`���p���`(�E�X�g���[�g)
	{ "data/SE/Syouryuuken.wav", 0 }	,											// �p���`����]���W�����v(������)
	{ "data/SE/Back_Fist.wav",  0 },													// �p���`����](����)
//------------------------�v���C���[�U���q�b�g���ėp---------------------------------//
	{ "data/SE/PowerIncrease.wav", 0 },										//�R���{�U���ėp(�q�b�g��)
//--------------------------------------------��]�n-----------------------------------------//
	{ "data/SE/SpinBasic.wav", 0 },												// ��]��{
	{ "data/SE/FlameDance.wav", 0 }	,											// �t���C���_���X
	{ "data/SE/Cartwheels.wav", 0 }	,											// ���]
	{ "data/SE/Break_Dance.wav", 0 }	,										// �u���C�N�_���X
//-------------------------------------���n�A�X�e�b�v-------------------------------------//
	{ "data/SE/Randing.wav", 0 }	,												// ���n��
	{ "data/SE/Step3.wav", 0 }	,													// �X�e�b�v
//--------------------------------�W�����v-------------------------------------------------------------//
	{ "data/SE/Step3.wav", 0 },														// �W�����v��{
	{ "data/SE/Riderkick.wav", 0 }	,												// ��яR��
	{ "data/SE/HammerPanch.wav", 0 }	,										// �n���}�[�p���`
	{ "data/SE/KickWave.wav", 0 }	,												// �R���g
//--------------------------------�D�x�̕�--------------------------------------------------------------//
	{ "data/SE/PowerIncrease.wav", 0 }	,												// �D�x�̕�
//--------------------------------�Z�̓G--------------------------------------------------------------//
	{ "data/SE/Armor_Action.wav", 0 }	,										// �Z�̓G_�U��
	{ "data/SE/Armor_Walk.wav", 0 }	,										// �Z�̓G_���s
//--------------------------------�{�����A�Ђ������Ȃ�----------------------------------------//
	{ "data/SE/Enemy_Attack_Generic.wav", 0 }	,						// �G�U���ėp
	{ "data/SE/VOICE/V_DATUYOU.wav", 0 },			// ����
	{ "data/SE/VOICE/V_JUMP_PUNCH.wav", 0 },			// ����
	{ "data/SE/VOICE/V_KICK_JUMP_PUNCH.wav", 0 },			// ����
	{ "data/SE/VOICE/V_PUNCH.wav", 0 },			// ����
	{ "data/SE/VOICE/V_SPIN.wav", 0 },			// ����

};

//=============================================================================
// ������
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (g_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundParam[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;

	if (g_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(��~)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundParam[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	g_apSourceVoice[label]->Start(0);


	return S_OK;

}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
//=============================================================================
// ���ʕω�
//=============================================================================
void ChangeVolume(SOUND_LABEL label, float fVolume)
{
	g_apSourceVoice[label]->SetVolume(fVolume);
}
//=============================================================================
// ���a�`�F�b�N
//=============================================================================
void CheckPos(D3DXVECTOR3 pos)
{
	//bool bGetRadius = false;
	//for (int nCntPos = 0; nCntPos < 3; nCntPos++)
	//{
	//	float fPosX = (g_aSoundParam[SOUND_LABEL_SE_WATERFALL].pos.x - pos.x);
	//	float fPosZ = (g_aSoundParam[SOUND_LABEL_SE_WATERFALL].pos.z + nCntPos*500 - pos.z);
	//	float fLength = fPosX*fPosX + fPosZ*fPosZ;	// �v���C���[�ƓG�̋���
	//	for (int nCntSound = 0; nCntSound < 100; nCntSound++)
	//	{
	//		float fRadius = float((3500 * 3500) / (nCntSound + 1));		// Active�͈�
	//		if (fRadius > fLength &&
	//			-fRadius < fLength)
	//		{// ���ʕω�
	//			g_apSourceVoice[SOUND_LABEL_SE_WATERFALL]->SetVolume(nCntSound / 10.0f);
	//			bGetRadius = true;
	//		}
	//	}
	//	if (bGetRadius == false)
	//	{
	//		g_apSourceVoice[SOUND_LABEL_SE_WATERFALL]->SetVolume(0.0f);

	//	}
	//}
}