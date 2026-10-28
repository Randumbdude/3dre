#include "input.h"

namespace input
{
    void handle_key_down(WPARAM key)
    {
        switch (key)
        {
        case 'A': KEY_A_PRESSED = 1; break;
        case 'B': KEY_B_PRESSED = 1; break;
        case 'C': KEY_C_PRESSED = 1; break;
        case 'D': KEY_D_PRESSED = 1; break;
        case 'E': KEY_E_PRESSED = 1; break;
        case 'F': KEY_F_PRESSED = 1; break;
        case 'G': KEY_G_PRESSED = 1; break;
        case 'H': KEY_H_PRESSED = 1; break;
        case 'I': KEY_I_PRESSED = 1; break;
        case 'J': KEY_J_PRESSED = 1; break;
        case 'K': KEY_K_PRESSED = 1; break;
        case 'L': KEY_L_PRESSED = 1; break;
        case 'M': KEY_M_PRESSED = 1; break;
        case 'N': KEY_N_PRESSED = 1; break;
        case 'O': KEY_O_PRESSED = 1; break;
        case 'P': KEY_P_PRESSED = 1; break;
        case 'Q': KEY_Q_PRESSED = 1; break;
        case 'R': KEY_R_PRESSED = 1; break;
        case 'S': KEY_S_PRESSED = 1; break;
        case 'T': KEY_T_PRESSED = 1; break;
        case 'U': KEY_U_PRESSED = 1; break;
        case 'V': KEY_V_PRESSED = 1; break;
        case 'W': KEY_W_PRESSED = 1; break;
        case 'X': KEY_X_PRESSED = 1; break;
        case 'Y': KEY_Y_PRESSED = 1; break;
        case 'Z': KEY_Z_PRESSED = 1; break;
        }
    }

    void handle_key_up(WPARAM key)
    {
        switch (key)
        {
        case 'A': KEY_A_PRESSED = 0; break;
        case 'B': KEY_B_PRESSED = 0; break;
        case 'C': KEY_C_PRESSED = 0; break;
        case 'D': KEY_D_PRESSED = 0; break;
        case 'E': KEY_E_PRESSED = 0; break;
        case 'F': KEY_F_PRESSED = 0; break;
        case 'G': KEY_G_PRESSED = 0; break;
        case 'H': KEY_H_PRESSED = 0; break;
        case 'I': KEY_I_PRESSED = 0; break;
        case 'J': KEY_J_PRESSED = 0; break;
        case 'K': KEY_K_PRESSED = 0; break;
        case 'L': KEY_L_PRESSED = 0; break;
        case 'M': KEY_M_PRESSED = 0; break;
        case 'N': KEY_N_PRESSED = 0; break;
        case 'O': KEY_O_PRESSED = 0; break;
        case 'P': KEY_P_PRESSED = 0; break;
        case 'Q': KEY_Q_PRESSED = 0; break;
        case 'R': KEY_R_PRESSED = 0; break;
        case 'S': KEY_S_PRESSED = 0; break;
        case 'T': KEY_T_PRESSED = 0; break;
        case 'U': KEY_U_PRESSED = 0; break;
        case 'V': KEY_V_PRESSED = 0; break;
        case 'W': KEY_W_PRESSED = 0; break;
        case 'X': KEY_X_PRESSED = 0; break;
        case 'Y': KEY_Y_PRESSED = 0; break;
        case 'Z': KEY_Z_PRESSED = 0; break;
        }
    }

    void reset_keys()
    {
        KEY_A_PRESSED = KEY_B_PRESSED = KEY_C_PRESSED = KEY_D_PRESSED = 0;
        KEY_E_PRESSED = KEY_F_PRESSED = KEY_G_PRESSED = KEY_H_PRESSED = 0;
        KEY_I_PRESSED = KEY_J_PRESSED = KEY_K_PRESSED = KEY_L_PRESSED = 0;
        KEY_M_PRESSED = KEY_N_PRESSED = KEY_O_PRESSED = KEY_P_PRESSED = 0;
        KEY_Q_PRESSED = KEY_R_PRESSED = KEY_S_PRESSED = KEY_T_PRESSED = 0;
        KEY_U_PRESSED = KEY_V_PRESSED = KEY_W_PRESSED = KEY_X_PRESSED = 0;
        KEY_Y_PRESSED = KEY_Z_PRESSED = 0;
    }
}
