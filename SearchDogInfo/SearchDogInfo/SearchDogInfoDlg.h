#include "OpenCam.h"
// SearchDogInfoDlg.h: 헤더 파일
//

#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/cvconfig.h"
#include "opencv2/dnn.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/flann.hpp"
#include "opencv2/gapi.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/opencv_modules.hpp"
#include "opencv2/photo.hpp"
#include "opencv2/stitching.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/world.hpp"


using namespace std;
using namespace cv;

// CSearchDogInfoDlg 대화 상자
class CSearchDogInfoDlg : public CDialogEx
{
// 생성입니다.
public:
	CSearchDogInfoDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCHDOGINFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	void DisplayImage(int PICTURE_TARGET, Mat targetMat);


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
	afx_msg void OnBnClickedOpencamera();
	afx_msg void OnBnClickedLodaimage();
};
