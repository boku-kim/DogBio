#pragma once
#include "afxwin.h"
// CameraMgr 대화 상자

class CameraMgr : public CDialogEx
{
	DECLARE_DYNAMIC(CameraMgr)

public:
	CameraMgr(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CameraMgr();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGCAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	static CameraMgr* GetInstance();
	static void ReleaseInstance();

	//eModError RegisterPassword(char* pszPassword, eSecurityType data_type);

private:
	static CameraMgr* m_Inst;

public:
	CStatic m_camerapic;
	VideoCapture* capture;
	Mat mat_frame; //카메라에서 가져오는 프레임
	Mat mat_temp;
	CImage cimage_mfc; //bitmap이미지를 담을 객체
	CStatic *m_picDog; //dog register에서 가져온것.
	BITMAPINFO* bitInfo;
	int imgWidth;
	int imgHeight;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnInitMenu(CMenu* pMenu);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtontakepic();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
