/**
 * @file 		tscadc_hw.h
 * @brief
 * @date 		19.02.2016
 * @author: 	T Jaehnichen
 *				HAW-Hamburg
 *          	Labor für technische Informatik
 *          	Berliner Tor  7
 *          	D-20099 Hamburg
 * @version 	
 * @details
 * @copybrief	Based on the StarterWareFree for AM335X provided by Texas Instrument (copyright notice see below)
 * 				https://sourceforge.net/p/starterwarefree/code/ci/master/tree/include/hw/hw_tsc_adc_ss.h
 * @copydetails Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 * 				Redistribution and use in source and binary forms, with or without
 *  			modification, are permitted provided that the following conditions
 *  			are met:
 *
 *   			  Redistributions of source code must retain the above copyright
 *  			  notice, this list of conditions and the following disclaimer.
 *
 *  			  Redistributions in binary form must reproduce the above copyright
 *  			  notice, this list of conditions and the following disclaimer in the
 *  			  documentation and/or other materials provided with the
 *   			  distribution.
 *
 *  			  Neither the name of Texas Instruments Incorporated nor the names of
 *  			  its contributors may be used to endorse or promote products derived
 *   			  from this software without specific prior written permission.
 *
 *  			THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  			"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  			LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  			A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  			OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  			SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  			LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  			DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  			THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  			(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  			OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/



#ifndef TSCADC_HW_H_
#define TSCADC_HW_H_

enum Fifo : unsigned char {
	FIFO_0	= 0,
	FIFO_1   = 1
};

const unsigned int BASE = 0x44E0D000;
const unsigned int SIZE = 0x00002000;


const unsigned int REVISION  			= 0x0;
const unsigned int SYSCONFIG  			= 0x10;
const unsigned int IRQ_EOI  				= 0x20;
const unsigned int IRQSTATUS_RAW 		= 0x24;
const unsigned int IRQSTATUS   			= 0x28;
const unsigned int IRQENABLE_SET   		= 0x2c;
const unsigned int IRQENABLE_CLR   		= 0x30;
const unsigned int IRQWAKEUP   			= 0x34;
const unsigned int DMAENABLE_SET   		= 0x38;
const unsigned int DMAENABLE_CLR   		= 0x3c;
const unsigned int CTRL   				= 0x40;
const unsigned int ADCSTAT   			= 0x44;
const unsigned int ADCRANGE   			= 0x48;
const unsigned int ADC_CLKDIV   			= 0x4c;
const unsigned int ADC_MISC   			= 0x50;
const unsigned int STEPENABLE   			= 0x54;
const unsigned int IDLECONFIG   			= 0x58;
const unsigned int TS_CHARGE_STEPCONFIG	= 0x5c;
const unsigned int TS_CHARGE_DELAY  		= 0x60;
inline constexpr unsigned int STEPCONFIG(unsigned int n){return( 0x64 + (n * 0x008)); };
inline constexpr unsigned int STEPDELAY(unsigned int n){ return( 0x68 + (n * 0x008)); };
inline constexpr unsigned int FIFOCOUNT(Fifo n){   		return( 0xe4 + (n * 0x00c));   };
inline constexpr unsigned int FIFOTHRESHOLD(Fifo n){  	return( 0xe8 + (n * 0x00c));   };
inline constexpr unsigned int DMAREQ(Fifo n){   			return( 0xec + (n * 0x00c));   };
inline constexpr unsigned int FIFODATA(Fifo n){   		return(0x100 + (n * 0x100)); };

/**************************************************************************\
 * Field Definition Macros
\**************************************************************************/

/* REVISION */
const unsigned int REVISION_CUSTOM   	= 0x000000C0u;
const unsigned int REVISION_CUSTOM_SHIFT	= 0x00000006u;

const unsigned int REVISION_FUNC   		= 0x0FFF0000u;
const unsigned int REVISION_FUNC_SHIFT	= 0x00000010u;

const unsigned int REVISION_R_RTL   		= 0x0000F800u;
const unsigned int REVISION_R_RTL_SHIFT  = 0x0000000Bu;

const unsigned int REVISION_SCHEME   	= 0xC0000000u;
const unsigned int REVISION_SCHEME_SHIFT	= 0x0000001Eu;

const unsigned int REVISION_X_MAJOR   		= 0x00000700u;
const unsigned int REVISION_X_MAJOR_SHIFT	= 0x00000008u;

const unsigned int REVISION_Y_MINOR   		= 0x0000003Fu;
const unsigned int REVISION_Y_MINOR_SHIFT	= 0x00000000u;


/* SYSCONFIG */
const unsigned int SYSCONFIG_IDLEMODE   					= 0x0000000Cu;
const unsigned int SYSCONFIG_IDLEMODE_SHIFT  			= 0x00000002u;
const unsigned int SYSCONFIG_IDLEMODE_FORCE  			= 0x0;
const unsigned int SYSCONFIG_IDLEMODE_NO_IDLE			= 0x1;
const unsigned int SYSCONFIG_IDLEMODE_SMART_IDLE 		= 0x2;
const unsigned int SYSCONFIG_IDLEMODE_SMART_IDLE_WAKEUP	= 0x3;


/* IRQ_EOI */
const unsigned int IRQ_EOI_LINE_NUMBER   	= 0x00000001u;
const unsigned int IRQ_EOI_LINE_NUMBER_SHIFT	= 0x00000000u;


/* IRQSTATUS_RAW */
const unsigned int IRQSTATUS_RAW_END_OF_SEQUENCE   				= 0x00000002u;
const unsigned int IRQSTATUS_RAW_END_OF_SEQUENCE_SHIFT   		= 0x00000001u;
const unsigned int IRQSTATUS_RAW_END_OF_SEQUENCE_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_END_OF_SEQUENCE_NOEVENTPEND   	= 0x0u;
const unsigned int IRQSTATUS_RAW_END_OF_SEQUENCE_SETEVENT   		= 0x1u;

const unsigned int IRQSTATUS_RAW_FIFO0_OVERRUN   			= 0x00000008u;
const unsigned int IRQSTATUS_RAW_FIFO0_OVERRUN_SHIFT   		= 0x00000003u;
const unsigned int IRQSTATUS_RAW_FIFO0_OVERRUN_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_FIFO0_OVERRUN_NOEVENTPEND   = 0x0u;
const unsigned int IRQSTATUS_RAW_FIFO0_OVERRUN_SETEVENT   	= 0x1u;

