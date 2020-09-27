/*
 * Global.h
 *
 *  Created on: 2020/09/27
 *      Author: MIBC
 */

#ifndef FRTOS_SKELETON_GLOBAL_H_
#define FRTOS_SKELETON_GLOBAL_H_

// USB接続種別
typedef enum
{
	USB_KIND_DISCONNECT = 0,		// USB未接続
	USB_KIND_CONNECT = 1,			// USB接続

} USB_KIND_ENUM;

// ADPCM出力種別
typedef enum
{
	ADPCM_OUTPUT_MODE_PWM = 0,		// PWM
	ADPCM_OUTPUT_MODE_DA = 1,		// DA
} ADPCM_OUTPUT_MODE_ENUM;

typedef struct
{
	USB_KIND_ENUM 					eUsbKind;			// USB接続種別


	ADPCM_OUTPUT_MODE_ENUM 			eAdpcmMode;

} GLOBAL_INFO_TABLE;










#endif /* FRTOS_SKELETON_GLOBAL_H_ */
