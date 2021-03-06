/***(C)2011***************************************************************
*
* Copyright (C) 2011 MIPS Tech, LLC
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived from this
* software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
****(C)2011**************************************************************/

/*************************************************************************
*
*   Description:	Warning test
*
*************************************************************************/

/*
 * This test checks that warnings work
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MEOS.h"

#define TSTACKSIZE 2000		/* MEOS timer task stack size */
#define STACKSIZE 1000
#define PRIORITIES 5
#define MAX_PRIORITY (PRIORITIES -1)

static KRN_TASK_T *bgtask;
static uint32_t timestack[TSTACKSIZE];
uint32_t istack[STACKSIZE];

static KRN_SCHEDULE_T sched;
static KRN_TASKQ_T schedQueues[PRIORITIES];

int32_t passed = -1;		/* Zero success, non-zero fail */

#define warn "Success"
#define nowarn "Can't happen!"

#ifdef __cplusplus
extern "C" {
#endif
	int32_t _DBG_insist(const char *file, const int line,
			    const char *message, ...);
#ifdef __cplusplus
}
#endif
int32_t _DBG_insist(const char *file, const int line, const char *message, ...)
{
	if (strcmp(message, warn) == 0) {
		DBG_logF("Warned on failure :)\n");
		/* Should happen - test succeeds */
		passed = 0;
		return 0;
	} else if (strcmp(message, nowarn) == 0) {
		/* Shouldn't happen - test fails */
		DBG_logF("Warned on success!\n");
		passed = -1;
	} else {
		/* Must have come from else where */
		(void)file;
		(void)line;
	}
	return 1;
}

/*
** FUNCTION:      main
**
** DESCRIPTION:   C main program for thread 1
**
** RETURNS:       int
*/
int main()
{
	DBG_logF("Warning Test\n");
	KRN_reset(&sched, schedQueues, MAX_PRIORITY, 0x73117a1e, istack,
		  STACKSIZE, NULL, 0);
	bgtask = KRN_startOS("Background Task");
	KRN_startTimerTask("Timer Task", timestack, TSTACKSIZE);
	BSP_init();
	/* Should happen - pass if it does */
	DBG_logF("Insist 0\n");
	DBG_insist(0, warn);
	/* Shouldn't happen - fail if it does */
	DBG_logF("Insist 1\n");
	DBG_insist(1, nowarn);
	DBG_logF("Done %" PRIu32 "\n", passed);
	return passed;
}
