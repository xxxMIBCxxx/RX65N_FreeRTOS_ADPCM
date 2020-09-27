/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "task_function.h"
#include "r_s2_peripheral_if.h"
#include "r_usb_hmsc_apl_config.h"
#include "r_usb_hmsc_apl.h"

#include "Global.h"
extern GLOBAL_INFO_TABLE			g_tGlobalInfo;


#define SOUND_FILE_PATH				( "sound.dat" )


#if 0
FRESULT scan_files (char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;   /* 非Unicode構成を想定 */


    res = f_opendir(&dir, path);                       /* ディレクトリを開く */
    if (res != FR_OK)
    {
    	printf("f_opendir Error! [ret:%d]\n",res);
    	return res;
    }

	i = strlen(path);
	for (;;)
	{
		res = f_readdir(&dir, &fno);                   /* ディレクトリ項目を1個読み出す */
        if (res != FR_OK || fno.fname[0] == 0)
        {
        	break;  /* エラーまたは項目無しのときは抜ける */
        }

        if (fno.fname[0] == '.')
        {
        	continue;             /* ドットエントリは無視 */
        }

        fn = fno.fname;
        if (fno.fattrib & AM_DIR)
        {                    /* ディレクトリ */
        	continue;
        }

        printf("%s/%s\n", path, fn);
    }

    return res;
}
#endif


void Task_Adpcm(void * pvParameters)
{
	FILINFO		tFileInfo;
    FIL         file;
    FRESULT     ret;

	g_tGlobalInfo.eAdpcmMode = ADPCM_OUTPUT_MODE_DA;

	// LED
	PORTA.PDR.BIT.B1 = 1;
	PORTA.PODR.BIT.B1 = 0;

	// SW
	PORT0.PDR.BIT.B0 = 0;
	PORT0.PCR.BIT.B0 = 1;


	while( 1 )
	{
		if (PORT0.PIDR.BIT.B0 == 0)
		{
			// USBが接続されているかを確認
			if (g_tGlobalInfo.eUsbKind == USB_KIND_DISCONNECT)
			{
				printf("USB is not connected!\n");
				vTaskDelay(1000);
			}
			else
			{
				// ファイルサイズ取得 & ファイル存在チェック
				ret = f_stat(SOUND_FILE_PATH,&tFileInfo);
				if (ret != 0)
				{
					printf("f_stat Error! [ret:%d]\n", ret);
				}
				else
				{
					printf("f_stat Success.\n");
					printf("file Name:%s\n",tFileInfo.fname);
					printf("file size:%u\n",tFileInfo.fsize);

					// 音声ファイルオープン
					ret = f_open(&file, SOUND_FILE_PATH,(FA_READ | FA_OPEN_EXISTING));
					if (ret != 0)
					{
						printf("File(%s) open failed! [ret:%d]\n", SOUND_FILE_PATH, ret);
						vTaskDelay(1000);
					}
					else
					{
						PORTA.PODR.BIT.B1 = 1;

						// エンコード開始
						R_decode_main(&file,&tFileInfo);

						f_close(&file);
						PORTA.PODR.BIT.B1 = 0;
					}
				}
			}
		}
	}
}
