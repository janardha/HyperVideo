// HyperVideoAuthorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HyperVideoAuthor.h"
#include "HyperVideoAuthorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	EnableActiveAccessibility();

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CHyperVideoAuthorDlg dialog




CHyperVideoAuthorDlg::CHyperVideoAuthorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHyperVideoAuthorDlg::IDD, pParent)
	, actionlistID(0)
	, videoNum(0)
	, primarySliderPosition(0)
	, secondaryVideoPositon(0)
	, totalLinks(0)
	, linkNum(0)
	, listNum(0)
	, bMouseSelectionEnabled (false)
	, bRubberBanding(false)
	, isBkgTransparent (false)
	, bInPaint(false)
	, bSetSliderMax(true)
	, keyIndex(0)
	, rectPerFrame(0)
	, bPrimaryVideoExists(false)
	, bSecondaryVideoExists(false)
	
	, m_linkName(_T(""))
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	
	//SetWindowSubclass(GetDlgItem(IDC_PRIMARY)->GetSafeHwnd(),&CHyperVideoAuthorDlg::PictureProcedure,0,0);
//	OrgProcedure = (WNDPROC)SetWindowLong(GetDlgItem(IDC_PRIMARY)->GetSafeHwnd(),GWL_WNDPROC, (LONG)PictureProcedure);
}

void CHyperVideoAuthorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_PRIMARYSLIDER, primarySliderPosition);
	DDV_MinMaxInt(pDX, primarySliderPosition, 1, 720);
	DDX_Slider(pDX, IDC_SECONDARYSLIDER, secondaryVideoPositon);
	DDV_MinMaxInt(pDX, secondaryVideoPositon, 1, 720);
	DDX_Control(pDX, IDC_ACTIONLIST, m_ActionCombo);
	DDX_Control(pDX, IDC_VIDEOSELECT, m_VideoList);


	DDX_Control(pDX, IDC_PRIMARYSLIDER, m_PrimarySlider);
	DDX_Control(pDX, IDC_SECONDARYSLIDER, m_SecondarySlider);
	DDX_Control(pDX, IDC_CONNECTVIDEO, m_Connect);
	DDX_Control(pDX, IDC_SAVE, m_Save);
	DDX_Control(pDX, IDC_HYPERLINKLIST, m_HyperListCtrl);
	DDX_CBString(pDX, IDC_HYPERLINKLIST, m_linkName);
}

BEGIN_MESSAGE_MAP(CHyperVideoAuthorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_CBN_SELCHANGE(IDC_VIDEOSELECT, &CHyperVideoAuthorDlg::OnCbnSelchangeLinkCombo)
	ON_CBN_SELCHANGE(IDC_ACTIONLIST, &CHyperVideoAuthorDlg::OnCbnSelchangeActionlist)
	
//	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_PRIMARYSLIDER, &CHyperVideoAuthorDlg::OnNMReleasedcapturePrimaryslider)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_PRIMARYSLIDER, &CHyperVideoAuthorDlg::OnNMReleasedcapturePrimaryslider)
ON_BN_CLICKED(IDC_CONNECTVIDEO, &CHyperVideoAuthorDlg::OnBnClickedConnectvideo)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SECONDARYSLIDER, &CHyperVideoAuthorDlg::OnNMReleasedcaptureSecondaryslider)
ON_BN_CLICKED(IDC_SAVE, &CHyperVideoAuthorDlg::OnBnClickedSave)

ON_CBN_SELCHANGE(IDC_HYPERLINKLIST, &CHyperVideoAuthorDlg::OnCbnSelchangeHyperlinklist)
END_MESSAGE_MAP()


// CHyperVideoAuthorDlg message handlers

