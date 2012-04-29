/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * SungMin Ha <sungmin82.ha@samsung.com>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 * 
 */

#include "imageutil.h"

#if 0 /* deleted by opensource (20090309) */
static char *folder_xpm[] = {
    "16 16 8 1",
    "   c None",
    ".  c #909000",
    "+  c #000000",
    "@  c #EFE8EF",
    "#  c #FFF8CF",
    "$  c #FFF890",
    "%  c #CFC860",
    "&  c #FFC890",
    "                ",
    "  .....+        ",
    " .@##$$.+       ",
    ".%%%%%%%......  ",
    ".###########$%+ ",
    ".#$$$$$$$$$$&%+ ",
    ".#$$$$$$$&$&$%+ ",
    ".#$$$$$$$$&$&%+ ",
    ".#$$$$$&$&$&$%+ ",
    ".#$$$$$$&$&$&%+ ",
    ".#$$$&$&$&$&&%+ ",
    ".#&$&$&$&$&&&%+ ",
    ".%%%%%%%%%%%%%+ ",
    " ++++++++++++++ ",
    "                ",
    "                "
};

static char *ofolder_xpm[] = {
    "16 16 12 1",
    "   c None",
    ".  c #808080",
    "+  c #E0E0D0",
    "@  c #4F484F",
    "#  c #909000",
    "$  c #FFF8EF",
    "%  c #CFC860",
    "&  c #003090",
    "*  c #7F7800",
    "=  c #FFC890",
    "-  c #FFF890",
    ";  c #2F3000",
    "        .       ",
    "       .+@      ",
    "   ###.$$+@     ",
    "  #%%.$$$$+@    ",
    "  #%.$$$&$$+@** ",
    "  #.+++&+&+++@* ",
    "############++@ ",
    "#$$$$$$$$$=%#++@",
    "#$-------=-=#+; ",
    " #---=--=-==%#; ",
    " #-----=-=-==#; ",
    " #-=--=-=-=-=#; ",
    "  #=-=-=-=-==#; ",
    "  ############; ",
    "   ;;;;;;;;;;;  ",
    "                "
};
#endif

/* XPM */
static char * ofolder_xpm[] = {
"16 22 14 1",
" 	c None",
".	c #848400",
"+	c #D6D67B",
"@	c #CECE7B",
"#	c #CECE73",
"$	c #C6C66B",
"%	c #BDBD5A",
"&	c #BDBD52",
"*	c #ADAD39",
"=	c #ADAD42",
"-	c #B5B54A",
";	c #C6C663",
">	c #CECE6B",
",	c #A5A5C6",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
" ....           ",
".+@@#.....      ",
".@@##$$%%&.     ",
".@#..........   ",
".#.*==--&%%;;.  ",
".#.==--&%%;;.   ",
"..==--&%%;;$.   ",
"..=--&%%;;$.    ",
".=--&%%;;$>.    ",
"...........     ",
"                ",
"     ,          ",
"                ",
"     ,          ",
"                ",
"     ,          "
};


/* XPM */
static char * folder_xpm[] = {
"16 22 15 1",
" 	c None",
".	c #848400",
"+	c #E7E79C",
"@	c #E7E794",
"#	c #DEDE8C",
"$	c #ADAD39",
"%	c #ADAD42",
"&	c #B5B54A",
"*	c #B5B552",
"=	c #BDBD5A",
"-	c #9C9C29",
";	c #A5A531",
">	c #C6C663",
",	c #C6C66B",
"'	c #CECE73",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
" ....           ",
".+@@#.......    ",
".....$%%&&*=.   ",
".-;;$%%&&*==.   ",
".;;$%%&&*===.   ",
".;$%%&&*===>.   ",
".$%%&&*===>,.   ",
".%%&&*===>,,.   ",
".%&&*===>,,'.   ",
".............   ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                "
};


#if 1
static char *check_page_xpm[] = {
    "16 16 5 1",
    "       c None s None",
    ".      c black",
    "X      c white",
    "o      c #808080",
    "r      c red",
    "                ",
    "   .......      ",
    "   .XXXXX..     ",
    "   .XoooX.X.    ",
    "   .XXXXX....   ",
    "   rrooooXoorr  ",
    "   rrXXXXXXrro  ",
    "   .rrooooorro  ",
    "   .rrXXXXrr.o  ",
    "   .Xrrooorr.o  ",
    "   .XXrrXrrX.o  ",
    "   .XoorrroX.o  ",
    "   .XXXXrXXX.o  ",
    "   ..........o  ",
    "    oooooooooo  ",
    "                "
};


