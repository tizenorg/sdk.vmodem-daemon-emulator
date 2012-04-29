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

#ifndef __IMAGE_UTIL_H__
#define __IMAGE_UTIL_H__

#include <gtk/gtk.h>
#include "utils.h"

extern GdkColormap *colormap;
extern GdkPixmap *pointer_on_image, *pointer_off_image;
extern GdkBitmap *pointer_on_mask, *pointer_off_mask;
extern GdkPixmap *bk_null_image, *bk_on_image, *bk_off_image;
extern GdkBitmap *bk_null_mask, *bk_on_mask, *bk_off_mask;
extern GdkPixmap *page_image, *check_image;
extern GdkBitmap *page_mask, *check_mask;
extern GdkPixmap *opened_image, *closed_image;
extern GdkBitmap *opened_mask, *closed_mask;

void init_icon_image ();

#endif
