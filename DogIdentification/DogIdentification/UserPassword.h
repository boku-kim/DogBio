#pragma once
#include "afxwin.h"
#include "StorageMgr.h"

// UserPassword 대화 상자

class UserPassword : public CDialogEx
{
	DECLARE_DYNAMIC(UserPassword)

public:
	UserPassword(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~UserPassword();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PASSWORD };
#endif

public:
	int CheckPW(CString str1, CString str2);
	int CheckArg(CString str3);
    int GetPasswdSuccess();
	CString GetPassword();
	///<StorageMgr m_stors;
	
private:
	CString m_strPassword;
	CString m_strPasswordcheck;
	CEdit m_editPassword1;
	CEdit m_editPassword2; //Check PW
	int ps1;
	int ps2;
	int ps3;
    int m_nSucces;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
