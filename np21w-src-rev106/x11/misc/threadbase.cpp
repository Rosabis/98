/**
 * @file	threadbase.cpp
 * @brief	スレ�?ド基底クラスの動作�?�定義を行いま�?
 */

#include "compiler.h"
#include "threadbase.h"

/**
 * コンストラクタ
 */
CThreadBase::CThreadBase()
	: m_bCreated(false)
	, m_bDone(false)
	, m_stack_size(0)
{
}

/**
 * �?ストラクタ
 */
CThreadBase::~CThreadBase()
{
	Stop();
}

/**
 * スレ�?ド開�?
 * @retval true 成功
 */
bool CThreadBase::Start()
{
	if (m_bCreated)
	{
		return false;
	}

	/* スタ�?ク サイズ調整 */
	pthread_attr_t tattr;
	::pthread_attr_init(&tattr);
	if (m_stack_size != 0)
	{
		::pthread_attr_setstacksize(&tattr, m_stack_size);
	}

	m_bDone = false;
	if (::pthread_create(&m_thread, &tattr, StartRoutine, this) != 0)
	{
		return false;
	}

	m_bCreated = true;
	return true;
}

/**
 * スレ�?ド終�?
 * @retval true 成功
 */
void CThreadBase::Stop()
{
	if (m_bCreated)
	{
		m_bDone = true;
		::pthread_join(m_thread, NULL);
		m_bCreated = false;
	}
}

/**
 * スレ�?ド�?��?
 * @param[in] arg this ポインタ
 * @retval 0 常に0
 */
void* CThreadBase::StartRoutine(void* arg)
{
	CThreadBase& obj = *(static_cast<CThreadBase*>(arg));
	while ((!obj.m_bDone) && (obj.Task()))
	{
	}

	return 0;
}
