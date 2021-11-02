/*
* Copyright (c) 2014-2016 IBM Corporation.
* Copyright (c) 2017, 2019-2021 MCCI Corporation.
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

#ifndef _lmic_bandplan_cn490_h_
# define _lmic_bandplan_cn490_h_

// preconditions for lmic_us_like.h
#define LMICuslike_getFirst500kHzDR()   (LORAWAN_DR4)
#define LMICuslike_getJoin125kHzDR()    (LORAWAN_DR0)

#ifndef _lmic_us_like_h_
# include "lmic_us_like.h"
#endif

// return maximum frame length (including PHY header) for this data rate (cn490); 0 --> not valid dr.
uint8_t LMIcn490_maxFrameLen(uint8_t dr);
// return maximum frame length (including PHY header) for this data rate; 0 --> not valid dr.
#define LMICbandplan_maxFrameLen(dr) LMIcn490_maxFrameLen(dr)

int8_t LMIcn490_pow2dbm(uint8_t mcmd_ladr_p1);
#define pow2dBm(mcmd_ladr_p1) LMIcn490_pow2dbm(mcmd_ladr_p1)

ostime_t LMICcn490_dr2hsym(uint8_t dr);
#define dr2hsym(dr) LMICcn490_dr2hsym(dr)


#define LMICbandplan_getInitialDrJoin() (LORAWAN_DR0)

void LMICcn490_setBcnRxParams(void);
#define LMICbandplan_setBcnRxParams() LMICcn490_setBcnRxParams()

u4_t LMICcn490_convFreq(xref2cu1_t ptr);
#define LMICbandplan_convFreq(ptr)      LMICcn490_convFreq(ptr)

void LMICcn490_initJoinLoop(void);
#define LMICbandplan_initJoinLoop()     LMICcn490_initJoinLoop()

void LMICcn490_setRx1Params(void);
#define LMICbandplan_setRx1Params()     LMICcn490_setRx1Params()

void LMICcn490_updateTx(ostime_t txbeg);
#define LMICbandplan_updateTx(txbeg)    LMICcn490_updateTx(txbeg)

#undef LMICbandplan_validDR
bit_t LMICcn490_validDR(dr_t dr);
#define LMICbandplan_validDR(dr)        LMICcn490_validDR(dr)

#endif // _lmic_bandplan_cn490_h_
