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

#ifndef _ROCKFACE_ANALYZE_H
#define _ROCKFACE_ANALYZE_H

#include "rockface_type.h"
#include "rockface_image.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 初始化人脸分析器
 * 
 * @param handle 需要初始化的Handle
 * @return @ref rockface_ret_t 
 */
rockface_ret_t rockface_init_analyzer(rockface_handle_t handle);

/**
 * 检测人脸关键点（68点）
 *
 * 68关键点如图1所示：
 * @image html res/face_landmark68.png Figure 1 人脸68关键点示意图
 *
 * @param handle [in] 已初始化的Handle（调用 @ref rockface_init_analyzer 函数初始化）
 * @param in_img [in] 输入图像
 * @param in_box [in] 人脸区域
 * @param out_landmark [out] 人脸关键点结果
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_landmark(rockface_handle_t handle, rockface_image_t* in_img, rockface_rect_t *in_box,
        rockface_landmark_t *out_landmark);

/**
 * 检测人脸关键点（5点），结果可用于计算人脸角度/人脸对齐
 *
 * @param handle [in] 已初始化的Handle（调用 @ref rockface_init_detector 函数初始化）
 * @param in_img [in] 输入图像
 * @param in_box [in] 人脸区域
 * @param out_landmark [out] 人脸关键点结果
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_landmark5(rockface_handle_t handle, rockface_image_t* in_img, rockface_rect_t *in_box,
        rockface_landmark_t *out_landmark);

/**
 * 获取人脸角度
 * 
 * @param in_landmark [in] 人脸关键点结果
 * @param out_angle [out] 人脸角度
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_angle(rockface_handle_t handle, rockface_landmark_t *in_landmark, rockface_angle_t *out_angle);

/**
 * 获取人脸属性（性别和年龄）
 * 
 * @param handle [in] 已初始化的Handle（调用 @ref rockface_init_analyzer 函数初始化）
 * @param in_img [in] 对齐后的人脸图像
 * @param attr [out] 人脸属性
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_attribute(rockface_handle_t handle, rockface_image_t *in_img, rockface_attribute_t *attr);


#ifdef __cplusplus
} //extern "C"
#endif

#endif // _ROCKFACE_ANALYZE_H