/*********************************************************************
*	File : INFFAST.H
*	Desc : Part of ZLib Compression
*
*	Comment: 
*
*
*
*	Date: 1/02/2002
*********************************************************************/ 
extern int inflate_fast OF((
    uInt,
    uInt,
    inflate_huft *,
    inflate_huft *,
    inflate_blocks_statef *,
    z_streamp ));