const unsigned int IRQSTATUS_RAW_FIFO0_THRESHOLD   				= 0x00000004u;
const unsigned int IRQSTATUS_RAW_FIFO0_THRESHOLD_SHIFT   		= 0x00000002u;
const unsigned int IRQSTATUS_RAW_FIFO0_THRESHOLD_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_FIFO0_THRESHOLD_NOEVENTPEND   	= 0x0u;
const unsigned int IRQSTATUS_RAW_FIFO0_THRESHOLD_SETEVENT   		= 0x1u;

const unsigned int IRQSTATUS_RAW_FIFO0_UNDERFLOW   				= 0x00000010u;
const unsigned int IRQSTATUS_RAW_FIFO0_UNDERFLOW_SHIFT   		= 0x00000004u;
const unsigned int IRQSTATUS_RAW_FIFO0_UNDERFLOW_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_FIFO0_UNDERFLOW_NOEVENTPEND   	= 0x0u;
const unsigned int IRQSTATUS_RAW_FIFO0_UNDERFLOW_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_RAW_FIFO1_OVERRUN   			= 0x00000040u;
const unsigned int IRQSTATUS_RAW_FIFO1_OVERRUN_SHIFT   		= 0x00000006u;
const unsigned int IRQSTATUS_RAW_FIFO1_OVERRUN_NOEVENTPEND	= 0x0u;
const unsigned int IRQSTATUS_RAW_FIFO1_OVERRUN_SETEVENT   	= 0x1u;

const unsigned int IRQSTATUS_RAW_FIFO1_THRESHOLD   				= 0x00000020u;
const unsigned int IRQSTATUS_RAW_FIFO1_THRESHOLD_SHIFT   		= 0x00000005u;
const unsigned int IRQSTATUS_RAW_FIFO1_THRESHOLD_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_FIFO1_THRESHOLD_NOEVENTPEND   	= 0x0u;
const unsigned int IRQSTATUS_RAW_FIFO1_THRESHOLD_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_RAW_FIFO1_UNDERFLOW   				= 0x00000080u;
const unsigned int IRQSTATUS_RAW_FIFO1_UNDERFLOW_SHIFT   		= 0x00000007u;
const unsigned int IRQSTATUS_RAW_FIFO1_UNDERFLOW_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_FIFO1_UNDERFLOW_NOEVENTPEND   	= 0x0u;
const unsigned int IRQSTATUS_RAW_FIFO1_UNDERFLOW_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_RAW_HW_PEN_EVENT		= 0x00000001u;
const unsigned int IRQSTATUS_RAW_HW_PEN_EVENT_SHIFT	= 0x00000000u;

const unsigned int IRQSTATUS_RAW_OUT_OF_RANGE				= 0x00000100u;
const unsigned int IRQSTATUS_RAW_OUT_OF_RANGE_SHIFT   		= 0x00000008u;
const unsigned int IRQSTATUS_RAW_OUT_OF_RANGE_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_OUT_OF_RANGE_NOEVENTPEND  	= 0x0u;
const unsigned int IRQSTATUS_RAW_OUT_OF_RANGE_SETEVENT   	= 0x1u;

const unsigned int IRQSTATUS_RAW_PEN_IRQ   				= 0x00000400u;
const unsigned int IRQSTATUS_RAW_PEN_IRQ_SHIFT   		= 0x0000000Au;
const unsigned int IRQSTATUS_RAW_PEN_IRQ_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_PEN_IRQ_NOEVENTPEND   	= 0x0u;
const unsigned int IRQSTATUS_RAW_PEN_IRQ_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_RAW_PEN_UP_EVENT   				= 0x00000200u;
const unsigned int IRQSTATUS_RAW_PEN_UP_EVENT_SHIFT   		= 0x00000009u;
const unsigned int IRQSTATUS_RAW_PEN_UP_EVENT_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_RAW_PEN_UP_EVENT_NOEVENTPEND	= 0x0u;
const unsigned int IRQSTATUS_RAW_PEN_UP_EVENT_SETEVENT		= 0x1u;

/* IRQSTATUS */
const unsigned int IRQSTATUS_END_OF_SEQUENCE					= 0x00000002u;
const unsigned int IRQSTATUS_END_OF_SEQUENCE_SHIFT			= 0x00000001u;
const unsigned int IRQSTATUS_END_OF_SEQUENCE_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_END_OF_SEQUENCE_NOEVENTPEND		= 0x0u;
const unsigned int IRQSTATUS_END_OF_SEQUENCE_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_FIFO0_OVERRUN				= 0x00000008u;
const unsigned int IRQSTATUS_FIFO0_OVERRUN_SHIFT			= 0x00000003u;
const unsigned int IRQSTATUS_FIFO0_OVERRUN_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_FIFO0_OVERRUN_NOEVENTPEND	= 0x0u;
const unsigned int IRQSTATUS_FIFO0_OVERRUN_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_FIFO0_THRESHOLD					= 0x00000004u;
const unsigned int IRQSTATUS_FIFO0_THRESHOLD_SHIFT			= 0x00000002u;
const unsigned int IRQSTATUS_FIFO0_THRESHOLD_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_FIFO0_THRESHOLD_NOEVENTPEND		= 0x0u;
const unsigned int IRQSTATUS_FIFO0_THRESHOLD_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_FIFO0_UNDERFLOW					= 0x00000010u;
const unsigned int IRQSTATUS_FIFO0_UNDERFLOW_SHIFT			= 0x00000004u;
const unsigned int IRQSTATUS_FIFO0_UNDERFLOW_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_FIFO0_UNDERFLOW_NOEVENTPEND		= 0x0u;
const unsigned int IRQSTATUS_FIFO0_UNDERFLOW_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_FIFO1_OVERRUN				= 0x00000040u;
const unsigned int IRQSTATUS_FIFO1_OVERRUN_SHIFT			= 0x00000006u;
const unsigned int IRQSTATUS_FIFO1_OVERRUN_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_FIFO1_OVERRUN_NOEVENTPEND	= 0x0u;
const unsigned int IRQSTATUS_FIFO1_OVERRUN_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_FIFO1_THRESHOLD					= 0x00000020u;
const unsigned int IRQSTATUS_FIFO1_THRESHOLD_SHIFT   		= 0x00000005u;
const unsigned int IRQSTATUS_FIFO1_THRESHOLD_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_FIFO1_THRESHOLD_NOEVENTPEND		= 0x0u;
const unsigned int IRQSTATUS_FIFO1_THRESHOLD_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_FIFO1_UNDERFLOW					= 0x00000080u;
const unsigned int IRQSTATUS_FIFO1_UNDERFLOW_SHIFT			= 0x00000007u;
const unsigned int IRQSTATUS_FIFO1_UNDERFLOW_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_FIFO1_UNDERFLOW_NOEVENTPEND		= 0x0u;
const unsigned int IRQSTATUS_FIFO1_UNDERFLOW_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_HW_PEN_EVENT		= 0x00000001u;
const unsigned int IRQSTATUS_HW_PEN_EVENT_SHIFT	= 0x00000000u;