BOOL CHyperVideoAuthorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	// TODO: Add extra initialization here
//	OrgProcedure = (WNDPROC)SetWindowLongPtr(GetDlgItem(IDC_PRIMARY)->GetSafeHwnd(),GWL_WNDPROC, (LONG_PTR)PictureProcedure);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHyperVideoAuthorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CHyperVideoAuthorDlg::OnRefresh()
{
	if(bPrimaryVideoExists)
	{
		paintFrame();
	}
	if(bSecondaryVideoExists)
	{
		paintSecondaryFrame();
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHyperVideoAuthorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

		/*if(bPrimaryVideoExists)
		{
			paintFrame();
		}
		if(bSecondaryVideoExists)
		{
			paintSecondaryFrame();
		}*/
	}
	else
	{
		if(bInPaint)
		{
			CRect tmpRect;
			CRect currect;
			
			CPen pen(PS_SOLID, 2, RGB(255,0,0));
			CPen *oldpen;

		    PAINTSTRUCT ps;
			CDC *dc;
			CBrush *pBrush, *pOldBrush;

			BeginPaint(&ps);

			dc =  GetDlgItem(IDC_PRIMARY)->GetDC();
			///////////paint frame trial///////////////////
			if(bRepaintFrame)
			{
				int idx = 0;
				bRepaintFrame = false;
				HDC hdc;
			
				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = displayFrame.getWidth();
				bmi.bmiHeader.biHeight = -displayFrame.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = displayFrame.getWidth()*displayFrame.getHeight();

				hdc = *dc;

				SetDIBitsToDevice(hdc,
								  0,0,displayFrame.getWidth(),displayFrame.getHeight(),
								  0,0,0,displayFrame.getHeight(),
								  displayFrame.getPrimaryFrameData(m_PrimarySlider.GetPos()),&bmi,DIB_RGB_COLORS);
				
				dc->SelectStockObject(NULL_BRUSH);
				while(idx < rectPerFrame)
				{
		
					dc->Rectangle(frameRectangles[2*idx].xpos,frameRectangles[2*idx].ypos,
						frameRectangles[2*idx+1].xpos,frameRectangles[2*idx+1].ypos);
			
					idx++;
				}
			}
			////////////////////////////////////////////////////////////////
			
			GetClientRect(tmpRect);

			oldpen = dc->SelectObject(&pen);

			dc->SelectStockObject(NULL_BRUSH);
			dc->SetDCBrushColor(RGB(255,255,255));
			
			currect.left = rectPts[0].xpos;
			currect.right = rectPts[1].xpos;
			currect.top = rectPts[0].ypos;
			currect.bottom = rectPts[1].ypos;
			
		//	currect.NormalizeRect();

			
			dc->Rectangle(currect);
			
			//dc->SelectObject(oldpen);
			////dc->SetROP2(R2_WHITE);
			EndPaint(&ps);
			dc->Detach();
		}
		else
			CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHyperVideoAuthorDlg::OnQueryDragIcon()
{
	paintFrame();
	return static_cast<HCURSOR>(m_hIcon);
}

void CHyperVideoAuthorDlg::OnCbnSelchangeLinkCombo()

{
	//GetDlgItem(IDC_VIDEOSELECT)->UpdateData(false);
	CString m_str;
	char subStr[10];
	int idx = 0;
	idx = m_VideoList.GetCurSel();

	videoNum = idx;
	// TODO: Add your control notification handler code here
	//Z:\rajaniaithal\USC_Work_Load\csci576\project
	switch(idx)
	{
	case 0:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo1.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 1;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo1.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
		//	MessageBox(m_str);
			break;
		}	
	case 1:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo2.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 2;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo2.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
		//	MessageBox(m_str);
			break;
		}
	case 2:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo3.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 3;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo3.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
		//	MessageBox(m_str);
			break;
		}
	case 3:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo4.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 4;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo4.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
	//		MessageBox(m_str);
			break;
		}
	case 4:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo5.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 5;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo5.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
	//		MessageBox(m_str);
			break;
		}
	case 5:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo6.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 6;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo6.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
		//	MessageBox(m_str);
			break;
		}
	case 6:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo7.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 7;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo7.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
		//	MessageBox(m_str);
			break;
		}
	case 7:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo8.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 8;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo8.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
	//		MessageBox(m_str);
			break;
		}
	case 8:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo9.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 9;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo9.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
	//		MessageBox(m_str);
			break;
		}
	case 9:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo10.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 10;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo10.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
	//		MessageBox(m_str);
			break;
		}
	case 10:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo11.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 11;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo11.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
	//		MessageBox(m_str);
			break;
		}
	case 11:
		{
			if(actionlistID == 0)
			{
				displayFrame.setPrimaryImagePath("vdo12.rgb");
				m_str =  displayFrame.getPrimaryImagePath();
				videoNum = 12;
			}
			else
			{
				displayFrame.setSecondaryImagePath("vdo12.rgb");
				m_str =  displayFrame.getSecondaryImagePath();
			}
	//		MessageBox(m_str);
			break;
		}
	}
	//displayFrame.setFrameSize();
	if(actionlistID == 0)
	{
		clearData();
		displayFrame.ReadPrimaryImage();
		readMetaDataFile();
		m_PrimarySlider.SetPos(0);
		paintFrame();
		bPrimaryVideoExists = true;
	}
	else
	{
		displayFrame.ReadSecondaryImage();
		m_Connect.EnableWindow(true);
		m_SecondarySlider.SetPos(0);
		paintSecondaryFrame();
		bSecondaryVideoExists = true;
	}
}

