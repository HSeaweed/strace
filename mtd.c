/*
 * Copyright (c) 2012 Mike Frysinger <vapier@gentoo.org>
 * Copyright (c) 2012-2018 The strace developers.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "defs.h"

#ifdef HAVE_STRUCT_MTD_WRITE_REQ

# include DEF_MPERS_TYPE(struct_mtd_oob_buf)

# include <linux/ioctl.h>
# include <mtd/mtd-abi.h>

typedef struct mtd_oob_buf struct_mtd_oob_buf;

#endif /* HAVE_STRUCT_MTD_WRITE_REQ */

#include MPERS_DEFS

#ifdef HAVE_STRUCT_MTD_WRITE_REQ

# include "print_fields.h"

# include "xlat/mtd_mode_options.h"
# include "xlat/mtd_file_mode_options.h"
# include "xlat/mtd_type_options.h"
# include "xlat/mtd_flags_options.h"
# include "xlat/mtd_otp_options.h"
# include "xlat/mtd_nandecc_options.h"

static void
decode_erase_info_user(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct erase_info_user einfo;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &einfo))
		return;

	tprint_struct_begin();
	PRINT_FIELD_X(einfo, start);
	tprint_struct_next();
	PRINT_FIELD_X(einfo, length);
	tprint_struct_end();
}

static void
decode_erase_info_user64(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct erase_info_user64 einfo64;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &einfo64))
		return;

	tprint_struct_begin();
	PRINT_FIELD_X(einfo64, start);
	tprint_struct_next();
	PRINT_FIELD_X(einfo64, length);
	tprint_struct_end();
}

static void
decode_mtd_oob_buf(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct_mtd_oob_buf mbuf;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &mbuf))
		return;
	tprint_struct_begin();
	PRINT_FIELD_X(mbuf, start);
	tprint_struct_next();
	PRINT_FIELD_X(mbuf, length);
	tprint_struct_next();
	PRINT_FIELD_PTR(mbuf, ptr);
	tprint_struct_end();
}

static void
decode_mtd_oob_buf64(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct mtd_oob_buf64 mbuf64;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &mbuf64))
		return;

	tprint_struct_begin();
	PRINT_FIELD_X(mbuf64, start);
	tprint_struct_next();
	PRINT_FIELD_X(mbuf64, length);
	tprint_struct_next();
	PRINT_FIELD_ADDR64(mbuf64, usr_ptr);
	tprint_struct_end();
}

static void
decode_otp_info(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct otp_info oinfo;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &oinfo))
		return;

	tprint_struct_begin();
	PRINT_FIELD_X(oinfo, start);
	tprint_struct_next();
	PRINT_FIELD_X(oinfo, length);
	tprint_struct_next();
	PRINT_FIELD_U(oinfo, locked);
	tprint_struct_end();
}

static void
decode_otp_select(struct tcb *const tcp, const kernel_ulong_t addr)
{
	unsigned int i;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &i))
		return;

	tprints("[");
	printxval(mtd_otp_options, i, "MTD_OTP_???");
	tprints("]");
}

static void
decode_mtd_write_req(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct mtd_write_req mreq;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &mreq))
		return;

	tprint_struct_begin();
	PRINT_FIELD_X(mreq, start);
	tprint_struct_next();
	PRINT_FIELD_X(mreq, len);
	tprint_struct_next();
	PRINT_FIELD_X(mreq, ooblen);
	tprint_struct_next();
	PRINT_FIELD_ADDR64(mreq, usr_data);
	tprint_struct_next();
	PRINT_FIELD_ADDR64(mreq, usr_oob);
	tprint_struct_next();
	PRINT_FIELD_XVAL(mreq, mode, mtd_mode_options, "MTD_OPS_???");
	tprint_struct_end();
}

static void
decode_mtd_info_user(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct mtd_info_user minfo;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &minfo))
		return;

	tprint_struct_begin();
	PRINT_FIELD_XVAL(minfo, type, mtd_type_options, "MTD_???");
	tprint_struct_next();
	PRINT_FIELD_FLAGS(minfo, flags, mtd_flags_options, "MTD_???");
	tprint_struct_next();
	PRINT_FIELD_X(minfo, size);
	tprint_struct_next();
	PRINT_FIELD_X(minfo, erasesize);
	tprint_struct_next();
	PRINT_FIELD_X(minfo, writesize);
	tprint_struct_next();
	PRINT_FIELD_X(minfo, oobsize);
	tprint_struct_next();
	PRINT_FIELD_X(minfo, padding);
	tprint_struct_end();
}

static bool
print_xint32x2_array_member(struct tcb *tcp, void *elem_buf, size_t elem_size,
			    void *data)
{
	print_local_array_ex(tcp, elem_buf, 2, sizeof(int),
			     print_xint32_array_member, NULL, 0, NULL, NULL);
	return true;
}

