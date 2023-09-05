#include <Windows.h>
#include <ctime>
#include <vector>
#include <string>
#include "image_arr.h" 

enum OperationMode {
    SINGLE_CENTER, // single centered image
    SINGLE_RANDOM, // single random location image (location decided at exe startup)
    MULTIPLE_RANDOM // new image at new location every frame
};

constexpr bool ALLOW_MEMLEAK = false;

constexpr int FPS = 15; // how many times a second should image be redrawn; higher values impact pc performance

constexpr int OPERATION_MODE = OperationMode::MULTIPLE_RANDOM;

void DrawImage(int start_x, int start_y, COLORREF* image_color_array, int image_width, int image_height) {
    
    if (ALLOW_MEMLEAK) {
        image_color_array = GetImageColorArray();
    }

    HBITMAP bitmap = CreateBitmap(
        image_width,
        image_height,
        1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
        32, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
        (void*) image_color_array); // pointer to array
    
    auto hdc = GetDC(NULL);

    // Temp HDC to copy picture
    HDC src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
    SelectObject(src, bitmap); // Inserting picture into our temp HDC
    // Copy image from temp HDC to window
    TransparentBlt(hdc, // Destination
        start_x,  // x and
        start_y,  // y - upper-left corner of place, where we'd like to copy
        image_width, // width of the region
        image_height, // height
        src, // source
        0,   // x and
        0,   // y of upper left corner  of part of the source, from where we'd like to copy
        image_width, // width of the region
        image_height, // height
        (UINT)RGB(0, 0, 255)); // Defined key transparency color
    
    DeleteObject(bitmap);
    DeleteDC(src); // Deleting temp HDC

}

BOOL Monitorenumproc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM monitorsSizesPointerParam) {

    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFOEX);
    
    GetMonitorInfo(hMonitor, &monitorInfo);

    std::vector<RECT>* monitorsSizesPointer = (std::vector<RECT>*) monitorsSizesPointerParam;

    monitorsSizesPointer->push_back(monitorInfo.rcMonitor);

    return true;
}

void GetScreenSizes(std::vector<RECT> &monitorsSizes) {

    EnumDisplayMonitors(NULL, NULL, Monitorenumproc, (LPARAM) &monitorsSizes);

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{

    std::vector<RECT> monitorsSizes;
    GetScreenSizes(monitorsSizes);


    srand(time(NULL));

    int offset_x = rand();
    int offset_y = rand();

    int sleep_time = 1000 / FPS;

    COLORREF* image_pixels = GetImageColorArray();

    while (true) {
        for (RECT monitorSize : monitorsSizes) {

            int screen_width = abs(monitorSize.left - monitorSize.right);
            int screen_height = abs(monitorSize.top - monitorSize.bottom);

            int screen_offset_x = monitorSize.left;
            int screen_offset_y = monitorSize.top;


            int start_x = (screen_width - IMG_WIDTH) / 2;
            int start_y = (screen_height - IMG_HEIGHT) / 2;

            int MAX_X = start_x * 2;
            int MAX_Y = start_y * 2;

            switch (OPERATION_MODE) {
            case OperationMode::SINGLE_CENTER:
                DrawImage(
                    start_x + screen_offset_x,
                    start_y + screen_offset_y,
                    image_pixels,
                    IMG_WIDTH,
                    IMG_HEIGHT
                );
                break;
            case OperationMode::SINGLE_RANDOM:
                DrawImage(
                    (start_x + offset_x) % MAX_X + screen_offset_x,
                    (start_y + offset_y) % MAX_Y + screen_offset_y,
                    image_pixels,
                    IMG_WIDTH,
                    IMG_HEIGHT
                );
                break;
            case OperationMode::MULTIPLE_RANDOM:
                DrawImage(
                    (start_x + offset_x) % MAX_X + screen_offset_x,
                    (start_y + offset_y) % MAX_Y + screen_offset_y,
                    image_pixels,
                    IMG_WIDTH,
                    IMG_HEIGHT
                );
                offset_x = rand();
                offset_y = rand();
                break;
            }
        }
        Sleep(sleep_time);
    }
    
    return 0;
}