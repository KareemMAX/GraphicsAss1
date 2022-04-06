#include <Windows.h>
#include <math.h>
void swap(int& x1, int& y1, int& x2, int& y2)
{
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

void swap(int& x, int& y)
{
    int tmp = x;
    x = y;
    y = tmp;
}

int Round(double x)
{
    return (int)(x + 0.5);
}

void DrawPoints(HDC hdc, int xc, int yc, int a, int b)
{
    SetPixel(hdc, xc + a, yc + b, RGB(100, 0, 0));
    SetPixel(hdc, xc - a, yc + b, RGB(0, 0, 0));
    SetPixel(hdc, xc - a, yc - b, RGB(0, 0, 0));
    SetPixel(hdc, xc + a, yc - b, RGB(0, 100, 0));
    SetPixel(hdc, xc - b, yc + a, RGB(0, 0, 0));
    SetPixel(hdc, xc - b, yc - a, RGB(100, 0, 0));
    SetPixel(hdc, xc + b, yc - a, RGB(0, 0, 100));
}

void CircleBresenham(HDC hdc, int xc, int yc, int R)
{
    int x = 0, y = R;
    int d = 1 - R;
    int c1 = 3, c2 = 5 - 2 * R;
    DrawPoints(hdc, xc, yc, x, y);
    while (x < y)
    {
        if (d < 0)
        {
            d += c1;
            c2 += 2;
        }
        else
        {
            d += c2;
            c2 += 4;
            y--;
        }
        c1 += 2;
        x++;
        DrawPoints(hdc, xc, yc, x, y);
    }
}

static int state = 0;
static int xc, yc, r1, r2;

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    int x, y;

    switch (mcode)
    {
    case WM_LBUTTONDOWN:
        hdc = GetDC(hwnd);

        switch (state) {
        case 0:
            xc = LOWORD(lp);
            yc = HIWORD(lp);
            break;
        case 1:
            x = LOWORD(lp);
            y = HIWORD(lp);
            r1 = (int)sqrt(((x - xc) * (x - xc)) + ((y - yc) * (y - yc)));
            CircleBresenham(hdc, xc, yc, r1);
            break;
        case 2:
            x = LOWORD(lp);
            y = HIWORD(lp);
            r2 = (int)sqrt(((x - xc) * (x - xc)) + ((y - yc) * (y - yc)));
            if (r1 > r2)
                swap(r1, r2);

            for (int i = r1; i <= r2; i++)
            {
                CircleBresenham(hdc, xc, yc, i);
            }
            break;
        }
        state++;
        if (state == 3)
            state = 0;

        ReleaseDC(hwnd, hdc);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = L"MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}