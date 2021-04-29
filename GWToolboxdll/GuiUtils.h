#pragma once

#include <ImGuiAddons.h>

namespace GuiUtils {
    enum class FontSize {
        text,
        header2,
        header1,
        widget_label,
        widget_small,
        widget_large
    };
    void LoadFonts();
    bool FontsLoaded();
    ImFont* GetFont(FontSize size);

    float GetPartyHealthbarHeight();
    float GetGWScaleMultiplier();

    // Reposition a rect within its container to make sure it isn't overflowing it.
    ImVec4& ClampRect(ImVec4& rect, ImVec4& viewport);

    std::string ToLower(std::string s);
    std::wstring ToLower(std::wstring s);
    std::string UrlEncode(std::string str);
    std::wstring RemovePunctuation(std::wstring s);
    std::string RemovePunctuation(std::string s);

    std::string WStringToString(const std::wstring& s);
    std::wstring StringToWString(const std::string& s);

    std::wstring SanitizePlayerName(std::wstring in);

    bool ParseInt(const char *str, int *val, int base = 0);
    bool ParseInt(const wchar_t *str, int *val, int base = 0);

    bool ParseUInt(const char *str, unsigned int *val, int base = 0);
    bool ParseUInt(const wchar_t *str, unsigned int *val, int base = 0);

    bool ParseFloat(const char *str, float *val);
    bool ParseFloat(const wchar_t *str, float *val);

    char *StrCopy(char *dest, const char *src, size_t dest_size);

    size_t wcstostr(char* dest, const wchar_t* src, size_t n);
    std::wstring ToWstr(std::string& str);

};
