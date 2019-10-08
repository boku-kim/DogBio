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
public:
	
	CStatic m_PIC;
	CImage m_bmpBitmap;
	afx_msg void OnBnClickedBtnLoadimg();
	afx_msg void OnBnClickedBtnOpencamera();
	afx_msg void OnBnClickedBtnSearch();
};
