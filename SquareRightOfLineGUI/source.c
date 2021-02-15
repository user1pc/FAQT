
#include <Windows.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "test_square_right_of_line_gui.h"

// Info indetifying my type of bitmap, a standard 32BPP bitmap
BITMAPINFO bmi;
BITMAPINFOHEADER bmih;
uint32_t* frame = NULL;

// square position, width
int32_t square_x = 512;
int32_t square_y = 512;
int32_t square_width_log2 = 8;
int32_t square_width = 256;

// line points
int32_t p1_x = 0;
int32_t p1_y = 0;
int32_t p2_x = 899;
int32_t p2_y = 899;

// user grab options
bool user_grabbing_p1 = false;
bool user_grabbing_p2 = false;

void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    uint32_t* pixels, int32_t width, int32_t height,
    uint32_t color)
{
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;

    if (abs(dx) > abs(dy))
    {
        if (x1 > x2)
        {
            int32_t temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        double slope = (double)dy / (double)dx;
        for (int32_t x = max(x1, 0); x <= min(x2, width - 1); x++)
        {
            int32_t y = y1 + (int32_t)((double)(x - x1) * slope);
            y = min(max(y, 0), height - 1);
            pixels[((height - 1) - y) * width + x] = color;
        }
    }
    else
    {
        if (y1 > y2)
        {
            int32_t temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        double slope = (double)dx / (double)dy;
        for (int32_t y = max(y1, 0); y <= min(y2, height - 1); y++)
        {
            int32_t x = x1 + (int32_t)((double)(y - y1) * slope);
            x = min(max(x, 0), width - 1);
            pixels[((height - 1) - y) * width + x] = color;
        }
    }
}

LRESULT CALLBACK WMCallback(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
    switch (message)
    {
    case WM_CREATE:
    {
        RECT region;
        GetWindowRect(hwnd, &region);
        bmih.biSize = sizeof(BITMAPINFOHEADER);
        uint32_t window_width = (region.right - region.left) + 1;
        uint32_t window_height = (region.bottom - region.top) + 1;

        // Setup the bitmap info
        bmih.biWidth = window_width;
        bmih.biHeight = window_height;
        bmih.biPlanes = 1;
        bmih.biBitCount = 32;
        bmih.biCompression = BI_RGB;
        bmih.biSizeImage = 0;
        bmih.biXPelsPerMeter = 1;
        bmih.biYPelsPerMeter = 1;
        bmih.biClrUsed = 0;
        bmih.biClrImportant = 0;
        bmi.bmiColors->rgbRed = 0;
        bmi.bmiColors->rgbGreen = 0;
        bmi.bmiColors->rgbBlue = 0;
        bmi.bmiHeader = bmih;

        // Allocate the drawing pixel buffer
        uint32_t* new_frame = realloc(frame, (size_t)window_width * (size_t)window_height * sizeof(uint32_t));
        if (new_frame)
            frame = new_frame;
        else
            exit(1); // Cant allocate the new frame
        break;
    }
    case WM_LBUTTONDOWN:
    {
        // User clicked down
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hwnd, &p);
        RECT region;
        GetWindowRect(hwnd, &region);
        uint32_t window_width = (region.right - region.left) + 1;
        uint32_t window_height = (region.bottom - region.top) + 1;

        // Check if the user clicked one of the two points
        if (abs(p.x - p1_x) < 5 && abs(p.y - p1_y) < 5)
            user_grabbing_p1 = true;
        else if (abs(p.x - p2_x) < 5 && abs(p.y - p2_y) < 5)
            user_grabbing_p2 = true;
        break;
    }
    case WM_LBUTTONUP:
    {
        // Release any points the user may have been grabbing
        user_grabbing_p1 = false;
        user_grabbing_p2 = false;
        break;
    }
    case WM_MOUSEMOVE:
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hwnd, &p);
        RECT region;
        GetWindowRect(hwnd, &region);
        uint32_t window_width = (region.right - region.left) + 1;
        uint32_t window_height = (region.bottom - region.top) + 1;
        // Update point info if the user is dragging one of the points
        if (user_grabbing_p1)
        {
            p1_x = p.x;
            p1_y = p.y;
        }
        if (user_grabbing_p2)
        {
            p2_x = p.x;
            p2_y = p.y;
        }

        // re-draw
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT painter;
        RECT region;
        GetWindowRect(hwnd, &region);
        uint32_t window_width = (region.right - region.left) + 1;
        uint32_t window_height = (region.bottom - region.top) + 1;

        // Color the background red
        for (uint32_t y = 0; y < window_height; y++)
        {
            for (uint32_t x = 0; x < window_width; x++)
            {
                uint32_t pixel = (255 << 24) | (255 << 16);
                frame[y * window_width + x] = pixel;
            }
        }

        // Draw the line
        draw_line(p1_x, p1_y, p2_x, p2_y, frame, window_width, window_height, 0);
        // draw the square outline
        uint32_t square_color = (255 << 24) | (100 << 16) | (25 << 8) | (130);
        draw_line(square_x, square_y, square_x + square_width, square_y, frame, window_width, window_height, square_color);
        draw_line(square_x + square_width, square_y, square_x + square_width, square_y + square_width, frame, window_width, window_height, square_color);
        draw_line(square_x + square_width, square_y + square_width, square_x, square_y + square_width, frame, window_width, window_height, square_color);
        draw_line(square_x, square_y + square_width, square_x, square_y, frame, window_width, window_height, square_color);

        // Draw the traversal stuff
        test_square_right_of_line_gui(frame, window_width, window_height, p1_x, p1_y, p2_x, p2_y, square_x, square_y, square_width_log2);

        // Draw the pixel buffer onto the screen
        HDC paint_dc = BeginPaint(hwnd, &painter);
        HDC my_dc = CreateCompatibleDC(paint_dc);
        HBITMAP drawing_bmp = CreateCompatibleBitmap(paint_dc, window_width, window_height);
        HGDIOBJ old_object = SelectObject(my_dc, drawing_bmp);

        SetDIBits(my_dc, drawing_bmp, 0, window_height, frame, &bmi, DIB_RGB_COLORS);
        BitBlt(paint_dc, 0, 0, window_width, window_height, my_dc, 0, 0, SRCCOPY);
        HGDIOBJ drawing_bmp_again = SelectObject(my_dc, old_object);
        DeleteObject(drawing_bmp);
        DeleteDC(my_dc);

        EndPaint(hwnd, &painter);
        break;
    }
    default:
        return DefWindowProc(hwnd, message, w_param, l_param);
        break;
    }
    return DefWindowProc(hwnd, message, w_param, l_param);
}


int WinMain(HINSTANCE instance_handle, HINSTANCE previous_instance, LPSTR coomand, int show_cmd)
{
    // Define the window the window
    WNDCLASSEX window_class;
    window_class.cbClsExtra = 0;
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.cbWndExtra = 0;
    window_class.hbrBackground = (HBRUSH)0;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hInstance = instance_handle;
    window_class.lpfnWndProc = &WMCallback;
    window_class.lpszClassName = L"MainClass";
    window_class.lpszMenuName = L"MenuName";
    window_class.style = 0;

    // Create the window
    RegisterClassEx(&window_class);
    HWND hwnd = CreateWindowEx(0, window_class.lpszClassName, L"GeometryTranseralSandbox",
        WS_POPUP, 300, 100, 900, 900,
        NULL, NULL, instance_handle, NULL);
    ShowWindow(hwnd, show_cmd);

    // Run the message loop
    MSG message;
    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return (int)message.wParam;
}
