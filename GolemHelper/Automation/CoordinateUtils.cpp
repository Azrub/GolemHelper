#include <Windows.h>
#include <string>
#include "CoordinateUtils.h"
#include "../Common/Globals.h"

void CoordinateUtils::GetScaledCoordinates(int baseX, int baseY, int* scaledX, int* scaledY) {
    if (!g_api) return;

    g_api->Log(ELogLevel_INFO, "GolemHelper", "GetScaledCoordinates CALLED");

    if (g_nexusLink && g_nexusLink->Width > 0 && g_nexusLink->Height > 0) {
        float uiScale = g_nexusLink->Scaling;
        float dpiScaleX, dpiScaleY;

        // Ultrawide 5120x1440
        if (g_nexusLink->Width == 5120 && g_nexusLink->Height == 1440) {
            if (uiScale >= 0.89f && uiScale <= 0.91f) {
                dpiScaleX = 2.814f;
                dpiScaleY = 0.888f;
                g_api->Log(ELogLevel_INFO, "GolemHelper", "ULTRAWIDE 5120x1440: APPLIED SMALL UI OFFSET");
            }
            else if (uiScale >= 1.09f && uiScale <= 1.15f) {
                dpiScaleX = 2.746f;
                dpiScaleY = 1.104f;
                g_api->Log(ELogLevel_INFO, "GolemHelper", "ULTRAWIDE 5120x1440: APPLIED LARGE UI OFFSET");
            }
            else if (uiScale >= 1.21f && uiScale <= 1.25f) {
                dpiScaleX = 2.713f;
                dpiScaleY = 1.208f;
                g_api->Log(ELogLevel_INFO, "GolemHelper", "ULTRAWIDE 5120x1440: APPLIED LARGER UI OFFSET");
            }
            else {
                dpiScaleX = 2.779f;
                dpiScaleY = 1.000f;
                g_api->Log(ELogLevel_INFO, "GolemHelper", "ULTRAWIDE 5120x1440: APPLIED NORMAL UI OFFSET");
            }
        }
        else {
            dpiScaleX = (float)g_nexusLink->Width / 1920.0f;
            dpiScaleY = 1.0f;

            char normalBuffer[150];
            sprintf_s(normalBuffer, "NORMAL MONITOR: Width=%d, Height=%d, scaleX=%.3f",
                g_nexusLink->Width, g_nexusLink->Height, dpiScaleX);
            g_api->Log(ELogLevel_INFO, "GolemHelper", normalBuffer);
        }

        char valuesBuffer[250];
        sprintf_s(valuesBuffer, "GetScaled INPUT: uiScale=%.3f, scaleX=%.3f, scaleY=%.3f, base=%d,%d",
            uiScale, dpiScaleX, dpiScaleY, baseX, baseY);
        g_api->Log(ELogLevel_INFO, "GolemHelper", valuesBuffer);

        int scaledForResolutionX = (int)(baseX * dpiScaleX);
        int scaledForResolutionY = (int)(baseY * dpiScaleY);

        int finalX = scaledForResolutionX;
        int finalY = scaledForResolutionY;

        if (!(g_nexusLink->Width == 5120 && g_nexusLink->Height == 1440)) {
            if (uiScale >= 0.89f && uiScale <= 0.91f) {
                finalX = scaledForResolutionX - (int)(scaledForResolutionX * 0.029f);
                finalY = scaledForResolutionY - (int)(scaledForResolutionY * 0.103f);
                g_api->Log(ELogLevel_INFO, "GolemHelper", "APPLIED SMALL UI OFFSET");
            }
            else if (uiScale >= 1.09f && uiScale <= 1.15f) {
                finalX = scaledForResolutionX - (int)(scaledForResolutionX * 0.053f);
                finalY = scaledForResolutionY + (int)(scaledForResolutionY * 0.095f);
                g_api->Log(ELogLevel_INFO, "GolemHelper", "APPLIED LARGE UI OFFSET");
            }
            else if (uiScale >= 1.21f && uiScale <= 1.25f) {
                finalX = scaledForResolutionX - (int)(scaledForResolutionX * 0.097f);
                finalY = scaledForResolutionY + (int)(scaledForResolutionY * 0.206f);
                g_api->Log(ELogLevel_INFO, "GolemHelper", "APPLIED LARGER UI OFFSET");
            }
            else {
                char buffer[100];
                sprintf_s(buffer, "NO UI OFFSET - uiScale %.3f", uiScale);
                g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
            }
        }

        *scaledX = finalX;
        *scaledY = finalY;

        char resultBuffer[250];
        sprintf_s(resultBuffer, "GetScaled RESULT: base=%d,%d -> scaled=%d,%d -> final=%d,%d",
            baseX, baseY, scaledForResolutionX, scaledForResolutionY, finalX, finalY);
        g_api->Log(ELogLevel_INFO, "GolemHelper", resultBuffer);
    }
    else {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "GetScaledCoordinates - Nexus data not available");

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        float dpiScale = (float)screenWidth / 1920.0f;

        *scaledX = (int)(baseX * dpiScale);
        *scaledY = baseY;
    }
}

void CoordinateUtils::DebugMousePosition() {
    if (!g_api) return;

    POINT mousePos;
    GetCursorPos(&mousePos);

    if (g_nexusLink && g_nexusLink->Width > 0 && g_nexusLink->Height > 0) {
        float uiScale = g_nexusLink->Scaling;
        float dpiScale = (float)g_nexusLink->Width / 1920.0f;
        float finalScaleX = uiScale * dpiScale;

        int baseX = (int)(mousePos.x / finalScaleX);
        int baseY = mousePos.y;

        g_state.debugCounter++;

        char buffer[450];
        sprintf_s(buffer, "=== DEBUG #%d === Resolution: %dx%d | Mouse: %d,%d | Base coords: %d,%d | Interface Size: %.2f | DPI Scale: %.3f | Final ScaleX: %.3f",
            g_state.debugCounter, g_nexusLink->Width, g_nexusLink->Height,
            mousePos.x, mousePos.y, baseX, baseY, uiScale, dpiScale, finalScaleX);
        g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
    }
    else {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Cannot debug - Nexus data not available");
    }
}

void CoordinateUtils::ClickAtScaled(int baseX, int baseY, int delay) {
    HWND gameWindow = GetForegroundWindow();
    if (!gameWindow) return;

    int scaledX, scaledY;
    GetScaledCoordinates(baseX, baseY, &scaledX, &scaledY);

    LPARAM lParam = MAKELPARAM(scaledX, scaledY);
    SendMessage(gameWindow, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    Sleep(10);
    SendMessage(gameWindow, WM_LBUTTONUP, 0, lParam);
    Sleep(delay);
}