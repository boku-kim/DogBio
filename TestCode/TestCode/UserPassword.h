#pragma once
#include "afxwin.h"
#include "StorageMgr.h"


// UserPassword ��ȭ �����Դϴ�.

class UserPassword : public CDialogEx
{
	DECLARE_DYNAMIC(UserPassword)

public:
	UserPassword(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~UserPassword();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_editPassword1;
    CEdit m_editPassword2; //Check PW
	int CheckPW(CString str1, CString str2);
	int CheckArg(CString str3);
    CString GetPassword();
	StorageMgr m_stors;
	int ps1;
	int ps2;
	int ps3;
private:
    CString m_strPassword;
	CString m_strPasswordcheck;

public:
    afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditPw2();
	afx_msg void OnEnChangeEditPw1();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnStnClickedStatic1();
};
