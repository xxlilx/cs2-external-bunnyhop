#include "include.h"

void bhop()
{
    static bool jump = false;
    if (!jump)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        cs2.WriteMemory<int>(Offsets::client + Offsets::dwForceJump, 65537);
        jump = true;
    }
    if (jump)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        cs2.WriteMemory<int>(Offsets::client + Offsets::dwForceJump, 256);
        jump = false;
    }
}

int main()
{
	while (true)
	{
        if (GetAsyncKeyState(VK_SPACE)) {
            bhop();
        }
	}

	return 0;
}