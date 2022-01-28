#include "win_service.h"
//#include <synchapi.h>
//
//VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
//{
//    DWORD Status = E_FAIL;
//
//    // Register our service control handler with the SCM
//    g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
//
//    if (g_StatusHandle == NULL)
//    {
//        goto EXIT;
//    }
//
//    // Tell the service controller we are starting
//    ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
//    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
//    g_ServiceStatus.dwControlsAccepted = 0;
//    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
//    g_ServiceStatus.dwWin32ExitCode = 0;
//    g_ServiceStatus.dwServiceSpecificExitCode = 0;
//    g_ServiceStatus.dwCheckPoint = 0;
//
//    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
//    {
//        OutputDebugString(_T(
//            "My Sample Service: ServiceMain: SetServiceStatus returned error"));
//    }
//
//    /*
//     * Perform tasks necessary to start the service here
//     */
//
//     // Create a service stop event to wait on later
//    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//    if (g_ServiceStopEvent == NULL)
//    {
//        // Error creating event
//        // Tell service controller we are stopped and exit
//        g_ServiceStatus.dwControlsAccepted = 0;
//        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
//        g_ServiceStatus.dwWin32ExitCode = GetLastError();
//        g_ServiceStatus.dwCheckPoint = 1;
//
//        if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
//        {
//            OutputDebugString(_T(
//                "My Sample Service: ServiceMain: SetServiceStatus returned error"));
//        }
//        goto EXIT;
//    }
//
//    // Tell the service controller we are started
//    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
//    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
//    g_ServiceStatus.dwWin32ExitCode = 0;
//    g_ServiceStatus.dwCheckPoint = 0;
//
//    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
//    {
//        OutputDebugString(_T(
//            "My Sample Service: ServiceMain: SetServiceStatus returned error"));
//    }
//
//    // Start a thread that will perform the main task of the service
//    HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);
//
//    // Wait until our worker thread exits signaling that the service needs to stop
//    WaitForSingleObject(hThread, INFINITE);
//
//
//    /*
//     * Perform any cleanup tasks
//     */
//
//    CloseHandle(g_ServiceStopEvent);
//
//    // Tell the service controller we are stopped
//    g_ServiceStatus.dwControlsAccepted = 0;
//    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
//    g_ServiceStatus.dwWin32ExitCode = 0;
//    g_ServiceStatus.dwCheckPoint = 3;
//
//    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
//    {
//        OutputDebugString(_T(
//            "My Sample Service: ServiceMain: SetServiceStatus returned error"));
//    }
//
//EXIT:
//    return;
//}
//
//VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode)
//{
//    switch (CtrlCode)
//    {
//    case SERVICE_CONTROL_STOP:
//
//        if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
//            break;
//
//        /*
//         * Perform tasks necessary to stop the service here
//         */
//        std::cout << "Service run";
//
//        g_ServiceStatus.dwControlsAccepted = 0;
//        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
//        g_ServiceStatus.dwWin32ExitCode = 0;
//        g_ServiceStatus.dwCheckPoint = 4;
//
//        if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
//        {
//            OutputDebugString(_T(
//                "My Sample Service: ServiceCtrlHandler: SetServiceStatus returned error"));
//        }
//
//        // This will signal the worker thread to start shutting down
//        SetEvent(g_ServiceStopEvent);
//
//        break;
//
//    default:
//        break;
//    }
//}
//
//DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
//{
//    //  Periodically check if the service has been requested to stop
//    while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
//    {
//        /*
//         * Perform main service function here
//         */
//
//         //  Simulate some work by sleeping
//        Sleep(300);
//    }
//
//    return ERROR_SUCCESS;
//}