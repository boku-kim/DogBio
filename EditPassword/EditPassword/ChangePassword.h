#pragma once


// ChangePassword 대화 상자

class ChangePassword : public CDialog
{
	DECLARE_DYNAMIC(ChangePassword)

public:
	ChangePassword(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ChangePassword();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGEPASSWORD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
