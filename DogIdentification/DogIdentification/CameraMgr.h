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
	enum { IDD = IDD_DIALOG_CAMERA };
#endif

private:
	static CameraMgr* m_Inst;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_camerapic; //picture control 변수
	RECT camera_r;//picture control 크기구함 
	VideoCapture* capture;
	
	CImage cimage_mfc; //bitmap이미지를 담을 객체
	BITMAPINFO* bitInfo;
	Mat mat_frame; //카메라에서 가져오는 프레임
	Mat mat_temp;
	int imgWidth;
	int imgHeight;

public:
	CStatic* m_picDog; //dog register에서 가져온것.
	static CameraMgr* GetInstance();
	static void ReleaseInstance();
	afx_msg void OnBnClickedBtnTake();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
