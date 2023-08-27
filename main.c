#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include<time.h>
#include <tchar.h>
#include "systemUsage.h"

#define DIV 1024
#define WIDTH 7
#define GB(x) (((x).HighPart << 2) + ((DWORD)(x).LowPart) / 1024.0 / 1024.0 / 1024.0)

#pragma warning(disable: 4996)   

#define true 1
typedef unsigned char U_CHAR;



//读取操作系统的名称
void GetSystemName(char* osname)
{
    char buf[128];
    SYSTEM_INFO info;        //用SYSTEM_INFO结构判断64位AMD处理器 
    GetSystemInfo(&info);    //调用GetSystemInfo函数填充结构 
    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    strcpy(buf, "unknown OperatingSystem.");

    if (GetVersionEx((OSVERSIONINFO*)&os))
    {
        //下面根据版本信息判断操作系统名称 
        switch (os.dwMajorVersion)//判断主版本号
        {
        case 4:
            switch (os.dwMinorVersion)//判断次版本号 
            {
            case 0:
                if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
                    strcpy(buf, "Microsoft Windows NT 4.0"); //1996年7月发布 
                else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
                    strcpy(buf, "Microsoft Windows 95");
                break;
            case 10:
                strcpy(buf, "Microsoft Windows 98");
                break;
            case 90:
                strcpy(buf, "Microsoft Windows Me");
                break;
            }
            break;

        case 5:
            switch (os.dwMinorVersion)  //再比较dwMinorVersion的值
            {
            case 0:
                strcpy(buf, "Microsoft Windows 2000"); //1999年12月发布
                break;

            case 1:
                strcpy(buf, "Microsoft Windows XP"); //2001年8月发布
                break;

            case 2:
                if (os.wProductType == VER_NT_WORKSTATION
                    && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                {
                    strcpy(buf, "Microsoft Windows XP Professional x64 Edition");
                }
                else if (GetSystemMetrics(SM_SERVERR2) == 0)
                    strcpy(buf, "Microsoft Windows Server 2003"); //2003年3月发布 
                else if (GetSystemMetrics(SM_SERVERR2) != 0)
                    strcpy(buf, "Microsoft Windows Server 2003 R2");
                break;
            }
            break;

        case 6:
            switch (os.dwMinorVersion)
            {
            case 0:
                if (os.wProductType == VER_NT_WORKSTATION)
                    strcpy(buf, "Microsoft Windows Vista");
                else
                    strcpy(buf, "Microsoft Windows Server 2008"); //服务器版本 
                break;
            case 1:
                if (os.wProductType == VER_NT_WORKSTATION)
                    strcpy(buf, "Microsoft Windows 7");
                else
                    strcpy(buf, "Microsoft Windows Server 2008 R2");
                break;
            }
            break;
        }
    }//if(GetVersionEx((OSVERSIONINFO *)&os))

    strcpy(osname, buf);
}


int main() 
{
    //记录数据准备
    int cpuUsage = 0;
    int memoryUsage = 0;
    char writeMode[4] = "";

    printf("请输入cpu占用率:");
    scanf("%d", &cpuUsage);
    printf("请输入内存占用率:");
    scanf("%d", &memoryUsage);
    getchar();//捕获回车防止下面gets捕获到回车以为输入结束，导致用户无法输入
    while (true)
    {
        printf("请输入写入方式(w/a):");
        gets(writeMode);
        if ((strcmp(writeMode, "w") == 0) || (strcmp(writeMode, "a") == 0))
        {
            break;
        }
        else
        {
            printf("输入有误\n");
        }
    }
    

    //系统运行时间
    clock_t start, finish;
    long long int duration;

    start = clock();//开始时间

    //系统版本名称
    char osname[20];
    GetSystemName(osname);
    printf("%s\n", osname);

    printf("\n");

    //CPU利用率
    while (true)
    {
        Sleep(1000);//一秒算一次

        //获取系统时间
        char buffer[128];
        getSystemTime(buffer);
        printf("%s\n", buffer);
        
        //运行时间
        finish = clock();//计时
        duration = (long long int)(finish - start) / CLOCKS_PER_SEC;
        printf("已运行%lld秒\n", duration);
        
        //CPU利用率
        U_CHAR  sysStateCpu[5];
        int cpu = (int)CpuUseage();
        printf("CPU使用率：%d%%\n", cpu);

        //内存使用率
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        printf("内存使用率：%ld%%\n", statex.dwMemoryLoad);//u_long
        
        printf("\n");//每一次完整的打印空一行
    
        if (cpu >= cpuUsage || statex.dwMemoryLoad >= memoryUsage)
        {
            FILE* abnormalLog_p = fopen("abnormalLog.txt",writeMode);//打开文件
            fprintf(abnormalLog_p, "%s\n", buffer);
            fprintf(abnormalLog_p, "已运行%lld秒\n", duration);
            fprintf(abnormalLog_p, "CPU使用率：%d%%\n", cpu);
            fprintf(abnormalLog_p, "内存使用率：%ld%%\n", statex.dwMemoryLoad);
            fputs("\n", abnormalLog_p);//每一次完整的打印空一行
            fclose(abnormalLog_p);//关闭文件
        }
    
    }

    
    // {
    //    //硬盘使用率 调用windows API
    //    ULARGE_INTEGER FreeBytesAvailableC, TotalNumberOfBytesC, TotalNumberOfFreeBytesC;
    //    ULARGE_INTEGER FreeBytesAvailableD, TotalNumberOfBytesD, TotalNumberOfFreeBytesD;
    //    ULARGE_INTEGER FreeBytesAvailableE, TotalNumberOfBytesE, TotalNumberOfFreeBytesE;
    //    ULARGE_INTEGER FreeBytesAvailableF, TotalNumberOfBytesF, TotalNumberOfFreeBytesF;

    //    GetDiskFreeSpaceEx(_T("C:"), &FreeBytesAvailableC, &TotalNumberOfBytesC, &TotalNumberOfFreeBytesC);
    //    GetDiskFreeSpaceEx(_T("D:"), &FreeBytesAvailableD, &TotalNumberOfBytesD, &TotalNumberOfFreeBytesD);
    //    GetDiskFreeSpaceEx(_T("E:"), &FreeBytesAvailableE, &TotalNumberOfBytesE, &TotalNumberOfFreeBytesE);
    //    GetDiskFreeSpaceEx(_T("F:"), &FreeBytesAvailableF, &TotalNumberOfBytesF, &TotalNumberOfFreeBytesF);
    //    /*参数 类型及说明
    //    lpRootPathName String，不包括卷名的磁盘根路径名
    //    lpFreeBytesAvailableToCaller LARGE_INTEGER，指定一个变量，用于容纳调用者可用的字节数量
    //    lpTotalNumberOfBytes LARGE_INTEGER，指定一个变量，用于容纳磁盘上的总字节数
    //    lpTotalNumberOfFreeBytes LARGE_INTEGER，指定一个变量，用于容纳磁盘上可用的字节数
    //    适用平台
    //    Windows 95 OSR2，Windows NT 4.0*/
    //    float totalHardDisk = GB(TotalNumberOfBytesC) + GB(TotalNumberOfBytesD) + GB(TotalNumberOfBytesE) + GB(TotalNumberOfBytesF);
    //    float freeHardDisk = GB(TotalNumberOfFreeBytesC) + GB(TotalNumberOfFreeBytesD) + GB(TotalNumberOfFreeBytesE) + GB(TotalNumberOfFreeBytesF);

    //    float hardDiskUsage = 1 - freeHardDisk / totalHardDisk;
    //}本程序不需要该功能，因此暂时屏蔽


    system("pause");

}