void CHyperVideoAuthorDlg::OnCbnSelchangeActionlist()
{
	// TODO: Add your control notification handler code here
	CString m_str;
	int idx = 0;
	char *ptr;
	char linkNameptr[20];
	int cleardata;
	idx = m_ActionCombo.GetCurSel();
	int val =0;
	actionlistID = idx;
	

	switch(idx)
	{
	case 0:
		{
			m_str = "Load Primary Video selected";
			//MessageBox(m_str);
			for(cleardata = 0; cleardata < NUMOFFRAMES; cleardata++)
				rectExist[cleardata] = false;
			break;
			displayFrame.setPrimarySelect(0);
		}	
	case 1:
		{
			m_str = "Load Secondary Video selected";
			//MessageBox(m_str);
			break;
			displayFrame.setPrimarySelect(1);
		}
	case 2:
		{
			if(m_popUpDlg.DoModal() == IDOK)
			{
				m_str = m_popUpDlg.m_nameStr;
				strcpy(linkNameptr, CStringA(m_str).GetString());
				ptr = strstr(linkNameptr, " ");
				if(ptr == NULL)
				{
					m_HyperListCtrl.AddString(m_str);

					m_ActionCombo.SetCurSel(0);
					actionlistID = 0;
					displayFrame.setPrimarySelect(0);
					m_HyperListCtrl.SetCurSel(totalLinks);
					createLinkData(m_str);
					
					
					bMouseSelectionEnabled = true;
				}
				else
				{
					m_str = "Cannot have spaces in link name";
					MessageBox(m_str);
				}
			}
			break;
		}
	}
	if(bSetSliderMax)
	{
		m_PrimarySlider.SetRangeMax(720,false);
		m_SecondarySlider.SetRangeMax(720,false);
		bSetSliderMax = false;
	}
}


void CHyperVideoAuthorDlg::OnNMReleasedcapturePrimaryslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	Interpolate(m_PrimarySlider.GetPos());
	paintFrame();
}
void CHyperVideoAuthorDlg::Interpolate(int frameNum)
{
	int a = 0;
	int b = 0;
	int totalPframes = 0;

	int index = 0;
	int keyIdx = 1;
	
	frameSet = false;
	rectPerFrame = 0;
	if(rectExist[frameNum])
		while(index < totalLinks+1)
		{
			do
			{
				if(videoLink[index].numOfKeyFrames > 1)
					if(videoLink[index].primaryKeyFrame[keyIdx-1] <= frameNum && videoLink[index].primaryKeyFrame[keyIdx] >= frameNum)
					{
						a = frameNum - videoLink[index].primaryKeyFrame[keyIdx-1];
						b = videoLink[index].primaryKeyFrame[keyIdx] - frameNum;
						totalPframes = videoLink[index].primaryKeyFrame[keyIdx] - videoLink[index].primaryKeyFrame[keyIdx-1];
					
					
						///minimum x and y position of the 
						frameRectangles[2*rectPerFrame].xpos = videoLink[index].primaryEnds[keyIdx-1].xposmin * b /totalPframes + 
																	videoLink[index].primaryEnds[keyIdx].xposmin * a /totalPframes;
						frameRectangles[2*rectPerFrame].ypos = videoLink[index].primaryEnds[keyIdx-1].yposmin * b /totalPframes + 
																	videoLink[index].primaryEnds[keyIdx].yposmin * a /totalPframes;

						frameRectangles[2*rectPerFrame+1].xpos = videoLink[index].primaryEnds[keyIdx-1].xposmax * b /totalPframes + 
																	videoLink[index].primaryEnds[keyIdx].xposmax * a /totalPframes;
						frameRectangles[2*rectPerFrame+1].ypos = videoLink[index].primaryEnds[keyIdx-1].yposmax * b /totalPframes + 
																	videoLink[index].primaryEnds[keyIdx].yposmax * a /totalPframes;

						if(index == linkNum)
						{
							linkRect[0].xpos = frameRectangles[2*rectPerFrame].xpos;
							linkRect[0].ypos = frameRectangles[2*rectPerFrame].ypos;
							linkRect[1].xpos = frameRectangles[2*rectPerFrame+1].xpos;
							linkRect[1].ypos = frameRectangles[2*rectPerFrame+1].ypos;
							frameSet = true;
						}
						rectPerFrame++;
					}
				keyIdx++;
			}while(keyIdx < videoLink[index].numOfKeyFrames);
			index++;
			keyIdx = 1;
		} 
		if(!frameSet)
		{
			linkRect[0].xpos = 0;
			linkRect[0].ypos = 0;
			linkRect[1].xpos = 0;
			linkRect[1].ypos = 0;
		}
}

