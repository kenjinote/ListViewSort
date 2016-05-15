#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib,"comctl32")

#include <windows.h>
#include <commctrl.h>

TCHAR szClassName[] = TEXT("Window");

int CALLBACK CompareFunc(LPARAM lp1, LPARAM lp2, LPARAM hList)
{
	static LV_FINDINFO lvf;
	static int         nItem1, nItem2;
	static TCHAR        buf1[30], buf2[30];

	lvf.flags = LVFI_PARAM;
	lvf.lParam = lp1;
	nItem1 = ListView_FindItem((HWND)hList, -1, &lvf);

	lvf.lParam = lp2;
	nItem2 = ListView_FindItem((HWND)hList, -1, &lvf);

	ListView_GetItemText((HWND)hList, nItem1, (int)0, buf1, sizeof(buf1));
	ListView_GetItemText((HWND)hList, nItem2, (int)0, buf2, sizeof(buf2));

	int n1 = _wtoi(buf1);
	int n2 = _wtoi(buf2);
	return n1 - n2;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	static HWND hButton;
	LV_COLUMN column;
	LV_ITEM item;
	switch (msg)
	{
	case WM_CREATE:
		InitCommonControls();
		hList = CreateWindow(WC_LISTVIEW, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT, 10, 10, 256, 512, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		column.fmt = LVCFMT_LEFT;
		column.cx = 100;
		column.pszText = TEXT("数値");
		column.iSubItem = 0;
		ListView_InsertColumn(hList, 0, &column);
		TCHAR szTemp[16];
		for (int i = 0; i<10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				wsprintf(szTemp, TEXT("%d (%d x %d)"), i * j, i, j);
				item.mask = LVIF_TEXT | LVIF_PARAM;
				item.pszText = szTemp;
				item.iItem = i;
				item.iSubItem = 0;
				item.lParam = i * j;
				ListView_InsertItem(hList, &item);
			}
		}
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("ソート"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hList, 10, 10, 128, HIWORD(lParam) - 20, TRUE);
		MoveWindow(hButton, 148, 10, 128, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			ListView_SortItems(hList, CompareFunc, hList);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("リストボックスのソートサンプル"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}