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
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "rockface.h"
#include "face_db.h"
#include "rockface_recognition.h"

static int is_image_file(const char *filename) {
    int len = strlen(filename);
    if (strcmp(&filename[len-3], "jpg") == 0 || strcmp(&filename[len-3], "png") == 0) {
        return 1;
    }
    return 0;
}

static int import_face_library(rockface_handle_t handle, const char *image_dir_path, const char * db_path) {
    int ret_code = 0;
    sqlite3 *db;
    open_db(db_path, &db);

    DIR *dir;
    struct dirent *ptr;
    dir = opendir(image_dir_path);
    struct stat statbuf;
    char image_path[256];

    int image_num = 0;
    while((ptr = readdir(dir)) != NULL) {
        if (is_image_file(ptr->d_name)) {
            image_num++;
        }
    }
    closedir(dir);

    printf("image num %d\n", image_num);

    face_data facedata;

    rockface_ret_t ret;
    dir = opendir(image_dir_path);
    int imported_num = 0;
    int index = 0;
    while((ptr = readdir(dir)) != NULL) {
        if (is_image_file(ptr->d_name) <= 0) {
            continue;
        }
        index++;
        snprintf(image_path, 256, "%s/%s", image_dir_path, ptr->d_name);
        printf("%s\n", image_path);

        memset(&facedata, 0, sizeof(face_data));
        // read image
        rockface_image_t image;
        ret = rockface_image_read(image_path, &image, 1);
        if (ret != ROCKFACE_RET_SUCCESS) {
            printf("%d [%d/%d] imported %s fail\n", index, imported_num, image_num, image_path);
            continue;
        }
        // get feature
        rockface_feature_t feature;
        memset(&feature, 0, sizeof(rockface_feature_t));
        ret = run_face_recognize(handle, &image, &feature);
        rockface_image_release(&image);
        if (ret != ROCKFACE_RET_SUCCESS) {
            printf("%d [%d/%d] imported %s fail\n", index, imported_num, image_num, image_path);
            continue;
        }
        strncpy(facedata.name, ptr->d_name, MAX_SIZE_NAME);
        memcpy(&(facedata.feature), &feature, sizeof(rockface_feature_t));
        // save to database
        int db_ret = insert_face(db, &facedata);
        if (db_ret != 0) {
            ret_code = -1;
            printf("%d [%d/%d] imported %s fail\n", index, imported_num, image_num, image_path);
            goto error;
        }
        printf("%d [%d/%d] imported %s success\n", index, imported_num, image_num, image_path);
        imported_num++;
    }
    printf("imported count: %d/%d\n", imported_num, image_num);

error:
    close_db(db);
    closedir(dir);
    return ret_code;
}

int main(int argc, char** argv) {

    rockface_ret_t ret;
    struct timeval tv;

    if( argc != 4 ){
        printf("\nUsage: import_face_library <licence file> <path_to_image_dir> <path_to_database_path>\n");
        return -1;
    }

    const char *licence_path = argv[1];

    // read image
    const char *img_dir_path = argv[2];
    const char *db_path = argv[3];

    /*************** Creat Handle ***************/
    rockface_handle_t face_handle = rockface_create_handle();

    ret = rockface_set_licence(face_handle, licence_path);
    if (ret < 0) {
        printf("Error: authorization error %d!", ret);
        return ret;
    }

    ret = rockface_init_detector(face_handle);
    ret = rockface_init_recognizer(face_handle);

    import_face_library(face_handle, img_dir_path, db_path);

    //release handle
    rockface_release_handle(face_handle);

    return 0;
}