static void
decode_nand_oobinfo(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct nand_oobinfo ninfo;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &ninfo))
		return;

	tprint_struct_begin();
	PRINT_FIELD_XVAL(ninfo, useecc, mtd_nandecc_options, "MTD_NANDECC_???");
	tprint_struct_next();
	PRINT_FIELD_X(ninfo, eccbytes);
	tprint_struct_next();
	PRINT_FIELD_ARRAY(ninfo, oobfree, tcp, print_xint32x2_array_member);
	tprint_struct_next();
	PRINT_FIELD_ARRAY(ninfo, eccpos, tcp, print_xint32_array_member);
	tprint_struct_end();
}

static bool
print_nand_oobfree_array_member(struct tcb *tcp, void *elem_buf,
				size_t elem_size, void *data)
{
	const struct nand_oobfree *p = elem_buf;
	tprint_struct_begin();
	PRINT_FIELD_X(*p, offset);
	tprint_struct_next();
	PRINT_FIELD_X(*p, length);
	tprint_struct_end();
	return true;
}

static void
decode_nand_ecclayout_user(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct nand_ecclayout_user nlay;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &nlay))
		return;

	tprint_struct_begin();
	PRINT_FIELD_X(nlay, eccbytes);
	tprint_struct_next();
	PRINT_FIELD_ARRAY(nlay, eccpos, tcp, print_xint32_array_member);
	tprint_struct_next();
	PRINT_FIELD_X(nlay, oobavail);
	tprint_struct_next();
	PRINT_FIELD_ARRAY(nlay, oobfree, tcp, print_nand_oobfree_array_member);
	tprint_struct_end();
}

static void
decode_mtd_ecc_stats(struct tcb *const tcp, const kernel_ulong_t addr)
{
	struct mtd_ecc_stats es;

	tprints(", ");
	if (umove_or_printaddr(tcp, addr, &es))
		return;

	tprint_struct_begin();
	PRINT_FIELD_X(es, corrected);
	tprint_struct_next();
	PRINT_FIELD_X(es, failed);
	tprint_struct_next();
	PRINT_FIELD_X(es, badblocks);
	tprint_struct_next();
	PRINT_FIELD_X(es, bbtblocks);
	tprint_struct_end();
}

MPERS_PRINTER_DECL(int, mtd_ioctl, struct tcb *const tcp,
		   const unsigned int code, const kernel_ulong_t arg)
{
	switch (code) {
	case MEMERASE:
	case MEMLOCK:
	case MEMUNLOCK:
	case MEMISLOCKED:
		decode_erase_info_user(tcp, arg);
		break;

	case MEMERASE64:
		decode_erase_info_user64(tcp, arg);
		break;

	case MEMWRITEOOB:
	case MEMREADOOB:
		decode_mtd_oob_buf(tcp, arg);
		break;

	case MEMWRITEOOB64:
	case MEMREADOOB64:
		decode_mtd_oob_buf64(tcp, arg);
		break;

	case MEMWRITE:
		decode_mtd_write_req(tcp, arg);
		break;

	case OTPGETREGIONINFO:
		if (entering(tcp))
			return 0;
		ATTRIBUTE_FALLTHROUGH;
	case OTPLOCK:
		decode_otp_info(tcp, arg);
		break;

	case OTPSELECT:
		decode_otp_select(tcp, arg);
		break;

	case MTDFILEMODE:
		tprints(", ");
		printxval64(mtd_file_mode_options, arg, "MTD_FILE_MODE_???");
		break;

	case MEMGETBADBLOCK:
	case MEMSETBADBLOCK:
		tprints(", ");
		printnum_int64(tcp, arg, "%" PRIu64);
		break;

	case MEMGETINFO:
		if (entering(tcp))
			return 0;
		decode_mtd_info_user(tcp, arg);
		break;

	case MEMGETOOBSEL:
		if (entering(tcp))
			return 0;
		decode_nand_oobinfo(tcp, arg);
		break;

	case ECCGETLAYOUT:
		if (entering(tcp))
			return 0;
		decode_nand_ecclayout_user(tcp, arg);
		break;

	case ECCGETSTATS:
		if (entering(tcp))
			return 0;
		decode_mtd_ecc_stats(tcp, arg);
		break;

	case OTPGETREGIONCOUNT:
		if (entering(tcp))
			return 0;
		tprints(", ");
		printnum_int(tcp, arg, "%u");
		break;

	case MEMGETREGIONCOUNT:
		if (entering(tcp))
			return 0;
		tprints(", ");
		printnum_int(tcp, arg, "%d");
		break;

	case MEMGETREGIONINFO:
		if (entering(tcp)) {
			struct region_info_user rinfo;

			tprints(", ");
			if (umove_or_printaddr(tcp, arg, &rinfo))
				break;
			tprint_struct_begin();
			PRINT_FIELD_X(rinfo, regionindex);
			return 0;
		} else {
			struct region_info_user rinfo;

			if (!syserror(tcp) && !umove(tcp, arg, &rinfo)) {
				tprint_struct_next();
				PRINT_FIELD_X(rinfo, offset);
				tprint_struct_next();
				PRINT_FIELD_X(rinfo, erasesize);
				tprint_struct_next();
				PRINT_FIELD_X(rinfo, numblocks);
			}
			tprint_struct_end();
			break;
		}

	default:
		return RVAL_DECODED;
	}

	return RVAL_IOCTL_DECODED;
}

#endif /* HAVE_STRUCT_MTD_WRITE_REQ */
