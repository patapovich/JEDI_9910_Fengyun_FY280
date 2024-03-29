
#include "mmp_msc.h"
#include "common/fat.h"


static F_DRIVER drv;
extern void* ctxt;
extern MMP_UINT8 testLun;

static int drv_readsector(F_DRIVER *driver,void *data,unsigned long sector)
{
    MMP_INT ret = 0;

    ret = mmpMscReadMultipleSector(ctxt, testLun, sector,1,data);
    if(ret)
    {
        return F_ERR_READ;
    }
    else
    {
        return F_NO_ERROR;
    }
}

static int drv_readmultiplesector(F_DRIVER *driver,void *data,unsigned long sector,int cnt)
{
    MMP_INT ret = 0;

    ret  = mmpMscReadMultipleSector(ctxt, testLun, sector,cnt,data);
    if(ret)
    {
        return F_ERR_READ;
    }
    else
    {
        return F_NO_ERROR;
    }
}

static int drv_writesector(F_DRIVER *driver,void *data,unsigned long sector)
{
    MMP_INT ret = 0;

    ret  = mmpMscWriteMultipleSector(ctxt, testLun, sector,1,data);
    if(ret)
    {
        return F_ERR_WRITE;
    }
    else
    {
        return F_NO_ERROR;
    }
}

static int drv_writemultiplesector(F_DRIVER *driver,void *data,unsigned long sector,int cnt)
{
    MMP_INT ret = 0;

    ret  = mmpMscWriteMultipleSector(ctxt, testLun, sector,cnt,data);
    if(ret)
    {
        return F_ERR_WRITE;
    }
    else
    {
        return F_NO_ERROR;
    }
}

static long drv_getstatus(F_DRIVER *driver)
{
    MMP_INT status = mmpMscGetStatus(ctxt, testLun);
    if(!status)
        return F_NO_ERROR;
    else
        return F_ST_MISSING;
}

static int drv_getphy(F_DRIVER *driver,F_PHY *phy)
{
    MMP_INT result = 0;
    MMP_UINT32 sectors = 0;
    MMP_UINT32 blockSize = 0;

    result = mmpMscGetCapacity(ctxt, testLun, &sectors, &blockSize);
    if(result)
        goto end;
        
    phy->number_of_cylinders = 0;
    phy->sector_per_track    = 63;
    phy->number_of_heads     = 255;
    phy->number_of_sectors   = sectors;
    phy->media_descriptor    = 0xf8; /* fixed drive */

end:
    return result;
}

static void drv_release(F_DRIVER *driver)
{
   mmpMscTerminate(ctxt, testLun);
}

F_DRIVER *msc_initfunc(unsigned long driver_param)
{
    MMP_INT ret = 0;

    ret  = mmpMscInitialize(ctxt, testLun);
    if (ret != MMP_RESULT_SUCCESS)
    {
        return 0;
    }
    else
    {
        drv.readsector=drv_readsector;
        drv.writesector=drv_writesector;
        drv.readmultiplesector=drv_readmultiplesector;
        drv.writemultiplesector=drv_writemultiplesector;
        drv.getstatus=drv_getstatus;
        drv.getphy=drv_getphy;
        drv.release=drv_release;
        drv.user_data=0x0001;

        return (&drv);
    }
}
