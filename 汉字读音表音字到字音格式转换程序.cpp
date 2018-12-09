// Win32Project1.cpp : ����Ӧ�ó������ڵ㡣
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

// ����������chang3�ĳ�ch��ng
std::string tone( std::string s ) {
	// ���˵��쳣���
	if (s.size() == 0)
		return s;
	char idx = s[s.size() - 1];
	if (idx<'1' || idx>'5')
		return s;
	s = s.substr(0, s.size() - 1);
	idx -= '1';

	const char * yuan[] = { "��������aa","��������oo","��������ee","��������ii","��������u","����������" };
	const char * ych = "aoeiuv";
	for (long i = 0; i < 6; i++)
	{
		size_t p = s.find(ych[i]);
		if (p == std::string::npos)
			continue;

		char c[3] = {0};
		*((wchar_t*)(&c)) = ((wchar_t*)yuan[i])[idx];
		if (c[0] > 0)
			c[1] = 0; // ��aoeiu����һ�ַ�
		if (yuan[i][5] == 'i' && s[p + 1] == 'u') // iu���б��ں�
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

	// ����m��n��ng��hng��
	idx += '1';
	return s+idx;
}

void pinyin() {
	std::string ss = FileRead("���ֶ�����GB2312��.txt");
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
				t[2] = 0; //��t���һ������
				auto r = sm.insert(std::make_pair(t, py));
				if (!r.second){ // ����ʧ�ܣ����Ƕ�����
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

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	pinyin();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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
