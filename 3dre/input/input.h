#pragma once
#include "3dre.h"
#include "keys.h"

namespace input
{
    void handle_key_down(WPARAM key);

    void handle_key_up(WPARAM key);

    void reset_keys();
};
