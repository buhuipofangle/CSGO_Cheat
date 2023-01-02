#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include<Windows.h>
#include<TlHelp32.h>
using namespace std;


class Hack {
public:
    /// <summary>
    /// 取进程PID
    /// </summary>
    /// <param name="processname">进程名</param>
    /// <returns>进程PID</returns>
    DWORD GetPid(const char* processname) {
        HWND gamehwnd = FindWindowA(nullptr, processname);
        DWORD PID = 0;
        GetWindowThreadProcessId(gamehwnd, &PID);
        return PID;
    }
    /// <summary>
    /// 取进程PID
    /// </summary>
    /// <param name="processname">进程名</param>
    /// <param name="isout">是否将结果打印到控制台</param>
    /// <returns>进程PID</returns>
    DWORD GetPid(const char* processname, bool isout) {
        HWND gamehwnd = FindWindowA(nullptr, processname);
        DWORD PID = 0;
        GetWindowThreadProcessId(gamehwnd, &PID);
        if (isout) {
            cout << "进程ID:" << PID << endl;
            return PID;
        }
        return PID;
    }
    /// <summary>
    /// 取模块地址
    /// </summary>
    /// <param name="PID">进程ID</param>
    /// <param name="modulename">模块名</param>
    /// <returns>模块地址</returns>
    DWORD HackGetModuleHandle(DWORD PID,const char* modulename) {
        HANDLE procnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
        if (INVALID_HANDLE_VALUE== procnap) {
            cout << "快照未能创建" << endl;
        }
        MODULEENTRY32W module_info;
        ZeroMemory(&module_info,sizeof(module_info));
        module_info.dwSize = sizeof(MODULEENTRY32);

        //参数转大写
        char module_name_up[1024];
        ZeroMemory(module_name_up,1024);
        strncpy(module_name_up, modulename, strlen(modulename));
        strupr(module_name_up);

        BOOL state = Module32First(procnap, &module_info);
        char szModuleUp[1024];
        while (state)
        {
            wcstombs(szModuleUp, module_info.szModule,strlen(modulename));
            _strupr(szModuleUp);
            
            if (strncmp(module_name_up,szModuleUp,strlen(modulename))==0) {
                //cout << "模块地址:" << hex << (ULONG32)module_info.modBaseAddr << endl;
                //wcout << "模块名称:" << module_info.szModule << endl;
                CloseHandle(procnap);
                return (DWORD)module_info.modBaseAddr;
            }
            state = Module32Next(procnap, &module_info);
           
        }
        CloseHandle(procnap);
        return 0;
    }

    HANDLE HackOpenProcess(DWORD PID) {
       return OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    }

private:


};


