﻿
// DogIdentificationDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"

#include "my_system.h"

#include "Util.h"


// CDogIdentificationDlg 대화 상자
class CDogIdentificationDlg : public CDialogEx
{
// 생성입니다.
public:
	CDogIdentificationDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOGIDENTIFICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    void GetCurrentModulePath(CString& strPath);

private:
    void DecideCurrentModulePath(CString& strPath);
    

    

private:
	bool m_bAlreadySetPasswd;
    CString m_currentPath;

public:
	eModError TestFunction01();
	afx_msg void OnBnClickedBtnEditpw();
	afx_msg void OnBnClickedBtnRegisterdoginfo();
	afx_msg void OnBnClickedBtnSearchdoginfo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
