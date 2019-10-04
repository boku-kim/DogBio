#pragma once
#include "afxwin.h"


// UserPassword 대화 상자입니다.

class UserPassword : public CDialogEx
{
	DECLARE_DYNAMIC(UserPassword)

public:
	UserPassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~UserPassword();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_editPassword1;
    CEdit m_editPassword2;

    CString GetPassword();
private:
    CString m_strPassword;
public:
    afx_msg void OnBnClickedOk();
};
