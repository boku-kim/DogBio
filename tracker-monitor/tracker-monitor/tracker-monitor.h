
// tracker-monitor.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CtrackermonitorApp:
// �� Ŭ������ ������ ���ؼ��� tracker-monitor.cpp�� �����Ͻʽÿ�.
//

class CtrackermonitorApp : public CWinApp
{
public:
	CtrackermonitorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CtrackermonitorApp theApp;