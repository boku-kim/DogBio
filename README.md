# Dog Biometrics Project
## test update

-2019.10.24까지의 업데이트 상황

1. DogRegister::Checkarg() 추가
	이름,종,주소,나이 없으면 에러
	나이 범위 벗어나면 에러(숫자말고 다른거 쳐도 에러)

2. CameraMgr.cpp 에 코드추가
	CameraMgr::OnBnClickedOk() 와 CameraMgr::OnBnClickedCancel()에 코드추가
	

	추가한 코드:

		if (capture)
		{
			delete capture;
		}
	추가한 이유:
		안하면 카메라가 안꺼짐 -> 안꺼지면 사진을 다시 등록하려고 누르면 MFC가 꺼짐

3-1. stdafx.h에 #define으로 SAVE_PATH랑 IMAGE_PATH 추가
	사용자의 컴퓨터 C:\DogIdentificationdn에 폴더를 만들고 이미지는 C:\DogIdentification\Image 에 저장하려고
	IMAGE_PATH를 현재는 "Image"로 했는데 그냥 " C:\DogIdentification\Image"로 바꿀까 고려중(3-3참조)


3-2. DogIdentificationDlg.cpp에 SAVE_PATH에 폴더가 없으면 만드는 코드 넣음
	어플을 실행하면, 존재 유무를 따진 후에 없으면 폴더를 만들 수 있음

3-3. DogResister.cpp에 Image 폴더 없으면 만들고 CImage이용해서 사진 정보 저장하는 코드 넣음(4랑 연동)
	이때 경로 지정 할때  SAVE_PATH와 IMAGE_PATH 를 합쳐서 지정하는데 그냥 IMAGE_PATH 하나로 쓸 수 있게 IMAGE_PATH 자체를 바꿀까 고려중
	사진 저장할때 저장 이름은 강아지 이름,
		이때 같은 이름의 강아지가 이미 있다면 이름_숫자, 로 저장
		숫자는 같은이름의 강아지가 한마리 있을때마다 늘어남.

4. CameraMgr.h에 CStatic* m_picDog 추가하고 DogResister.cpp 에서 open camera버튼 누르면 m_bmpBitmap이랑 연동시킴 (이미지를 저장하기 위해)
	DogRegister::OnBnClickedBtnOpencamera() 에서 pCameraInst->save_img = &m_bmpBitmap; 로 연동
	CameraMgr::OnBnClickedOk() 부분에서 save_img를 통해 저장

5. CameraMgr::OnBnClickedOk()에서 StretchDIBits함수 수정
	사진이 틀어져서 저장 되길래 높이 너비 수정함, 원래 코드가 기억이 나지 않지만
	StretchDIBits(cimage_mfc.GetDC(), 0, 0, rect.Width(), rect.Height(), 0, 0, imgWidth, imgHeight, mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY); 에서
	imgWidth, imgHeight 부분만 바꿈