static char *page_xpm[] = {
    "16 16 4 1",
    "       c None s None",
    ".      c black",
    "X      c white",
    "o      c #808080",
    "                ",
    "   .......      ",
    "   .XXXXX..     ",
    "   .XoooX.X.    ",
    "   .XXXXX....   ",
    "   .XooooXoo.o  ",
    "   .XXXXXXXX.o  ",
    "   .XooooooX.o  ",
    "   .XXXXXXXX.o  ",
    "   .XooooooX.o  ",
    "   .XXXXXXXX.o  ",
    "   .XooooooX.o  ",
    "   .XXXXXXXX.o  ",
    "   ..........o  ",
    "    oooooooooo  ",
    "                "
};
#endif


static char *bk_on_xpm[] = {
    "16 16 3 1",
    "   c None",
    ".  c red",
    "o  c black",
    "                ",
    "     oooooo     ",
    "   oo......oo   ",
    "  o..........o  ",
    "  o..........o  ",
    " oo...........o ",
    " o............o ",
    " o............o ",
    " o............o ",
    " oo..........oo ",
    "  o..........o  ",
    "  o..........o  ",
    "   oo......oo   ",
    "     oooooo     ",
    "                ",
    "                "
};


static char *bk_off_xpm[] = {
    "16 16 3 1",
    "   c None",
    ".  c blue",
    "o  c black",
    "                ",
    "     oooooo     ",
    "   oo......oo   ",
    "  o..........o  ",
    "  o..........o  ",
    " oo...........o ",
    " o............o ",
    " o............o ",
    " o............o ",
    " oo..........oo ",
    "  o..........o  ",
    "  o..........o  ",
    "   oo......oo   ",
    "     oooooo     ",
    "                ",
    "                "
};


static char *bk_null_xpm[] = {
    "16 16 1 1",
    "       c None s None",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                "
};


#if 0
static char *pointer_off_xpm[] = {
    "16 16 2 1",
    "   c None",
    ".  c black",
    "                ",
    "                ",
    "  ............  ",
    "  ............  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ............  ",
    "  ............  ",
    "                ",
    "                "
};


static char *pointer_on_xpm[] = {
    "16 16 2 1",
    "   c None",
    ".  c black",
    "                ",
    "                ",
    "  ............  ",
    "  ............  ",
    "  ..        ..  ",
    "  ..        ..  ",
    "  ...      ...  ",
    "  ....    ....  ",
    "  .. ...... ..  ",
    "  ..  ....  ..  ",
    "  ..   ..   ..  ",
    "  ..        ..  ",
    "  ............  ",
    "  ............  ",
    "                ",
    "                "
};
#endif


/* XPM */
static char * pointer_on_xpm[] = {
"16 16 3 1",
" 	c None",
".	c #FFFFFF",
"+	c #000000",
"................",
".++++++++++++++.",
".+............+.",
".+.........++.+.",
".+.........++.+.",
".+........++..+.",
".+........++..+.",
".+..++...++...+.",
".+..++...++...+.",
".+...++.++....+.",
".+...++.++....+.",
".+....+++.....+.",
".+....+++.....+.",
".+............+.",
".++++++++++++++.",
"................"
};

/* XPM */
static char * pointer_off_xpm[] = {
"16 16 3 1",
" 	c None",
".	c #FFFFFF",
"+	c #000000",
"................",
".++++++++++++++.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".+............+.",
".++++++++++++++.",
"................"
};


GdkColormap *colormap;

GdkPixmap *pointer_on_image, *pointer_off_image;
GdkBitmap *pointer_on_mask, *pointer_off_mask;

GdkPixmap *bk_null_image = NULL, *bk_on_image = NULL, *bk_off_image = NULL;
GdkBitmap *bk_null_mask = NULL, *bk_on_mask = NULL, *bk_off_mask = NULL;

GdkPixmap *page_image, *check_image;
GdkBitmap *page_mask, *check_mask;

GdkPixmap *opened_image, *closed_image;
GdkBitmap *opened_mask, *closed_mask;


/**
	@brief	initialize image icon
	@return	void
*/
void init_icon_image ()
{
    colormap = gdk_colormap_get_system ();
    pointer_on_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap,
					       	&pointer_on_mask, NULL, pointer_on_xpm);
    pointer_off_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap,
					       	&pointer_off_mask, NULL,pointer_off_xpm);

    bk_on_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
							&bk_on_mask, NULL, bk_on_xpm);
    bk_off_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
							&bk_off_mask, NULL, bk_off_xpm);
    bk_null_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
							&bk_null_mask, NULL, bk_null_xpm);

    page_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
							&page_mask, NULL, page_xpm);
    check_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
							&check_mask, NULL, check_page_xpm);

    opened_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
							&opened_mask, NULL, ofolder_xpm);
    closed_image = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
							&closed_mask, NULL, folder_xpm);
}
