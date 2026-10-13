/*!
 * @file	guard.h
 * @brief	クリ�?ィカル セクション クラスの宣言およびインターフェイスの定義をしま�?
 */

#pragma once

#include <pthread.h>

/*!
 * @brief クリ�?ィカル セクション クラス
 */
class CGuard
{
public:
	/*! コンストラクタ */
	CGuard() { ::pthread_mutex_init(&m_cs, NULL); }

	/*! �?ストラクタ */
	~CGuard() { ::pthread_mutex_destroy(&m_cs); }

	/*! クリ�?ィカル セクション開�? */
	void Enter() { ::pthread_mutex_lock(&m_cs); }

	/*! クリ�?ィカル セクション終�? */
	void Leave() { ::pthread_mutex_unlock(&m_cs); }

private:
	pthread_mutex_t m_cs;		//!< クリ�?ィカル セクション�?報
};
