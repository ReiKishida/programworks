//=============================================================================
//
// サウンド処理 [sound.h]
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
		SOUND_LABEL_SE_PICKUP,			// 取得(SE)
		SOUND_LABEL_SE_HIT,				// ヒット(SE)
		SOUND_LABEL_SE_DECIDE,			// 決定(SE)
		SOUND_LABEL_SE_RUN,				// RUN(SE)
		SOUND_LABEL_SE_FIRE,			// FIRE(SE)
		SOUND_LABEL_SE_FIRE2,			// FIRE2(SE)
		SOUND_LABEL_SE_FIRE3,			// FIRE3(SE)
		SOUND_LABEL_SE_PLAYERATTACK_0,	// プレイヤーの攻撃１
		SOUND_LABEL_SE_PLAYERATTACK_1,	// プレイヤーの攻撃２
		SOUND_LABEL_SE_PLAYERATTACK_2,	// プレイヤーの攻撃３
		SOUND_LABEL_SE_PLAYERHIT_1,		// プレイヤーの被ダメージ１
		SOUND_LABEL_SE_PLAYERHIT_2,		// プレイヤーの被ダメージ２
		SOUND_LABEL_SE_HIT_LIGHT,		// ヒット音（弱）
		SOUND_LABEL_SE_HIT_MEDIUM,		// ヒット音（中）
		SOUND_LABEL_SE_HIT_LARGE,		// ヒット音（大）
		SOUND_LABEL_SE_HIT_HIGH,		// ヒット音（蹴り上げ）
		SOUND_LABEL_SE_FLASH,			//　光
		SOUND_LABEL_SE_PHONE,			//　電話
		SOUND_LABEL_SE_SMALLHIT,		//　小物ヒット時
		SOUND_LABEL_SE_HITDRAM,		//　小物ヒット
		SOUND_LABEL_SE_JIHANKI,		//　自販機
		SOUND_LABEL_MAX
	} SOUND_LABEL;
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;


	CSound();	//コンストラクタ
	~CSound();	//デストラクタ
	HRESULT InitSound(HWND hWnd);	//初期化
	void UninitSound(void);			//終了処理
	HRESULT PlaySound(SOUND_LABEL label);//サウンドの再生
	void StopSound(SOUND_LABEL label);//サウンドの停止
	void StopSound(void);//サウンドの停止
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },		// タイトル
		{ "data/BGM/tutorial.wav", -1 },	// チュートリアル
		{ "data/BGM/game.wav", -1 },		// ゲーム
		{ "data/BGM/boss.wav", -1 },		// ボス
		{ "data/BGM/result.wav", 0 },		// リザルト
		{ "data/BGM/ranking.wav",-1 },		// ランキング
		{ "data/SE/item_pickup.wav", 0 },	// 取得
		{ "data/SE/hit.wav", 0 },			// 被ダメージ(相手)
		{ "data/SE/decide.wav", 0 },		// 決定
		{ "data/SE/run.wav", 0 },			// 走り
		{ "data/SE/fire/fire.wav", 0 },		//炎１
		{ "data/SE/fire/fire2.wav", 0 },		//炎２
		{ "data/SE/fire/fire3.wav", 0 },		//炎
		{ "data/SE/0_voice_player/player_Attack_0.wav", 0 },		//プレイヤーの攻撃1
		{ "data/SE/0_voice_player/player_Attack_1.wav", 0 },		//プレイヤーの攻撃2
		{ "data/SE/0_voice_player/player_Attack_2.wav", 0 },		//プレイヤーの攻撃3
		{ "data/SE/0_voice_player/player_hit1.wav", 0 },			//プレイヤーのダメージ１
		{ "data/SE/0_voice_player/player_hit2.wav", 0 },				//プレイヤーのダメージ２
		{ "data/SE/hit/light.wav", 0 },				//弱攻撃
		{ "data/SE/hit/medium.wav", 0 },				//中攻撃
		{ "data/SE/hit/large.wav", 0 }	,			//大攻撃
		{ "data/SE/hit/high.wav", 0 },			//蹴り上げ
		{ "data/SE/flash.wav", 0 },			//光
		{ "data/SE/phone.wav", 0 },			//電話
		{ "data/SE/hit.wav", 0 },			//ヒット
		{ "data/SE/goin.wav", 0 },			//ガイーン
		{ "data/SE/jihanki.wav", 0 }			//自販機

	};


};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
