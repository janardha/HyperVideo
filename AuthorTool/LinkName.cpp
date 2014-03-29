// LinkName.cpp : implementation file
//

#include "stdafx.h"
#include "HyperVideoAuthor.h"
#include "LinkName.h"
#include "afxdialogex.h"


// LinkName dialog

IMPLEMENT_DYNAMIC(LinkName, CDialog)

LinkName::LinkName(CWnd* pParent /*=NULL*/)
	: CDialog(LinkName::IDD, pParent)
	, m_nameStr(_T(""))
{
	//GetDlgItem(IDC_EDIT1)->SetFocus();
}

LinkName::~LinkName()
{
}

void LinkName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Name, m_nameStr);
}


BEGIN_MESSAGE_MAP(LinkName, CDialog)
END_MESSAGE_MAP()


// LinkName message handlers
