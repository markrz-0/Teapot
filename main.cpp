#include <Windows.h>
#include <ctime>
#include "image_arr.hpp"


void DrawImage(int start_x, int start_y) {
    int img_heigth = IMG_COLOR_ARR_LENGTH / IMG_WIDTH;
    HBITMAP bitmap = CreateBitmap(
        IMG_WIDTH,
        img_heigth,
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
        img_heigth, // height
        src, // source
        0,   // x and
        0,   // y of upper left corner  of part of the source, from where we'd like to copy
        IMG_WIDTH, // width of the region
        img_heigth, // height
        (UINT)RGB(0, 0, 255)); // Defined key transparency color
    
    DeleteObject(bitmap);
    DeleteDC(src); // Deleting temp HDC

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    int img_heigth = IMG_COLOR_ARR_LENGTH / IMG_WIDTH;

    int start_x = (screenWidth - IMG_WIDTH) / 2;
    int start_y = (screenHeight - img_heigth) / 2;

    int MAX_X = start_x * 2;
    int MAX_Y = start_y * 2;

    int offset_x = 0;
    int offset_y = 0;

    srand(time(NULL));

    while(true) {
        DrawImage(
            (start_x + offset_x) % MAX_X,
            (start_y + offset_y) % MAX_Y
        );
        offset_x = rand();
        offset_y = rand();
        Sleep(100);
    }
    
    return 0;
}