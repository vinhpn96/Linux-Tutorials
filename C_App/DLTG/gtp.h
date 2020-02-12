#ifndef _GTP_H_
#define _GTP_H_

/* General GTP protocol related definitions. */
#define GTPU_PORT	2152
#define GTP_TPDU	255

struct gtp1_header {	/* According to 3GPP TS 29.060. */
	Uint8	flags;
	Uint8	type;
	Uint16	length;
	Uint32	tid;
} __attribute__ ((packed));

#define GTP1_F_NPDU	0x01
#define GTP1_F_SEQ	0x02
#define GTP1_F_EXTHDR	0x04
#define GTP1_F_MASK	0x07

#endif