const unsigned int IRQSTATUS_OUT_OF_RANGE				= 0x00000100u;
const unsigned int IRQSTATUS_OUT_OF_RANGE_SHIFT			= 0x00000008u;
const unsigned int IRQSTATUS_OUT_OF_RANGE_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_OUT_OF_RANGE_NOEVENTPEND	= 0x0u;
const unsigned int IRQSTATUS_OUT_OF_RANGE_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_PEN_IRQ					= 0x00000400u;
const unsigned int IRQSTATUS_PEN_IRQ_SHIFT			= 0x0000000Au;
const unsigned int IRQSTATUS_PEN_IRQ_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_PEN_IRQ_NOEVENTPEND		= 0x0u;
const unsigned int IRQSTATUS_PEN_IRQ_SETEVENT		= 0x1u;

const unsigned int IRQSTATUS_PEN_UP_EVENT				= 0x00000200u;
const unsigned int IRQSTATUS_PEN_UP_EVENT_SHIFT			= 0x00000009u;
const unsigned int IRQSTATUS_PEN_UP_EVENT_EVENTPENDING	= 0x1u;
const unsigned int IRQSTATUS_PEN_UP_EVENT_NOEVENTPEND	= 0x0u;
const unsigned int IRQSTATUS_PEN_UP_EVENT_SETEVENT		= 0x1u;


/* IRQENABLE_SET */
const unsigned int IRQENABLE_SET_END_OF_SEQUENCE				= 0x00000002u;
const unsigned int IRQENABLE_SET_END_OF_SEQUENCE_SHIFT		= 0x00000001u;
const unsigned int IRQENABLE_SET_END_OF_SEQUENCE_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_END_OF_SEQUENCE_ENABLE		= 0x1u;
const unsigned int IRQENABLE_SET_END_OF_SEQUENCE_ENABLED		= 0x1u;

const unsigned int IRQENABLE_SET_FIFO0_OVERRUN			= 0x00000008u;
const unsigned int IRQENABLE_SET_FIFO0_OVERRUN_SHIFT		= 0x00000003u;
const unsigned int IRQENABLE_SET_FIFO0_OVERRUN_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_FIFO0_OVERRUN_ENABLE	= 0x1u;
const unsigned int IRQENABLE_SET_FIFO0_OVERRUN_ENABLED	= 0x1u;

const unsigned int IRQENABLE_SET_FIFO0_THRESHOLD			= 0x00000004u;
const unsigned int IRQENABLE_SET_FIFO0_THRESHOLD_SHIFT	= 0x00000002u;

const unsigned int IRQENABLE_SET_FIFO0_UNDERFLOW				= 0x00000010u;
const unsigned int IRQENABLE_SET_FIFO0_UNDERFLOW_SHIFT		= 0x00000004u;
const unsigned int IRQENABLE_SET_FIFO0_UNDERFLOW_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_FIFO0_UNDERFLOW_ENABLE		= 0x1u;
const unsigned int IRQENABLE_SET_FIFO0_UNDERFLOW_ENABLED		= 0x1u;

const unsigned int IRQENABLE_SET_FIFO1_OVERRUN				= 0x00000040u;
const unsigned int IRQENABLE_SET_FIFO1_OVERRUN_SHIFT			= 0x00000006u;
const unsigned int IRQENABLE_SET_FIFO1_OVERRUN_DISABLED		= 0x0u;
const unsigned int IRQENABLE_SET_FIFO1_OVERRUN_ENABLE		= 0x1u;
const unsigned int IRQENABLE_SET_FIFO1_OVERRUN_ENABLED		= 0x1u;

const unsigned int IRQENABLE_SET_FIFO1_THRESHOLD				= 0x00000020u;
const unsigned int IRQENABLE_SET_FIFO1_THRESHOLD_SHIFT		= 0x00000005u;
const unsigned int IRQENABLE_SET_FIFO1_THRESHOLD_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_FIFO1_THRESHOLD_ENABLE		= 0x1u;
const unsigned int IRQENABLE_SET_FIFO1_THRESHOLD_ENABLED		= 0x1u;

const unsigned int IRQENABLE_SET_FIFO1_UNDERFLOW				= 0x00000080u;
const unsigned int IRQENABLE_SET_FIFO1_UNDERFLOW_SHIFT		= 0x00000007u;
const unsigned int IRQENABLE_SET_FIFO1_UNDERFLOW_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_FIFO1_UNDERFLOW_ENABLE		= 0x1u;
const unsigned int IRQENABLE_SET_FIFO1_UNDERFLOW_ENABLED		= 0x1u;

const unsigned int IRQENABLE_SET_HW_PEN_EVENT_ASYNC			= 0x00000001u;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_ASYNC_SHIFT	= 0x00000000u;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_ASYNC_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_ASYNC_ENABLE	= 0x1u;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_ASYNC_ENABLED	= 0x1u;

const unsigned int IRQENABLE_SET_HW_PEN_EVENT_SYNC			= 0x00000400u;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_SYNC_SHIFT		= 0x0000000Au;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_SYNC_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_SYNC_ENABLE	= 0x1u;
const unsigned int IRQENABLE_SET_HW_PEN_EVENT_SYNC_ENABLED	= 0x1u;

