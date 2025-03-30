#include <windows.h>
#include <iostream>

int main()
{
    std::cout << "Introduceti numarul de pagini N: ";
    int N;
    std::cin >> N;

    //  page size
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    DWORD pageSize = si.dwPageSize;

    std::cout << "Page size este: " << pageSize << " bytes.\n";

    // dimensiunea totala
    SIZE_T totalSize = static_cast<SIZE_T>(N) * pageSize;

    LPVOID p = VirtualAlloc(
        nullptr,           // adresa sugerata (nullptr -> lasam sistemul sa aleaga)
        totalSize,         // dimensiune de alocat (in bytes)
        MEM_RESERVE | MEM_COMMIT, // tipul de alocare
        PAGE_READWRITE     // drepturi de acces
    );

    if (p == nullptr)
    {
        std::cerr << "Eroare la alocarea memoriei cu VirtualAlloc.\n";
        return 1;
    }

    std::cout << "Memorie alocata pentru " << N << " pagini ("
        << totalSize << " bytes).\n";

    if (!VirtualFree(p, 0, MEM_RELEASE))
    {
        std::cerr << "Eroare la eliberarea memoriei cu VirtualFree.\n";
    }
    else
    {
        std::cout << "Memorie eliberata.\n";
    }

    return 0;
}
