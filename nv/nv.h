/*
 * Copyright (C) 2023 Junbo Zheng. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _NV_H_
#define _NV_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#if CONFIG_NV_DEBUG_LOG
#define nv_log(fmt, args...)                               \
    printf("[%s : %d]: " fmt, __func__, __LINE__, ##args); \
    fflush(stdout);
#else
#define nv_log(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NV_DATA_U8 = 0,          ///< U8
    NV_DATA_S8,              ///< S8
    NV_DATA_U16,             ///< U16
    NV_DATA_S16,             ///< S16
    NV_DATA_U32,             ///< U32
    NV_DATA_S32,             ///< S32
    NV_DATA_U64,             ///< U64
    NV_DATA_S64,             ///< S64
    NV_DATA_FLOAT,           ///< float
    NV_DATA_DOUBLE,          ///< double
    NV_DATA_STR,             ///< only string
    NV_DATA_STRING_ARRAY,    ///< string array
    NV_DATA_INT_ARRAY,       ///< int array
    NV_DATA_FLOAT_ARRAY,     ///< float array
    NV_DATA_DOUBLE_ARRAY,    ///< double array
    NV_DATA_IP,              ///< IP Address  (uint32_t array)
    NV_DATA_MAC              ///< MAC Address (uint32_t array)
} nv_data_type_t;

/**
 * nv_init
 * @param file nv file path
 * @param data nv json format data from file
 */
void nv_init(const char* file, void* data);

/**
 * nv_write to file
 * @param file nv file path
 * @param data data buffer
 * @return     boolean
 */
bool nv_write(const char* file, void* data);

/**
 * nv_read from file
 * @param file nv file path
 * @param data data buffer
 * @return     boolean
 */
bool nv_read(const char* file, void* data);

/**
 * nv_sync to file
 * @param file  nv file path
 * @param key   nv key
 * @param value data buffer
 * @param len   data buffer length
 * @param type  data type
 */
void nv_sync(const char* file, char* key, void* value, uint32_t len,
             nv_data_type_t type);

/**
 * nv_get
 * @param file  nv file path
 * @param key   nv key
 * @param value data buffer
 * @param len   data buffer length
 * @param type  data type
 * @return      boolean
 */
bool nv_get(const char* file, char* key, char* value, uint32_t len,
            nv_data_type_t type);

/**
 * nv_delete
 * @param file nv file path
 * @param key  nv key
 * @return     boolean
 */
bool nv_delete(const char* file, char* key);

#ifdef __cplusplus
}
#endif

#endif /* _NV_H_ */
