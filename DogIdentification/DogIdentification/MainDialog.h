#pragma once


// MainDialog 대화 상자

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)

public:
	MainDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MainDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnEditpw();
	afx_msg void OnBnClickedBtnRegisterdoginfo();
	afx_msg void OnBnClickedBtnSearchdoginfo();
};
