#pragma once


// CameraMgr 대화 상자

class OpenCam : public CDialogEx
{
	DECLARE_DYNAMIC(OpenCam)

public:
	OpenCam(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~OpenCam();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Camera };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTakephoto();
	VideoCapture* capture;
	CImage cimage_mfc;
	Mat mat_frame; //카메라에서 가져오는 프레임
	Mat mat_temp;
	CStatic m_camera;
	CStatic* m_picDog; //dog register에서 가져온것.
	BITMAPINFO* bitInfo;
	afx_msg void OnStnClickedCamera();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
