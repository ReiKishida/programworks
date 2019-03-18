////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// サウンド処理 [sound.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//******************************************************************************************************************************
// サウンドファイル
//******************************************************************************************************************************
typedef enum
{
	SOUND_LABEL_TITLEBGM = 0,		// タイトル
	SOUND_LABEL_TUTORIAL,			// チュートリアル
	SOUND_LABEL_GAMEBGM1,			// ゲーム
	SOUND_LABEL_BOSSBGM,			// ボス
	SOUND_LABEL_GAMECLEARE,			// クリア
	SOUND_LABEL_GAMEOVER,			// ゲームオーバー
	SOUND_LABEL_RANKING,			// ランキング
	SOUND_LABEL_DEATH,				// 死亡
	SOUND_LABEL_SE_DECIDE,			// 決定
	SOUND_LABEL_SE_SELECT,			// 選ぶ
	SOUND_LABEL_SE_PAUSE,			// ポーズ
	SOUND_LABEL_SE_ITEMGET,			// アイテムゲット
	SOUND_LABEL_SE_HIT_0,			// ヒット
	SOUND_LABEL_SE_HIT_1,			// ヒット
	SOUND_LABEL_SE_LOCKON,			// ロックオン
	SOUND_LABEL_SE_GRAUND,			// 地面
	SOUND_LABEL_SE_EXPLOSION,		// 爆発
	SOUND_LABEL_SE_ATTACKWOOD,		// 木攻撃
	SOUND_LABEL_VOICE_ATTACK,		// 攻撃ボイス
	SOUND_LABEL_VOICE_DAMAGE,		// ダメージ受ける
	SOUND_LABEL_VOICE_DEATH,		// 死ぬ
	SOUND_LABEL_VOICE_TUTORIALEND,	// チュートリアル終わり
	SOUND_LABEL_SE_GAMECLEARUI,		// ゲームクリア
	SOUND_LABEL_SE_GAMEOVERUI,		// ゲームオーバー
	SOUND_LABEL_SE_TORI_MESSAGEWINDOW,		// message
	//--------------------------------キック系-------------------------------------------------------------------//
	SOUND_LABEL_SE_KICK,											// キック基本
	SOUND_LABEL_SE_KICK_KICK,											// キック基本
	SOUND_LABEL_SE_KICK_KICK_KICK,									// キック→キック→キック(サマーソルト)
	SOUND_LABEL_SE_KICK_JUMP,										// キック→ジャンプ(ボルケーノカット)
	SOUND_LABEL_SE_KICK_JUMP_PUNCH,									// キック→ジャンプ→パンチ(叩きつけ)
	SOUND_LABEL_SE_KICK_JUMP_PUNCH2,									// キック→ジャンプ→パンチ(叩きつけ)
	SOUND_LABEL_SE_KICK_SPIN,										// キック→ジャンプ→パンチ(回し蹴り)
//--------------------------------パンチ系------------------------------------------------------------------//
SOUND_LABEL_SE_PUNCH,											// パンチ基本
SOUND_LABEL_SE_PUNCH_PUNCH,											// パンチ→パンチ
	SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH,								// パンチ→パンチ→パンチ(右ストレート)
	SOUND_LABEL_SE_PUNCH_SPIN_JUMP,									// パンチ→回転→ジャンプ(昇竜拳)
	SOUND_LABEL_SE_PUNCH_SPIN,										// パンチ→回転(裏拳)
	//--------------------------------プレイヤー汎用モーション------------------------------------------------------//
	SOUND_LABEL_SE_COMBO_GENERIC,									// コンボ攻撃汎用(ヒット音)
	//--------------------------------回転系--------------------------------------------------------------------//
	SOUND_LABEL_SE_SPIN,											// 回転基本
	SOUND_LABEL_SE_SPIN_KICK,										// フレイムダンス
	SOUND_LABEL_SE_SPIN_PUNCH,										// 側転
	SOUND_LABEL_SE_SPIN_KICK_SPIN,									// ブレイクダンス
	//--------------------------------着地、ステップ-----------------------------------------------------//
	SOUND_LABEL_SE_RANDING,											// 着地音
	SOUND_LABEL_SE_STEP,											// ステップ
	//--------------------------------ジャンプ-------------------------------------------------------------//
	SOUND_LABEL_SE_JUMP,											// ジャンプ
	SOUND_LABEL_SE_JUMP_KICK,										// 飛び蹴り
	SOUND_LABEL_SE_JUMP_PUNCH,										// ハンマーパンチ
	SOUND_LABEL_SE_JUMP_SPIN,										// 蹴撃波
	//--------------------------------奪踊の舞--------------------------------------------------------------//
	SOUND_LABEL_SE_DATUYOU,											// 奪踊の舞
	//--------------------------------鎧の敵-----------------------------------------------------------------//
	SOUND_LABEL_SE_ARMOR_ACTION,									// 鎧の敵の攻撃
	SOUND_LABEL_SE_ARMOR_WALK,										// 鎧の敵の歩行
	//--------------------------------本ちゃん、ひげちゃんなど----------------------------------------//
	SOUND_LABEL_SE_ENEMY_ATTACK_GENERIC,							// 敵攻撃汎用
	
	SOUND_LABEL_VOICE_DATUYOU,							// 奪踊
	SOUND_LABEL_VOICE_JUMP_PUNCH,							// 奪踊
	SOUND_LABEL_VOICE_KICK_JUMP_PUNCH,							// 奪踊
	SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH,							// 奪踊
	SOUND_LABEL_VOICE_SPIN_PUNCH_KICK,							// 奪踊

	SOUND_LABEL_MAX
}SOUND_LABEL;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);	// 再生
void StopSound(SOUND_LABEL label);		// Uninitで同じ名前で止める
void StopSound(void);
void ChangeVolume(SOUND_LABEL label, float fVolume);
void CheckPos(D3DXVECTOR3 pos);
#endif
