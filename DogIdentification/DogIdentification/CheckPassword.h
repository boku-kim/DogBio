#pragma once


// CheckPassword 대화 상자

class CheckPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CheckPassword)

public:
	CheckPassword(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CheckPassword();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHECKPASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOk();
};
