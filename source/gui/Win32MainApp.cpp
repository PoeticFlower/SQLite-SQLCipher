#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

/*****************************************************************************
 *
 * MODULE  : Global Variables
 * PURPOSE : Defines the global variables.
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: References the example code in Visual Studio.
 *
 *****************************************************************************/

HINSTANCE hInst;                             // current instance
TCHAR     szTitle      [MAX_LOADSTRING];     // The title bar text
TCHAR     szWindowClass[MAX_LOADSTRING];     // the main window class name

/*****************************************************************************
 *
 * MODULE  : Forward Declarations
 * PURPOSE : Forward declarations of functions included in this code module.
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: References the example code in Visual Studio.
 *
 *****************************************************************************/

ATOM                GuiAppRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance       (HINSTANCE, int);
LRESULT CALLBACK    WndProc            (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About              (HWND, UINT, WPARAM, LPARAM);

/*****************************************************************************
 *
 * FUNCTION: Win32MainAppEntry(HINSTANCE, HINSTANCE, LPTSTR, int)
 * PURPOSE : Implements the entry point of Windows OS for the application.
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: References the example code in Visual Studio.
 *
 *****************************************************************************/

int APIENTRY Win32MainAppEntry(_In_     HINSTANCE hInstance,
                               _In_opt_ HINSTANCE hPrevInstance,
                               _In_     LPTSTR    lpCmdLine,
                               _In_     int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  HACCEL hAccelTable;
  MSG    msg;

  // Initialize global strings
  LoadString(hInstance, IDS_MAINAPP_TITLE, szTitle      , MAX_LOADSTRING);
  LoadString(hInstance, IDC_WIN32MAINAPP , szWindowClass, MAX_LOADSTRING);
  GuiAppRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  hAccelTable = LoadAccelerators(
    hInstance, MAKEINTRESOURCE(IDC_WIN32MAINAPP));

  // Main message loop:
  while (GetMessage(&msg, NULL, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage (&msg);
    }
  }

  return (int)msg.wParam;
}

/*****************************************************************************
 *
 * FUNCTION: GuiAppRegisterClass(HINSTANCE hInstance)
 * PURPOSE : Registers the window class.
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: References the example code in Visual Studio.
 *
 *****************************************************************************/

ATOM              GuiAppRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEX wcex;

  wcex.cbSize          = sizeof(WNDCLASSEX);

  wcex.style           = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc     = WndProc;
  wcex.cbClsExtra      = 0;
  wcex.cbWndExtra      = 0;
  wcex.hInstance       = hInstance;
  wcex.hIcon           = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINAPP_NORMAL));
  wcex.hIconSm         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINAPP_SMALL));
  wcex.hCursor         = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName    = MAKEINTRESOURCE(IDC_WIN32MAINAPP);
  wcex.lpszClassName   = szWindowClass;

  return RegisterClassEx(&wcex);
}

/*****************************************************************************
 *
 * FUNCTION: InitInstance(HINSTANCE hInstance, int nCmdShow)
 * PURPOSE : Saves instance handle and creates main window.
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: References the example code in Visual Studio.
 *           In this function, we save the instance handle in a global
 *           variable and create and display the main program window.
 *
 *****************************************************************************/

BOOL              InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  // Store instance handle in our global variable
  hInst = hInstance;

  HWND hWnd = CreateWindowEx(
    WS_EX_ACCEPTFILES,
    szWindowClass,
    szTitle,
    WS_OVERLAPPEDWINDOW,
    200,//CW_USEDEFAULT,
    200,
    500,//CW_USEDEFAULT,
    300,
    NULL,
    NULL,
    hInstance,
    NULL);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow  (hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

/*****************************************************************************
 *
 * FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
 * PURPOSE : Processes messages for the main window.
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: References the example code in Visual Studio.
 *           WM_COMMAND - process the application menu
 *           WM_PAINT   - Paint the main window
 *           WM_DESTROY - post a quit message and return
 *
 *****************************************************************************/

LRESULT CALLBACK  WndProc(HWND    hWnd,
                          UINT    message,
                          WPARAM  wParam,
                          LPARAM  lParam)
{
  int         wmId, wmEvent;
  PAINTSTRUCT ps;
  HDC         hdc;

  switch (message)
  {
  case WM_COMMAND:
    wmId    = LOWORD(wParam);
    wmEvent = HIWORD(wParam);
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    // TODO: Add any drawing code here...
    EndPaint(hWnd, &ps);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*****************************************************************************
 *
 * FUNCTION: About(HWND, UINT, WPARAM, LPARAM)
 * PURPOSE : Message handler for about box.
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN> on 20151130
 * COMMENTS: References the example code in Visual Studio.
 *
 *****************************************************************************/

INT_PTR CALLBACK  About(HWND    hDlg,
                        UINT    message,
                        WPARAM  wParam,
                        LPARAM  lParam)
{
  UNREFERENCED_PARAMETER(lParam);

  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}
