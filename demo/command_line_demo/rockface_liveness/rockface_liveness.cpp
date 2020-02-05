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
#include <stdio.h>
#include <memory.h>
#include <sys/time.h>
#include <stdlib.h>

#include "rockface.h"

rockface_det_t *get_max_face(rockface_det_array_t *face_array) {
    if (face_array->count == 0) {
        return NULL;
    }
    rockface_det_t *max_face = NULL;
    int i;
    for (i = 0; i < face_array->count; i++) {
        rockface_det_t *cur_face = &(face_array->face[i]);
        if (max_face == NULL) {
            max_face = cur_face;
            continue;
        }
        int cur_face_box_area = (cur_face->box.right - cur_face->box.left) * (cur_face->box.bottom - cur_face->box.top);
        int max_face_box_area = (max_face->box.right - max_face->box.left) * (max_face->box.bottom - max_face->box.top);
        if (cur_face_box_area > max_face_box_area) {
            max_face = cur_face;
        }
    }
    return max_face;
}

int run_face_liveness(rockface_handle_t face_handle,  rockface_image_t *in_image, rockface_liveness_t *result) {
    
    rockface_ret_t ret;
    rockface_image_t in_rgb_img;
    rockface_det_array_t face_array;
    rockface_landmark_t out_landmark;
    rockface_image_t out_aligned_img;
    rockface_det_t* max_face;

    memset(&in_rgb_img, 0, sizeof(rockface_image_t));
    memset(&out_aligned_img, 0, sizeof(rockface_image_t));
    memset(&face_array, 0, sizeof(rockface_det_array_t));

    in_rgb_img.width = in_image->width;
    in_rgb_img.height = in_image->height;
    in_rgb_img.pixel_format = ROCKFACE_PIXEL_FORMAT_RGB888;

    ret = rockface_image_convert(in_image, &in_rgb_img, ROCKFACE_IMAGE_TRANSFORM_NONE);
    if (ret != ROCKFACE_RET_SUCCESS) {
        goto error;
    }

    // detect face
    ret = rockface_detect(face_handle, &in_rgb_img, &face_array);
    if (ret != ROCKFACE_RET_SUCCESS) {
        printf("rockface_face_detect error %d\n", ret);
        goto error;
    }

    // Get max face
    max_face = get_max_face(&face_array);
    if (max_face == NULL) {
        printf("error no face detected\n");
        goto error;
    }

    // Get face feature
    ret = rockface_liveness_detect(face_handle, in_image, &(max_face->box), result);
    if (ret != ROCKFACE_RET_SUCCESS) {
        printf("error detect face liveness %d\n", ret);
        goto error;
    }
error:
    // Release Aligned Image
    rockface_image_release(&in_rgb_img);
    rockface_image_release(&out_aligned_img);

    return ret;
}

int main(int argc, char** argv) {

    rockface_ret_t ret;
    struct timeval tv;

    if( argc != 3 ){
        printf("\nUsage: face_liveness <licence file> <path_to_image>\n");
        return -1;
    }

    const char *licence_path = argv[1];
    const char *img_path = argv[2];

    rockface_handle_t face_handle = rockface_create_handle();

    ret = rockface_set_licence(face_handle, licence_path);
    if (ret < 0) {
        printf("Error: authorization error %d!", ret);
        return ret;
    }

    ret = rockface_init_detector(face_handle);
    ret = rockface_init_liveness_detector(face_handle);

    // read image
    rockface_image_t input_image;
    printf("img_path=%s\n", img_path);
    rockface_image_read(img_path, &input_image, 0);

    rockface_liveness_t out_result;
    memset(&out_result, 0, sizeof(rockface_liveness_t));

    run_face_liveness(face_handle, &input_image, &out_result);

    printf("real_score: %f fake_score: %f \n", out_result.real_score, out_result.fake_score);

    // release image
    rockface_image_release(&input_image);

    //release handle
    rockface_release_handle(face_handle);

    return 0;
}
