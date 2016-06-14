#include "stdafx.h"

#ifdef    GUI_APPLICATION_USING_MFC
#include "MFCMainApp.h"
// The one and only CMFCMainApp object
CMFCMainApp theApp;
#else
#include "Win32MainApp.h"
int APIENTRY _tWinMain(_In_     HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_     LPTSTR    lpCmdLine,
                       _In_     int       nCmdShow)
{
  return Win32MainAppEntry(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
#endif // GUI_APPLICATION_USING_MFC