const unsigned int IRQENABLE_SET_OUT_OF_RANGE			= 0x00000100u;
const unsigned int IRQENABLE_SET_OUT_OF_RANGE_SHIFT		= 0x00000008u;
const unsigned int IRQENABLE_SET_OUT_OF_RANGE_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_OUT_OF_RANGE_ENABLE		= 0x1u;
const unsigned int IRQENABLE_SET_OUT_OF_RANGE_ENABLED	= 0x1u;

const unsigned int IRQENABLE_SET_PEN_UP_EVENT			= 0x00000200u;
const unsigned int IRQENABLE_SET_PEN_UP_EVENT_SHIFT		= 0x00000009u;
const unsigned int IRQENABLE_SET_PEN_UP_EVENT_DISABLED	= 0x0u;
const unsigned int IRQENABLE_SET_PEN_UP_EVENT_ENABLE		= 0x1u;
const unsigned int IRQENABLE_SET_PEN_UP_EVENT_ENABLED	= 0x1u;


/* IRQENABLE_CLR */
const unsigned int IRQENABLE_CLR_END_OF_SEQUENCE				= 0x00000002u;
const unsigned int IRQENABLE_CLR_END_OF_SEQUENCE_SHIFT		= 0x00000001u;
const unsigned int IRQENABLE_CLR_END_OF_SEQUENCE_DISABLED	= 0x0u;
const unsigned int IRQENABLE_CLR_END_OF_SEQUENCE_ENABLE		= 0x1u;
const unsigned int IRQENABLE_CLR_END_OF_SEQUENCE_ENABLED		= 0x1u;

const unsigned int IRQENABLE_CLR_FIFO0_OVERRUN			= 0x00000008u;
const unsigned int IRQENABLE_CLR_FIFO0_OVERRUN_SHIFT		= 0x00000003u;
const unsigned int IRQENABLE_CLR_FIFO0_OVERRUN_DISABLED	= 0x0u;
const unsigned int IRQENABLE_CLR_FIFO0_OVERRUN_ENABLE	= 0x1u;
const unsigned int IRQENABLE_CLR_FIFO0_OVERRUN_ENABLED	= 0x1u;

const unsigned int IRQENABLE_CLR_FIFO0_THRESHOLD			= 0x00000004u;
const unsigned int IRQENABLE_CLR_FIFO0_THRESHOLD_SHIFT	= 0x00000002u;

const unsigned int IRQENABLE_CLR_FIFO0_UNDERFLOW				= 0x00000010u;
const unsigned int IRQENABLE_CLR_FIFO0_UNDERFLOW_SHIFT		= 0x00000004u;
const unsigned int IRQENABLE_CLR_FIFO0_UNDERFLOW_DISABLED	= 0x0u;
const unsigned int IRQENABLE_CLR_FIFO0_UNDERFLOW_ENABLE		= 0x1u;
const unsigned int IRQENABLE_CLR_FIFO0_UNDERFLOW_ENABLED		= 0x1u;

const unsigned int IRQENABLE_CLR_FIFO1_OVERRUN   = 0x00000040u;
const unsigned int IRQENABLE_CLR_FIFO1_OVERRUN_SHIFT   = 0x00000006u;
const unsigned int IRQENABLE_CLR_FIFO1_OVERRUN_DISABLED   = 0x0u;
const unsigned int IRQENABLE_CLR_FIFO1_OVERRUN_ENABLE   = 0x1u;
const unsigned int IRQENABLE_CLR_FIFO1_OVERRUN_ENABLED   = 0x1u;

const unsigned int IRQENABLE_CLR_FIFO1_THRESHOLD   = 0x00000020u;
const unsigned int IRQENABLE_CLR_FIFO1_THRESHOLD_SHIFT   = 0x00000005u;
const unsigned int IRQENABLE_CLR_FIFO1_THRESHOLD_DISABLED   = 0x0u;
const unsigned int IRQENABLE_CLR_FIFO1_THRESHOLD_ENABLE   = 0x1u;
const unsigned int IRQENABLE_CLR_FIFO1_THRESHOLD_ENABLED   = 0x1u;

const unsigned int IRQENABLE_CLR_FIFO1_UNDERFLOW   = 0x00000080u;
const unsigned int IRQENABLE_CLR_FIFO1_UNDERFLOW_SHIFT   = 0x00000007u;
const unsigned int IRQENABLE_CLR_FIFO1_UNDERFLOW_DISABLED   = 0x0u;
const unsigned int IRQENABLE_CLR_FIFO1_UNDERFLOW_ENABLE   = 0x1u;
const unsigned int IRQENABLE_CLR_FIFO1_UNDERFLOW_ENABLED   = 0x1u;

const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_ASYNC   = 0x00000001u;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_ASYNC_SHIFT   = 0x00000000u;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_ASYNC_DISABLED   = 0x0u;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_ASYNC_ENABLE   = 0x1u;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_ASYNC_ENABLED   = 0x1u;

const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_SYNC   = 0x00000400u;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_SYNC_SHIFT   = 0x0000000Au;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_SYNC_DISABLED   = 0x0u;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_SYNC_ENABLE   = 0x1u;
const unsigned int IRQENABLE_CLR_HW_PEN_EVENT_SYNC_ENABLED   = 0x1u;

const unsigned int IRQENABLE_CLR_OUT_OF_RANGE   = 0x00000100u;
const unsigned int IRQENABLE_CLR_OUT_OF_RANGE_SHIFT   = 0x00000008u;
const unsigned int IRQENABLE_CLR_OUT_OF_RANGE_DISABLED   = 0x0u;
const unsigned int IRQENABLE_CLR_OUT_OF_RANGE_ENABLE   = 0x1u;
const unsigned int IRQENABLE_CLR_OUT_OF_RANGE_ENABLED   = 0x1u;

const unsigned int IRQENABLE_CLR_PEN_UP_EVENT   = 0x00000200u;
const unsigned int IRQENABLE_CLR_PEN_UP_EVENT_SHIFT   = 0x00000009u;
const unsigned int IRQENABLE_CLR_PEN_UP_EVENT_DISABLED   = 0x0u;
const unsigned int IRQENABLE_CLR_PEN_UP_EVENT_ENABLE   = 0x1u;
const unsigned int IRQENABLE_CLR_PEN_UP_EVENT_ENABLED   = 0x1u;


