#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <string>

// "setupapi.lib" la Linker -> Input -> Additional Dependencies

int main()
{
    // Definim GUID-ul pentru dispozitive USB
    static const GUID GUID_USB =
    { 0xA5DCBF10, 0x6530, 0x11D2,
      { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };

    // Obtinem lista de dispozitive USB active
    HDEVINFO listaDispozitive = SetupDiGetClassDevs(
        &GUID_USB,
        nullptr,
        nullptr,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
    );

    if (listaDispozitive == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Eroare la obtinerea dispozitivelor USB.\n";
        return 1;
    }

    SP_DEVINFO_DATA infoDispozitiv;
    infoDispozitiv.cbSize = sizeof(SP_DEVINFO_DATA);

    DWORD deviceIndex = 0;
    int totalDispozitive = 0;

    std::wcout << L"Lista dispozitivelor USB conectate:\n";
    std::wcout << L"-----------------------------------\n";

    while (SetupDiEnumDeviceInfo(listaDispozitive, deviceIndex, &infoDispozitiv))
    {
        deviceIndex++;
        totalDispozitive++;

        TCHAR bufferNumeDispozitiv[1024] = { 0 };

        BOOL descriereGasita = SetupDiGetDeviceRegistryProperty(
            listaDispozitive,
            &infoDispozitiv,
            SPDRP_DEVICEDESC,
            nullptr,
            (PBYTE)bufferNumeDispozitiv,
            sizeof(bufferNumeDispozitiv),
            nullptr
        );

        if (!descriereGasita)
        {
            descriereGasita = SetupDiGetDeviceRegistryProperty(
                listaDispozitive,
                &infoDispozitiv,
                SPDRP_FRIENDLYNAME,
                nullptr,
                (PBYTE)bufferNumeDispozitiv,
                sizeof(bufferNumeDispozitiv),
                nullptr
            );
        }

        std::wcout << L"Dispozitiv USB #" << totalDispozitive << L": ";

        if (descriereGasita)
        {
            std::wcout << bufferNumeDispozitiv << std::endl;
        }
        else
        {
            std::wcout << L"(Descriere indisponibila)\n";
        }
    }

    SetupDiDestroyDeviceInfoList(listaDispozitive);

    std::wcout << L"\nTotal dispozitive detectate: " << totalDispozitive << std::endl;
    std::cout << "\nApasati Enter pentru a inchide...";
    std::cin.get();
    return 0;
}
