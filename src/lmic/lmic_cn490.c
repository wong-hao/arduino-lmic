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

#define LMIC_DR_LEGACY 0

#include "lmic_bandplan.h"

#if defined(CFG_cn490)
// ================================================================================
//
// BEG: CN490 related stuff
//

CONST_TABLE(u1_t, _DR2RPS_CRC)[] = {
        ILLEGAL_RPS,				// [-1]
        MAKERPS(SF12, BW125, CR_4_5, 0, 0),	// [0]
        MAKERPS(SF11, BW125, CR_4_5, 0, 0),	// [1]
        MAKERPS(SF10, BW125, CR_4_5, 0, 0),	// [2]
        MAKERPS(SF9 , BW125, CR_4_5, 0, 0),	// [3]
        MAKERPS(SF8 , BW125, CR_4_5, 0, 0),	// [4]
        MAKERPS(SF7 , BW125, CR_4_5, 0, 0),	// [5]
        ILLEGAL_RPS				// [6]
};

bit_t
LMICcn490_validDR(dr_t dr) {
        // use subtract here to avoid overflow
        if (dr >= LENOF_TABLE(_DR2RPS_CRC) - 2)
                return 0;
        return TABLE_GET_U1(_DR2RPS_CRC, dr+1)!=ILLEGAL_RPS;
}

static CONST_TABLE(u1_t, maxFrameLens)[] = {
        59+5, 59+5, 59+5, 123+5, 230+5, 230+5
};

uint8_t LMIcn490_maxFrameLen(uint8_t dr) {
        if (dr < LENOF_TABLE(maxFrameLens))
                return TABLE_GET_U1(maxFrameLens, dr);
        else
                return 0;
}

static CONST_TABLE(s1_t, TXPOWLEVELS)[] = {
        19.15, 17.15, 15.15, 13.15, 11.15, 9.15, 7.15, 5.15
};

int8_t LMIcn490_pow2dbm(uint8_t mcmd_ladr_p1) {
        uint8_t const pindex = (mcmd_ladr_p1&MCMD_LinkADRReq_POW_MASK)>>MCMD_LinkADRReq_POW_SHIFT;
        if (pindex < LENOF_TABLE(TXPOWLEVELS)) {
                return TABLE_GET_S1(TXPOWLEVELS, pindex);
        } else {
                return -128;
        }
}

static CONST_TABLE(ostime_t, DR2HSYM_osticks)[] = {
		us2osticksRound(128 << 7),	// DR_SF12
		us2osticksRound(128 << 6),	// DR_SF11
		us2osticksRound(128 << 5),	// DR_SF10
		us2osticksRound(128 << 4),	// DR_SF9
		us2osticksRound(128 << 3),	// DR_SF8
		us2osticksRound(128 << 2),	// DR_SF7
};

ostime_t LMICcn490_dr2hsym(uint8_t dr) {
        return TABLE_GET_OSTIME(DR2HSYM_osticks, (dr) & 7);  // map DR_SFnCR -> 0-6
}



u4_t LMICcn490_convFreq(xref2cu1_t ptr) {
        u4_t freq = (os_rlsbf4(ptr - 1) >> 8) * 100;
        if (freq < CN490_FREQ_MIN || freq > CN490_FREQ_MAX)
                freq = 0;
        return freq;
}

///
/// \brief query number of default channels.
///
///     For CN, we have no programmable channels; all channels
///     are fixed. Return the total channel count.
///
u1_t LMIC_queryNumDefaultChannels() {
        return 96 + 48;
}

///
/// \brief LMIC_setupChannel for CN
///
/// \note there are no progammable channels for CN490, so this API
///     always returns FALSE.
///
bit_t LMIC_setupChannel(u1_t chidx, u4_t freq, u2_t drmap, s1_t band) {
        LMIC_API_PARAMETER(chidx);
        LMIC_API_PARAMETER(freq);
        LMIC_API_PARAMETER(drmap);
        LMIC_API_PARAMETER(band);

        return 0; // channels 0..96 are hardwired
}

bit_t LMIC_disableChannel(u1_t channel) {
        bit_t result = 0;
        if (channel < 96) {
                if (ENABLED_CHANNEL(channel)) {
                        result = 1;
                        LMIC.activeChannels125khz--;
                }
                LMIC.channelMap[channel >> 4] &= ~(1 << (channel & 0xF));
        }
        return result;
}