void CHyperVideoAuthorDlg::OnBnClickedConnectvideo()
{
	// TODO: Add your control notification handler code here
	
	videoLink[linkNum].secondaryLinkFrame = m_SecondarySlider.GetPos();
	strcpy(videoLink[linkNum].secondaryPath,displayFrame.getSecondaryImagePath());
	
	m_Save.EnableWindow(true);
	
	keyIndex = 0;
	
}
void CHyperVideoAuthorDlg::paintFrame()
{
	int idx = 0;
	
	bInPaint = false;

	HWND hWnd = GetDlgItem(IDC_PRIMARY)->GetSafeHwnd();
	int wmId, wmEvent;
	LPPAINTSTRUCT ps;
	HDC hdc;
	//LPRECT rt;
	CDC *dc;
	//m_PrimaryFrame.GetClientRect(rt);
//	BeginPaint(ps);

	BITMAPINFO bmi;
	CBitmap bitmap;
	memset(&bmi,0,sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = displayFrame.getWidth();
	bmi.bmiHeader.biHeight = -displayFrame.getHeight();  // Use negative height.  DIB is top-down.
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = displayFrame.getWidth()*displayFrame.getHeight();

	dc =  GetDlgItem(IDC_PRIMARY)->GetDC();

	hdc = *dc;
	//m_PrimaryFrame.SetBitmap()
	//
	SetDIBitsToDevice(hdc,
					  0,0,displayFrame.getWidth(),displayFrame.getHeight(),
					  0,0,0,displayFrame.getHeight(),
					  displayFrame.getPrimaryFrameData(m_PrimarySlider.GetPos()),&bmi,DIB_RGB_COLORS);
	
	

	if(rectExist[m_PrimarySlider.GetPos()])
	{
		/*CString m_str;
		m_str = "Insert hyperlink selected";
		MessageBox(m_str);*/
		CPen pen(PS_SOLID, 2, RGB(255,255,255));
		CPen *oldpen;
		
		RECT tmpRect;
		PAINTSTRUCT ps;
		CDC *dc;

		int idx = 0;
		BeginPaint(&ps);

		dc = GetDlgItem(IDC_PRIMARY)->GetDC();
		
		oldpen = dc->SelectObject(&pen);
		dc->SelectStockObject(NULL_BRUSH);
		dc->SetDCBrushColor(RGB(255,255,255));
		
		
		while(idx < rectPerFrame)
		{
		
			dc->Rectangle(frameRectangles[2*idx].xpos,frameRectangles[2*idx].ypos,
				frameRectangles[2*idx+1].xpos,frameRectangles[2*idx+1].ypos);
			
			idx++;
		}

		CPen pennew(PS_SOLID, 2, RGB(255,0,0));
		oldpen = dc->SelectObject(&pennew);

		dc->Rectangle(linkRect[0].xpos,linkRect[0].ypos,
				linkRect[1].xpos,linkRect[1].ypos);

		dc->SelectObject(oldpen);

		EndPaint(&ps);
	}

//	EndPaint(ps);
}

void CHyperVideoAuthorDlg::paintSecondaryFrame()
{
	int size = 0;
	
	HWND hWnd = GetDlgItem(IDC_SECONDARY)->GetSafeHwnd();
	int wmId, wmEvent;
	LPPAINTSTRUCT ps;
	HDC hdc;
	CDC *dc;
	
	BITMAPINFO bmi;
	CBitmap bitmap;
	memset(&bmi,0,sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = displayFrame.getWidth();
	bmi.bmiHeader.biHeight = -displayFrame.getHeight();  // Use negative height.  DIB is top-down.
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = displayFrame.getWidth()*displayFrame.getHeight();


	dc =  GetDlgItem(IDC_SECONDARY)->GetDC();
	hdc = *dc;
	SetDIBitsToDevice(hdc,
					  0,0,displayFrame.getWidth(),displayFrame.getHeight(),
					  0,0,0,displayFrame.getHeight(),
					  displayFrame.getSecondaryFrameData(m_SecondarySlider.GetPos()),&bmi,DIB_RGB_COLORS);
	
}

void CHyperVideoAuthorDlg::OnNMReleasedcaptureSecondaryslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	paintSecondaryFrame();
}

BEGIN_EVENTSINK_MAP(CHyperVideoAuthorDlg, CDialog)
	ON_EVENT(CHyperVideoAuthorDlg, IDC_PRIMARYACTIVEX, DISPID_MOUSEMOVE, CHyperVideoAuthorDlg::MouseMovePrimaryactivex, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	ON_EVENT(CHyperVideoAuthorDlg, IDC_PRIMARYACTIVEX, DISPID_MOUSEDOWN, CHyperVideoAuthorDlg::MouseDownPrimaryactivex, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	ON_EVENT(CHyperVideoAuthorDlg, IDC_PRIMARYACTIVEX, DISPID_MOUSEUP, CHyperVideoAuthorDlg::MouseUpPrimaryactivex, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
END_EVENTSINK_MAP()

void CHyperVideoAuthorDlg::MouseMovePrimaryactivex(short Button, short Shift, float X, float Y)
{
	CPoint point;	
	int priFrameNum = m_PrimarySlider.GetPos();
	// TODO: Add your message handler code here
	 if (bMouseSelectionEnabled)
        if (bRubberBanding)
        {
			GetCursorPos(&point);
            ScreenToClient(&point);
			
			curMousePos.xpos = point.x-17;
			curMousePos.ypos = point.y-88;

			if(videoLink[linkNum].rectangleExists[priFrameNum])
			{
				if(abs(clickDownPos.xpos - resize[1].xpos) < TOLERANCE)
				{
					if(abs(curMousePos.ypos - clickDownPos.ypos) > TOLERANCE)
					{
						rectPts[1].xpos = curMousePos.xpos;
						rectPts[1].ypos = curMousePos.ypos;
					}
					else
					{
						rectPts[1].xpos = curMousePos.xpos;
						rectPts[1].ypos = resize[1].ypos;
					}
					rectPts[0].xpos = resize[0].xpos;
					rectPts[0].ypos = resize[0].ypos;
				}
				else if(abs(clickDownPos.xpos - resize[0].xpos) < TOLERANCE)
				{
					if(abs(curMousePos.ypos - clickDownPos.ypos) > TOLERANCE)
					{
						rectPts[0].xpos = curMousePos.xpos;
						rectPts[0].ypos = curMousePos.ypos;
					}
					else
					{
						rectPts[0].xpos = curMousePos.xpos;
						rectPts[0].ypos = resize[0].ypos;
					}
					rectPts[1].xpos = resize[1].xpos;
					rectPts[1].ypos = resize[1].ypos;
				}
				else if(abs(clickDownPos.ypos - resize[1].ypos)< TOLERANCE)
				{
					if(abs(curMousePos.xpos - clickDownPos.xpos) > TOLERANCE)
					{
						rectPts[1].xpos = curMousePos.xpos;
						rectPts[1].ypos = curMousePos.ypos;
					}
					else
					{
						rectPts[1].xpos = resize[1].xpos;
						rectPts[1].ypos = curMousePos.ypos;
					}
					rectPts[0].xpos = resize[0].xpos;
					rectPts[0].ypos = resize[0].ypos;
				}
				else if(abs(clickDownPos.ypos - resize[0].ypos) < TOLERANCE)
				{
					if(abs(curMousePos.xpos - clickDownPos.xpos) > TOLERANCE)
					{
						rectPts[0].xpos = curMousePos.xpos;
						rectPts[0].ypos = curMousePos.ypos;
					}
					else
					{
						rectPts[0].xpos = resize[0].xpos;
						rectPts[0].ypos = curMousePos.ypos;
					}
					rectPts[1].xpos = resize[1].xpos;
					rectPts[1].ypos = resize[1].ypos;
				}
				else
				{
					rectPts[0].xpos = min(curMousePos.xpos, clickDownPos.xpos);
					rectPts[1].xpos = max(curMousePos.xpos, clickDownPos.xpos);
					rectPts[0].ypos = min(curMousePos.ypos, clickDownPos.ypos);
					rectPts[1].ypos = max(curMousePos.ypos, clickDownPos.ypos);
				}
			}
			else
			{
				rectPts[0].xpos = min(curMousePos.xpos, clickDownPos.xpos);
				rectPts[1].xpos = max(curMousePos.xpos, clickDownPos.xpos);
				rectPts[0].ypos = min(curMousePos.ypos, clickDownPos.ypos);
				rectPts[1].ypos = max(curMousePos.ypos, clickDownPos.ypos);
			}
       
            InvalidateRect(NULL, false);
        }
}

void CHyperVideoAuthorDlg::MouseDownPrimaryactivex(short Button, short Shift, float X, float Y)
{
	 CPoint point;
	 int priFrameNum = m_PrimarySlider.GetPos();
	 int maxItr = 0;

	 keyIndexSet = false;
	// TODO: Add your message handler code here
	 if(!bMouseSelectionEnabled)
	 {
		 CString str;
		 str = "Create Hyperlink";
		 MessageBox(str);
	 }
	if (bMouseSelectionEnabled)
    {
		GetCursorPos(&point);
		ScreenToClient(&point);
        bRubberBanding = true;
		bInPaint = true;
		clickDownPos.xpos = point.x-17;
		clickDownPos.ypos = point.y-88;

		if(videoLink[linkNum].rectangleExists[priFrameNum])
		{
			maxItr = videoLink[linkNum].numOfKeyFrames;
			for(int idx = 0; idx < maxItr; idx++)
			{
				if(priFrameNum == videoLink[linkNum].primaryKeyFrame[idx])
				{
					keyIndex = idx;
					keyIndexSet = true;
					break;
				}
				
			}

			if(!keyIndexSet)
			{
				keyIndex = videoLink[linkNum].numOfKeyFrames;
				videoLink[linkNum].numOfKeyFrames++;
			}
			resize[0].xpos = frameRectangles[2*linkNum].xpos;
			resize[0].ypos = frameRectangles[2*linkNum].ypos;
			resize[1].xpos = frameRectangles[2*linkNum+1].xpos;
			resize[1].ypos = frameRectangles[2*linkNum+1].ypos;
			
		}
		else
		{
			if(keyIndex == 0)
			{
				videoLink[linkNum].numOfKeyFrames = 1; //first keyframe
				if(videoLink[linkNum].primaryKeyFrame[keyIndex] < 0)
					videoLink[linkNum].primaryKeyFrame[keyIndex] = m_PrimarySlider.GetPos();
			}
			if(videoLink[linkNum].primaryKeyFrame[keyIndex] != m_PrimarySlider.GetPos())
			{
				keyIndex++;
				videoLink[linkNum].numOfKeyFrames++; // subsequent keyframes
			}

			curMousePos = clickDownPos;
			rectPts[0].xpos = min(curMousePos.xpos, clickDownPos.xpos);
			rectPts[1].xpos = max(curMousePos.xpos, clickDownPos.xpos);
			rectPts[0].ypos = min(curMousePos.ypos, clickDownPos.ypos);
			rectPts[1].ypos = max(curMousePos.ypos, clickDownPos.ypos);
		}
        
       
        InvalidateRect(NULL, isBkgTransparent);
	}
}

void CHyperVideoAuthorDlg::MouseUpPrimaryactivex(short Button, short Shift, float X, float Y)
{
	int position;
	CPoint point;
	CRect rect;
	int xdifference =0, ydifference =0;
	int priFrameNum = m_PrimarySlider.GetPos();
	// TODO: Add your message handler code here
	if (bMouseSelectionEnabled)
		if (bRubberBanding)
		{
			bRubberBanding = false;
			//bInPaint = true;
			
			GetCursorPos(&point);
			ScreenToClient(&point);
			clickUpPos.xpos = point.x-17;
			clickUpPos.ypos = point.y-88;

			if(videoLink[linkNum].rectangleExists[priFrameNum])
			{
				if(abs(clickDownPos.xpos - resize[1].xpos) < TOLERANCE)
				{
					if(abs(clickUpPos.ypos - clickDownPos.ypos) > TOLERANCE)
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmax = clickUpPos.xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmax = clickUpPos.ypos;
					}
					else
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmax = clickUpPos.xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmax = resize[1].ypos;
					}
					videoLink[linkNum].primaryEnds[keyIndex].xposmin = resize[0].xpos;
					videoLink[linkNum].primaryEnds[keyIndex].yposmin = resize[0].ypos;
				}
				else if(abs(clickDownPos.xpos - resize[0].xpos) < TOLERANCE)
				{
					if(abs(clickUpPos.ypos - clickDownPos.ypos) > TOLERANCE)
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmin = clickUpPos.xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmin = clickUpPos.ypos;
					}
					else
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmin = clickUpPos.xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmin = resize[0].ypos;
					}
					videoLink[linkNum].primaryEnds[keyIndex].xposmax = resize[1].xpos;
					videoLink[linkNum].primaryEnds[keyIndex].yposmax = resize[1].ypos;
				}
				else if(abs(clickDownPos.ypos - resize[1].ypos)< TOLERANCE)
				{
					if(abs(clickUpPos.xpos - clickDownPos.xpos) > TOLERANCE)
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmax = clickUpPos.xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmax = clickUpPos.ypos;
					}
					else
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmax = resize[1].xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmax = clickUpPos.ypos;
					}
					videoLink[linkNum].primaryEnds[keyIndex].xposmin = resize[0].xpos;
					videoLink[linkNum].primaryEnds[keyIndex].yposmin = resize[0].ypos;
				}
				else if(abs(clickDownPos.ypos - resize[0].ypos) < TOLERANCE)
				{
					if(abs(clickUpPos.xpos - clickDownPos.xpos) > TOLERANCE)
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmin = clickUpPos.xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmin = clickUpPos.ypos;
					}
					else
					{
						videoLink[linkNum].primaryEnds[keyIndex].xposmin = resize[0].xpos;
						videoLink[linkNum].primaryEnds[keyIndex].yposmin = clickUpPos.ypos;
					}
					videoLink[linkNum].primaryEnds[keyIndex].xposmax = resize[1].xpos;
					videoLink[linkNum].primaryEnds[keyIndex].yposmax = resize[1].ypos;
				}
				else
				{
					videoLink[linkNum].primaryEnds[keyIndex].xposmin = min(clickUpPos.xpos, clickDownPos.xpos);
					videoLink[linkNum].primaryEnds[keyIndex].xposmax = max(clickUpPos.xpos, clickDownPos.xpos);
					videoLink[linkNum].primaryEnds[keyIndex].yposmin = min(clickUpPos.ypos, clickDownPos.ypos);
					videoLink[linkNum].primaryEnds[keyIndex].yposmax = max(clickUpPos.ypos, clickDownPos.ypos);
				}

			}
			else
			{
				videoLink[linkNum].primaryEnds[keyIndex].xposmin = min(clickUpPos.xpos, clickDownPos.xpos);
				videoLink[linkNum].primaryEnds[keyIndex].xposmax = max(clickUpPos.xpos, clickDownPos.xpos);
				videoLink[linkNum].primaryEnds[keyIndex].yposmin = min(clickUpPos.ypos, clickDownPos.ypos);
				videoLink[linkNum].primaryEnds[keyIndex].yposmax = max(clickUpPos.ypos, clickDownPos.ypos);
				/*
				position = m_PrimarySlider.GetPos();
				videoLink[linkNum].primaryKeyFrame[keyIndex] = position;
				rectExist[position] = true;*/
			}

			linkRect[0].xpos = videoLink[linkNum].primaryEnds[keyIndex].xposmin;
			linkRect[0].ypos = videoLink[linkNum].primaryEnds[keyIndex].yposmin;
			linkRect[1].xpos = videoLink[linkNum].primaryEnds[keyIndex].xposmax;
			linkRect[1].ypos = videoLink[linkNum].primaryEnds[keyIndex].yposmax;

			
			videoLink[linkNum].primaryKeyFrame[keyIndex] = priFrameNum;
			rectExist[priFrameNum] = true;
			
			
			if(keyIndex > 0)
			{
				for(int idx = videoLink[linkNum].primaryKeyFrame[keyIndex-1]; 
					idx < videoLink[linkNum].primaryKeyFrame[keyIndex]; idx++)
				{
					rectExist[idx] = true;
					videoLink[linkNum].rectangleExists[idx] = true;
				}
			}
			
			bRepaintFrame = true;
			if(keyIndex > 1)
			{
				sortFrameNum();
			}
			Interpolate(priFrameNum);
			//paintFrame();
			InvalidateRect( NULL, isBkgTransparent);
		}
}

void CHyperVideoAuthorDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	FILE *metaFile;
	
	int idx = 0;
	char subStr[10] = ".rgb";
	char destStr[10] = ".meta";
	char fileName[_MAX_PATH];

	char *ptr;
	CString str;
	strcpy(fileName,displayFrame.getPrimaryImagePath());
	
	ptr = strstr(fileName, subStr);
	strncpy(ptr, destStr,6);

	metaFile = fopen(fileName,"wb");

	while(idx < totalLinks)
	{
		fprintf(metaFile,"%d\t", videoLink[idx].linkID);
		fprintf(metaFile,"%s\t", videoLink[idx].LinkName);
		fprintf(metaFile,"%d\n", videoLink[idx].numOfKeyFrames);
		
		fprintf(metaFile,"%s\t", videoLink[idx].secondaryPath);
		fprintf(metaFile,"%d\n", videoLink[idx].secondaryLinkFrame);

		for(int index = 0; index< videoLink[idx].numOfKeyFrames; index++)
		{
			fprintf(metaFile,"%d\t", videoLink[idx].primaryKeyFrame[index]);
			fprintf(metaFile,"%d\t", videoLink[idx].primaryEnds[index].xposmin);
			fprintf(metaFile,"%d\t", videoLink[idx].primaryEnds[index].yposmin);
			fprintf(metaFile,"%d\t", videoLink[idx].primaryEnds[index].xposmax);
			fprintf(metaFile,"%d\n", videoLink[idx].primaryEnds[index].yposmax);

		}
		idx++;
	}

	fclose(metaFile);
}

