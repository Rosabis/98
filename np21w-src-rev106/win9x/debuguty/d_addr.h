/**
 * @file	d_addr.h
 * @brief	アドレス入力ダイアログ (np2debug より移�?)
 */

#pragma once

#include "compiler.h"

/// アドレス入力ダイアログ
/// --------------------
// seg:off また�?�物�?アドレスを�?�力させ、結果�? (seg << 16) | off で返す�?
// キャンセル時�?� 0xffffffff�?
//!
//! @param[in] hParent 親ウィンドウ
//! @param[in] nSeg    初期セグメン�?
//! @param[in] nOff    初期オフセ�?�?
//! @return (seg << 16) | off、キャンセル�? 0xffffffff
UINT32 daddr_input(HWND hParent, UINT16 nSeg, UINT16 nOff);
/// --------------------
