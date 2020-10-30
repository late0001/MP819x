//*****************************************************************************
//  Created:    2015/02/19   11:42
//  File base:  Tester Common
//  Copyright:  iTest
//  Version:
//
//  Purpose:    This header file defines the types and definitions used to
//              get analyse result and information from the WT Tester.
//*****************************************************************************

#ifndef _TESTER_COMMON_H_
#define _TESTER_COMMON_H_

#include "AnalyseDef.h"

//#define STD_CALL

#ifdef STD_CALL
#define CALL_MODE __stdcall
#else
#define CALL_MODE
#endif

//*****************************************************************************
//                            SPECTRUM
//*****************************************************************************
//Carrier leakage in dB.Value is returned
#define WT_RES_SPECTRUM_CARR_LEAKAGE              "Spec_carrier_leakage"

//OBW(99%) in Hz.Value is returned
#define WT_RES_SPECTRUM_OBW_99                    "Spec_Obw"

//Spectrum mask error point in %.Value is returned
#define WT_RES_SPECTRUM_MASK_ERR_PERCENT          "Spec_mask_err"

//Frequency of the max power in Hz.Value is returned
#define WT_RES_SPECTRUM_PEAK_POW_FREQ             "Spec_peak_freq"

//Spectrum data. Vector is returned.
#define WT_RES_SPECTRUM_DATA                      "Spec_data"

//Spectrum Mask Margin data. Complex Vector is returned.
#define WT_RES_SPECTRUM_MARGIN                    "Spec_margin"

//Frequency in Hz.Value is returned. CW only.
#define WT_RES_CW_FREQ_OFFSET                     "CW_spec_freq_offset"

//*****************************************************************************
//                            IQ
//*****************************************************************************
//RAW data. Complex vector is returned.
#define WT_RES_RAW_DATA                           "Raw_data"

//*****************************************************************************
//                            POWER
//*****************************************************************************
// Frame Count by Power Detectoring. Value is returned.
#define WT_RES_POWER_FRAME_CNT                    "Pow_frame_count"

// Power frame (no gap) in dB. Value is returned.
#define WT_RES_POWER_FRAME_DB                     "Pow_frame"

// RMS Power in dB. Value is returned.
#define WT_RES_POWER_ALL_DB                       "Pow_all"

// Power Peak in dB. Value is returned.
#define WT_RES_POWER_PEAK_DB                      "Pow_peak"

//*****************************************************************************
//                            WIFI Frame
//*****************************************************************************
// EVM for entire frame in dB. Value is returned.
#define WT_RES_FRAME_EVM_ALL                      "evm.all"

// EVM for entire frame in %. Value is returned.
#define WT_RES_FRAME_EVM_ALL_PERCENT              "evm.all(%)"

// EVM peak value in dB. Value is returned.
#define WT_RES_FRAME_EVM_PEAK                     "evm.pk"

// EVM peak value in %. Value is returned.
#define WT_RES_FRAME_EVM_PEAK_PERCENT             "evm.pk(%)"

// Frequency Error in Hz. Value is returned.
#define WT_RES_FRAME_FREQ_ERR                     "signal.freqerr"

// Symbol Clock Error in ppm. Value is returned.
#define WT_RES_FRAME_SYMBOL_CLOCK_ERR             "signal.symclockerr"

// IQ Match Amplitude Error in dB. Value is returned.
#define WT_RES_FRAME_IQ_MATCH_AMP                 "iqmatch.amp"

// IQ Match Phase Error in Deg. Value is returned.
#define WT_RES_FRAME_IQ_MATCH_PHASE               "iqmatch.phase"

// IQ Phase Error in Deg. Value is returned.
#define WT_RES_FRAME_PHASE_ERROR                  "phase.error"

// Data rate in Mbps. Value is returned.
#define WT_RES_FRAME_DATA_RATE_MBPS               "Data_rate_Mbps"

// Ramp on time. Value is returned.
#define WT_RES_FRAME_RAMP_ON_TIME                 "ramp.on_time"

// Ramp off time. Value is returned.
#define WT_RES_FRAME_RAMP_OFF_TIME                "ramp.off_time"

// Number of symbols. Value is returned. OFDM only
#define WT_RES_FRAME_OFDM_NUMBER_SYMBOLS          "ofdm.more_res.PLCP.Nspp"

// EVM for data part of frame.dB. Value is returned. OFDM only
#define WT_RES_FRAME_EVM_DATA_DB                  "evm.data"

// EVM for pilot part of frame.dB. Value is returned. OFDM only
#define WT_RES_FRAME_EVM_PILOT_DB                 "evm.pilot"

// Spectral flatness Passed Or Failed. 1(1.0) - Passed; 0(0.0) - Failed. OFDM only
#define WT_RES_SPECTRUM_FLATNESS_PASSED           "flatness.passed"

