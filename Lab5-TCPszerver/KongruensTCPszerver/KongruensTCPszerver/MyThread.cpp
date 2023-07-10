#include "SysThread.h"
#include <limits.h>
#include <windows.h>


SysThread::SysThread(void)
{
	m_bRunning = false;
	m_bExited = true;
	m_thread = INVALID_HANDLE_VALUE;
}

SysThread::~SysThread()
{
}

void MyThread::run(void)
{
}