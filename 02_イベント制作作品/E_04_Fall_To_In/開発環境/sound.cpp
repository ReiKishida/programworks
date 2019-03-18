//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "sound.h"
#include "DebugProc.h"
//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CSound::~CSound()
{

}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			// ソースボイスの生成
			hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound][nCntPlayer], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
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
			// オーディオバッファの登録
			m_apSourceVoice[nCntSound][nCntPlayer]->SubmitSourceBuffer(&buffer);
		}
	}
	return S_OK;
}
//=============================================================================
// 更新処理
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
				{// ボリューム変更
					Stop(nCntPlayer, LABEL(nCntSound));
					m_abFadeOut[nCntSound][nCntPlayer] = false;
				}
			}
		}
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if (m_apSourceVoice[nCntSound][nCntPlayer])
			{
				// 一時停止
				m_apSourceVoice[nCntSound][nCntPlayer]->Stop(0);

				// ソースボイスの破棄
				m_apSourceVoice[nCntSound][nCntPlayer]->DestroyVoice();
				m_apSourceVoice[nCntSound][nCntPlayer] = NULL;

			}
		}
		// オーディオデータの開放
		free(m_apDataAudio[nCntSound]);
		m_apDataAudio[nCntSound] = NULL;

	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(停止)
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

	// 状態取得
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice[label][nIdy]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label][nIdy]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label][nIdy]->SubmitSourceBuffer(&buffer);
	m_apSourceVoice[label][nIdy]->SetVolume(1.0f* m_aSoundParam[label].fAddVolume);
	m_apSourceVoice[label][nIdy]->SetFrequencyRatio(1.0f);

	// 再生
	m_apSourceVoice[label][nIdy]->Start(0);

	m_abFadeOut[label][nIdy] = false;

	return S_OK;

}

//=============================================================================
// セグメント停止
//=============================================================================
void CSound::Stop(int nIdy,LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	// 状態取得
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice[label][nIdy]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label][nIdy]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止
//=============================================================================
void CSound::Stop(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if (m_apSourceVoice[nCntSound][nCntPlayer])
			{
				// 一時停止
				m_apSourceVoice[nCntSound][nCntPlayer]->Stop(0);

				// オーディオバッファの削除
				m_apSourceVoice[nCntSound][nCntPlayer]->FlushSourceBuffers();
			}
		}
	}
}

//=============================================================================
// チャンクのチェック
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
//=============================================================================
// 音量変化 0.0f～1.0f
//=============================================================================
void CSound::ChangeVolume(int nIdy, LABEL label, float fVolume)
{
	m_apSourceVoice[label][nIdy]->SetVolume(fVolume* m_aSoundParam[label].fAddVolume);
}
//=============================================================================
// ピッチ変化 0.0f(１オク下）1.0f(通常)　2.0f(１オク上)
//=============================================================================
void CSound::ChangePitch(int nIdy, LABEL label, float fPitch)
{
	m_apSourceVoice[label][nIdy]->SetFrequencyRatio(fPitch);
}
//=============================================================================
// 一時停止
//=============================================================================
void CSound::OnceStop(int nIdy, LABEL label, bool bStop)
{
	XAUDIO2_VOICE_STATE xa2state;
	// 状態取得
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		if (bStop)
		{// 一時停止
			m_apSourceVoice[label][nIdy]->Stop(0);
		}
		else
		{// 開始
			m_apSourceVoice[label][nIdy]->Start(0);
		}
	}
}
//=============================================================================
// 範囲チェック
//=============================================================================
float CSound::ChangeVolumeRange(int nIdy, LABEL label, D3DXVECTOR3 sound, D3DXVECTOR3 pos, float MaxRange, float MinRange)
{
	if (m_abFadeOut[label][nIdy])return 0;
	if (!GetPlaySound(nIdy, label))return 0;
	float PosX = (sound.x - pos.x);		// (マシーンの体の当たり判定の座標.x - 当たり判定の座標.x)
	float PosY = (sound.y - pos.y);		// (マシーンの体の当たり判定の座標.y - 当たり判定の座標.y)
	float PosZ = (sound.z - pos.z);		// (マシーンの体の当たり判定の座標.z - 当たり判定の座標.z)

	float fPosLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);				// それぞれを2乗してすべて足して距離を出す
																					// 二つの当たり判定の[半径]の距離を計算
	float fMaxRadius = MaxRange * MaxRange;	// 二つの半径を足した値を2乗して、半径の距離を計算を計算
	float fMinRadius = MinRange * MinRange;	// 二つの半径を足した値を2乗して、半径の距離を計算を計算
	float fLevel = 0;

	if (fMaxRadius > fPosLenght &&
		-fMaxRadius < fPosLenght)
	{// 座標の距離が半径の距離よりも小さかったら
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
	// 状態取得
	m_apSourceVoice[label][nIdy]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		bPlay = true;
	}
	return bPlay;
}
void CSound::SetFadeOut(int nIdy,LABEL label,bool bFadeOut)
{
	m_abFadeOut [label ][nIdy]= bFadeOut;
}