// HyperVideoAuthorDlg.h : header file
//

#pragma once
//#include "afxwin.h"
#include "imageParam.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Winuser.h"
#include "LinkName.h"
#include "string.h"

#define NUMOFFRAMES	720
#define TOLERANCE 3
// CHyperVideoAuthorDlg dialog
class CHyperVideoAuthorDlg : public CDialog
{
// Construction
public:
	CHyperVideoAuthorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HYPERVIDEOAUTHOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnRefresh();
	afx_msg LRESULT WNDPROC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	LinkName m_popUpDlg;
public:
	struct linkStruct
	{
		int linkID;
		int primaryKeyFrame[100];
		int secondaryLinkFrame;
		int numOfKeyFrames;
		struct framePoints
		{
			int xposmin;
			int yposmin;
			int xposmax;
			int yposmax;

		}primaryEnds[200];
		char secondaryPath[_MAX_PATH];
		char LinkName[40];
		bool rectangleExists[NUMOFFRAMES];

	}videoLink[20];

	struct pointPos
	{
		int xpos;
		int ypos;
	}rectPts[4], curMousePos, clickDownPos, clickUpPos, frameRectangles[40], resize[2], linkRect[2];

	VideoDisplay displayFrame;
	
	//WNDPROC OrgProcedure, NewProcedure;

	HWND hWin;
	int actionlistID;
	int linkNum;
	int totalLinks;
	int videoNum;
	bool bRubberBanding;
	bool bMouseSelectionEnabled;
	bool isBkgTransparent;
	bool bInPaint;
	bool bSetSliderMax;
	bool bRepaintFrame;
	int keyIndex;
	bool rectExist[NUMOFFRAMES];
	bool keyIndexSet;
	bool frameSet;
	bool bPrimaryVideoExists;
	bool bSecondaryVideoExists;
	int listNum;
	int rectPerFrame;
	int primarySliderPosition;
	int secondaryVideoPositon;
	char primaryPath[_MAX_PATH];
	char secondaryPath[_MAX_PATH];
	CComboBox m_ActionCombo;
	CComboBox m_VideoList;
	CSliderCtrl m_PrimarySlider;
	CSliderCtrl m_SecondarySlider;
	
	afx_msg void OnCbnSelchangeLinkCombo();

	afx_msg void OnCbnSelchangeActionlist();
	
	void Interpolate(int frameNum);
	
	void paintFrame();
	
	void paintSecondaryFrame();

	afx_msg void OnNMReleasedcapturePrimaryslider(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedConnectvideo();
	
	afx_msg void OnNMReleasedcaptureSecondaryslider(NMHDR *pNMHDR, LRESULT *pResult);

//	LRESULT CALLBACK PictureProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR refPtr);
	DECLARE_EVENTSINK_MAP()
	void MouseMovePrimaryactivex(short Button, short Shift, float X, float Y);
	void MouseDownPrimaryactivex(short Button, short Shift, float X, float Y);
	void MouseUpPrimaryactivex(short Button, short Shift, float X, float Y);
	CButton m_Connect;
	afx_msg void OnBnClickedSave();
	CButton m_Save;
	
	CComboBox m_HyperListCtrl;
	CString m_linkName;
	
	afx_msg void OnCbnSelchangeHyperlinklist();

	void createLinkData(CString str);
	void readMetaDataFile();
	void sortFrameNum();
	void clearData();
};