/* IRQWAKEUP */
const unsigned int IRQWAKEUP_WAKEEN0   = 0x00000001u;
const unsigned int IRQWAKEUP_WAKEEN0_SHIFT   = 0x00000000u;


/* DMAENABLE_SET */
const unsigned int DMAENABLE_SET_ENABLE0   = 0x00000001u;
const unsigned int DMAENABLE_SET_ENABLE0_SHIFT   = 0x00000000u;

const unsigned int DMAENABLE_SET_ENABLE1   = 0x00000002u;
const unsigned int DMAENABLE_SET_ENABLE1_SHIFT   = 0x00000001u;


/* DMAENABLE_CLR */
const unsigned int DMAENABLE_CLR_ENABLE0   = 0x00000001u;
const unsigned int DMAENABLE_CLR_ENABLE0_SHIFT   = 0x00000000u;

const unsigned int DMAENABLE_CLR_ENABLE1   = 0x00000002u;
const unsigned int DMAENABLE_CLR_ENABLE1_SHIFT   = 0x00000001u;


/* CTRL */
const unsigned int CTRL_ADC_BIAS_SELECT   = 0x00000008u;
const unsigned int CTRL_ADC_BIAS_SELECT_SHIFT   = 0x00000003u;
const unsigned int CTRL_ADC_BIAS_SELECT_EXTERNAL   = 0x1u;
const unsigned int CTRL_ADC_BIAS_SELECT_INTERNAL   = 0x0u;

const unsigned int CTRL_AFE_PEN_CTRL   = 0x00000060u;
const unsigned int CTRL_AFE_PEN_CTRL_SHIFT   = 0x00000005u;

const unsigned int CTRL_ENABLE   = 0x00000001u;
const unsigned int CTRL_ENABLE_SHIFT   = 0x00000000u;
const unsigned int CTRL_ENABLE_DISABLE   = 0x0u;
const unsigned int CTRL_ENABLE_ENABLE   = 0x1u;

const unsigned int CTRL_HW_EVENT_MAPPING   = 0x00000100u;
const unsigned int CTRL_HW_EVENT_MAPPING_SHIFT   = 0x00000008u;
const unsigned int CTRL_HW_EVENT_MAPPING_HWEVENTINPUT   = 0x1u;
const unsigned int CTRL_HW_EVENT_MAPPING_PENTOUCHIRQ   = 0x0u;

const unsigned int CTRL_HW_PREEMPT   = 0x00000200u;
const unsigned int CTRL_HW_PREEMPT_SHIFT   = 0x00000009u;
const unsigned int CTRL_HW_PREEMPT_NOPREEMPT   = 0x0u;
const unsigned int CTRL_HW_PREEMPT_PREEMPT   = 0x1u;

const unsigned int CTRL_POWER_DOWN   = 0x00000010u;
const unsigned int CTRL_POWER_DOWN_SHIFT   = 0x00000004u;
const unsigned int CTRL_POWER_DOWN_AFEPOWERDOWN   = 0x1u;
const unsigned int CTRL_POWER_DOWN_AFEPOWERUP   = 0x0u;

const unsigned int CTRL_STEPCONFIG_WRITEPROTECT_N   = 0x00000004u;
const unsigned int CTRL_STEPCONFIG_WRITEPROTECT_N_SHIFT   = 0x00000002u;
const unsigned int CTRL_STEPCONFIG_WRITEPROTECT_N_NOTPROTECTED   = 0x1u;
const unsigned int CTRL_STEPCONFIG_WRITEPROTECT_N_PROTECTED   = 0x0u;

const unsigned int CTRL_STERP_ID_TAG   = 0x00000002u;
const unsigned int CTRL_STERP_ID_TAG_SHIFT   = 0x00000001u;
const unsigned int CTRL_STERP_ID_TAG_CHANNELID   = 0x1u;
const unsigned int CTRL_STERP_ID_TAG_WRZERO   = 0x0u;

const unsigned int CTRL_TOUCH_SCREEN_ENABLE   = 0x00000080u;
const unsigned int CTRL_TOUCH_SCREEN_ENABLE_SHIFT   = 0x00000007u;
const unsigned int CTRL_TOUCH_SCREEN_ENABLE_DISABLED   = 0x0u;
const unsigned int CTRL_TOUCH_SCREEN_ENABLE_ENABLED   = 0x1u;


/* ADCSTAT */
const unsigned int ADCSTAT_FSM_BUSY   = 0x00000020u;
const unsigned int ADCSTAT_FSM_BUSY_SHIFT   = 0x00000005u;
const unsigned int ADCSTAT_FSM_BUSY_BUSY   = 0x1u;
const unsigned int ADCSTAT_FSM_BUSY_IDLE   = 0x0u;

const unsigned int ADCSTAT_PEN_IRQ0   = 0x00000040u;
const unsigned int ADCSTAT_PEN_IRQ0_SHIFT   = 0x00000006u;

const unsigned int ADCSTAT_PEN_IRQ1   = 0x00000080u;
const unsigned int ADCSTAT_PEN_IRQ1_SHIFT   = 0x00000007u;

const unsigned int ADCSTAT_STEPID   = 0x0000001Fu;
const unsigned int ADCSTAT_STEPID_SHIFT   = 0x00000000u;
const unsigned int ADCSTAT_STEPID_CHARGE   = 0x11u;
const unsigned int ADCSTAT_STEPID_IDLE   = 0x10u;
const unsigned int ADCSTAT_STEPID_STEP1   = 0x0u;
const unsigned int ADCSTAT_STEPID_STEP10   = 0x9u;
const unsigned int ADCSTAT_STEPID_STEP11   = 0xAu;
const unsigned int ADCSTAT_STEPID_STEP12   = 0xBu;
const unsigned int ADCSTAT_STEPID_STEP13   = 0xCu;
const unsigned int ADCSTAT_STEPID_STEP14   = 0xDu;
const unsigned int ADCSTAT_STEPID_STEP15   = 0xEu;
const unsigned int ADCSTAT_STEPID_STEP16   = 0xFu;
const unsigned int ADCSTAT_STEPID_STEP2   = 0x1u;
const unsigned int ADCSTAT_STEPID_STEP3   = 0x2u;
const unsigned int ADCSTAT_STEPID_STEP4   = 0x3u;
const unsigned int ADCSTAT_STEPID_STEP5   = 0x4u;
const unsigned int ADCSTAT_STEPID_STEP6   = 0x5u;
const unsigned int ADCSTAT_STEPID_STEP7   = 0x6u;
const unsigned int ADCSTAT_STEPID_STEP8   = 0x7u;
const unsigned int ADCSTAT_STEPID_STEP9   = 0x8u;


