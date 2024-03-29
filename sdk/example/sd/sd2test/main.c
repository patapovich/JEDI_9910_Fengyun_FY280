/*
 * Copyright (c) 2007 SMedia Technology Corp. All Rights Reserved.
 */

#include "mmp_dma.h"
#include "mmp_sd.h"
#include "stdio.h"
#include "common/fat.h"
#include "common/common.h"
#include "chkdsk.h"
#include "config.h"
#include "pal/timer.h"
#include "host/host.h"

#if defined(__FREERTOS__)
#include "FreeRTOS.h"
#include "task.h"
#endif


#if defined(__FREERTOS__)
xTaskHandle main_task_handle = NULL;
portTASK_FUNCTION_PROTO(main_task_func, params);
#endif

//#define TEST_PERFORMANCE
#define TEST_2CARD_RW_FILE
//#define TEST_LOW_LEVEL

extern void f_dotest(void);
extern F_DRIVER *sd_initfunc(unsigned long driver_param);
extern F_DRIVER *sd2_initfunc(unsigned long driver_param);

int _f_poweron(void) 
{
    int ret;
    ret=f_initvolume(0, sd_initfunc, F_AUTO_ASSIGN);
    if(ret)
        return ret;
    ret=f_initvolume(1, sd2_initfunc, F_AUTO_ASSIGN);

    return ret;
}

int _f_poweroff(void) 
{
    int ret;

    ret = f_delvolume(0);
    ret = f_delvolume(1);

    return ret; 
}

void _f_dump(char *s) {
    printf("%s\n", s);
}

long _f_result(long testnum,long result) {
    printf("test number %d failed with error %d\n", testnum, result);
    return(testnum);
}


void dir_test(void) {
    FN_FIND find;
    if (!f_findfirst("A:/*.mp3",&find)) {
        do {
            printf ("filename:%s",find.filename);
            if (find.attr&F_ATTR_DIR) {
                printf (" directory\n");
            }
            else {
                printf (" size %d\n",find.filesize);
            }
        } while (!f_findnext(&find));
    }
}

void TestFS(void)
{
    MMP_INT result = 0;

    LOG_INFO "Start File System Test!!\n" LOG_END
    f_init();
    f_enterFS();
    result = _f_poweron();
    if(result)
    {
        LOG_ERROR "_f_poweron() return error code 0x%08X \n", result LOG_END
    }
    if(f_chdrive(0) != F_NO_ERROR)
    {
        printf("ERROR: Change drive failed.\n");
        return;
    }
    f_dotest();
    LOG_INFO "End File System Test!!\n" LOG_END
}

