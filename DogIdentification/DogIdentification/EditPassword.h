#pragma once
#include "CheckPassword.h"
#include "ChangePassword.h"

// EditPassword 대화 상자

class EditPassword : public CDialogEx
{
	DECLARE_DYNAMIC(EditPassword)

public:
	CheckPassword* checkpw;
	ChangePassword* changepw;
	CString INPUT_pw1;
	CString INPUT_pw2;
	EditPassword(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EditPassword();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDITPASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCheck();
	afx_msg void OnBnClickedBtnChange();
	afx_msg void OnBnClickedBtnCancel();
	CEdit m_newpw1;
	CEdit m_newpw2;
};
