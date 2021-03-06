// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DIALOG_MAIN.h" 
//-----------------------------------------------------------------------------
#define szRDS _RXST("sdk")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
static int s_nIndex = 2;
static bool s_bIsMenuLoaded = false;
void
addMenuThroughCom(/*bool bIsMenuLoaded*/)
{

	IAcadApplication *pAcad;
	IAcadMenuBar *pMenuBar;
	IAcadMenuGroups *pMenuGroups;
	IAcadMenuGroup *pMenuGroup;//菜单组
	IAcadPopupMenus *pPopUpMenus;//顶菜单集
	IAcadPopupMenu *pPopUpMenu;//顶菜单
	IAcadPopupMenuItem *pPopUpMenuItem;//下拉的菜单

	HRESULT hr = NOERROR;
	LPUNKNOWN pUnk = NULL;
	LPDISPATCH pAcadDisp = acedGetIDispatch(TRUE); 
	if(pAcadDisp==NULL)
		return;

	hr = pAcadDisp->QueryInterface(IID_IAcadApplication,(void**)&pAcad);
	pAcadDisp->Release();
	if (FAILED(hr))
		return;

	pAcad->put_Visible(true);
	pAcad->get_MenuBar(&pMenuBar);
	pAcad->get_MenuGroups(&pMenuGroups);
	pAcad->Release();
	long numberOfMenus;
	pMenuBar->get_Count(&numberOfMenus);
	s_nIndex = numberOfMenus - 2;

	pMenuBar->Release();

	VARIANT index;
	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = 0;

	pMenuGroups->Item(index, &pMenuGroup);
	pMenuGroups->Release();

	pMenuGroup->get_Menus(&pPopUpMenus);
	pMenuGroup->Release();

	TCHAR strMenuName[256];
	_tcscpy(strMenuName, _T("插件(&C)"));//菜单项

	// Enables the menu to be loaded/unloaded with the same command.
	if (!s_bIsMenuLoaded) {
		pPopUpMenus->Add(strMenuName, &pPopUpMenu);

		if (pPopUpMenu != NULL) {

			TCHAR strMenuItemName[256];
			TCHAR strMenuItemMacro[256];
			_tcscpy(strMenuItemName, _T("铁路数据提取"));//下拉子菜单
			_tcscpy(strMenuItemMacro, _T("Read "));//点击下拉子菜单时执行的命令,最后以空格或'\n'结束

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = 0;
			pPopUpMenu->AddMenuItem(index, strMenuItemName, strMenuItemMacro, &pPopUpMenuItem);

			//VariantInit(&index);
			//V_VT(&index) = VT_I4;
			//V_I4(&index) = 1;
			//pPopUpMenu->AddSeparator(index, &pPopUpMenuItem);

			//_tcscpy(strMenuItemName, _T("&G贵州"));//下拉子菜单
			//_tcscpy(strMenuItemMacro, _T("Guizhou "));//点击下拉子菜单时执行的命令,最后以空格或'\n'结束

			//VariantInit(&index);
			//V_VT(&index) = VT_I4;
			//V_I4(&index) = 2;
			//pPopUpMenu->AddMenuItem(index, strMenuItemName, strMenuItemMacro, &pPopUpMenuItem);

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = s_nIndex;
			pPopUpMenu->InsertInMenuBar(index);

			pPopUpMenuItem->Release();
			s_bIsMenuLoaded = true;
		}
		else 
		{
			acutPrintf(_T("\nMenu not created."));
			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = s_nIndex ;
			pPopUpMenus->InsertMenuInMenuBar(strMenuName,index);
			s_bIsMenuLoaded = true;
		}
	} 
	else 
	{
		//arx本身所带例子程序这一部分是不对的。
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = s_nIndex-1;
		
		HRESULT hr;
		hr = pPopUpMenus->RemoveMenuFromMenuBar(index);
		VariantClear(&index);
		acedUpdateDisplay();
		s_bIsMenuLoaded = false;
	}

	pPopUpMenus->Release();
}

class CArxProject3App : public AcRxArxApp {

public:
	CArxProject3App () : AcRxArxApp () {}
	//CAcToolBar* m_ptoolbar;
	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here
		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		addMenuThroughCom();
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Unload dependencies here
		addMenuThroughCom();
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
		//----- Self-register COM server upon loading.
		if ( FAILED(::DllRegisterServer ()) )
			acutPrintf (_RXST("Failed to register COM server.\n")) ;
	}


	// ----- sdkArxProject3._Read command
	static void sdkArxProject3_Read(void)
	{
		// Add your code for command sdkArxProject3._MyCommand1 here
		DIALOG_MAIN dlg;
		dlg.DoModal();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject3App)

ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, sdkArxProject3, _Read, read, ACRX_CMD_TRANSPARENT, NULL)

