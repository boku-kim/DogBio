#pragma once


// OpenCamera 대화 상자

class OpenCamera : public CDialogEx
{
	DECLARE_DYNAMIC(OpenCamera)

public:
	OpenCamera(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~OpenCamera();

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
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTakephoto();
	virtual BOOL OnInitDialog();
	CButton m_camera;
}
