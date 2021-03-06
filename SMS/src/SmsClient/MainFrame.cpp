#include "StdAfx.h"
#include "mainfrm.h"
#include "ServiceManager.h"
#include "userManager.h"
#include "SmsClientDlg.h"
#include "SmsConfig.h"
#include "ContactManager.h"
#include "SmsMessageLib.h"
#include "SmsIconTray.h"

HINSTANCE CMainFrame::m_hResModule = NULL;
CMainFrame* CMainFrame::m_pMainFrame = NULL;
CSmsClientView* CMainFrame::m_pClientView = NULL;

CMainFrame::CMainFrame(CServiceManager* p):m_view(this), m_strInforStatus(_T("就绪"))
{
	m_hResModule = NULL;
	m_pServiceManager = p;
	m_pSmsConfig = new CSmsClientConfig;

	m_pMainFrame = this;
	m_pClientView = &m_view;

	m_nTimer = 0;
	m_eCurrentStatus = STOPPED;

	m_nSendedCount = 0;
	m_nReceivedCount = 0;
	m_bExit = FALSE;
	m_bSaveWinPos = TRUE;
	m_pTray = new CSmsTray;
}
CMainFrame::~CMainFrame()
{
	if(m_pSmsConfig != NULL)
	{
		delete m_pSmsConfig;
		m_pSmsConfig = NULL;
	}
	if(m_pTray != NULL)
	{
		delete m_pTray;
		m_pTray = NULL;
	}
}
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}
void CMainFrame::OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD dwItemData)
{

}
void CMainFrame::PrepareToolBarMenu(UINT nMenuID, HMENU hMenu)
{
}
VOID CMainFrame::LoadSms()
{
	m_pServiceManager->OnInitStep(ON_LOAD_TOSEND);
	m_view.LoadListContent(CSmsClientView::SMS_TOSEND_VIEW);
	m_pServiceManager->OnInitStep(ON_LOAD_SENDED);
	m_view.LoadListContent(CSmsClientView::SMS_SENDED_VIEW);
	m_pServiceManager->OnInitStep(ON_LOAD_RECEIVED);
	m_view.LoadListContent(CSmsClientView::SMS_RECEIVED_VIEW);
	m_pServiceManager->OnInitStep(ON_LOAD_DELETED);
	m_view.LoadListContent(CSmsClientView::SMS_DELETED_VIEW);
	m_pServiceManager->OnInitStep(ON_LOAD_CONTACT);
	m_view.LoadContacts();
	m_pServiceManager->OnInitStep(ON_FINISHED);
}
LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//m_pServiceManager->OnInitStep(ON_START);
	m_pServiceManager->OnInitStep(ON_START);

	m_hResModule = (HINSTANCE)GetModuleHandle(NULL);

	//move window 
	CRect rect;
	m_pSmsConfig->GetWinPos(rect);
	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	m_view.LoadViewSplitConfig();


	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	m_CmdBar.AttachMenu(GetMenu());
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	m_CmdBar.LoadImages(IDR_VIEW_LIST);
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX);
	AddToolbarButtonText(hWndToolBar, ID_SYSTEM_CONNECT);
	AddToolbarButtonText(hWndToolBar, ID_SYSTEM_DISCONNECT);
	AddToolbarButtonText(hWndToolBar, ID_SYSTEM_SETTING);
	AddToolbarButtonText(hWndToolBar, ID_SMS_NEW);
	AddToolbarButtonText(hWndToolBar, ID_SMS_REPLY);
	AddToolbarButtonText(hWndToolBar, ID_SMS_TRANSFER);
	AddToolbarButtonText(hWndToolBar, ID_SMS_DELETE);
	AddToolbarButtonText(hWndToolBar, ID_SMS_SEARCH);
	AddToolbarButtonText(hWndToolBar, ID_TOOL_CONTACTLIST);
	AddToolbarButtonText(hWndToolBar, ID_TOOL_SMSLIB);
	AddToolbarButtonText(hWndToolBar, ID_SYSTEM_EXIT);
	AddToolbarButtonText(hWndToolBar, ID_SMS_CLEAR);
	
	AddToolBarDropDownMenu(hWndToolBar, ID_VIEW_CURRENT, IDR_VIEW_CURRENT);
	AddToolbarButtonText(hWndToolBar, ID_VIEW_CURRENT);