bit_t LMIC_enableChannel(u1_t channel) {
        bit_t result = 0;
        if (channel < 96) {
                if (!ENABLED_CHANNEL(channel)) {
                        result = 1;
                        LMIC.activeChannels125khz++;
                }
                LMIC.channelMap[channel >> 4] |= (1 << (channel & 0xF));
        }
        return result;
}

bit_t  LMIC_enableSubBand(u1_t band) {
        ASSERT(band < 12);
        u1_t start = band * 8;
        u1_t end = start + 8;
        bit_t result = 0;

        // enable all eight 125 kHz channels in this subband
        for (int channel = start; channel < end; ++channel)
                result |= LMIC_enableChannel(channel);

        return result;
}

bit_t  LMIC_disableSubBand(u1_t band) {
        ASSERT(band < 12);
        u1_t start = band * 8;
        u1_t end = start + 8;
        bit_t result = 0;

        // disable all eight 125 kHz channels in this subband
        for (int channel = start; channel < end; ++channel)
                result |= LMIC_disableChannel(channel);

        return result;
}

bit_t  LMIC_selectSubBand(u1_t band) {
        bit_t result = 0;

        ASSERT(band < 12);
        for (int b = 0; b<8; ++b) {
                if (band == b)
                        result |= LMIC_enableSubBand(b);
                else
                        result |= LMIC_disableSubBand(b);
        }
        return result;
}

// from LoRaWAN 5.8: mapping from txParam to MaxEIRP
static CONST_TABLE(s1_t, TXMAXEIRP)[16] = {
	8, 10, 12, 13, 14, 16, 18, 20, 21, 24, 26, 27, 29, 30, 33, 36
};

static int8_t LMICcn490_getMaxEIRP(uint8_t mcmd_txparam) {
        // if uninitialized, return default.
	if (mcmd_txparam == 0xFF)
		return CN490_TX_EIRP_MAX_DBM;
	else
		return TABLE_GET_S1(
			TXMAXEIRP,
			(mcmd_txparam & MCMD_TxParam_MaxEIRP_MASK) >>
				MCMD_TxParam_MaxEIRP_SHIFT
			);
}

void LMICcn490_updateTx(ostime_t txbeg) {
        u1_t chnl = LMIC.txChnl;
        if (chnl < 96) {
                
                //接到ADR下发指令后不知为何上行信道会重新选择到88-95，只能进行手动削减偏移
                if(CN490_125kHz_UPFBASE + chnl*CN490_125kHz_UPFSTEP > CN490_125kHz_UPFBASE + 63*CN490_125kHz_UPFSTEP){
                        LMIC.freq = CN490_125kHz_UPFBASE + chnl*CN490_125kHz_UPFSTEP - 8*CN490_125kHz_UPFSTEP;
                }else{
                         LMIC.freq = CN490_125kHz_UPFBASE + chnl*CN490_125kHz_UPFSTEP;
                }

                LMIC.txpow = LMICcn490_getMaxEIRP(LMIC.txParam);
        }
}

#if !defined(DISABLE_BEACONS)
void LMICcn490_setBcnRxParams(void) {
        LMIC.dataLen = 0;
        LMIC.freq = CN490_125kHz_DNFBASE + LMIC.bcnChnl * CN490_125kHz_DNFSTEP;
        LMIC.rps = setIh(setNocrc(dndr2rps((dr_t)DR_BCN), 1), LEN_BCN);
}
#endif // !DISABLE_BEACONS

// set the Rx1 dndr, rps.
void LMICcn490_setRx1Params(void) {
    u1_t const txdr = LMIC.dndr;
    u1_t candidateDr;
    LMIC.freq = CN490_125kHz_DNFBASE + (LMIC.txChnl & 0x7) * CN490_125kHz_DNFSTEP;
    
    if ( /* TX datarate */txdr < LORAWAN_DR5)
            candidateDr = txdr + 0 - LMIC.rx1DrOffset;
    else
            candidateDr = LORAWAN_DR5 - LMIC.rx1DrOffset;

    if (candidateDr < LORAWAN_DR0)
            candidateDr = LORAWAN_DR0;
    else if (candidateDr > LORAWAN_DR5)
            candidateDr = LORAWAN_DR5;

    LMIC.dndr = candidateDr;
    LMIC.rps = dndr2rps(LMIC.dndr);
}

void LMICcn490_initJoinLoop(void) {
	// LMIC.txParam is set to 0xFF by the central code at init time.
	LMICuslike_initJoinLoop();
	
	// initialize the adrTxPower.
	LMIC.adrTxPow = 19.15; // dBm

}

//
// END: CN490 related stuff
//
// ================================================================================
#endif
