
// dogdog191002Dlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "TestDlg.h"

typedef struct _Dogdata {
	CString name;
	CString age;
	CString species;
	CString address;
}Dogdata;


// Cdogdog191002Dlg 대화 상자
class Cdogdog191002Dlg : public CDialogEx
{
// 생성입니다.
public:
	Cdogdog191002Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOGDOG191002_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	void EditOnMode();
	void EditOffMode();
	

protected:
	CEdit _edit_name;
	CEdit _edit_age;
	CEdit _edit_species;
	CEdit _edit_address;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonLoadImg();
protected:
	CButton _btn_load_image;
	CButton _btn_open_camera;
	afx_msg void OnBnClickedButtonOpenCamera();
	CButton _btn_left;
	CButton _btn_right;
	Dogdata _dog_info;
	bool e_flag;
};