/* ADCRANGE */
const unsigned int ADCRANGE_HIGH_RANGE_DATA   = 0x0FFF0000u;
const unsigned int ADCRANGE_HIGH_RANGE_DATA_SHIFT   = 0x00000010u;

const unsigned int ADCRANGE_LOW_RANGE_DATA   = 0x00000FFFu;
const unsigned int ADCRANGE_LOW_RANGE_DATA_SHIFT   = 0x00000000u;


/* ADC_CLKDIV */
const unsigned int ADC_CLKDIV_ADC_CLK_DIV   = 0x00007FFFu;
const unsigned int ADC_CLKDIV_ADC_CLK_DIV_SHIFT   = 0x00000000u;


/* ADC_MISC */
const unsigned int ADC_MISC_AFE_SPARE_INPUT   = 0x0000000Fu;
const unsigned int ADC_MISC_AFE_SPARE_INPUT_SHIFT   = 0x00000000u;

const unsigned int ADC_MISC_AFE_SPARE_OUTPUT   = 0x000000F0u;
const unsigned int ADC_MISC_AFE_SPARE_OUTPUT_SHIFT   = 0x00000004u;


/* STEPENABLE */
const unsigned int STEPENABLE_STEP1   = 0x00000002u;
const unsigned int STEPENABLE_STEP1_SHIFT   = 0x00000001u;

const unsigned int STEPENABLE_STEP10   = 0x00000400u;
const unsigned int STEPENABLE_STEP10_SHIFT   = 0x0000000Au;

const unsigned int STEPENABLE_STEP11   = 0x00000800u;
const unsigned int STEPENABLE_STEP11_SHIFT   = 0x0000000Bu;

const unsigned int STEPENABLE_STEP12   = 0x00001000u;
const unsigned int STEPENABLE_STEP12_SHIFT   = 0x0000000Cu;

const unsigned int STEPENABLE_STEP13   = 0x00002000u;
const unsigned int STEPENABLE_STEP13_SHIFT   = 0x0000000Du;

const unsigned int STEPENABLE_STEP14   = 0x00004000u;
const unsigned int STEPENABLE_STEP14_SHIFT   = 0x0000000Eu;

const unsigned int STEPENABLE_STEP15   = 0x00008000u;
const unsigned int STEPENABLE_STEP15_SHIFT   = 0x0000000Fu;

const unsigned int STEPENABLE_STEP16   = 0x00010000u;
const unsigned int STEPENABLE_STEP16_SHIFT   = 0x00000010u;

const unsigned int STEPENABLE_STEP2   = 0x00000004u;
const unsigned int STEPENABLE_STEP2_SHIFT   = 0x00000002u;

const unsigned int STEPENABLE_STEP3   = 0x00000008u;
const unsigned int STEPENABLE_STEP3_SHIFT   = 0x00000003u;

const unsigned int STEPENABLE_STEP4   = 0x00000010u;
const unsigned int STEPENABLE_STEP4_SHIFT   = 0x00000004u;

const unsigned int STEPENABLE_STEP5   = 0x00000020u;
const unsigned int STEPENABLE_STEP5_SHIFT   = 0x00000005u;

const unsigned int STEPENABLE_STEP6   = 0x00000040u;
const unsigned int STEPENABLE_STEP6_SHIFT   = 0x00000006u;

const unsigned int STEPENABLE_STEP7   = 0x00000080u;
const unsigned int STEPENABLE_STEP7_SHIFT   = 0x00000007u;

const unsigned int STEPENABLE_STEP8   = 0x00000100u;
const unsigned int STEPENABLE_STEP8_SHIFT   = 0x00000008u;

const unsigned int STEPENABLE_STEP9   = 0x00000200u;
const unsigned int STEPENABLE_STEP9_SHIFT   = 0x00000009u;

const unsigned int STEPENABLE_TS_CHARGE   = 0x00000001u;
const unsigned int STEPENABLE_TS_CHARGE_SHIFT   = 0x00000000u;


/* IDLECONFIG */
const unsigned int IDLECONFIG_DIFF_CNTRL   = 0x02000000u;
const unsigned int IDLECONFIG_DIFF_CNTRL_SHIFT   = 0x00000019u;
const unsigned int IDLECONFIG_DIFF_CNTRL_DIFFERENTIAL   = 0x1u;
const unsigned int IDLECONFIG_DIFF_CNTRL_SINGLE   = 0x0u;

const unsigned int IDLECONFIG_SEL_INM_SWM   = 0x00078000u;
const unsigned int IDLECONFIG_SEL_INM_SWM_SHIFT   = 0x0000000Fu;
const unsigned int IDLECONFIG_SEL_INM_SWM_CHANNEL_1   = 0x000u;
const unsigned int IDLECONFIG_SEL_INM_SWM_CHANNEL_8   = 0x0111u;

const unsigned int IDLECONFIG_SEL_INP_SWC   = 0x00780000u;
const unsigned int IDLECONFIG_SEL_INP_SWC_SHIFT   = 0x00000013u;
const unsigned int IDLECONFIG_SEL_INP_SWC_CHANNEL_1   = 0x000u;
const unsigned int IDLECONFIG_SEL_INP_SWC_CHANNEL_8   = 0x0111u;

const unsigned int IDLECONFIG_SEL_RFM_SWC   = 0x01800000u;
const unsigned int IDLECONFIG_SEL_RFM_SWC_SHIFT   = 0x00000017u;
const unsigned int IDLECONFIG_SEL_RFM_SWC_ADCREFM   = 0x3u;
const unsigned int IDLECONFIG_SEL_RFM_SWC_VSSA   = 0x0u;
const unsigned int IDLECONFIG_SEL_RFM_SWC_XNUR   = 0x1u;
const unsigned int IDLECONFIG_SEL_RFM_SWC_YNLR   = 0x2u;

