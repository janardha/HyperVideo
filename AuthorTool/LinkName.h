#pragma once


// LinkName dialog

class LinkName : public CDialog
{
	DECLARE_DYNAMIC(LinkName)

public:
	LinkName(CWnd* pParent = NULL);   // standard constructor
	virtual ~LinkName();

// Dialog Data
	enum { IDD = IDD_LinkNameEntry };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_nameStr;
};
