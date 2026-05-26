/*********************************************************************
*	File : INFCODES.H
*	Desc : Part of ZLib Compression
*
*	Comment: 
*
*
*
*	Date: 1/02/2002
*********************************************************************/ 
struct inflate_codes_state;
typedef struct inflate_codes_state FAR inflate_codes_statef;

extern inflate_codes_statef *inflate_codes_new OF((
    uInt, uInt,
    inflate_huft *, inflate_huft *,
    z_streamp ));

extern int inflate_codes OF((
    inflate_blocks_statef *,
    z_streamp ,
    int));

extern void inflate_codes_free OF((
    inflate_codes_statef *,
    z_streamp ));

