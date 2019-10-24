# Dog Biometrics Project
## test update


-2019.10.24까지의 업데이트 상황
=======
## test update

-2019.10.24������ ����Ʈ ��Ȳ

1. DogRegister::Checkarg() �߰�
	�̸�,�,�ּ�,���� ����� ����
	���� ��� ����� ����(���ڸ��� �ٸ��� �ĵ� ����)

2. CameraMgr.cpp �� �ڵ��߰�
	CameraMgr::OnBnClickedOk() �� CameraMgr::OnBnClickedCancel()�� �ڵ��߰�
	

	�߰��� �ڵ�:

		if (capture)
		{
			delete capture;
		}
	�߰��� ���:
		���ϸ� ī�޶� �Ȳ��� -> �Ȳ���� ����� �ٽ� ����Ϸ�� ������ MFC�� ����

3-1. stdafx.h�� #define��� SAVE_PATH�� IMAGE_PATH �߰�
	������� ��ǻ�� C:\DogIdentificationdn�� ��� ����� �̹���� C:\DogIdentification\Image �� �����Ϸ��
	IMAGE_PATH�� ����� "Image"�� �ߴµ� �׳� " C:\DogIdentification\Image"�� �ٲܱ� �����(3-3���)


3-2. DogIdentificationDlg.cpp�� SAVE_PATH�� ��� ����� ����� �ڵ� ���
	����� �����ϸ�, ��� ����� ���� �Ŀ� ����� ��� ���� �� ���

3-3. DogResister.cpp�� Image ��� ����� ����� CImage�̿��ؼ� ���� ��� �����ϴ� �ڵ� ���(4�� ����)
	�̶� ��� ��� �Ҷ�  SAVE_PATH�� IMAGE_PATH �� ���ļ� ����ϴµ� �׳� IMAGE_PATH �ϳ��� �� �� �ְ� IMAGE_PATH ��ü�� �ٲܱ� �����
	���� �����Ҷ� ���� �̸�� ������ �̸�,
		�̶� ��� �̸��� ������ �̹� �ִٸ� �̸�_����, �� ����
		���ڴ� ����̸��� ������ �Ѹ��� ��������� �þ.

4. CameraMgr.h�� CStatic* m_picDog �߰��ϰ� DogResister.cpp ���� open camera��ư ������ m_bmpBitmap�̶� ������Ŵ (�̹��� �����ϱ� ���)
	DogRegister::OnBnClickedBtnOpencamera() ���� pCameraInst->save_img = &m_bmpBitmap; �� ����
	CameraMgr::OnBnClickedOk() �κп��� save_img�� ���� ����

5. CameraMgr::OnBnClickedOk()���� StretchDIBits�Լ� ���
	������ Ʋ����� ���� �Ǳ淡 ���� �ʺ� �����, �� �ڵ尡 ����� ���� ����
	StretchDIBits(cimage_mfc.GetDC(), 0, 0, rect.Width(), rect.Height(), 0, 0, imgWidth, imgHeight, mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY); ����
	imgWidth, imgHeight �κи� �ٲ�



=======

# [MFC]

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



## [MFC]
## 마친 일
- UserPassword(완료)

	- 저장된 패스워드가 없을 때 패스워드 등록 창으로 넘어가서 패스워드 등록
- Login(완료)
	- 저장된 패스워드가 있을 때 로그인 창으로 넘어가서 패스워드 비교후 로그인
- EditPassword(완료)

	- 패스워드 체크
	- 패스워드 변경 후, 변경 패스워드 저장
~~~
 패스워드 파일의 저장경로는 my_define.h 파일에 명시되어 있다. 
 ~~~
- CamerMgr(완료)
	- 사진찍기
- DogRegister(부분완료)

	- local에서 이미지 불러오기
	- webcam에서 사진 찍어 띄우기(CameraMgr에서 찍은 사진 받아서 띄움)
    - db에 강아지 정보 등록(이미지 제외한 모든 정보)

- SearchDogInfo(부분완료)

	- local에서 이미지 불러오기
	- webcam에서 사진 찍어 띄우기(CameraMgr에서 찍은 사진 받아서 띄움)
	- 강아지정보 Search( 현재는 이미지검색이 안되므로 db의 제일 마지막에 등록되어있는 강아지 정보 불러옴)
	- 강아지정보 변경 & db에 저장
	- 강아지정보 삭제

## 해야할 일
- DogRegister

	- 이미지 분석 & 분석정보 받아오기
	- 분석한 이미지가 기존에 존재하는 강아지 정보인지 Search
	- 이미지 분석정보 저장
- SearchDogInfo
  - 이미지 검색해서 강아지 정보 받아오기
- 창닫기 버튼 비활성화

- memory leak 해결

[RaceCar]


# [RaceCar]

