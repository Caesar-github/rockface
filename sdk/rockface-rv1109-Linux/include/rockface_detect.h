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

#ifndef _ROCKFACE_DETECT_H
#define _ROCKFACE_DETECT_H

#include "rockface_type.h"
#include "rockface_image.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 初始化人脸检测器
 * 
 * @param handle 需要初始化的Handle
 * @return @ref rockface_ret_t 
 */
rockface_ret_t rockface_init_detector(rockface_handle_t handle);


/**
 * 初始化人形检测器
 * 
 * @param handle 需要初始化的Handle
 * @return @ref rockface_ret_t 
 */
rockface_ret_t rockface_init_person_detector(rockface_handle_t handle);


/**
 * 人脸检测
 * 
 * @param handle [in] 已初始化的Handle（调用 @ref rockface_init_detector 函数初始化）
 * @param in_img [in] 输入图像
 * @param face_array [out] 人脸检测结果
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_detect(rockface_handle_t handle, rockface_image_t *in_img, rockface_det_array_t *face_array);


/**
 * 人形检测
 * 
 * @param handle [in] 已初始化的Handle（调用 @ref rockface_init_person_detector 函数初始化）
 * @param in_img [in] 输入图像
 * @param face_array [out] 人形检测结果
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_person_detect(rockface_handle_t handle, rockface_image_t *in_img, rockface_det_person_array_t *person_array);


/**
 * 人脸矫正对齐
 * 
 * @param handle [in] 已初始化的Handle（调用 @ref rockface_init_detector 函数初始化）
 * @param in_img [in] 输入图像（需要原始图像）
 * @param in_box [in] 人脸区域
 * @param in_landmark [in] 人脸关键点（5点）。如果为NULL，函数内部会自己获取
 * @param out_img [out] 对齐后的人脸图像（用完后需要调用 @ref rockface_image_release 释放）
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_align(rockface_handle_t handle, rockface_image_t *in_img, rockface_rect_t *in_box, rockface_landmark_t *in_landmark, rockface_image_t *out_img);


/**
 * 人脸跟踪，在连续视频帧时使用，会对当前场景的检测的结果分配跟踪ID，连续帧下对同一目标保持相同的跟踪ID
 * 
 * @param handle [in] 已初始化的Handle（调用 @ref rockface_init_detector 函数初始化）
 * @param in_img [in] 输入图像
 * @param max_track_time [in] 最大跟踪时间（避免偶然漏检导致目标丢失）
 * @param in_track_objects [in] 人脸检测结果
 * @param out_track_objects [out] 已跟踪的人脸列表
 * @return @ref rockface_ret_t
 */
rockface_ret_t rockface_track(rockface_handle_t handle, rockface_image_t *in_img, int max_track_time,
        rockface_det_array_t* in_track_face, rockface_det_array_t* out_track_face);


#ifdef __cplusplus
} //extern "C"
#endif

#endif // _ROCKFACE_DETECT_H