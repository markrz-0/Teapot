#include <Windows.h>
#include <ctime>
#include "image_arr.hpp" 

enum OperationMode {
    SINGLE_CENTER, // single centered image
    SINGLE_RANDOM, // single random location image (location decided at exe startup)
    MULTIPLE_RANDOM // new image at new location every frame
};

// used for calculation where screen center is
constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

constexpr int FPS = 15; // how many times a second should image be redrawn; higher values impact pc performance

constexpr int OPERATION_MODE = OperationMode::MULTIPLE_RANDOM;


void DrawImage(int start_x, int start_y) {
    HBITMAP bitmap = CreateBitmap(
        IMG_WIDTH,
        IMG_HEIGHT,
        1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
        32, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
        (void*) IMG_COLOR_ARR); // pointer to array
    
    auto hdc = GetDC(NULL);

    // Temp HDC to copy picture
    HDC src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
    SelectObject(src, bitmap); // Inserting picture into our temp HDC
    // Copy image from temp HDC to window
    TransparentBlt(hdc, // Destination
        start_x,  // x and
        start_y,  // y - upper-left corner of place, where we'd like to copy
        IMG_WIDTH, // width of the region
        IMG_HEIGHT, // height
        src, // source
        0,   // x and
        0,   // y of upper left corner  of part of the source, from where we'd like to copy
        IMG_WIDTH, // width of the region
        IMG_HEIGHT, // height
        (UINT)RGB(0, 0, 255)); // Defined key transparency color
    
    DeleteObject(bitmap);
    DeleteDC(src); // Deleting temp HDC

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{

    int start_x = (SCREEN_WIDTH - IMG_WIDTH) / 2;
    int start_y = (SCREEN_HEIGHT - IMG_HEIGHT) / 2;

    int MAX_X = start_x * 2;
    int MAX_Y = start_y * 2;

    srand(time(NULL));

    int offset_x = rand();
    int offset_y = rand();

    int sleep_time = 1000 / FPS;

    while(true) {
        switch (OPERATION_MODE) {
        case OperationMode::SINGLE_CENTER:
            DrawImage(start_x, start_y);
            break;
        case OperationMode::SINGLE_RANDOM:
            DrawImage(
                (start_x + offset_x) % MAX_X,
                (start_y + offset_y) % MAX_Y
            );
            break;
        case OperationMode::MULTIPLE_RANDOM:
            DrawImage(
                (start_x + offset_x) % MAX_X,
                (start_y + offset_y) % MAX_Y
            );
            offset_x = rand();
            offset_y = rand();
            break;
        }
        Sleep(sleep_time);
    }
    
    return 0;
}