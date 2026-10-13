/**
 * @file	threadbase.h
 * @brief	スレ�?ド基底クラスの宣言およびインターフェイスの定義をしま�?
 */

#pragma once

#include <pthread.h>
#include <unistd.h>

/**
 * @brief スレ�?ド基底クラス
 */
class CThreadBase
{
public:
	CThreadBase();
	virtual ~CThreadBase();

	bool Start();
	void Stop();
	void SetStackSize(size_t stack_size);
	static void Delay(unsigned int usec);

protected:
	virtual bool Task()=0;		//!< スレ�?�? タスク

private:
	pthread_t m_thread;			//!< スレ�?�? フラグ
	bool m_bCreated;			//!< スレ�?ド作�?�フラグ
	bool m_bDone;				//!< 終�?フラグ
	size_t m_stack_size;		//!< スタ�?ク サイズ
	static void* StartRoutine(void* arg);
};

/**
 * スタ�?ク サイズの設�?
 * @param[in] stack_size スタ�?ク サイズ
 */
inline void CThreadBase::SetStackSize(size_t stack_size)
{
	m_stack_size = stack_size;
}

/**
 * スリー�?
 * @param[in] usec マイクロ�?
 */
inline void CThreadBase::Delay(unsigned int usec)
{
	::usleep(usec);
}