// 	CToolBarCtrl toolBar;
// 	toolBar.Create(m_hWnd);
// 	HWND hWndToolBar = toolBar;
// 	CImageList imglist;
// 	imglist.Create(IDB_BITMAP1, 24, 0, RGB(255,0,255));
// 	toolBar.SetImageList(imglist);
// 	toolBar.SetButtonSize(24, 24);
// 	TBBUTTON  btn;
// 	btn.iBitmap = 0;
// 	btn.idCommand = ID_SYSTEM_CONNECT;
// 	btn.fsState = TBSTATE_ENABLED ;
// 	btn.fsStyle = TBSTYLE_LIST ;
// 	toolBar.InsertButton(0, &btn);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);


	//create statusbar
	INT panes[] = {PANE_INFO, PANE_COUNT, PANE_COMPANY, PANE_STATUS};
	m_statusBar.Create(m_hWnd);
	m_hWndStatusBar = m_statusBar;
	m_statusBar.SetPanes(panes, sizeof(panes)/sizeof(INT), TRUE);


	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	UpdateLayout();
	m_view.UpdateSplit();

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);
	UIEnable(ID_SYSTEM_DISCONNECT, FALSE);
	UIEnable(ID_SYSTEM_CONNECT, TRUE);
	
	m_pTray->AddTray();

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	//PostMessage(WM_LOAD_DATA, NULL, NULL);
	m_pServiceManager->OnInitStep(ON_CREATE);
	return 0;
}
LRESULT CMainFrame::OnToolContact(WORD , WORD , HWND , BOOL& )
{
	CContactManagerDlg dlg;
	dlg.DoModal();

	return 0;
}
LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}
VOID CMainFrame::GetErrorMsg(INT nError, CString& msg)
{
	switch(nError)
	{
	case SMS_CLIENT_ERROR_CREATE_SOCKET:
		msg  = _T("SOCKET创建失败");
		break;
	case SMS_CLIENT_ERROR_IP_ADDRESS:
		msg = _T("不合法的服务IP地址");
		break;
	case SMS_CLIENT_ERROR_CONNECT:
		msg = _T("服务器连接失败");
		break;
	case SMS_CLIENT_ERROR_CREATE_THREAD:
		msg = _T("通讯线程创建失败");
		break;
	case SMS_CLIENT_ERROR_CREATE_AGENT:
		msg = _T("通讯代理窗口创建失败");
		break;
	case SMS_CLIENT_ERROR_ACCOUNT_USED:
		msg = _T("帐号已被使用");
		break;
	case SMS_CLIENT_ERROR_ACCOUNT_NOTEXIST:
		msg = _T("帐号错误");
		break;

	default:
		msg = _T("未知错误");
	}
}

