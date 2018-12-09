// Win32Project1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Project1.h"

#include <string>
#include <deque>
#include <vector>
#include <map>

template <class T1, class T2, class T3 >
struct delta { T1 a; T2 b; T3 c; delta() {}delta(T1 _a, T2 _b, T3 _c) :a(_a), b(_b), c(_c) {} };
template <class T1, class T2, class T3, class T4 >
struct quad { T1 a; T2 b; T3 c; T4 d; quad(T1 _a, T2 _b, T3 _c, T4 _d) :a(_a), b(_b), c(_c), d(_d) {} };
typedef delta<std::string, std::string, std::string> deltas;
typedef quad<std::string, std::string, std::string, std::string> quads;

std::string FileRead(const char * sFileName)
{
	FILE * f = fopen(sFileName, "rb");
	if (!f)
		return "";

	fseek(f, 0, SEEK_END);
	long z = ftell(f);
	char * s = new char[z + 1];
	*s = 0;
	fseek(f, 0, SEEK_SET);
	fread(s, z, 1, f);
	fclose(f);

	s[z] = 0;
	std::string s1 = s;
	delete s;
	return s1;
}

bool FileWrite(const char * sFileName, const char * sBuf, long len)
{
	FILE * f = fopen(sFileName, "wb");
	if (!f)
		return false;
	fwrite(sBuf, len, 1, f);
	fclose(f);

	return true;
}


class CStrPair : public std::pair< std::string, std::string >
{
public:
	CStrPair(const char * s, const char * sp)
	{
		const char * t = strstr(s, sp);
		if (t)
		{
			this->first.append(s, t - s);
			this->second.append(t + strlen(sp));
		}
		else
		{
			this->first.append(s);
		}
	}
};

class CStrVector : public std::vector< std::string >
{
public:
	CStrVector(const char * s = _T(""), const char * sp = _T(""))
	{
		if (s && s[0] != 0)
			Parse(s, sp);
	}
	bool Parse(const char * s, const char * sp)
	{
		std::string sTemp;
		for (long i = 0; s[i] != 0; i++)
		{
			if (0 == strncmp(s + i, sp, strlen(sp)))
			{
				push_back(sTemp);
				i += (long)strlen(sp) - 1;
				sTemp = _T("");
				continue;
			}
			sTemp += s[i];
		}

		if (sTemp != _T(""))
			push_back(sTemp);

		return true;
	}
};

class CStrMap : public std::map< std::string, std::string >
{
public:
	CStrMap(const char * s, const char * sp1, const char * sp2)
	{
		const char * t = s;

		while (t)
		{
			std::string key, val;
			const char * s1 = strstr(t, sp1);
			if (!s1)
				break;
			key.append(t, s1 - t);
			s1 += strlen(sp1);

			const char * s2 = strstr(s1, sp2);
			if (!s2)
			{
				val = s1;
				this->insert(std::make_pair(key, val));
				break;
			}
			else
			{
				val.append(s1, s2 - s1);
				this->insert(std::make_pair(key, val));
				t = s2 + strlen(sp2);
			}
		}
	}
};

class CStrDeq : public std::deque< std::pair<std::string, std::string> >
{
public:
	CStrDeq() {}
	CStrDeq(const char * s, const char * sp1, const char * sp2)
	{
		const char * t = s;

		while (t)
		{
			std::string key, val;
			const char * s1 = strstr(t, sp1);
			if (!s1)
				break;
			key.append(t, s1 - t);
			s1 += strlen(sp1);

			const char * s2 = strstr(s1, sp2);
			if (!s2)
			{
				val = s1;
				this->push_back(std::make_pair(key, val));
				break;
			}
			else
			{
				val.append(s1, s2 - s1);
				this->push_back(std::make_pair(key, val));
				t = s2 + strlen(sp2);
			}
		}
	}
	void push_pair(const char * key, const char * val)
	{
		push_back(std::make_pair(key, val));
	}

	std::string operator[](std::string key)
	{
		std::deque< std::pair<std::string, std::string> >::iterator pos;
		for (pos = this->begin(); pos != this->end(); pos++)
		{
			if (pos->first == key)
				return pos->second;
		}

		return "";
	}
};

// 加声调，将chang3改成chǎng
std::string tone( std::string s ) {
	// 过滤掉异常情况
	if (s.size() == 0)
		return s;
	char idx = s[s.size() - 1];
	if (idx<'1' || idx>'5')
		return s;
	s = s.substr(0, s.size() - 1);
	idx -= '1';

	const char * yuan[] = { "āáǎàaa","ōóǒòoo","ēéěèee","īíǐìii","ūúǔùu","ǖǘǚǜü" };
	const char * ych = "aoeiuv";
	for (long i = 0; i < 6; i++)
	{
		size_t p = s.find(ych[i]);
		if (p == std::string::npos)
			continue;

		char c[3] = {0};
		*((wchar_t*)(&c)) = ((wchar_t*)yuan[i])[idx];
		if (c[0] > 0)
			c[1] = 0; // 若aoeiu，仅一字符
		if (yuan[i][5] == 'i' && s[p + 1] == 'u') // iu并列标在后
		{
			s.replace(p+1, p+2, c);
			return s;
		}
		else if (yuan[i][5] == 'u' && s[p + 1] == 'i')
		{
			s.replace(p + 1, p + 2, c);
			return s;
		}
		else {
			s.replace(p, p + 1, c);
			return s;
		}
	}

	// 少数m、n、ng、hng等
	idx += '1';
	return s+idx;
}

void pinyin() {
	std::string ss = FileRead("汉字读音表GB2312版.txt");
	CStrVector sv(ss.c_str(), "\r\n");

	std::map<std::string, std::string> sm;
	for (auto p = sv.begin(); p != sv.end(); p++)
	{
		if ((*p).find(":") != std::string::npos)
		{
			CStrPair sp(p->c_str(), ":");

			std::string py = tone(sp.first);
			char * t = (char *)sp.second.c_str();
			while (t[0] != 0) {
				char c = t[2];
				t[2] = 0; //将t变成一个汉字
				auto r = sm.insert(std::make_pair(t, py));
				if (!r.second){ // 插入失败，这是多音字
					r.first->second += ","+ py;
				}
				t[2] = c;
				t += 2;
			}
		}
	}

	for (auto p = sm.begin(); p != sm.end(); p++)
	{
		char s[128] = {0};
		sprintf(s, "%s:%s\n", p->first.c_str(), p->second.c_str());
		OutputDebugString(s);
	}

	MessageBox(NULL, "pinyin", "", MB_OK);
}


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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	pinyin();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

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



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
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
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
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
        }
        break;
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
