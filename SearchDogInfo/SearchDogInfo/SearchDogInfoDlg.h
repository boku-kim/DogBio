
// SearchDogInfoDlg.h: 헤더 파일
//

#pragma once
#include "CameraMgr.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// CSearchDogInfoDlg 대화 상자
class CSearchDogInfoDlg : public CDialogEx
{
// 생성입니다.
public:
	CSearchDogInfoDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CImage m_bmpBitmap;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCHDOGINFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpencamera();
	afx_msg void OnBnClickedLodaimage();
	CStatic m_PIC;
};