void CHyperVideoAuthorDlg::OnCbnSelchangeHyperlinklist()
{
	// TODO: Add your control notification handler code here
	linkNum = m_HyperListCtrl.GetCurSel();
	keyIndex = videoLink[linkNum].numOfKeyFrames;
	//if(videoLink[linkNum].rectangleExists[m_PrimarySlider.GetPos()])
	//{
	//	//display rectangle of that diffrent color
	//}
}

void CHyperVideoAuthorDlg::createLinkData(CString str)
{
	int index;
	keyIndex = 0;
	totalLinks++;

	linkNum = m_HyperListCtrl.GetCurSel();
	
	videoLink[linkNum].linkID = linkNum;
	strcpy(videoLink[linkNum].LinkName, CStringA(str).GetString());
	videoLink[linkNum].numOfKeyFrames = 0;

	for(index = 0; index < NUMOFFRAMES; index++)
		videoLink[linkNum].rectangleExists[index] = false;
	
}

void CHyperVideoAuthorDlg::readMetaDataFile()
{
	FILE* fptr;

	char subStr[10] = ".rgb";
	char destStr[10] = ".meta";
	char fileName[_MAX_PATH];

	char *ptr;

	CString str;
	strcpy(fileName,displayFrame.getPrimaryImagePath());
	
	ptr = strstr(fileName, subStr);
	strncpy(ptr, destStr,6);

	fptr = fopen(fileName,"rb");

	if(fptr!= NULL)
	{
		int idx = 0;

		while(!feof(fptr))
		{
			fscanf(fptr,"%d\t", &videoLink[idx].linkID);
			
			fscanf(fptr,"%s\t", &videoLink[idx].LinkName);
			fscanf(fptr,"%d\n", &videoLink[idx].numOfKeyFrames);
		
			fscanf(fptr,"%s\t", &videoLink[idx].secondaryPath);
			fscanf(fptr,"%d\n", &videoLink[idx].secondaryLinkFrame);

			for(int index = 0; index< videoLink[idx].numOfKeyFrames; index++)
			{
				fscanf(fptr,"%d\t", &videoLink[idx].primaryKeyFrame[index]);
				fscanf(fptr,"%d\t", &videoLink[idx].primaryEnds[index].xposmin);
				fscanf(fptr,"%d\t", &videoLink[idx].primaryEnds[index].yposmin);
				fscanf(fptr,"%d\t", &videoLink[idx].primaryEnds[index].xposmax);
				fscanf(fptr,"%d\n", &videoLink[idx].primaryEnds[index].yposmax);
			}
			for(int i = videoLink[linkNum].primaryKeyFrame[0];
				i < videoLink[linkNum].primaryKeyFrame[videoLink[idx].numOfKeyFrames-1]; i++)
			{
				rectExist[i] = true;
				videoLink[linkNum].rectangleExists[i] = true;
			}
			m_HyperListCtrl.AddString(CString(videoLink[idx].LinkName));
			idx++;
		}
		m_HyperListCtrl.SetCurSel(0);
		totalLinks = idx;
		bMouseSelectionEnabled = true;
		fclose(fptr);
	}
}

