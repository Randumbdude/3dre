#pragma once
#include <Windows.h>

namespace input
{
    void handle_key_down(WPARAM key);

    void handle_key_up(WPARAM key);
}