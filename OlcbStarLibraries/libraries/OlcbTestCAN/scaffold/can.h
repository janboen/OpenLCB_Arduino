// coding: utf-8
// ----------------------------------------------------------------------------
/*
 * Copyright (c) 2007 Fabian Greif, Roboterclub Aachen e.V.
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
/**
 * \file    can.h
 * \brief   Header-Datei für das allgemeine CAN Interface
 */
// ----------------------------------------------------------------------------

// $Id$ 


#ifndef CAN_H
#define CAN_H

#include <stdint.h>

#if defined (__cplusplus)
	extern "C" {
#endif

#define SUPPORT_EXTENDED_CANID 1


typedef struct
{
	#if SUPPORT_EXTENDED_CANID	
		uint32_t id;				//!< ID der Nachricht (11 oder 29 Bit)
		struct {
			int rtr : 1;			//!< Remote-Transmit-Request-Frame?
			int extended : 1;		//!< extended ID?
		} flags;
	#else
		uint16_t id;				//!< ID der Nachricht (11 Bit)
		struct {
			int rtr : 1;			//!< Remote-Transmit-Request-Frame?
		} flags;
	#endif
	
	uint8_t length;				//!< Anzahl der Datenbytes
	uint8_t data[8];			//!< Die Daten der CAN Nachricht
	
	#if SUPPORT_TIMESTAMPS
		uint16_t timestamp;
	#endif
} tCAN;


#define	BITRATE_125_KBPS	4
extern bool can_init(uint8_t bitrate);
extern uint8_t can_send_message(const tCAN *msg);
extern uint8_t can_get_message(tCAN *msg);


#if defined (__cplusplus)
}
#endif

#endif // CAN_H