const unsigned int IDLECONFIG_SEL_RFP_SWC   = 0x00007000u;
const unsigned int IDLECONFIG_SEL_RFP_SWC_SHIFT   = 0x0000000Cu;
const unsigned int IDLECONFIG_SEL_RFP_SWC_ADCREFP   = 0x3u;
const unsigned int IDLECONFIG_SEL_RFP_SWC_VDDA   = 0x0u;
const unsigned int IDLECONFIG_SEL_RFP_SWC_XPUL   = 0x1u;
const unsigned int IDLECONFIG_SEL_RFP_SWC_YPLL   = 0x2u;

const unsigned int IDLECONFIG_WPNSW_SWC   = 0x00000800u;
const unsigned int IDLECONFIG_WPNSW_SWC_SHIFT   = 0x0000000Bu;

const unsigned int IDLECONFIG_XNNSW_SWC   = 0x00000040u;
const unsigned int IDLECONFIG_XNNSW_SWC_SHIFT   = 0x00000006u;

const unsigned int IDLECONFIG_XNPSW_SWC   = 0x00000200u;
const unsigned int IDLECONFIG_XNPSW_SWC_SHIFT   = 0x00000009u;

const unsigned int IDLECONFIG_XPPSW_SWC   = 0x00000020u;
const unsigned int IDLECONFIG_XPPSW_SWC_SHIFT   = 0x00000005u;

const unsigned int IDLECONFIG_YNNSW_SWC   = 0x00000100u;
const unsigned int IDLECONFIG_YNNSW_SWC_SHIFT   = 0x00000008u;

const unsigned int IDLECONFIG_YPNSW_SWC   = 0x00000400u;
const unsigned int IDLECONFIG_YPNSW_SWC_SHIFT   = 0x0000000Au;

const unsigned int IDLECONFIG_YPPSW_SWC   = 0x00000080u;
const unsigned int IDLECONFIG_YPPSW_SWC_SHIFT   = 0x00000007u;


/* TS_CHARGE_STEPCONFIG */
const unsigned int TS_CHARGE_STEPCONFIG_DIFF_CNTRL   = 0x02000000u;
const unsigned int TS_CHARGE_STEPCONFIG_DIFF_CNTRL_SHIFT   = 0x00000019u;
const unsigned int TS_CHARGE_STEPCONFIG_DIFF_CNTRL_DIFFERENTIAL   = 0x1u;
const unsigned int TS_CHARGE_STEPCONFIG_DIFF_CNTRL_SINGLE   = 0x0u;

const unsigned int TS_CHARGE_STEPCONFIG_SEL_INM_SWM   = 0x00078000u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_INM_SWM_SHIFT   = 0x0000000Fu;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_INM_SWM_CHANNEL_1   = 0x000u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_INM_SWM_CHANNEL_8   = 0x0111u;

const unsigned int TS_CHARGE_STEPCONFIG_SEL_INP_SWC   = 0x00780000u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_INP_SWC_SHIFT   = 0x00000013u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_INP_SWC_CHANNEL_1   = 0x000u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_INP_SWC_CHANNEL_8   = 0x0111u;

const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFM_SWC   = 0x01800000u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFM_SWC_SHIFT   = 0x00000017u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFM_SWC_ADCREFM   = 0x3u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFM_SWC_VSSA   = 0x0u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFM_SWC_XNUR   = 0x1u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFM_SWC_YNLR   = 0x2u;

const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFP_SWC   = 0x00007000u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFP_SWC_SHIFT   = 0x0000000Cu;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFP_SWC_ADCREFP   = 0x3u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFP_SWC_VDDA   = 0x0u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFP_SWC_XPUL   = 0x1u;
const unsigned int TS_CHARGE_STEPCONFIG_SEL_RFP_SWC_YPLL   = 0x2u;

const unsigned int TS_CHARGE_STEPCONFIG_WPNSW_SWC   = 0x00000800u;
const unsigned int TS_CHARGE_STEPCONFIG_WPNSW_SWC_SHIFT   = 0x0000000Bu;

const unsigned int TS_CHARGE_STEPCONFIG_XNNSW_SWC   = 0x00000040u;
const unsigned int TS_CHARGE_STEPCONFIG_XNNSW_SWC_SHIFT   = 0x00000006u;

const unsigned int TS_CHARGE_STEPCONFIG_XNPSW_SWC   = 0x00000200u;
const unsigned int TS_CHARGE_STEPCONFIG_XNPSW_SWC_SHIFT   = 0x00000009u;

const unsigned int TS_CHARGE_STEPCONFIG_XPPSW_SWC   = 0x00000020u;
const unsigned int TS_CHARGE_STEPCONFIG_XPPSW_SWC_SHIFT   = 0x00000005u;

const unsigned int TS_CHARGE_STEPCONFIG_YNNSW_SWC   = 0x00000100u;
const unsigned int TS_CHARGE_STEPCONFIG_YNNSW_SWC_SHIFT   = 0x00000008u;

const unsigned int TS_CHARGE_STEPCONFIG_YPNSW_SWC   = 0x00000400u;
const unsigned int TS_CHARGE_STEPCONFIG_YPNSW_SWC_SHIFT   = 0x0000000Au;

const unsigned int TS_CHARGE_STEPCONFIG_YPPSW_SWC   = 0x00000080u;
const unsigned int TS_CHARGE_STEPCONFIG_YPPSW_SWC_SHIFT   = 0x00000007u;


/* TS_CHARGE_DELAY */
const unsigned int TS_CHARGE_DELAY_OPEN_DELAY   = 0x0003FFFFu;
const unsigned int TS_CHARGE_DELAY_OPEN_DELAY_SHIFT   = 0x00000000u;


/* STEPCONFIG */
const unsigned int  STEPCONFIG_AVERAGING   = 0x0000001Cu;
const unsigned int  STEPCONFIG_AVERAGING_SHIFT   = 0x00000002u;
const unsigned int  STEPCONFIG_AVERAGING_16SAMPLESAVG   = 0x4u;
const unsigned int  STEPCONFIG_AVERAGING_2SAMPLESAVG   = 0x1u;
const unsigned int  STEPCONFIG_AVERAGING_4SAMPLESAVG   = 0x2u;
const unsigned int  STEPCONFIG_AVERAGING_8SAMPLESAVG   = 0x3u;
const unsigned int  STEPCONFIG_AVERAGING_NOAVG   = 0x0u;

