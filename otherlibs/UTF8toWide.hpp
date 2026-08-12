#ifndef __INC_UTF8_TO_WIDE__
#define __INC_UTF8_TO_WIDE__

#include <Windows.h>
#include <string>
#include <exception>
#include <stdexcept>

/**
 * @brief 将UTF8字符串转换为宽字符串
 * @param utf8 输入的UTF8字符串
 * @return 转换后的宽字符串
 * @throw std::runtime_error 转换失败时抛出
 */
std::wstring utf8ToWide(const std::string &utf8)
{
    if (utf8.empty())
        return L"";

    int wideSize = MultiByteToWideChar(
        CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()),
        nullptr, 0);
    if (wideSize == 0)
    {
        throw std::runtime_error("UTF8 to wide conversion failed: " +
                                 std::to_string(GetLastError()));
    }

    std::wstring wide(wideSize, L'\0');
    if (MultiByteToWideChar(
            CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()),
            &wide[0], wideSize) == 0)
    {
        throw std::runtime_error("UTF8 to wide conversion failed: " +
                                 std::to_string(GetLastError()));
    }

    return wide;
}

/**
 * @brief 将宽字符串转换为UTF8字符串
 * @param wide 输入的宽字符串
 * @return 转换后的UTF8字符串
 * @throw std::runtime_error 转换失败时抛出
 */
std::string wideToUtf8(const std::wstring &wide)
{
    if (wide.empty())
        return "";

    int utf8Size = WideCharToMultiByte(
        CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()),
        nullptr, 0, nullptr, nullptr);
    if (utf8Size == 0)
    {
        throw std::runtime_error("Wide to UTF8 conversion failed: " +
                                 std::to_string(GetLastError()));
    }

    std::string utf8(utf8Size, '\0');
    if (WideCharToMultiByte(
            CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()),
            &utf8[0], utf8Size, nullptr, nullptr) == 0)
    {
        throw std::runtime_error("Wide to UTF8 conversion failed: " +
                                 std::to_string(GetLastError()));
    }

    return utf8;
}

#endif