LRESULT CMainFrame::OnSocketClientEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CSocketClient* pClient = m_pServiceManager->GetSmsClient();
	INT nErrorCode = pClient->GetLastError();
	CString strMsg;
	CString msg;
	BOOL bOK;
	CSmsUser* pUser = m_pServiceManager->GetUserManager()->GetCurrentUser(bOK);
	BOOL bMsgBox = FALSE;

	switch(wParam)
	{
	case SMS_CLIENT_CONNECT_WP:
		if(lParam == TRUE)
		{
			UIEnable(ID_SYSTEM_DISCONNECT, TRUE);
			UIEnable(ID_SYSTEM_CONNECT, FALSE);
			m_nTimer = SetTimer(1, 1000);
			m_eCurrentStatus = IDLE;
			ResetStatusBar();
		}
		else
		{
			UIEnable(ID_SYSTEM_DISCONNECT, FALSE);
			UIEnable(ID_SYSTEM_CONNECT, TRUE);
			m_eCurrentStatus = STOPPED;
			ResetStatusBar();

			GetErrorMsg(nErrorCode, msg);
			strMsg.Format(_T("短信服务连接失败，原因:%s!"), msg);
			MessageBox(strMsg, _T("提示信息"), MB_OK|MB_ICONERROR);
			
		}
		break;

	case SMS_CLIENT_SENDED_WP:
		if(m_pClientView->DoSended1Message((CSMSMessage*)lParam))
		{
			m_nSendedCount++;
			m_eCurrentStatus = IDLE;
			ResetStatusBar();
			PostMessage(WM_VIEW_CHANGED, NULL, NULL);
		}
		
		break;
	
	case SMS_CLIENT_RECEIVED_WP:
		m_nReceivedCount++;
		m_pClientView->DoReceivedMessage((CSMSMessage*)lParam);
		ResetStatusBar();
		PostMessage(WM_VIEW_CHANGED, NULL, NULL);
		break;
	
	case SMS_CLIENT_STOPPED_WP:
		bMsgBox = (m_eCurrentStatus != STOPPED);
		Disconnect();
		if(bMsgBox)
		{
			MessageBox(_T("断开与短信服务器的连接,可能原因为短信服务被停止或是网络故障!"), 
				_T("提示信息"), MB_OK|MB_ICONINFORMATION);	
		}

		break;
	}
	return 0;
}
LRESULT CMainFrame::OnTimer(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam == 1)
	{
		if(m_eCurrentStatus == IDLE)
		{
			if(m_pClientView->DoSend1Message())
			{
				m_eCurrentStatus = BUSY;
			}	
		}
	}
	return 0;
}
VOID CMainFrame::Disconnect()
{

	if(m_nTimer != 0)
	{
		KillTimer(1);
		m_nTimer = 0;
	}
	m_eCurrentStatus = STOPPED;

	CSocketClient* pClient = m_pServiceManager->GetSmsClient();
	if(pClient->IsRunning())
	{
		pClient ->DisConnect();
	}
	m_pClientView->OnDisconnect();

	UIEnable(ID_SYSTEM_DISCONNECT, FALSE);
	UIEnable(ID_SYSTEM_CONNECT, TRUE);

	m_nReceivedCount = 0;
	m_nSendedCount = 0;
	ResetStatusBar();
}
VOID CMainFrame::StartConnect()
{
	if(m_pServiceManager != NULL)
	{
		BOOL bOK(FALSE);
		CSocketClient* pClient = m_pServiceManager->GetSmsClient();
		CSmsUser* pUser = m_pServiceManager->GetUserManager()->GetCurrentUser(bOK);
		if(!bOK)
		{
			MessageBox(_T("服务连接失败，原因:数据库连接失败!"), _T("错误提示"),MB_OK|MB_ICONERROR);
		}
		else
		{
			m_eCurrentStatus = STARTING;
			UIEnable(ID_SYSTEM_DISCONNECT, FALSE);
			UIEnable(ID_SYSTEM_CONNECT, FALSE);
			ResetStatusBar();

			pClient->Connect(pUser->m_strServer,pUser->m_nPort,pUser->m_strName,pUser->m_strPwd);
			
		}
	}
}
LRESULT CMainFrame::OnConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	StartConnect();
	return 0;
}
LRESULT CMainFrame::OnDisconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Disconnect();

	return 0;
}
LRESULT CMainFrame::OnSetting(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CUserManager* pUserManager = m_pServiceManager->GetUserManager();
	CSettingDlg dlg(pUserManager);
	dlg.DoModal();

	return 0;
}
LRESULT CMainFrame::OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	
	ShowWindow(SW_HIDE);

	if(m_bExit)
	{
		if(m_eCurrentStatus != STOPPED)
		{
			Disconnect();
		}
		if(!IsIconic())
		{
			CRect rect;
			GetWindowRect(&rect);
			m_pSmsConfig->SaveWinPos(rect);
			m_view.SaveViewSplitConfig();	
		}

		m_pTray->DeleteTray();
		bHandled = FALSE;
	}

	return 0;
}
LRESULT CMainFrame::OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	m_bExit = TRUE;
	PostMessage(WM_CLOSE, 0, 0);
	return 0;
}
LRESULT CMainFrame::OnToolLib(WORD , WORD , HWND , BOOL& )
{
	CMessageLibDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	ResetStatusBar();
	return 0;
}

