#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(void)
{
    HKEY hKey = NULL;
    LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Services"),
        0,
        KEY_READ,
        &hKey);
    if (lRet != ERROR_SUCCESS)
    {
        _tprintf(TEXT("Eroare la deschiderea cheii Services! Cod=%ld\n"), lRet);
        return 1;
    }

    DWORD dwIndex = 0;
    TCHAR szSubKey[256];
    DWORD cchSubKey = 256;
    FILETIME ftLastWrite;

    while (true)
    {
        cchSubKey = _countof(szSubKey);
        LONG enumStatus = RegEnumKeyEx(hKey,
            dwIndex,
            szSubKey,
            &cchSubKey,
            NULL, NULL, NULL,
            &ftLastWrite);
        if (enumStatus == ERROR_NO_MORE_ITEMS)
            break;

        if (enumStatus == ERROR_SUCCESS)
        {
            TCHAR szFullPath[512];
            _stprintf_s(szFullPath, TEXT("SYSTEM\\CurrentControlSet\\Services\\%s"), szSubKey);

            TCHAR szImagePath[512];
            DWORD cbData = sizeof(szImagePath);

            LONG valStatus = RegGetValue(HKEY_LOCAL_MACHINE,
                szFullPath,
                TEXT("ImagePath"),
                RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ,
                NULL,
                szImagePath,
                &cbData);

            if (valStatus == ERROR_SUCCESS)
            {
                _tprintf(TEXT("Subcheie: %s\nImagePath: %s\n\n"), szSubKey, szImagePath);
            }
        }
        ++dwIndex;
    }

    RegCloseKey(hKey);
    return 0;
}
