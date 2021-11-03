/*
* Copyright (c) 2014-2016 IBM Corporation.
* All rights reserved.
*
* Copyright (c) 2017 MCCI Corporation
* All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  * Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*  * Neither the name of the <organization> nor the
*    names of its contributors may be used to endorse or promote products
*    derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _lorabase_cn490_h_
#define _lorabase_cn490_h_

#ifndef _LMIC_CONFIG_PRECONDITIONS_H_
# include "lmic_config_preconditions.h"
#endif

/****************************************************************************\
|
| Basic definitions for cn490 (always in scope)
|
\****************************************************************************/

// Frequency plan for US 490MHz ISM band
// data rates
enum _dr_cn490_t {
        CN490_DR_SF12 = 0,
        CN490_DR_SF11,
        CN490_DR_SF10,
        CN490_DR_SF9,
        CN490_DR_SF8,
        CN490_DR_SF7,
        CN490_DR_NONE
};

// Default frequency plan for CN 490MHz
enum {
        //CN490_125kHz_UPFBASE = 470300000, //由于不会修改LMIC_selectSubBand、LMICcn490_setRx1Params中对于掩码的设置，只能通过偏移初始值达到选择80-87信道的结果
        CN490_125kHz_UPFBASE = 475100000,
        CN490_125kHz_UPFSTEP = 200000,
        
        //CN490_125kHz_DNFBASE = 500300000,
        CN490_125kHz_DNFBASE = 506700000,
        CN490_125kHz_DNFSTEP = 200000
};
enum {
        CN490_FDOWN = 505300000,      //      (RX2 freq, DR0)
};
enum {
        CN490_FREQ_MIN = 470000000,
        CN490_FREQ_MAX = 510000000
};
enum {
        CN490_TX_EIRP_MAX_DBM = 19      // 19.15 dBm
};

enum { DR_PAGE_CN490 = 0x10 * (LMIC_REGION_cn490 - 1) };

enum { CN490_LMIC_REGION_EIRP = 1 };         // // region uses EIRP

#endif /* _lorabase_cn490_h_ */