VOID CMainFrame::ResetStatusBar()
{
	CRect rt(0, 0, 0, 0);
	if(!GetWindowRect(&rt)) return;
	if(!m_statusBar.IsWindow()) return;

	INT nCompany(0);
	if(rt.Width() > PANE_ALL_MIN_WIDTH)
	{
		nCompany = rt.Width() - PANE_ALL_MIN_WIDTH;
	}
	
	m_statusBar.SetPaneWidth(PANE_INFO, PANE_INFO_WIDTH);
	m_statusBar.SetPaneWidth(PANE_COUNT, PANE_COUNT_WIDTH);
	m_statusBar.SetPaneWidth(PANE_COMPANY, nCompany);
	m_statusBar.SetPaneWidth(PANE_STATUS, PANE_STATUS_WIDTH);

	m_statusBar.SetPaneText(PANE_INFO, m_strInforStatus);
	
	CString strCount;
	strCount.Format(_T("已发/已收短信: %d/%d"), m_nSendedCount, m_nReceivedCount);
	m_statusBar.SetPaneText(PANE_COUNT, strCount);

	CString strInfo;
	LoadString(m_hResModule, IDS_COMPANY_INFO, strInfo.GetBuffer(256), 256);
	strInfo.ReleaseBuffer();
	m_statusBar.SetPaneText(PANE_COMPANY, strInfo);

	HICON hIcon = NULL;
//	CString strTip;
	switch(m_eCurrentStatus)
	{
	case STOPPED:
		hIcon = LoadIcon(m_hResModule, MAKEINTRESOURCE(IDI_DISCONNECTED));
//		strTip = _T("未连上短信服务器");
		break;
	case STARTING:
		hIcon = LoadIcon(m_hResModule, MAKEINTRESOURCE(IDI_CONNECTING));
	//	strTip = _T("正在连接短信服务器");
		break;
	default:
		hIcon = LoadIcon(m_hResModule, MAKEINTRESOURCE(IDI_CONNECTED));
	//	strTip = _T("已连上短信服务器");
		break;
	}
	if(hIcon != NULL)
	{
		m_statusBar.SetPaneIcon(PANE_STATUS, hIcon);
		DestroyIcon(hIcon);
	}

	m_pTray->ModifyTray(m_eCurrentStatus);
//	m_statusBar.SetPaneTipText(PANE_STATUS, strTip);	

}

LRESULT CMainFrame::OnViewChanged(UINT, WPARAM, LPARAM, BOOL&)
{
	CSmsClientView::SMS_LIST_VIEW_MODE mode = m_view.GetCurrentViewMode();
	INT nCount = m_view.GetCurrentCount(FALSE);

	switch(mode)
	{
		case CSmsClientView::SMS_TOSEND_VIEW:
			m_strInforStatus.Format(_T("当前共有%d条待发短信"), nCount);
			break;
		case CSmsClientView::SMS_SENDED_VIEW:
			m_strInforStatus.Format(_T("当前共有%d条已发短信"), nCount);
			break;
		case CSmsClientView::SMS_RECEIVED_VIEW:
			m_strInforStatus.Format(_T("当前共有%d条已收短信"), nCount);
			break;
		case CSmsClientView::SMS_DELETED_VIEW:
			m_strInforStatus.Format(_T("当前共有%d条已删短信"), nCount);
			break;
	}

	ResetStatusBar();

	return 0;
}

