
// KarhunenLoeve.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CKarhunenLoeveApp:
// Сведения о реализации этого класса: KarhunenLoeve.cpp
//

class CKarhunenLoeveApp : public CWinApp
{
public:
	CKarhunenLoeveApp();

	// Переопределение
public:
	virtual BOOL InitInstance();

	// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CKarhunenLoeveApp theApp;
