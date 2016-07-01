#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <tchar.h>
#include <Psapi.h>
#include <iostream>
#pragma comment (lib,"Psapi.lib")

BOOL GetProcessList();
BOOL ListProcessModules(DWORD dwPID);
BOOL ListProcessThreads(DWORD dwOwnerPID);
void printError(const char * msg);

BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
    TCHAR			szDriveStr[500];
    TCHAR			szDrive[3];
    TCHAR			szDevName[100];
    INT				cchDevName;
    INT				i;

    //检查参数
    if (!pszDosPath || !pszNtPath)
        return FALSE;

    //获取本地磁盘字符串
    if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
    {
        for (i = 0; szDriveStr[i]; i += 4)
        {
            if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
                continue;

            szDrive[0] = szDriveStr[i];
            szDrive[1] = szDriveStr[i + 1];
            szDrive[2] = '\0';
            if (!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备名
                return FALSE;

            cchDevName = lstrlen(szDevName);
            if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中
            {
                lstrcpy(pszNtPath, szDrive);//复制驱动器
                lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径

                return TRUE;
            }
        }
    }

    lstrcpy(pszNtPath, pszDosPath);

    return FALSE;
}

//获取进程完整路径
BOOL GetProcessFullPath(DWORD dwPID, TCHAR pszFullPath[MAX_PATH])
{
    TCHAR		szImagePath[MAX_PATH];
    HANDLE		hProcess;
    if (!pszFullPath)
        return FALSE;

    pszFullPath[0] = '\0';
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
    if (!hProcess)
        return FALSE;

    if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    if (!DosPathToNtPath(szImagePath, pszFullPath))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    CloseHandle(hProcess);

    _tprintf(_T("%d,%s \r\n"), dwPID, pszFullPath);
    return TRUE;
}


BOOL GetProcessList()
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printError("CreateToolhelp32Snapshot (of processes)");
        return(FALSE);
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        printError("Process32First"); // Show cause of failure
        CloseHandle(hProcessSnap);    // Must clean up the
        //   snapshot object!
        return(FALSE);
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do
    {
        printf("\n\n"
            "=====================================================");
        printf("\nPROCESS NAME:  %s", pe32.szExeFile);
        printf("\n"
            "-----------------------------------------------------");

        // Retrieve the priority class.
        dwPriorityClass = 0;
        hProcess = OpenProcess(
            PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        if (hProcess == NULL)
            printError("OpenProcess");
        else
        {
            dwPriorityClass = GetPriorityClass(hProcess);
            if (!dwPriorityClass)
                printError("GetPriorityClass");
            CloseHandle(hProcess);
        }

        printf("\n  process ID        = 0x%08X", pe32.th32ProcessID);
        printf("\n  thread count      = %d", pe32.cntThreads);
        printf("\n  parent process ID = 0x%08X",
            pe32.th32ParentProcessID);
        printf("\n  Priority Base     = %d", pe32.pcPriClassBase);
        if (dwPriorityClass)
            printf("\n  Priority Class    = %d", dwPriorityClass);

        // List the modules and threads associated with this process
        ListProcessModules(pe32.th32ProcessID);
        ListProcessThreads(pe32.th32ProcessID);

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return(TRUE);
}


BOOL ListProcessModules(DWORD dwPID)
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;

    // Take a snapshot of all modules in the specified process.
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        printError("CreateToolhelp32Snapshot (of modules)");
        return(FALSE);
    }

    // Set the size of the structure before using it.
    me32.dwSize = sizeof(MODULEENTRY32);

    // Retrieve information about the first module,
    // and exit if unsuccessful
    if (!Module32First(hModuleSnap, &me32))
    {
        printError("Module32First"); // Show cause of failure
        CloseHandle(hModuleSnap);    // Must clean up the
        //   snapshot object!
        return(FALSE);
    }

    // Now walk the module list of the process,
    // and display information about each module
    do
    {
        printf("\n\n     MODULE NAME:     %s",
            me32.szModule);
        printf("\n     executable     = %s",
            me32.szExePath);
        printf("\n     process ID     = 0x%08X",
            me32.th32ProcessID);
        printf("\n     ref count (g)  =     0x%04X",
            me32.GlblcntUsage);
        printf("\n     ref count (p)  =     0x%04X",
            me32.ProccntUsage);
        printf("\n     base address   = 0x%08X",
            (DWORD)me32.modBaseAddr);
        printf("\n     base size      = %d",
            me32.modBaseSize);

    } while (Module32Next(hModuleSnap, &me32));

    CloseHandle(hModuleSnap);
    return(TRUE);
}

BOOL ListProcessThreads(DWORD dwOwnerPID)
{
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    // Take a snapshot of all running threads  
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE)
        return(FALSE);

    // Fill in the size of the structure before using it. 
    te32.dwSize = sizeof(THREADENTRY32);

    // Retrieve information about the first thread,
    // and exit if unsuccessful
    if (!Thread32First(hThreadSnap, &te32))
    {
        printError("Thread32First"); // Show cause of failure
        CloseHandle(hThreadSnap);    // Must clean up the
        //   snapshot object!
        return(FALSE);
    }

    // Now walk the thread list of the system,
    // and display information about each thread
    // associated with the specified process
    do
    {
        if (te32.th32OwnerProcessID == dwOwnerPID)
        {
            printf("\n\n     THREAD ID      = 0x%08X",
                te32.th32ThreadID);
            printf("\n     base priority  = %d", te32.tpBasePri);
            printf("\n     delta priority = %d", te32.tpDeltaPri);
        }
    } while (Thread32Next(hThreadSnap, &te32));

    CloseHandle(hThreadSnap);
    return(TRUE);
}

void printError(const char * msg)
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default lang.
        sysMsg, 256, NULL);

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while ((*p > 31) || (*p == 9))
        ++p;
    do { *p-- = 0; } while ((p >= sysMsg) &&
        ((*p == '.') || (*p < 33)));

    // Display the message
    printf("\n  WARNING: %s failed with error %d (%s)",
        msg, eNum, sysMsg);
}


int main(int argc, char* argv[])
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return NULL;
    }
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);

    BOOL fOk;
    for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
    {
        TCHAR szProcessName[MAX_PATH] = { 0 };
        GetProcessFullPath(pe.th32ProcessID, szProcessName);
    }

    //std::cout << "--------------------------------------------------------------\n";
    //GetProcessList();

    return 0;
}
