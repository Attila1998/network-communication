#pragma once
#include <list>
#include <iostream>
#include <windows.h>
#include <limits.h>

class SysThread{

private:
	friend DWORD WINAPI runStub(LPVOID mthread);
	volatile bool m_bRunning;
	volatile bool m_bExited;
	HANDLE m_thread;

public:
	SysThread(void);
	virtual ~SysThread();
	virtual bool start(void);
	virtual bool stop(unsigned int timeout = 0);

	inline volatile bool& isRunning(void)
	{
		return m_bRunning;
	}
	inline volatile bool& isExited(void)
	{
		return m_bExited;
	}

protected:
	virtual void run(void); //Ezt a metodust a sz�rmaztatott
	//oszt�lyban fel�l kell �rni.Ide kell be�rni az utas�t�s szekvenci�t
		//amit a sz�lunk v�gre kell hajtson
public:
	static const unsigned int INFINIT_WAIT;
};