LRESULT CMainFrame::OnAbout(WORD, WORD, HWND, BOOL&)
{
	CSplashDlg dlg;
	dlg.DoModal();

	return 0;
}

LRESULT CMainFrame::OnMenuSelect(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	bHandled = TRUE;

	if(m_hWndStatusBar == NULL)
		return 1;

	WORD wFlags = HIWORD(wParam);
	if(wFlags == 0xFFFF && lParam == NULL)   // menu closing
	{
		::SendMessage(m_hWndStatusBar, SB_SIMPLE, FALSE, 0L);
	}
	else
	{
		const int cchBuff = 256;
		TCHAR szBuff[cchBuff];
		szBuff[0] = 0;
		if(!(wFlags & MF_POPUP))
		{
			WORD wID = LOWORD(wParam);
			// check for special cases
			if(wID >= 0xF000 && wID < 0xF1F0)                              // system menu IDs
				wID = (WORD)(((wID - 0xF000) >> 4) + ATL_IDS_SCFIRST);
			else if(wID >= ID_FILE_MRU_FIRST && wID <= ID_FILE_MRU_LAST)   // MRU items
				wID = ATL_IDS_MRU_FILE;
			else if(wID >= ATL_IDM_FIRST_MDICHILD)                         // MDI child windows
				wID = ATL_IDS_MDICHILD;

#if (_ATL_VER >= 0x0700)
			int nRet = ::LoadString(ATL::_AtlBaseModule.GetResourceInstance(), wID, szBuff, cchBuff);
#else //!(_ATL_VER >= 0x0700)
			int nRet = ::LoadString(_Module.GetResourceInstance(), wID, szBuff, cchBuff);
#endif //!(_ATL_VER >= 0x0700)
			for(int i = 0; i < nRet; i++)
			{
				if(szBuff[i] == _T('\n'))
				{
					szBuff[i] = 0;
					break;
				}
			}
		}
		
		m_statusBar.SetPaneText(PANE_INFO, szBuff);
	}

	return 1;

}
LRESULT CMainFrame::OnShowWindow(WORD, WORD, HWND, BOOL&)
{
	ShowWindow(SW_SHOW);
	if(IsIconic())
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	SetForegroundWindow(m_hWnd);

	return 0;
}
LRESULT CMainFrame::OnTrayMsg(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if(lParam == WM_LBUTTONDBLCLK || lParam == WM_LBUTTONDOWN)
	{
		PostMessage(WM_COMMAND, ID_SYSTEM_SHOW, 0);
	}
	if(lParam == WM_RBUTTONDOWN)
	{
		HMENU hMenu = ::LoadMenu(GetResourceModule(), MAKEINTRESOURCE(IDR_SEND_LIST));
		if(hMenu != NULL)
		{
			HMENU hSubMen = GetSubMenu(hMenu, 2);
			if(hSubMen != NULL)
			{
				MENUITEMINFO mi;
				mi.cbSize = sizeof(mi);
				mi.fMask = MIIM_STATE;
				if(GetMenuItemInfo(hSubMen, ID_SYSTEM_SHOW, FALSE, &mi))
				{
					mi.fState |= MFS_DEFAULT;
					SetMenuItemInfo(hSubMen, ID_SYSTEM_SHOW, FALSE, &mi);
				}

				CPoint pt;
				GetCursorPos(&pt);
				SetForegroundWindow(m_hWnd);
				TrackPopupMenu(hSubMen, TPM_LEFTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
			}
			DestroyMenu(hMenu);
		}
		

	}

	return 0;
}

LRESULT CMainFrame::OnQuestion(WORD, WORD ,HWND, BOOL&)
{
	CInformationDlg dlg;
	dlg.DoModal();

	return 0;
}