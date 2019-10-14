#pragma once
#include "CameraMgr.h"

// SearchDogInfo 대화 상자

class SearchDogInfo : public CDialogEx
{
	DECLARE_DYNAMIC(SearchDogInfo)

public:
	SearchDogInfo(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SearchDogInfo();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEARCHDOGINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_edit_dogname;
	CEdit m_editdogage;
	CEdit m_editspecies;
	CEdit m_editaddr;
	UINT m_radiogender;

	CString m_dogName;
	CString m_dogAge;
	CString m_dogSpecies;
	CString m_dogAddr;
	CString m_gender;

	bool e_flag;
	HICON m_hIcon;
public:
	
	CStatic m_PIC;
	CImage m_bmpBitmap;
	afx_msg void OnBnClickedBtnLoadimg();
	afx_msg void OnBnClickedBtnOpencamera();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void RadioCtrl(UINT ID);
	void EditOnMode();
	void EditOffMode();

	
	CButton m_btn_load;
	CButton m_btn_opencam;
	CButton m_btn_left;
	CButton m_btn_right;
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	virtual BOOL OnInitDialog();
};
