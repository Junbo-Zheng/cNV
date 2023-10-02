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

#include <stdint.h>
#include <stdbool.h>

#define NV_PATH       "./nv.json"

#if CONFIG_NV_DEBUG_MOCK_DATA
#define NV_KEY_NAME   "name"
#define NV_KEY_SEX    "sex"
#define NV_KEY_AGE    "age"
#define NV_KEY_HEIGHT "height"
#endif

typedef struct {
#if CONFIG_NV_DEBUG_MOCK_DATA
    uint8_t name[32];
    uint8_t sex[32];
    uint8_t age;
    uint8_t height;
#endif
} nv_t;

#ifdef __cplusplus
extern "C"
{
#endif

void nv_init(void);

void nv_sync(char* key, char* value, uint8_t len);

bool nv_write(char* data);

bool nv_read(char* data);

nv_t* nv_get(void);

int nv_str2hex(char* str, uint8_t* hex, uint32_t hex_len);

#if CONFIG_NV_DEBUG_MOCK_DATA
void nv_debug(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NV_H_ */