void TestPerformance(void)
{
#define TEST_SIZE   2*1024*1024

    MMP_INT result = 0;
    MMP_ULONG startTime = 0;
    MMP_ULONG duration = 0;
    MMP_INT i;
    static MMP_UINT8 buffer[256*512];

    result = mmpSdInitialize();
    if(result)
        goto end;

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*1)); i++)
    {
        result = mmpSdReadMultipleSector(512, 1, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO " Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*2)); i++)
    {
        result = mmpSdReadMultipleSector(512, 2, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "2 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*4)); i++)
    {
        result = mmpSdReadMultipleSector(512, 4, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "4 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*8)); i++)
    {
        result = mmpSdReadMultipleSector(512, 8, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "8 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*16)); i++)
    {
        result = mmpSdReadMultipleSector(512, 16, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "16 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*32)); i++)
    {
        result = mmpSdReadMultipleSector(512, 32, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "32 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*64)); i++)
    {
        result = mmpSdReadMultipleSector(512, 64, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "64 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*128)); i++)
    {
        result = mmpSdReadMultipleSector(512, 128, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "128 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

    startTime = PalGetClock();
    for(i=0; i<(TEST_SIZE/(512*256)); i++)
    {
        result = mmpSdReadMultipleSector(512, 256, buffer);
        if(result)
            goto end;
    }
    duration = PalGetDuration(startTime);
    LOG_INFO "256 Read rate: %f K bytes/sec, startTime = %d, duration = %d \n", (((double)(TEST_SIZE/1024))/duration)*1000, startTime, duration LOG_END

end:
    mmpSdTerminate();
    while(1);
}

void TestReadWriteFile(void)
{
#define BUFSIZE        (256*1024)
#define FILE_SIZE_MB   20
#define FILE_SIZE_BYTE (FILE_SIZE_MB * BUFSIZE)

    MMP_INT result = 0;
    FN_FILE* file1 = MMP_NULL;
    FN_FILE* file2 = MMP_NULL;
    MMP_UINT8* bufferW1 = MMP_NULL;
    MMP_UINT8* bufferW2 = MMP_NULL;
    MMP_UINT8* bufferR1 = MMP_NULL;
    MMP_UINT8* bufferR2 = MMP_NULL;
    MMP_UINT i = 0;
    MMP_UINT diff = 0;

    LOG_INFO "\n" LOG_END
    LOG_INFO "Start R/W Test!!\n" LOG_END

    f_init();
	f_enterFS();

    result = _f_poweron();
    if(result)
    {
        LOG_ERROR "_f_poweron() return error code 0x%08X \n", result LOG_END
        while(1);
        goto end;
    }

    bufferW1 = (MMP_UINT8*)malloc(BUFSIZE);
    if(!bufferW1)
    {
        LOG_ERROR " Allocate memory for WRITE1 fail! \n" LOG_END
        while(1);
        goto end;
    }
    bufferW2 = (MMP_UINT8*)malloc(BUFSIZE);
    if(!bufferW2)
    {
        LOG_ERROR " Allocate memory for WRITE2 fail! \n" LOG_END
        goto end;
    }
    for(i=0; i<BUFSIZE; i++) /** fill pattern */
    {
        bufferW1[i] = i % 0x100;
    }
    for(i=0; i<BUFSIZE; i++) /** fill pattern */
    {
        bufferW2[i] = (i+0x7F) % 0x100;
    }

    //============================================
    /** write data to SD1/SD2 */
    LOG_DATA "\n Open A:/sd1_test.dat for SD1 write!!\n" LOG_END
    file1 = f_open("A:/sd1_test.dat", "w");
    if(!file1)
    {
        LOG_ERROR "f_open() fail!! A:/sd1_test.dat \n" LOG_END
        goto end;
    }
    LOG_DATA "\n Open B:/sd2_test.dat for SD2 write!!\n\n" LOG_END
    file2 = f_open("B:/sd2_test.dat", "w");
    if(!file2)
    {
        LOG_ERROR "f_open() fail!! B:/sd2_test.dat \n" LOG_END
        goto end;
    }
    for(i=0; i<FILE_SIZE_MB; i++)
    {
        LOG_INFO " write data to SD1.. \n" LOG_END
        result = f_write(bufferW1, 1, BUFSIZE, file1);
        if(result != BUFSIZE)
        {
            LOG_ERROR " real write size 0x%X != write size 0x%X \n", result, BUFSIZE LOG_END
            goto end;
        }
        LOG_INFO " write data to SD2.. \n" LOG_END
        result = f_write(bufferW2, 1, BUFSIZE, file2);
        if(result != BUFSIZE)
        {
            LOG_ERROR " real write size 0x%X != write size 0x%X \n", result, BUFSIZE LOG_END
            goto end;
        }
    }
    result = 0;
    f_flush(file1);
    f_flush(file2);
    LOG_INFO " Write finish!\n" LOG_END
    if(file1)
    {
        f_close(file1);
        file1 = MMP_NULL;
    }
    if(file2)
    {
        f_close(file2);
        file2 = MMP_NULL;
    }


    //============================================
    /** Read and compare data from SD1/SD2 */
    LOG_INFO "\n" LOG_END
    LOG_INFO "Compare Data!!\n" LOG_END
    bufferR1 = (MMP_UINT8*)malloc(BUFSIZE);
    if(!bufferR1)
    {
        LOG_ERROR " Allocate memory for READ1 fail! \n" LOG_END
        goto end;
    }
    bufferR2 = (MMP_UINT8*)malloc(BUFSIZE);
    if(!bufferR2)
    {
        LOG_ERROR " Allocate memory for READ2 fail! \n" LOG_END
        goto end;
    }
    LOG_DATA "\n Open A:/sd1_test.dat for SD1 read!!\n" LOG_END
    file1 = f_open("A:/sd1_test.dat", "r");
    if(!file1)
    {
        LOG_ERROR "f_open() fail!! \n" LOG_END
        goto end;
    }
    LOG_DATA "\n Open B:/sd2_test.dat for SD2 read!!\n" LOG_END
    file2 = f_open("B:/sd2_test.dat", "r");
    if(!file2)
    {
        LOG_ERROR "f_open() fail!! \n" LOG_END
        goto end;
    }
    for(i=0; i<FILE_SIZE_MB; i++)
    {
        /** for SD1 */
        memset(bufferR1, 0x0, BUFSIZE);
        LOG_INFO " read data from SD1.. \n" LOG_END
        result = f_read(bufferR1, 1, BUFSIZE, file1);
        if(result != BUFSIZE)
        {
            LOG_ERROR " read back size 0x%X != read size 0x%X \n", result, BUFSIZE LOG_END
            goto end;
        }
        diff = memcmp((void*)bufferW1, (void*)bufferR1, BUFSIZE);
        if(diff)
            LOG_ERROR " diff = %d \n", diff LOG_END

        if(diff)
		{
			MMP_UINT32 j = 0;
			LOG_ERROR " i = %d \n", i LOG_END
			for(j=0; j<BUFSIZE; j++)
			{
				if(bufferW1[j] != bufferR1[j])
					LOG_ERROR " write buffer[%X] = %02X, read buffer1[%X] = %02X \n", j, bufferW1[j], j, bufferR1[j] LOG_END
			}
            while(1);
		}

        /** for SD2 */
        memset(bufferR2, 0x0, BUFSIZE);
        LOG_INFO " read data from SD2.. \n" LOG_END
        result = f_read(bufferR2, 1, BUFSIZE, file2);
        if(result != BUFSIZE)
        {
            LOG_ERROR " read back size 0x%X != read size 0x%X \n", result, BUFSIZE LOG_END
            goto end;
        }
        diff = memcmp((void*)bufferW2, (void*)bufferR2, BUFSIZE);
        if(diff)
            LOG_ERROR " diff = %d \n", diff LOG_END

        if(diff)
		{
			MMP_UINT32 j = 0;
			LOG_ERROR " i = %d \n", i LOG_END
			for(j=0; j<BUFSIZE; j++)
			{
				if(bufferW2[j] != bufferR2[j])
					LOG_ERROR " write buffer[%X] = %02X, read buffer1[%X] = %02X \n", j, bufferW2[j], j, bufferR2[j] LOG_END
			}
            while(1);
		}
    }
    result = 0;
    LOG_INFO "Compare Data End!!\n" LOG_END
    if(file1)
    {
        f_close(file1);
        file1 = MMP_NULL;
    }
    if(file2)
    {
        f_close(file2);
        file2 = MMP_NULL;
    }

end:
    if(bufferW1)
    {
        free(bufferW1);
        bufferW1 = MMP_NULL;
    }
    if(bufferW2)
    {
        free(bufferW2);
        bufferW2 = MMP_NULL;
    }
    if(bufferR1)
    {
        free(bufferR1);
        bufferR1 = MMP_NULL;
    }
    if(bufferR2)
    {
        free(bufferR2);
        bufferR2 = MMP_NULL;
    }
    if(file1)
    {
        f_close(file1);
        file1 = MMP_NULL;
    }
    if(file2)
    {
        f_close(file2);
        file2 = MMP_NULL;
    }
    _f_poweroff();
    if(result)
    {
        LOG_ERROR " Error code = 0x%08X \n", result LOG_END
        while(1);
    }
    MMP_Sleep(1000);
    LOG_INFO "End Test!!\n" LOG_END
}

void TestLowLevel(void)
{
#define SECTOR_CNT  3
    MMP_INT result=0;
    static MMP_UINT8 sd1_r1[512*SECTOR_CNT];
    static MMP_UINT8 sd1_r2[512*SECTOR_CNT];
    static MMP_UINT8 sd2_r1[512*SECTOR_CNT];
    static MMP_UINT8 sd2_r2[512*SECTOR_CNT];
    MMP_INT diff1, diff2;
    printf(" sd1 init...\n");
    result = mmpSdInitialize();
    if(result)
        goto end;

    printf(" sd2 init...\n");
    result = mmpSd2Initialize();
    if(result)
        goto end;

    printf("## sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r1);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r1);
    if(result)
        goto end;

    printf("##  sd1 write...\n");
    result = mmpSdWriteMultipleSector(512, SECTOR_CNT, sd1_r1);
    if(result)
        goto end;

    printf("##  sd2 write...\n");
    result = mmpSd2WriteMultipleSector(512, SECTOR_CNT, sd2_r1);
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#if 1 // ok
    printf(" \n\n Case 1:\n");
    printf("##  sd1 terminate...\n");
    mmpSdTerminate();

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;

    printf(" sd1 init...\n");
    result = mmpSdInitialize();
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif
#if 1 // ok
    printf(" \n\n Case 2:\n");
    printf("##  sd2 terminate...\n");
    mmpSd2Terminate();

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf(" sd2 init...\n");
    result = mmpSd2Initialize();
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif
#if 1 // ok
    printf(" \n\n Case 3:\n");
    printf("##  sd1 terminate...\n");
    mmpSdTerminate();

    printf(" sd1 init...\n");
    result = mmpSdInitialize();
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif
#if 1 // ok
    printf(" \n\n Case 4:\n");
    printf("##  sd1 terminate...\n");
    mmpSdTerminate();

    printf("##  sd2 terminate...\n");
    mmpSd2Terminate();

    printf(" sd1 init...\n");
    result = mmpSdInitialize();
    if(result)
        goto end;

    printf(" sd2 init...\n");
    result = mmpSd2Initialize();
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif
#if 1 // ok
    printf(" \n\n Case 5:\n");
    printf("##  sd2 terminate...\n");
    mmpSd2Terminate();

    printf("##  sd1 terminate...\n");
    mmpSdTerminate();

    printf(" sd1 init...\n");
    result = mmpSdInitialize();
    if(result)
        goto end;

    printf(" sd2 init...\n");
    result = mmpSd2Initialize();
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif
#if 1 // ok
    printf(" \n\n Case 6:\n");
    printf("##  sd2 terminate...\n");
    mmpSd2Terminate();

    printf("##  sd1 terminate...\n");
    mmpSdTerminate();

    printf(" sd2 init...\n");
    result = mmpSd2Initialize();
    if(result)
        goto end;

    printf(" sd1 init...\n");
    result = mmpSdInitialize();
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif
#if 1 // ok
    printf(" \n\n Case 7:\n");
    printf("##  sd1 terminate...\n");
    mmpSdTerminate();

    printf("##  sd2 terminate...\n");
    mmpSd2Terminate();

    printf(" sd2 init...\n");
    result = mmpSd2Initialize();
    if(result)
        goto end;

    printf(" sd1 init...\n");
    result = mmpSdInitialize();
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif
#if 1 // fail
    printf(" \n\n Case 8:\n");
    printf("##  sd2 terminate...\n");
    mmpSd2Terminate();

    printf(" sd2 init...\n");
    result = mmpSd2Initialize();
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;

    printf("##  sd1 read...\n");
    result = mmpSdReadMultipleSector(512, SECTOR_CNT, sd1_r2);
    if(result)
        goto end;

    printf("##  sd2 read...\n");
    result = mmpSd2ReadMultipleSector(512, SECTOR_CNT, sd2_r2);
    if(result)
        goto end;
#endif

#if 1
    diff1 = memcmp((void*)sd1_r1, (void*)sd1_r2, 512*SECTOR_CNT);
    if(diff1)
        LOG_ERROR " sd1 compare data fail! diff1 %d \n", diff1 LOG_END

    diff2 = memcmp((void*)sd2_r1, (void*)sd2_r2, 512*SECTOR_CNT);
    if(diff1)
        LOG_ERROR " sd2 compare data fail! diff2 %d \n", diff2 LOG_END
#endif
end:
    printf(" sd1 terminate...\n");
    mmpSdTerminate();
    printf(" sd2 terminate...\n");
    mmpSd2Terminate();
    if(result)
        while(1);
}

#include "mem/mem.h"

void DoTest(void)
{
	MMP_INT result = 0;
MEM_Allocate(4, MEM_USER_SD); // FPGA test
{
#include "host/gpio.h"
    MMP_UINT32 gpio_card_power_enable = 10;
    GPIO_Enable(gpio_card_power_enable);
    GPIO_SetMode(gpio_card_power_enable, GPIO_MODE_OUTPUT);
    GPIO_SetState(gpio_card_power_enable, GPIO_STATE_LO);  // power up
}
	result = mmpDmaInitialize();
	if(result)
		goto end;

    #if defined(TEST_PERFORMANCE)
    while(1)
        TestPerformance();

    #elif defined(TEST_2CARD_RW_FILE)
    while(1)
        TestReadWriteFile();

    #elif defined(TEST_LOW_LEVEL)
    while(1)
        TestLowLevel();
    #endif

end:
	return;
}

int  main(int argc, char** argv)
{
#if defined(__FREERTOS__)
    signed portBASE_TYPE ret = pdFAIL;

    HOST_GetChipVersion();

    ret = xTaskCreate(main_task_func, "sdtest_main",
        configMINIMAL_STACK_SIZE * 2,
        NULL, tskIDLE_PRIORITY + 1, &main_task_handle );
    if (pdFAIL == ret) {
        printf(" ERROR: Failed to create main task!!\n");
        return 1;
    }

    vTaskStartScheduler();
#endif

    DoTest();

    return 1;
}


#if defined(__FREERTOS__)
portTASK_FUNCTION(main_task_func, params)
{
    MMP_INT result = 0;

    DoTest();
}
#endif
