// CameraMgr.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "CameraMgr.h"
#include "afxdialogex.h"

using namespace cv;
// CameraMgr 대화 상자

IMPLEMENT_DYNAMIC(CameraMgr, CDialogEx)

CameraMgr* CameraMgr::m_Inst = NULL;
CameraMgr::CameraMgr(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGCAMERA, pParent)
{

}

CameraMgr::~CameraMgr()
{
}

void CameraMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICCAMERAPIC, m_camerapic);
}


BEGIN_MESSAGE_MAP(CameraMgr, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//	ON_WM_INITMENU()
	ON_BN_CLICKED(IDC_BTNTAKEPIC, &CameraMgr::OnBnClickedBtntakepic)
	ON_BN_CLICKED(IDCANCEL, &CameraMgr::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CameraMgr::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTNTAKEPIC, &CameraMgr::OnBnClickedBtntakepic)
END_MESSAGE_MAP()

CameraMgr* CameraMgr::GetInstance()
{
	if (m_Inst == NULL)
	{
		m_Inst = new CameraMgr();
	}
	return m_Inst;
}

void CameraMgr::ReleaseInstance()
{
	if (m_Inst)
	{
		delete m_Inst;
	}
}

// CameraMgr 메시지 처리기


void CameraMgr::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CameraMgr::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
	//mat_frame가 입력 이미지입니다. 
	capture->read(mat_frame);


	//이곳에 OpenCV 함수들을 적용합니다.

	//여기에서는 그레이스케일 이미지로 변환합니다.
	//cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);


	//화면에 보여주기 위한 처리입니다.
	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_frame.cols % 4);
	}


	if (border > 0 || mat_frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_frame;
	}


	RECT r;
	m_camerapic.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);

	bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//그레이스케일 인경우 팔레트가 필요
	if (bpp == 8)
	{
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect

	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		imgWidth = mat_temp.cols - border;
		imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	HDC dc = ::GetDC(m_camerapic.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0); //화면에 띄움

	::ReleaseDC(m_camerapic.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CameraMgr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	capture = new VideoCapture(0);
	if (!capture->isOpened())
	{
		MessageBox(_T("캠을 열수 없습니다. \n"));
	}

	//웹캠 크기를  320x240으로 지정    
	capture->set(CAP_PROP_FRAME_WIDTH, 320);
	capture->set(CAP_PROP_FRAME_HEIGHT, 240);

	SetTimer(1000, 30, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CameraMgr::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CameraMgr::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect rect;
	m_picDog->GetClientRect(rect);
	int iwidth, iheight;
	iwidth = rect.Width();
	iheight = rect.Height();

	cimage_mfc.Create(iwidth, iwidth, 24);
	StretchDIBits(cimage_mfc.GetDC(), 0, 0, iwidth, iheight, 0, 0, iwidth, iheight, mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);

	HDC dc = ::GetDC(m_picDog->m_hWnd);
	//cimage_mfc.BitBlt(dc, 0, 0); //화면에 띄움
	cimage_mfc.TransparentBlt(dc, 0, 0, iwidth, iheight, SRCCOPY);
	::ReleaseDC(m_camerapic.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

	CDialogEx::OnOK();
}


void CameraMgr::OnBnClickedBtntakepic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//카메라 화면 찍은걸로 멈춤
	KillTimer(1000);
}