// Spectral flatness section value. Complex Vector is returned.  OFDM only
#define WT_RES_SPECTRUM_FLATNESS_SECTION_VALUE    "flatness.section.value"

// Spectral flatness section margin. Complex Vector is returned. OFDM only
#define WT_RES_SPECTRUM_FLATNESS_SECTION_MARGIN   "flatness.section.margin"

// IQ DC Offset For 11B in dB. Value is returned. DSSS/CCK only
#define WT_RES_FRAME_IQ_OFFSET_11B                "iq.offset"

// RF Carrier Suppression for 11B in dB. Value is returned. DSSS/CCK only
#define WT_RES_FRAME_CARRIER_SUPPRESSION_11B      "carrier.suppression"

//Psdu Length In Frame. WIFI only
#define WT_RES_FRAME_PSDU_LENGTH                  "psdu.length"
//*****************************************************************************
//                          BT   Frame
//*****************************************************************************
//BT BR Freq.Drift,Hz(Payload 10101010)
#define WT_RES_BT_FRAME_CARR_FREQ_DRIFT           "BT_CARR_FREQ_DRIFT"

//BT BR Freq.DriftRate,Hz(Payload 10101010)
#define WT_RES_BT_FRAME_CARR_FREQ_DRIFT_RATE      "BT_CARR_FREQ_DRIFT_RATE"

//BT BR Max Initial freq offset of each burst detected in Hz.
#define WT_RES_BT_FRAME_MAX_CARR_FREQ             "BT_MAX_CARR_FREQ"

//BT BR Delta F1(and WT_ANA_PARM_BT_CARR_FREQ_DRIFT) valid
#define WT_RES_BT_FRAME_DELTA_F1_VALID            "BT_DELTA_F1_VALID"

//BT BR Delta F1 avg,Hz(Payload 00001111)
#define WT_RES_BT_FRAME_DELTA_F1_AVG              "BT_DELTA_F1_AVG"

//BT BR Delta F2 valid
#define WT_RES_BT_FRAME_DELTA_F2_VALID            "BT_DELTA_F2_VALID"

//BT BR Delta F2 avg,Hz(Payload 10101010)
#define WT_RES_BT_FRAME_DELTA_F2_AVG              "BT_DELTA_F2_AVG"

//BT BR Delta F2 max,Hz(Payload 10101010)
#define WT_RES_BT_FRAME_DELTA_F2_MAX              "BT_DELTA_F2_MAX"

//BT EDR DEVM Valid
#define WT_RES_BT_FRAME_BT_DEVM_VALID             "BT_DEVM_VALID"

//BT EDR Initial freq error Complex vector, Hz
#define WT_RES_BT_FRAME_CARR_FREQ_BUF             "BT_CARR_FREQ_BUF"

//BT EDR DEVM,%
#define WT_RES_BT_FRAME_BT_DEVM                   "BT_DEVM"

//BT EDR DEVM Peak,%
#define WT_RES_BT_FRAME_BT_DEVM_PEAK              "BT_DEVM_PEAK"

//BT EDR Power Diff,dB
#define WT_RES_BT_FRAME_BT_POW_DIFF               "BT_POW_DIFF"

//BT EDR DEVM （Less than 20% for 3M, 30% for 2M)
#define WT_RES_BT_FRAME_BT_99PCT                  "BT_99PCT"

//BT EDR Omega I,Hz
#define WT_RES_BT_FRAME_EDR_Omega_I               "BT_Omega_I"

//BT EDR Omega O,Hz
#define WT_RES_BT_FRAME_EDR_Omega_O               "BT_Omega_O"

//BT EDR Omega IO,Hz
#define WT_RES_BT_FRAME_EDR_Omega_IO              "BT_Omega_IO"

//BT Bandwidth-20dB Passed Or Failed. 1(1.0) - Passed; 0(0.0) - Failed.
#define WT_RES_BT_FRAME_BW20dB_Passed             "BT_BW20dB_Passed"

//BT Bandwidth-20dB value
#define WT_RES_BT_FRAME_BW20dB                    "BT_BW20dB_Value"

//BT BLE Freq Drift Detail Valid
#define WT_RES_BT_FRAME_BLE_DRIFT_DETAIL_VALID    "BT_BLE_Drift_Detail_VALID"

//BT BLE FnMax,Hz
#define WT_RES_BT_FRAME_BLE_FnMax                 "BT_|Fn| Max"

//BT BLE F0FnMax,Hz
#define WT_RES_BT_FRAME_BLE_F0FnMax               "BT_|F0-Fn| Max"

//BT BLE Delta_F1F0,Hz
#define WT_RES_BT_FRAME_BLE_Delta_F1F0            "BT_|F1-F0|"

//BT BLE F0Fn5_Max,Hz
#define WT_RES_BT_FRAME_BLE_FnFn5_Max             "BT_|Fn-Fn-5| Max"

