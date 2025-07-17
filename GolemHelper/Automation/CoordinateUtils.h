#pragma once

class CoordinateUtils {
public:
    static void GetScaledCoordinates(int baseX, int baseY, int* scaledX, int* scaledY);
    static void DebugMousePosition();
    static void ClickAtScaled(int baseX, int baseY, int delay = 25);
};