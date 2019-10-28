# Dog Biometrics Project

## test update

-2019.10.24ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Æ® ï¿½ï¿½È²

1. DogRegister::Checkarg() ï¿½ß°ï¿½
	ï¿½Ì¸ï¿½,ï¿?ï¿½Ö¼ï¿½,ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½
	ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½î³ªï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½Ú¸ï¿½ï¿½ï¿½ ï¿½Ù¸ï¿½ï¿½ï¿½ ï¿½Äµï¿½ ï¿½ï¿½ï¿½ï¿½)

2. CameraMgr.cpp ï¿½ï¿½ ï¿½Úµï¿½ï¿½ß°ï¿½
	CameraMgr::OnBnClickedOk() ï¿½ï¿½ CameraMgr::OnBnClickedCancel()ï¿½ï¿½ ï¿½Úµï¿½ï¿½ß°ï¿½
	

	ï¿½ß°ï¿½ï¿½ï¿½ ï¿½Úµï¿½:

		if (capture)
		{
			delete capture;
		}
	ï¿½ß°ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?
		ï¿½ï¿½ï¿½Ï¸ï¿½ Ä«ï¿½Þ¶ï¿½ ï¿½È²ï¿½ï¿½ï¿½ -> ï¿½È²ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½Ù½ï¿½ ï¿½ï¿½ï¿½ï¿½Ï·ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ MFCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½

3-1. stdafx.hï¿½ï¿½ #defineï¿½ï¿½ï¿?SAVE_PATHï¿½ï¿½ IMAGE_PATH ï¿½ß°ï¿½
	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½Ç»ï¿½ï¿½ C:\DogIdentificationdnï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½Ì¹ï¿½ï¿½ï¿½ï¿?C:\DogIdentification\Image ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï·ï¿½ï¿?
	IMAGE_PATHï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?"Image"ï¿½ï¿½ ï¿½ß´Âµï¿½ ï¿½×³ï¿½ " C:\DogIdentification\Image"ï¿½ï¿½ ï¿½Ù²Ü±ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?3-3ï¿½ï¿½ï¿?


3-2. DogIdentificationDlg.cppï¿½ï¿½ SAVE_PATHï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½Úµï¿½ ï¿½ï¿½ï¿?
	ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½Ï¸ï¿½, ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ ï¿½Ä¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿?

3-3. DogResister.cppï¿½ï¿½ Image ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?CImageï¿½Ì¿ï¿½ï¿½Ø¼ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½Úµï¿½ ï¿½ï¿½ï¿?4ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½)
	ï¿½Ì¶ï¿½ ï¿½ï¿½ï¿?ï¿½ï¿½ï¿?ï¿½Ò¶ï¿½  SAVE_PATHï¿½ï¿½ IMAGE_PATH ï¿½ï¿½ ï¿½ï¿½ï¿½Ä¼ï¿½ ï¿½ï¿½ï¿½ï¿½Ï´Âµï¿?ï¿½×³ï¿½ IMAGE_PATH ï¿½Ï³ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ ï¿½Ö°ï¿½ IMAGE_PATH ï¿½ï¿½Ã¼ï¿½ï¿½ ï¿½Ù²Ü±ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?
	ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ò¶ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ì¸ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ì¸ï¿½,
		ï¿½Ì¶ï¿½ ï¿½ï¿½ï¿?ï¿½Ì¸ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ì¹ï¿½ ï¿½Ö´Ù¸ï¿½ ï¿½Ì¸ï¿½_ï¿½ï¿½ï¿½ï¿½, ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		ï¿½ï¿½ï¿½Ú´ï¿½ ï¿½ï¿½ï¿½ï¿½Ì¸ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ñ¸ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½Ã¾î³².

