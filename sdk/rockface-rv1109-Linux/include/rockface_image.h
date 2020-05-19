/****************************************************************************
*
*    Copyright (c) 2017 - 2019 by Rockchip Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Rockchip Corporation. This is proprietary information owned by
*    Rockchip Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Rockchip Corporation.
*
*****************************************************************************/

#ifndef _ROCKFACE_IMAGE_H
#define _ROCKFACE_IMAGE_H

#include "rockface_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 读取图像文件(使用后必须调用 @ref rockface_image_release 释放)
 * 
 * @param img_path [in] Image file path
 * @param image [out] Read Image
 * @param flag [in] 0: gray; 1: color
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_image_read(const char *img_path, rockface_image_t *image, int flag);

/**
 * 将图像保存成图像文件(支持jpg或png)
 * 
 * @param path [in] File path to write
 * @param img [in] Image to write
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_image_write(const char *path, rockface_image_t *img);

/**
 * 将图像的原始数据保存成RGB24文件
 * 
 * @param path [in] File path to write
 * @param img [in] Image to write
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_image_write_raw(const char *path, rockface_image_t *img);

/**
 * 释放rockface_image_t对象中动态分配的数据
 * 
 * @param img 要释放的rockface_image_t
 * @return @ref rockface_ret_t 
 */
rockface_ret_t rockface_image_release(rockface_image_t *img);

/**
 * 对图像进行缩放、旋转、颜色通道转换
 * 
 * @param src [in] Source image
 * @param dst [out] Destination image
 * @param mode [in] transform mode
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_image_convert(rockface_image_t *src, rockface_image_t *dst, rockface_image_transform_mode mode);

/**
 * 计算图像清晰度
 * 
 * @param img [in] Image
 * @param clarity [in] Clarity result
 * @return rockface_ret_t 
 */
rockface_ret_t rockface_image_clarity(rockface_image_t *img, float *clarity);

rockface_ret_t rockface_image_draw_rect(rockface_image_t *img, rockface_point_t pt1, rockface_point_t pt2,
                                         rockface_color_t color, int thickness);
rockface_ret_t rockface_image_draw_text(rockface_image_t *img, const char *text, rockface_point_t pt,
                                         rockface_color_t color, int thickness);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // _ROCKFACE_IMAGE_H