void CHyperVideoAuthorDlg::sortFrameNum()
{
	int ii=0, jj=0;
	int frameidx1 = 0;
	int pos=0;
	for(ii = 1; ii < videoLink[linkNum].numOfKeyFrames; ii++)
	{
		for(jj = 0; jj< videoLink[linkNum].numOfKeyFrames-ii; jj++)
		{
			if(videoLink[linkNum].primaryKeyFrame[jj] > videoLink[linkNum].primaryKeyFrame[jj+1])
			{
				//swap values
				frameidx1 = videoLink[linkNum].primaryKeyFrame[jj];
				videoLink[linkNum].primaryKeyFrame[jj] = videoLink[linkNum].primaryKeyFrame[jj+1];
				videoLink[linkNum].primaryKeyFrame[jj+1] = frameidx1;

				pos = videoLink[linkNum].primaryEnds[jj].xposmax;
				videoLink[linkNum].primaryEnds[jj].xposmax = videoLink[linkNum].primaryEnds[jj+1].xposmax;
				videoLink[linkNum].primaryEnds[jj+1].xposmax = pos;

				pos = videoLink[linkNum].primaryEnds[jj].yposmax;
				videoLink[linkNum].primaryEnds[jj].yposmax = videoLink[linkNum].primaryEnds[jj+1].yposmax;
				videoLink[linkNum].primaryEnds[jj+1].yposmax = pos;

				pos = videoLink[linkNum].primaryEnds[jj].xposmin;
				videoLink[linkNum].primaryEnds[jj].xposmin = videoLink[linkNum].primaryEnds[jj+1].xposmin;
				videoLink[linkNum].primaryEnds[jj+1].xposmin = pos;

				pos = videoLink[linkNum].primaryEnds[jj].yposmin;
				videoLink[linkNum].primaryEnds[jj].yposmin = videoLink[linkNum].primaryEnds[jj+1].yposmin;
				videoLink[linkNum].primaryEnds[jj+1].yposmin = pos;
			}
		}
	}
}

void CHyperVideoAuthorDlg::clearData()
{
	int idx = 0;

	while(idx < totalLinks)
	{
		videoLink[idx].linkID = 0;
		videoLink[idx].numOfKeyFrames = 0;
		for(int index = 0; index < NUMOFFRAMES; index++)
		{
			videoLink[idx].rectangleExists[index] = false;
			rectExist[index] = false; 
		}
		m_HyperListCtrl.DeleteString(idx);
		idx++;
	}
}