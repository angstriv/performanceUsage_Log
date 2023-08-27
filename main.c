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
    //��¼����׼��
    int cpuUsage = 0;
    int memoryUsage = 0;
    char writeMode[4] = "";

    printf("������cpuռ����:");
    scanf("%d", &cpuUsage);
    printf("�������ڴ�ռ����:");
    scanf("%d", &memoryUsage);
    getchar();//����س���ֹ����gets���񵽻س���Ϊ��������������û��޷�����
    while (true)
    {
        printf("������д�뷽ʽ(w/a):");
        gets(writeMode);
        if ((strcmp(writeMode, "w") == 0) || (strcmp(writeMode, "a") == 0))
        {
            break;
        }
        else
        {
            printf("��������\n");
        }
    }
    

    //ϵͳ����ʱ��
    clock_t start, finish;
    long long int duration;

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
        printf("%s\n", buffer);
        
        //����ʱ��
        finish = clock();//��ʱ
        duration = (long long int)(finish - start) / CLOCKS_PER_SEC;
        printf("������%lld��\n", duration);
        
        //CPU������
        U_CHAR  sysStateCpu[5];
        int cpu = (int)CpuUseage();
        printf("CPUʹ���ʣ�%d%%\n", cpu);

        //�ڴ�ʹ����
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        printf("�ڴ�ʹ���ʣ�%ld%%\n", statex.dwMemoryLoad);//u_long
        
        printf("\n");//ÿһ�������Ĵ�ӡ��һ��
    
        if (cpu >= cpuUsage || statex.dwMemoryLoad >= memoryUsage)
        {
            FILE* abnormalLog_p = fopen("abnormalLog.txt",writeMode);//���ļ�
            fprintf(abnormalLog_p, "%s\n", buffer);
            fprintf(abnormalLog_p, "������%lld��\n", duration);
            fprintf(abnormalLog_p, "CPUʹ���ʣ�%d%%\n", cpu);
            fprintf(abnormalLog_p, "�ڴ�ʹ���ʣ�%ld%%\n", statex.dwMemoryLoad);
            fputs("\n", abnormalLog_p);//ÿһ�������Ĵ�ӡ��һ��
            fclose(abnormalLog_p);//�ر��ļ�
        }
    
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