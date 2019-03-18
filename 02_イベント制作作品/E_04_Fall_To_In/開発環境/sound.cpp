//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "sound.h"
#include "DebugProc.h"
//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSound::CSound()
{
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			m_abFadeOut[nCntSound][nCntPlayer] = false;
		}
	}
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSound::~CSound()
{

}
//=============================================================================
// ������
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (m_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
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
		hFile = CreateFile(m_aSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
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
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			// �\�[�X�{�C�X�̐���
			hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound][nCntPlayer], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aSoundParam[nCntSound].nCntLoop;
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			// �I�[�f�B�I�o�b�t�@�̓o�^
			m_apSourceVoice[nCntSound][nCntPlayer]->SubmitSourceBuffer(&buffer);
		}
	}
	return S_OK;
}
//=============================================================================
// �X�V����
//=============================================================================
void CSound::Update(void)
{
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		float fVolume = 0;

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if (m_abFadeOut[nCntSound][nCntPlayer])
			{
				m_apSourceVoice[nCntSound][nCntPlayer]->GetVolume(&fVolume);
				fVolume -= 0.1f;
				m_apSourceVoice[nCntSound][nCntPlayer]->SetVolume(fVolume);
				if (fVolume <= 0.0f)
				{// �{�����[���ύX
					Stop(nCntPlayer, LABEL(nCntSound));
					m_abFadeOut[nCntSound][nCntPlayer] = false;
				}
			}
		}
	}
}
//=============================================================================
// �I������
//=============================================================================
void CSound::Uninit(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if (m_apSourceVoice[nCntSound][nCntPlayer])
			{
				// �ꎞ��~
				m_apSourceVoice[nCntSound][nCntPlayer]->Stop(0);

				// �\�[�X�{�C�X�̔j��
				m_apSourceVoice[nCntSound][nCntPlayer]->DestroyVoice();
				m_apSourceVoice[nCntSound][nCntPlayer] = NULL;

			}
		}
		// �I�[�f�B�I�f�[�^�̊J��
		free(m_apDataAudio[nCntSound]);
		m_apDataAudio[nCntSound] = NULL;

	}

	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(��~)
//=============================================================================
HRESULT CSound::PlaySound(int nIdy,LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSoundParam[label].nCntLoop;

	// ��Ԏ擾
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_apSourceVoice[label][nIdy]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label][nIdy]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice[label][nIdy]->SubmitSourceBuffer(&buffer);
	m_apSourceVoice[label][nIdy]->SetVolume(1.0f* m_aSoundParam[label].fAddVolume);
	m_apSourceVoice[label][nIdy]->SetFrequencyRatio(1.0f);

	// �Đ�
	m_apSourceVoice[label][nIdy]->Start(0);

	m_abFadeOut[label][nIdy] = false;

	return S_OK;

}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void CSound::Stop(int nIdy,LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	// ��Ԏ擾
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_apSourceVoice[label][nIdy]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label][nIdy]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void CSound::Stop(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if (m_apSourceVoice[nCntSound][nCntPlayer])
			{
				// �ꎞ��~
				m_apSourceVoice[nCntSound][nCntPlayer]->Stop(0);

				// �I�[�f�B�I�o�b�t�@�̍폜
				m_apSourceVoice[nCntSound][nCntPlayer]->FlushSourceBuffers();
			}
		}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
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
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
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
// ���ʕω� 0.0f�`1.0f
//=============================================================================
void CSound::ChangeVolume(int nIdy, LABEL label, float fVolume)
{
	m_apSourceVoice[label][nIdy]->SetVolume(fVolume* m_aSoundParam[label].fAddVolume);
}
//=============================================================================
// �s�b�`�ω� 0.0f(�P�I�N���j1.0f(�ʏ�)�@2.0f(�P�I�N��)
//=============================================================================
void CSound::ChangePitch(int nIdy, LABEL label, float fPitch)
{
	m_apSourceVoice[label][nIdy]->SetFrequencyRatio(fPitch);
}
//=============================================================================
// �ꎞ��~
//=============================================================================
void CSound::OnceStop(int nIdy, LABEL label, bool bStop)
{
	XAUDIO2_VOICE_STATE xa2state;
	// ��Ԏ擾
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		if (bStop)
		{// �ꎞ��~
			m_apSourceVoice[label][nIdy]->Stop(0);
		}
		else
		{// �J�n
			m_apSourceVoice[label][nIdy]->Start(0);
		}
	}
}
//=============================================================================
// �͈̓`�F�b�N
//=============================================================================
float CSound::ChangeVolumeRange(int nIdy, LABEL label, D3DXVECTOR3 sound, D3DXVECTOR3 pos, float MaxRange, float MinRange)
{
	if (m_abFadeOut[label][nIdy])return 0;
	if (!GetPlaySound(nIdy, label))return 0;
	float PosX = (sound.x - pos.x);		// (�}�V�[���̑̂̓����蔻��̍��W.x - �����蔻��̍��W.x)
	float PosY = (sound.y - pos.y);		// (�}�V�[���̑̂̓����蔻��̍��W.y - �����蔻��̍��W.y)
	float PosZ = (sound.z - pos.z);		// (�}�V�[���̑̂̓����蔻��̍��W.z - �����蔻��̍��W.z)

	float fPosLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);				// ���ꂼ���2�悵�Ă��ׂđ����ċ������o��
																					// ��̓����蔻���[���a]�̋������v�Z
	float fMaxRadius = MaxRange * MaxRange;	// ��̔��a�𑫂����l��2�悵�āA���a�̋������v�Z���v�Z
	float fMinRadius = MinRange * MinRange;	// ��̔��a�𑫂����l��2�悵�āA���a�̋������v�Z���v�Z
	float fLevel = 0;

	if (fMaxRadius > fPosLenght &&
		-fMaxRadius < fPosLenght)
	{// ���W�̋��������a�̋�������������������
		fLevel = fabsf(fMinRadius / (fPosLenght + 0.001f))*m_aSoundParam[label].fAddVolume;
		if (fMinRadius > fPosLenght &&
			-fMinRadius < fPosLenght)
		{
			fLevel = 1.0f;
		}
	}

	m_apSourceVoice[label][nIdy]->SetVolume(fLevel);
	return fLevel;
}

bool CSound::GetPlaySound(int nIdy, LABEL label)
{
	bool bPlay = false;
	XAUDIO2_VOICE_STATE xa2state;
	// ��Ԏ擾
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		bPlay = true;
	}
	return bPlay;
}
void CSound::SetFadeOut(int nIdy,LABEL label,bool bFadeOut)
{
	m_abFadeOut [label ][nIdy]= bFadeOut;
}