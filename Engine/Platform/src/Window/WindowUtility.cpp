
#include "WindowUtility.hpp"

#ifdef _WIN32
#define _WIN32_WINNT 0x0603
#include <shellscalingapi.h>
#include <windows.h>
#pragma comment(lib, "Shcore.lib")
#elif defined(__APPLE__)
#include <CoreGraphics/CoreGraphics.h>
#endif

float MCEngine::WindowUtility::GetDPIScale()
{
#ifdef _WIN32
    HMONITOR monitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONEAREST);
    UINT dpiX = 96, dpiY = 96;
    HRESULT hr = GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    if (FAILED(hr))
    {
        dpiX = dpiY = 96;
    }
    return dpiX / 96.0f;

#elif defined(__APPLE__)
    CGDirectDisplayID displayID = CGMainDisplayID();
    CGSize displaySize = CGDisplayScreenSize(displayID);
    size_t pixelWidth = CGDisplayPixelsWide(displayID);
    float scaleX = (float)pixelWidth / (displaySize.width / 25.4f * 72.0f);
    return scaleX;

#else // Linux
    return 1.0f;
#endif
}

std::pair<int, int> MCEngine::WindowUtility::GetScreenResolution()
{
#ifdef _WIN32
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    return {width, height};

#elif defined(__APPLE__)
    CGDirectDisplayID displayID = CGMainDisplayID();
    size_t width = CGDisplayPixelsWide(displayID);
    size_t height = CGDisplayPixelsHigh(displayID);
    return {static_cast<int>(width), static_cast<int>(height)};

#elif defined(__linux__)
    Display *d = XOpenDisplay(NULL);
    if (!d)
        return {0, 0};
    Screen *s = DefaultScreenOfDisplay(d);
    int width = s->width;
    int height = s->height;
    XCloseDisplay(d);
    return {width, height};

#else
    return {0, 0};
#endif
}

int MCEngine::WindowUtility::GetScreenResolutionScale()
{
    auto &&[width, height] = GetScreenResolution();
    return width / 1280;
}