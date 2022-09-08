#include"utils.h"


namespace Utils
{
    BOOL getHWND(HWND hwnd, LPARAM lParam)
    {
        DWORD lpdwProcessId;
        GetWindowThreadProcessId(hwnd, &lpdwProcessId);
        auto wi = reinterpret_cast<WindowInfo*>(lParam);
        if (lpdwProcessId == wi->prcoessId)
        {
            wi->hwnd = hwnd;
            return FALSE;
        }
        return TRUE;

    }

    std::string genreateString(int length)
    {
        auto randchar = []() -> char
        {
            const char charset[] =
                "0123456789"
                "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[rand() % max_index];
        };
        std::string str(length, 0);
        str[0] = 'f';
        std::generate_n(str.begin() + 1, length - 1, randchar);
        return str;
    }


}