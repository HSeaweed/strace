/*
 * Copyright (c) 2014-2018 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "defs.h"

#include DEF_MPERS_TYPE(utimbuf_t)

#include <utime.h>

typedef struct utimbuf utimbuf_t;

#include MPERS_DEFS

#include "print_fields.h"

SYS_FUNC(utime)
{
	utimbuf_t u;

	printpath(tcp, tcp->u_arg[0]);
	tprints(", ");
	if (!umove_or_printaddr(tcp, tcp->u_arg[1], &u)) {
		tprint_struct_begin();
		PRINT_FIELD_D(u, actime);
		tprints_comment(sprinttime(u.actime));
		tprint_struct_next();
		PRINT_FIELD_D(u, modtime);
		tprints_comment(sprinttime(u.modtime));
		tprint_struct_end();
	}

	return RVAL_DECODED;
}
