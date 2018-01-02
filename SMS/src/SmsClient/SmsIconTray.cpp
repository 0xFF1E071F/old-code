#include "StdAfx.h"
#include "SmsIconTray.h"

VOID CIconTray::AddTray(NOTIFYICONDATA& nid)
{
	Shell_NotifyIcon(NIM_ADD, &nid);
}
VOID CIconTray::ModifyTray(NOTIFYICONDATA& nid)
{
	Shell_NotifyIcon(NIM_MODIFY, &nid);
}
VOID CIconTray::DeleteTray(NOTIFYICONDATA& nid)
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

VOID CSmsTray::AddTray()
{	
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = *CMainFrame::GetMainFrame();
	nid.uID = 1;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_SHELL_MSG;
	HICON	 hIcon = ::LoadIcon(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDI_DISCONNECTED_TRAY));
	nid.hIcon = hIcon;
	lstrcpy(nid.szTip, _T("���ſͻ��� - δ����"));
	if(hIcon != NULL)
	{
		::DestroyIcon(hIcon);
	}

	m_tray.AddTray(nid);
}

VOID CSmsTray::ModifyTray(CMainFrame::ENUM_SMS_CLIENT_STATUS state)
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = *CMainFrame::GetMainFrame();
	nid.uID = 1;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_SHELL_MSG;
	HICON hIcon = NULL;
	TCHAR szTip[64] = {0};
	if(state == CMainFrame::STOPPED)
	{
		lstrcpy(szTip, _T("���ſͻ��� - δ����"));
		hIcon = ::LoadIcon(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDI_DISCONNECTED_TRAY));
	}
	else if(state == CMainFrame::STARTING)
	{
		lstrcpy(szTip, _T("���ſͻ��� - ��������"));
		hIcon = ::LoadIcon(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDI_CONNECTING_TRAY));
	}
	else
	{
		lstrcpy(szTip, _T("���ſͻ��� - ������"));
		hIcon = ::LoadIcon(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDI_CONNECTED_TRAY));
	}
	nid.hIcon = hIcon;
	lstrcpy(nid.szTip, szTip);
	if(hIcon != NULL)
	{
		DestroyIcon(hIcon);
	}
	
	m_tray.ModifyTray(nid);
}

VOID CSmsTray::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = *CMainFrame::GetMainFrame();
	nid.uID = 1;
	m_tray.DeleteTray(nid);
}