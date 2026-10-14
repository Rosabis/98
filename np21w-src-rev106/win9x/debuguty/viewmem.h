/**
 * @file	viewmem.h
 * @brief	DebugUty 用メモリ読み込みクラスの宣言およびインターフェイスの定義をします
 */

#pragma once

/**
 * @brief メモリ読み込み
 */
struct DebugUtyViewMemory
{
	UINT8	vram;		//!< VRAM バンク
	UINT8	itf;		//!< ITF バンク
	UINT8	A20;		//!< A20 ライン
	UINT8	vram_ovr;	//!< manual VRAM bank override flag
	UINT8	a20_ovr;	//!< manual A20 line override flag

	DebugUtyViewMemory();
	void Update();
	void Read(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer) const;
};