4. CameraMgr.hï¿½ï¿½ CStatic* m_picDog ï¿½ß°ï¿½ï¿½Ï°ï¿½ DogResister.cpp ï¿½ï¿½ï¿½ï¿½ open cameraï¿½ï¿½Æ° ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ m_bmpBitmapï¿½Ì¶ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å´ (ï¿½Ì¹ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï±ï¿½ ï¿½ï¿½ï¿?
	DogRegister::OnBnClickedBtnOpencamera() ï¿½ï¿½ï¿½ï¿½ pCameraInst->save_img = &m_bmpBitmap; ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	CameraMgr::OnBnClickedOk() ï¿½ÎºÐ¿ï¿½ï¿½ï¿½ save_imgï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½

5. CameraMgr::OnBnClickedOk()ï¿½ï¿½ï¿½ï¿½ StretchDIBitsï¿½Ô¼ï¿½ ï¿½ï¿½ï¿?
	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Æ²ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ ï¿½Ç±æ·¡ ï¿½ï¿½ï¿½ï¿½ ï¿½Êºï¿½ ï¿½ï¿½ï¿½ï¿½ï¿? ï¿½ï¿½ ï¿½Úµå°¡ ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	StretchDIBits(cimage_mfc.GetDC(), 0, 0, rect.Width(), rect.Height(), 0, 0, imgWidth, imgHeight, mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY); ï¿½ï¿½ï¿½ï¿½
	imgWidth, imgHeight ï¿½ÎºÐ¸ï¿½ ï¿½Ù²ï¿½



=======

# [MFC]

## ë§ˆì¹œ ??
- UserPassword(?„ë£Œ)
	- ?€?¥ëœ ?¨ìŠ¤?Œë“œê°€ ?†ì„ ???¨ìŠ¤?Œë“œ ?±ë¡ ì°½ìœ¼ë¡??˜ì–´ê°€???¨ìŠ¤?Œë“œ ?±ë¡
- Login(?„ë£Œ) 
	- ?€?¥ëœ ?¨ìŠ¤?Œë“œê°€ ?ˆì„ ??ë¡œê·¸??ì°½ìœ¼ë¡??˜ì–´ê°€???¨ìŠ¤?Œë“œ ë¹„êµ??ë¡œê·¸??

- EditPassword(?„ë£Œ)
	- ?¨ìŠ¤?Œë“œ ì²´í¬
	- ?¨ìŠ¤?Œë“œ ë³€ê²??? ë³€ê²??¨ìŠ¤?Œë“œ ?€??

~~~
?¨ìŠ¤?Œë“œ ?Œì¼???€?¥ê²½ë¡œëŠ” my_define.h ?Œì¼??ëª…ì‹œ?˜ì–´ ?ˆë‹¤. 
~~~
- CamerMgr(?„ë£Œ)
	- ?¬ì§„ì°ê¸°

- DogRegister(ë¶€ë¶„ì™„ë£?
	- local?ì„œ ?´ë?ì§€ ë¶ˆëŸ¬?¤ê¸°
	- webcam?ì„œ ?¬ì§„ ì°ì–´ ?„ìš°ê¸?CameraMgr?ì„œ ì°ì? ?¬ì§„ ë°›ì•„???„ì?)
	- db??ê°•ì•„ì§€ ?•ë³´ ?±ë¡(?´ë?ì§€ ?œì™¸??ëª¨ë“  ?•ë³´)

- SearchDogInfo(ë¶€ë¶„ì™„ë£?
	- local?ì„œ ?´ë?ì§€ ë¶ˆëŸ¬?¤ê¸°
	- webcam?ì„œ ?¬ì§„ ì°ì–´ ?„ìš°ê¸?CameraMgr?ì„œ ì°ì? ?¬ì§„ ë°›ì•„???„ì?)
	- ê°•ì•„ì§€?•ë³´ Search( ?„ìž¬???´ë?ì§€ê²€?‰ì´ ?ˆë˜ë¯€ë¡?db???œì¼ ë§ˆì?ë§‰ì— ?±ë¡?˜ì–´?ˆëŠ” ê°•ì•„ì§€ ?•ë³´ ë¶ˆëŸ¬??
	- ê°•ì•„ì§€?•ë³´ ë³€ê²?& db???€??
	- ê°•ì•„ì§€?•ë³´ ?? œ

## ?´ì•¼????
- DogRegister
	- ?´ë?ì§€ ë¶„ì„ & ë¶„ì„?•ë³´ ë°›ì•„?¤ê¸°
	- ë¶„ì„???´ë?ì§€ê°€ ê¸°ì¡´??ì¡´ìž¬?˜ëŠ” ê°•ì•„ì§€ ?•ë³´?¸ì? Search
	- ?´ë?ì§€ ë¶„ì„?•ë³´ ?€??

- SearchDogInfo
	- ?´ë?ì§€ ê²€?‰í•´??ê°•ì•„ì§€ ?•ë³´ ë°›ì•„?¤ê¸°

- ì°½ë‹«ê¸?ë²„íŠ¼ ë¹„í™œ?±í™”

- memory leak ?´ê²°


# [RaceCar]




-2019.10.24 ¿ÀÈÄ±îÁöÀÇ ¾÷µ¥ÀÌÆ® »óÈ²

1. CateraMgr.cpp ÀÇ void CameraMgr::OnBnClickedOk()¿¡¼­ Àß¸øµÈ DC°¡ Release µÇ°íÀÖ±â¿¡ ¼öÁ¤ÇÔ.
	::ReleaseDC(m_camerapic->m_hWnd, dc);  ¿¡¼­ ::ReleaseDC(m_picDog->m_hWnd, dc); ·Î ¼öÁ¤

2. DbAccess¿¡ Image_path¿¡ °ü·ÃµÈ ºÎºÐµé Ãß°¡

3. SearchDogInfo¿¡ search/edit/delete ±â´É Ãß°¡
	
	search -> ¾ÆÁ÷µµ ÀÏ´ÜÀº Á¦ÀÏ ¸¶Áö¸·¿¡ ÀúÀåµÈ Á¤º¸¸¦ Ã£´Â´Ù.
		   -> ÀÌ¹ÌÁö ¾øÀ¸¸é ÀÌ¹ÌÁö°¡ ¾ø´Ù´Â ¾Ë¸²Ã¢ÀÌ ¶á´Ù.

	edit   -> ¼öÁ¤ÇÒ ¶§¿¡´Â ÀÏ´Ü ÀúÀåµÇ¾îÀÖ´ø »çÁø ÆÄÀÏÀ» Áö¿ì°í, ÇöÀç ÁöÁ¤µÈ ÀÌ¸§À¸·Î ´Ù½Ã ÀúÀåÇÑ´Ù. (¹æ½ÄÀº dogRegister¿Í µ¿ÀÏ)
		   -> DbAccess::DbSetting(m_dogName, m_dogAge, m_gender, m_dogSpecies, m_dogAddr, file_name); ¿¡¼­ ¸¶Áö¸·ÀÌ file_nameÀÎ ÀÌÀ¯´Â »õ·Î¿î °æ·Î ÀúÀåÇÏ·Á°í.
		   -> edit ÇÒ¶§ ÀÌ»óÇÑ °ªÀÌ µé¾î¿À´Â°É ¸·±â À§ÇØ DogRegister¿¡ ÀÖ´ø Checkarg()ÇÔ¼ö º¹ºÙÇÔ -> ±Ùµ¥ '°°Àº ¹®ÀÚ ³Ö´Â°ÍÀ» ¸·¾Æ¾ß ÇÑ´Ù,,,, db¿¡ ³ÖÀ» ¶§ ¿¡·¯³²...

	delete -> »çÁø ÆÄÀÏÀÌ ÀÖÀ¸¸é »çÁøÀ» Áö¿ì°í, db_table¾ÈÀÇ Á¤º¸¸¦ ´Ù Áö¿î´Ù.
		   -> Áö¿î ÈÄ¿¡´Â È­¸é ÃÊ±âÈ­ (Invalidate(true); »ç¿ë)



-2019.10.28 3:30  ±îÁöÀÇ ¾÷µ¥ÀÌÆ® »óÈ²

1. »ç¿ëÀÚ ºñ¹Ð¹øÈ£ Cµå¶óÀÌºê¿¡ Æú´õ »ý¼ºÈÄ ÆÄÀÏ »ý¼ºÇØ¼­ ¸¸µé¾î Áú ¼ö ÀÖ°Ô º¯°æ
2. ·Î±×ÀÎÃ¢¿¡¼­ ·Î±×ÀÎÃ¢ x ´­·¯¼­ ²ô¸é ·Î±×ÀÎ ÀÌÈÄÃ¢À¸·Î ³Ñ¾î°¡±æ·¡ xÇ¥ ´©¸£¸é MFC ÇÁ·Î±×·¥ÀÌ Á¾·áµÇ°Ô ¼öÁ¤


