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
	SOUND_LABEL_TITLEBGM = 0,	// タイトル
	SOUND_LABEL_SELECTBGM,		// セレクト
	SOUND_LABEL_CHARASELECTBGM,	// キャラセレクト
	SOUND_LABEL_GAMEBGM000,		// ゲーム000
	SOUND_LABEL_GAMEBGM001,		// ゲーム001
	SOUND_LABEL_RESULTBGM,		// リザルト
	SOUND_LABEL_HIT000,		// ヒット000
	SOUND_LABEL_HIT001,		// ヒット001
	SOUND_LABEL_JUMP,		// ジャンプ
	SOUND_LABEL_LAND,		// 着地
	SOUND_LABEL_MOVE,		// 移動
	SOUND_LABEL_GAMEEND000,		// ゲーム終了000
	SOUND_LABEL_GAMESETTING000,		// ゲームセッティング000
	SOUND_LABEL_GAMESTART000,		// ゲームスタート000
	SOUND_LABEL_GAMESTART001,		// ゲームスタート001
	SOUND_LABEL_TIMEUP000,		// タイムアップ000
	SOUND_LABEL_PAUSE000,		// ポーズ000
	SOUND_LABEL_SYSTEMSELECT000,		// ポーズ000
	//ロボット　SE
	SOUND_ROBOT_GARD,      //ロボット　ガード
	SOUND_ROBOT_BUTOBI,    //ロボット　ぶっ飛び
	SOUND_ROBOT_KAIHI,     //ロボット　回避
	SOUND_ROBOT_ATTACK000,    //ロボット　攻撃 000
	SOUND_ROBOT_ATTACK001,    //ロボット　攻撃 001
	SOUND_ROBOT_ATTACK002,    //ロボット　攻撃 002
	SOUND_ROBOT_ATTACK003,    //ロボット　攻撃 003
	SOUND_ROBOT_ATTACK004,    //ロボット　攻撃 004
	SOUND_ROBOT_ATTACK005,    //ロボット　攻撃 005
	SOUND_ROBOT_HUNOU,     //ロボット　戦闘不能
	SOUND_ROBOT_NAGERU,    //ロボット　投げる
	SOUND_ROBOT_DAMEJI,    //ロボット　被ダメージ
	SOUND_ROBOT_IDOU,    //ロボット　移動
	SOUND_ROBOT_JANP,    //ロボット　ジャンプ
	//河童　SE
	SOUND_KAPA_GARD,        //河童　ガード
	SOUND_KAPA_BUTOBI,      //河童　ぶっ飛び
	SOUND_KAPA_KAIHI,       //河童　回避
	SOUND_KAPA_ATTACK000,      //河童　攻撃 000
	SOUND_KAPA_ATTACK001,      //河童　攻撃 001
	SOUND_KAPA_ATTACK002,      //河童　攻撃 002
	SOUND_KAPA_ATTACK003,      //河童　攻撃 003
	SOUND_KAPA_ATTACK004,      //河童　攻撃 004
	SOUND_KAPA_ATTACK005,      //河童　攻撃 005
	SOUND_KAPA_HUNOU000,       //河童　戦闘不能
	SOUND_KAPA_HUNOU001,       //河童　戦闘不能
	SOUND_KAPA_NAGERU,      //河童　投げる
	SOUND_KAPA_DAMEJI000,      //河童　被ダメージ
	SOUND_KAPA_DAMEJI001,      //河童　被ダメージ
	SOUND_KAPA_DAMEJI002,      //河童　被ダメージ
	SOUND_KAPA_VICTORY,   //河童　勝利時
	SOUND_KAPA_IDOU,   //河童　移動
	//人間
	SOUND_HUMAN_GARD000,       //人間　ガード
	SOUND_HUMAN_GARD001,       //人間　ガード
	SOUND_HUMAN_BUTOBI,     //人間　ぶっ飛び
	SOUND_HUMAN_KAIHI,      //人間　回避
	SOUND_HUMAN_ATTACK000,     //人間　攻撃 000
	SOUND_HUMAN_ATTACK001,     //人間　攻撃 001
	SOUND_HUMAN_ATTACK002,     //人間　攻撃 002
	SOUND_HUMAN_ATTACK003,     //人間　攻撃 003
	SOUND_HUMAN_ATTACK004,     //人間　攻撃 004
	SOUND_HUMAN_ATTACK005,     //人間　攻撃 005
	SOUND_HUMAN_ATTACK006,     //人間　攻撃 006
	SOUND_HUMAN_ATTACK007,     //人間　攻撃 007
	SOUND_HUMAN_ATTACK008,     //人間　攻撃 008
	SOUND_HUMAN_ATTACK009,     //人間　攻撃 009
	SOUND_HUMAN_HUNOU,      //人間　戦闘不能
	SOUND_HUMAN_NAGERU,     //人間　投げる
	SOUND_HUMAN_DAMEJI,     //人間　被ダメージ
	SOUND_HUMAN_VICTORY,  //人間　勝利時　

	//天狗
	SOUND_TENGU_GARD,       //天狗　ガード
	SOUND_TENGU_BUTOBI,     //天狗　ぶっ飛び
	SOUND_TENGU_KAIHI,      //天狗　回避
	SOUND_TENGU_ATTACK000,     //天狗　攻撃 000
	SOUND_TENGU_ATTACK001,     //天狗　攻撃 001
	SOUND_TENGU_ATTACK002,     //天狗　攻撃 002
	SOUND_TENGU_ATTACK003,     //天狗　攻撃 003
	SOUND_TENGU_ATTACK004,     //天狗　攻撃 004
	SOUND_TENGU_ATTACK005,     //天狗　攻撃 005
	SOUND_TENGU_HUNOU000,      //天狗　戦闘不能 000
	SOUND_TENGU_HUNOU001,      //天狗　戦闘不能 001
	SOUND_TENGU_NAGERU,     //天狗　投げる
	SOUND_TENGU_DAMEJI,     //天狗　被ダメ
	SOUND_TENGU_VICTORY,    //天狗　勝利時　
	////狐　SE
	SOUND_FOX_GARD,         //狐　ガード
	SOUND_FOX_BUTOBI,       //狐　ぶっ飛び
	SOUND_FOX_KAIHI,        //狐　回避
	SOUND_FOX_ATTACK000,       //狐　攻撃000
	SOUND_FOX_ATTACK001,       //狐　攻撃001
	SOUND_FOX_ATTACK002,       //狐　攻撃002
	SOUND_FOX_ATTACK003,       //狐　攻撃003
	SOUND_FOX_ATTACK004,       //狐　攻撃004
	SOUND_FOX_ATTACK005,       //狐　攻撃005
	SOUND_FOX_HUNOU,        //狐　戦闘不能
	SOUND_FOX_NAGERU,       //狐　投げる
	SOUND_FOX_DAMEJI,       //狐　被ダメージ
	SOUND_FOX_VICTORY,     //狐　勝利時
	// 戦闘系
	SOUND_EFFECT_WIND000,	// 風１
	SOUND_EFFECT_WIND001,	// 風2
	SOUND_EFFECT_SRASH000,	// ひっかき000
	SOUND_EFFECT_SRASH001,	// ひっかき001
	SOUND_EFFECT_SRASH002,	// ひっかき002
	SOUND_EFFECT_FLAME000,	//火炎放射000
	SOUND_EFFECT_FLAME001,	//火炎放射001
	SOUND_EFFECT_BOMB000,	// 爆発
	SOUND_EFFECT_BOMB001,	// 爆発
	SOUND_EFFECT_GUARD,	// ガード
	SOUND_EFFECT_GUARD_CRASH,	// ガードクラッシュ
	// 太鼓
	SOUND_TAIKO_1,	// 太鼓
	SOUND_TAIKO_2,	// 太鼓
	SOUND_TAIKO_3,	// 太鼓
	SOUND_TAIKO_4,	// 太鼓
	// 歓声
	SOUND_KANSEI_1,	// 歓声
	SOUND_KANSEI_2,// 歓声
	// システム
	SOUND_SELECTMOVE,// カーソル移動
	SOUND_DECISION,	// 決定
	SOUND_DECISION_1,	// 決定
	SOUND_CUTIN,		// カットイン
	SOUND_SPECIALROGO,	// SPECIALロゴ
	SOUND_CHARACTOR_SELECT,// キャラクター選択
	SOUND_CHARACTOR_READY,	// 準備
	SOUND_ALERT,	// 警告音
	SOUND_SUNDER,	// 雷
	SOUND_RAIN,// 雨
	SOUND_FLOOD,// 氾濫
	SOUND_LIFE,		// ライフアップ
	SOUND_ATTACK,	// パワーアップ
	SOUND_SPEED,	// スピードアップ
	SOUND_METEO,	// メテオ
	SOUND_EXPLOSION,// 爆発
		SOUND_GUST,// 突風
		SOUND_TIMELIMIT,// タイム
		SOUND_RESULT_HAC01,// リザルトSE
		SOUND_RESULT_HAC02,// リザルトSE
		SOUND_RESULT_TEIKI,// 定時だ～～～～
		SOUND_RESULT_SOKKOU,// 速攻家に帰ろう！
		SOUND_RESULT_GARASU,// ガラス割れ
		SOUND_HIT_HAC,// ひっと音
		SOUND_CHAR,
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
