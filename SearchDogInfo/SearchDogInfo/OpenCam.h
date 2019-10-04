#pragma once


// OpenCam 대화 상자

class OpenCam : public CDialogEx
{
	DECLARE_DYNAMIC(OpenCam)

public:
	OpenCam(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~OpenCam();

	VideoCapture* capture;
	Mat mat_frame;
	CImage cimage_mfc;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Camera };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTakephoto();
	//CButton m_camera;
	CStatic m_camera;
	afx_msg void OnStnClickedCamera();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
