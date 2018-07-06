// ServerShutdownDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerShutdown.h"
#include "ServerShutdownDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "JetByteTools\Win32Tools\Exception.h"

/////////////////////////////////////////////////////////////////////////////
// CServerShutdownDlg dialog

CServerShutdownDlg::CServerShutdownDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerShutdownDlg::IDD, pParent),
      m_shutdownEvent(_T("JetByteToolsServerShutdown"), false),
      m_pauseResumeEvent(_T("JetByteToolsServerPauseResume"), false)
{
	//{{AFX_DATA_INIT(CServerShutdownDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerShutdownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerShutdownDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerShutdownDlg, CDialog)
	//{{AFX_MSG_MAP(CServerShutdownDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SHUTDOWN, OnShutdown)
	ON_BN_CLICKED(IDC_PAUSERESUME, OnPauseResume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerShutdownDlg message handlers

BOOL CServerShutdownDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerShutdownDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerShutdownDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CServerShutdownDlg::OnShutdown() 
{
   try
   {
	   m_shutdownEvent.Pulse();
   }
   catch(JetByteTools::Win32::CException &e)
   {
      e.MessageBox();
   }
}

void CServerShutdownDlg::OnPauseResume() 
{
   try
   {
	   m_pauseResumeEvent.Pulse();
   }
   catch(JetByteTools::Win32::CException &e)
   {
      e.MessageBox();
   }
}