const unsigned int  STEPCONFIG_DIFF_CNTRL   = 0x02000000u;
const unsigned int  STEPCONFIG_DIFF_CNTRL_SHIFT   = 0x00000019u;

const unsigned int  STEPCONFIG_FIFO_SELECT   = 0x04000000u;
const unsigned int  STEPCONFIG_FIFO_SELECT_SHIFT   = 0x0000001Au;
const unsigned int  STEPCONFIG_FIFO_SELECT_FIFO_0   = 0x0u;
const unsigned int  STEPCONFIG_FIFO_SELECT_FIFO_1= 0x1u;

const unsigned int  STEPCONFIG_MODE					    = 0x00000003u;
const unsigned int  STEPCONFIG_MODE_SHIFT   				= 0x00000000u;
const unsigned int  STEPCONFIG_MODE_HW_SYNC_CONTINUOUS	= 0x3u;
const unsigned int  STEPCONFIG_MODE_HW_SYNC_ONESHOT   	= 0x2u;
const unsigned int  STEPCONFIG_MODE_SW_EN_CONTINUOUS   	= 0x1u;
const unsigned int  STEPCONFIG_MODE_SW_EN_ONESHOT   		= 0x0u;

const unsigned int  STEPCONFIG_RANGE_CHECK   		= 0x08000000u;
const unsigned int  STEPCONFIG_RANGE_CHECK_SHIFT   	= 0x0000001Bu;
const unsigned int  STEPCONFIG_RANGE_CHECK_DISABLE   = 0x0u;
const unsigned int  STEPCONFIG_RANGE_CHECK_ENABLE	= 0x1u;

const unsigned int  STEPCONFIG_SEL_INM_SWM   	= 0x00078000u;
const unsigned int  STEPCONFIG_SEL_INM_SWM_SHIFT = 0x0000000Fu;

const unsigned int  STEPCONFIG_SEL_INP_SWC   	= 0x00780000u;
const unsigned int  STEPCONFIG_SEL_INP_SWC_SHIFT = 0x00000013u;

const unsigned int  STEPCONFIG_SEL_RFM_SWC   	= 0x01800000u;
const unsigned int  STEPCONFIG_SEL_RFM_SWC_SHIFT = 0x00000017u;

const unsigned int  STEPCONFIG_SEL_RFP_SWC   	= 0x00007000u;
const unsigned int  STEPCONFIG_SEL_RFP_SWC_SHIFT	= 0x0000000Cu;

const unsigned int  STEPCONFIG_WPNSW_SWC   		= 0x00000800u;
const unsigned int  STEPCONFIG_WPNSW_SWC_SHIFT   = 0x0000000Bu;

const unsigned int  STEPCONFIG_XNNSW_SWC   		= 0x00000040u;
const unsigned int  STEPCONFIG_XNNSW_SWC_SHIFT   = 0x00000006u;

const unsigned int  STEPCONFIG_XNPSW_SWC   		= 0x00000200u;
const unsigned int  STEPCONFIG_XNPSW_SWC_SHIFT   = 0x00000009u;

const unsigned int  STEPCONFIG_XPPSW_SWC   		= 0x00000020u;
const unsigned int  STEPCONFIG_XPPSW_SWC_SHIFT   = 0x00000005u;

const unsigned int  STEPCONFIG_YNNSW_SWC   		= 0x00000100u;
const unsigned int  STEPCONFIG_YNNSW_SWC_SHIFT	= 0x00000008u;

const unsigned int  STEPCONFIG_YPNSW_SWC   		= 0x00000400u;
const unsigned int  STEPCONFIG_YPNSW_SWC_SHIFT	= 0x0000000Au;

const unsigned int  STEPCONFIG_YPPSW_SWC   		= 0x00000080u;
const unsigned int  STEPCONFIG_YPPSW_SWC_SHIFT	= 0x00000007u;


/* STEPDELAY */
const unsigned int  STEPDELAY_OPEN_DELAY   		= 0x0003FFFFu;
const unsigned int  STEPDELAY_OPEN_DELAY_SHIFT	= 0x00000000u;

const unsigned int  STEPDELAY_SAMPLE_DELAY   	= 0xFF000000u;
const unsigned int  STEPDELAY_SAMPLE_DELAY_SHIFT	= 0x00000018u;


/* FIFO0COUNT */
const unsigned int FIFO0COUNT_WORDS_IN_FIFO0   		= 0x0000007Fu;
const unsigned int FIFO0COUNT_WORDS_IN_FIFO0_SHIFT	= 0x00000000u;


/* FIFO0THRESHOLD */
const unsigned int FIFO0THRESHOLD_FIFO0_THRESHOLD_LEVEL   		= 0x0000003Fu;
const unsigned int FIFO0THRESHOLD_FIFO0_THRESHOLD_LEVEL_SHIFT	= 0x00000000u;


/* DMA0REQ */
const unsigned int DMA0REQ_DMA_REQUEST_LEVEL   		= 0x0000003Fu;
const unsigned int DMA0REQ_DMA_REQUEST_LEVEL_SHIFT	= 0x00000000u;


/* FIFO1COUNT */
const unsigned int FIFO1COUNT_WORDS_IN_FIFO1   		= 0x0000007Fu;
const unsigned int FIFO1COUNT_WORDS_IN_FIFO1_SHIFT	= 0x00000000u;


/* FIFO1THRESHOLD */
const unsigned int FIFO1THRESHOLD_FIFO1_THRESHOLD_LEVEL   		= 0x0000003Fu;
const unsigned int FIFO1THRESHOLD_FIFO1_THRESHOLD_LEVEL_SHIFT	= 0x00000000u;


/* DMA1REQ */
const unsigned int DMA1REQ_DMA_REQUEST_LEVEL   		= 0x0000003Fu;
const unsigned int DMA1REQ_DMA_REQUEST_LEVEL_SHIFT	= 0x00000000u;


/* FIFO0DATA */
const unsigned int FIFODATA_ADCCHLNID   		= 0x000F0000u;
const unsigned int FIFODATA_ADCCHLNID_SHIFT	= 0x00000010u;

const unsigned int FIFODATA_ADC_DATA   		= 0x00000FFFu;
const unsigned int FIFODATA_ADC_DATA_SHIFT	= 0x00000000u;



#endif /* TSCADC_HW_H_ */
