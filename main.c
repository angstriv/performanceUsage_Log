#include <stdio.h>
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



//��ȡ����ϵͳ������
void GetSystemName(char* osname)
{
    char buf[128];
    SYSTEM_INFO info;        //��SYSTEM_INFO�ṹ�ж�64λAMD������ 
    GetSystemInfo(&info);    //����GetSystemInfo�������ṹ 
    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    strcpy(buf, "unknown OperatingSystem.");

    if (GetVersionEx((OSVERSIONINFO*)&os))
    {
        //������ݰ汾��Ϣ�жϲ���ϵͳ���� 
        switch (os.dwMajorVersion)//�ж����汾��
        {
        case 4:
            switch (os.dwMinorVersion)//�жϴΰ汾�� 
            {
            case 0:
                if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
                    strcpy(buf, "Microsoft Windows NT 4.0"); //1996��7�·��� 
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
            switch (os.dwMinorVersion)  //�ٱȽ�dwMinorVersion��ֵ
            {
            case 0:
                strcpy(buf, "Microsoft Windows 2000"); //1999��12�·���
                break;

            case 1:
                strcpy(buf, "Microsoft Windows XP"); //2001��8�·���
                break;

            case 2:
                if (os.wProductType == VER_NT_WORKSTATION
                    && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                {
                    strcpy(buf, "Microsoft Windows XP Professional x64 Edition");
                }
                else if (GetSystemMetrics(SM_SERVERR2) == 0)
                    strcpy(buf, "Microsoft Windows Server 2003"); //2003��3�·��� 
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
                    strcpy(buf, "Microsoft Windows Server 2008"); //�������汾 
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
    //ϵͳ����ʱ��
    clock_t start, finish;
    double duration;

    start = clock();//��ʼʱ��

    //ϵͳ�汾����
    char osname[20];
    GetSystemName(osname);
    printf("%s\n", osname);

    printf("\n");

    //CPU������
    while (true)
    {
        Sleep(1000);//һ����һ��

        //��ȡϵͳʱ��
        char buffer[128];
        getSystemTime(buffer);
        puts(buffer);
        
        //����ʱ��
        finish = clock();//��ʱ
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        printf("������%.0f ��\n", duration);
        
        //CPU������
        U_CHAR  sysStateCpu[5];
        double cpu = CpuUseage();
        sprintf((char*)sysStateCpu, "%.2lf", cpu);
        printf("CPUʹ���ʣ�%s%%\n", sysStateCpu);

        //�ڴ�ʹ����
        char bufPreCPU[5];
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        sprintf(bufPreCPU, "�ڴ�ʹ���ʣ�%ld%%\n", statex.dwMemoryLoad);
        puts(bufPreCPU);
    }

    
    // {
    //    //Ӳ��ʹ���� ����windows API
    //    ULARGE_INTEGER FreeBytesAvailableC, TotalNumberOfBytesC, TotalNumberOfFreeBytesC;
    //    ULARGE_INTEGER FreeBytesAvailableD, TotalNumberOfBytesD, TotalNumberOfFreeBytesD;
    //    ULARGE_INTEGER FreeBytesAvailableE, TotalNumberOfBytesE, TotalNumberOfFreeBytesE;
    //    ULARGE_INTEGER FreeBytesAvailableF, TotalNumberOfBytesF, TotalNumberOfFreeBytesF;

    //    GetDiskFreeSpaceEx(_T("C:"), &FreeBytesAvailableC, &TotalNumberOfBytesC, &TotalNumberOfFreeBytesC);
    //    GetDiskFreeSpaceEx(_T("D:"), &FreeBytesAvailableD, &TotalNumberOfBytesD, &TotalNumberOfFreeBytesD);
    //    GetDiskFreeSpaceEx(_T("E:"), &FreeBytesAvailableE, &TotalNumberOfBytesE, &TotalNumberOfFreeBytesE);
    //    GetDiskFreeSpaceEx(_T("F:"), &FreeBytesAvailableF, &TotalNumberOfBytesF, &TotalNumberOfFreeBytesF);
    //    /*���� ���ͼ�˵��
    //    lpRootPathName String�������������Ĵ��̸�·����
    //    lpFreeBytesAvailableToCaller LARGE_INTEGER��ָ��һ���������������ɵ����߿��õ��ֽ�����
    //    lpTotalNumberOfBytes LARGE_INTEGER��ָ��һ���������������ɴ����ϵ����ֽ���
    //    lpTotalNumberOfFreeBytes LARGE_INTEGER��ָ��һ���������������ɴ����Ͽ��õ��ֽ���
    //    ����ƽ̨
    //    Windows 95 OSR2��Windows NT 4.0*/
    //    float totalHardDisk = GB(TotalNumberOfBytesC) + GB(TotalNumberOfBytesD) + GB(TotalNumberOfBytesE) + GB(TotalNumberOfBytesF);
    //    float freeHardDisk = GB(TotalNumberOfFreeBytesC) + GB(TotalNumberOfFreeBytesD) + GB(TotalNumberOfFreeBytesE) + GB(TotalNumberOfFreeBytesF);

    //    float hardDiskUsage = 1 - freeHardDisk / totalHardDisk;
    //}��������Ҫ�ù��ܣ������ʱ����



    system("pause");

}