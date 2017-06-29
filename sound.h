//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#include "xaudio2.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_SE_ATTACK,		// 
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_JUMP,		// ジャンプ音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDPARAM;

//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound{
private:
	static IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// 各音素材のパラメータ
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

public:
	CSound( );
	~CSound( );

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);
};

#endif