//BT BR/BLE Spectrum Adjacent Channel Power(ACP) dBm,Vector is returned
#define WT_RES_BT_SPETRUM_ADJACENT_CHANNEL_POWER  "BT_Spectrum_Acp"
//*****************************************************************************
//                          Zigbee   Frame
//*****************************************************************************
//Zigbee EVM(PSDU), dB
#define WT_RES_ZIGBEE_FRAME_EVM_PSDU              "Zigbee.evm(psdu)"

//Zigbee EVM(PSDU), %
#define WT_RES_ZIGBEE_FRAME_EVM_PSDU_PERCENT      "Zigbee.evm(psdu).percent"

//Zigbee EVM(SHR+PHR), dB
#define WT_RES_ZIGBEE_FRAME_EVM_SHRPHR            "Zigbee.evm(shr+phr)"

//Zigbee EVM(SHR+PHR), %
#define WT_RES_ZIGBEE_FRAME_EVM_SHRPHR_PERCENT    "Zigbee.evm(shr+phr).percent"

//Zigbee EVM Offect ,dB
#define WT_RES_ZIGBEE_FRAME_EVM_OFFSET            "Zigbee.evm_offset"

//Zigbee EVM Offect ,%
#define WT_RES_ZIGBEE_FRAME_EVM_OFFSET_PERCENT    "Zigbee.evm_offset.percent"
//*****************************************************************************
//                            Error Code
//*****************************************************************************
enum WT_ERR_CODE_ENUM
{

	WT_ERR_CODE_OK,
	WT_ERR_CODE_CONNECT_FAIL,            //连接失败、未建立或已中断
	WT_ERR_CODE_UNKNOW_PARAMETER,        //参数有误
	WT_ERR_CODE_OUT_OF_MEMORY,           //内存不足
	WT_ERR_CODE_NO_DATA_CAPTURED,        //未抓取数据
	WT_ERR_CODE_TIMEOUT,                 //超时
	WT_ERR_CODE_VsgInaccuracy,           //指定的VSG功率不准确
	WT_ERR_CODE_GENERAL_ERROR,           //其他错误

	WT_ERR_CODE_BANDWIDTH_ERROR,         //带宽设置错误
	WT_ERR_CODE_SIGNALTYPE_ERROR,        //信号类型错误
	WT_ERR_CODE_FRM_ERROR,               //11n帧类型错误
	WT_ERR_CODE_PARAMETER_MISMATCH,      //参数关联错误
	WT_ERR_CODE_PSDU_ERROR,              //PSDU设置错误
	WT_ERR_CODE_PSDU_CONVERT_FAIL,       //Mac格式错误
	WT_ERR_CODE_OUTDATA_INVALID,         //存放输出数据的指针错误
	WT_ERR_CODE_GENERATE_FAIL,           //生成信号失败

	WT_ERR_CODE_TESTER_NO_WAVE,          //仪器没有指定的波形文件
	WT_ERR_CODE_NO_LICENSE_SUPPORT,      //没有对应的license支持

	WT_ERR_CODE_LAST
};

typedef double  Complex[2];

#define MAX_TESTER_COUNT 4

enum WT_TESTER_TYPE
{
	WT160,
	WT200,
	WT208 = 3,
	WT208C,
	WT400 = 6
};

//*****************************************************************************
//                            VSG Status
//*****************************************************************************
enum WT_VSG_STATE
{
	WT_VSG_STATE_DONE,
	WT_VSG_STATE_RUNNING,
	WT_VSG_STATE_TIMEOUT,
	WT_VSG_STATE_ERR_DONE,
	WT_VSG_STATE_WAITING
};

typedef struct
{
	char ip[16];                         // IP地址
	char SubMask[16];                    // 子网掩码
	char GateAddr[16];                   // 网关
	char SN[80];                         // SN码
	char name[40];                       // 别名        "iTest Tester"
	char MacAddr[18];                    // MAC 地址 "DC-A4-B5-C7-E1-F8"
	char FW_Version[40];                 // 固件版本
	char HW_Version[40];                 // 硬件版本
	char RF_Version[40];                 // 射频板版本
	char cal_date[20];                   // 校准日期 "2012-05-04 18:36:56"
} TesterInfo;

typedef struct
{
	double  freq;                        //中心频率,单位：Hz
	int     vsa_port;                    //接收端口，WT_PORT_ENUM
	int     vsg_port;                    //发送端口，WT_PORT_ENUM
	int     waveType;                    //Wave类型，WT_SIGNAL_ENUM，仅当为SIG_USERFILE时才读取wave文件，WT-208仪器专用
	char    *wave;                       //VSG数据文件,如果为null,只更新设备配置，不更新wave文件
	int     TimeoutWaiting;              //在多连接情况下，等待的最大时间，单位sec
} CableVerifyParameter;

#endif