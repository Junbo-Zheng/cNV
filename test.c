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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"

#include "nv.h"

#define NV_KEY_NAME         "name"
#define NV_KEY_SEX          "sex"
#define NV_KEY_AGE          "age"
#define NV_KEY_HEIGHT       "height"
#define NV_KEY_ID           "id"
#define NV_KEY_HIGH         "high"
#define NV_KEY_TEMP_FLOAT   "temp_float"
#define NV_KEY_TEMP_DOUBLE  "temp_double"
#define NV_KEY_SCORE_STR    "score_str"
#define NV_KEY_SCORE_INT    "score_int"
#define NV_KEY_SCORE_FLOAT  "score_float"
#define NV_KEY_SCORE_DOUBLE "score_double"
#define NV_KEY_IP           "IP"
#define NV_KEY_MAC          "MAC"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#ifndef CONFIG_NV_PATH
#define CONFIG_NV_PATH "./nv.json"
#endif

typedef struct {
    uint8_t name[32];
    uint8_t age;
    uint8_t height;
} nv_t;

static nv_t nv = { 0 };

__attribute__((unused)) static void cjson_create_item(void)
{
    cJSON* root = cJSON_CreateObject();

#if CONFIG_NV_DEBUG_MOCK_DATA
    cJSON_AddStringToObject(root, NV_KEY_NAME, "zhansan");
    cJSON_AddStringToObject(root, NV_KEY_SEX, "male");
    cJSON_AddNumberToObject(root, NV_KEY_AGE, 20);
    cJSON_AddNumberToObject(root, NV_KEY_HEIGHT, 175);
#endif

    char* json_str = cJSON_Print(root);
    nv_write(CONFIG_NV_PATH, json_str);

    cJSON_free(json_str);
    cJSON_Delete(root);
}

__attribute__((unused)) static void cjson_add_item(void)
{
    char json[999]
        = { "{\"name\":\"ZhangSan\",\"age\":20,\"height\":180,\"weight\":60}" };
    cJSON* cjson_pars = cJSON_Parse(json);
    if (NULL == cjson_pars) {
        return;
    }

    char* p_json = cJSON_PrintUnformatted(cjson_pars);

    printf("add before\n");
    if (p_json) {
        printf("%s\n", p_json);
    }

    cJSON_AddStringToObject(cjson_pars, "sex", "male");
    cJSON_AddNumberToObject(cjson_pars, "math", 90.50);
    cJSON_AddNumberToObject(cjson_pars, "class", 1107);

    p_json = cJSON_PrintUnformatted(cjson_pars);

    printf("add after\n");
    if (p_json) {
        printf("%s\n", p_json);
    }

    /* cJSON_PrintUnformatted() and cJSON_Print() 函数的返回值char *
     * 需要用户释放，否则会造成内存泄露. */
    cJSON_free(p_json);
    /* cJSON_Parse() 函数的返回值cJSON * 需要用户自己释放，否则会造成内泄露. */
    cJSON_Delete(cjson_pars);
}

void cjson_delete_item(void)
{
    char json[999]
        = { "{\"name\":\"ZhangSan\",\"age\":20,\"height\":180,\"weight\":60}" };
    cJSON* cjson_pars = cJSON_Parse(json);
    if (NULL == cjson_pars) {
        return;
    }

    char* p_json = NULL;
    p_json = cJSON_PrintUnformatted(cjson_pars);

    printf("delete before\n");
    if (p_json) {
        printf("%s\n", p_json);
    }

    cJSON_DeleteItemFromObject(cjson_pars, "age");

    p_json = cJSON_PrintUnformatted(cjson_pars);

    printf("delete after\n");
    if (p_json) {
        printf("%s\n", p_json);
    }

    /* cJSON_PrintUnformatted() and cJSON_Print() 函数的返回值char *
     * 需要用户释放，否则会造成内存泄露. */
    cJSON_free(p_json);
    /* cJSON_Parse() 函数的返回值cJSON * 需要用户自己释放，否则会造成内泄露. */
    cJSON_Delete(cjson_pars);
}

void cjson_update_item(void)
{
    char json[999]
        = { "{\"name\":\"ZhangSan\",\"age\":20,\"height\":180,\"weight\":60}" };
    cJSON* cjson_pars = cJSON_Parse(json);
    if (NULL == cjson_pars) {
        return;
    }

    char* p_json = NULL;
    p_json = cJSON_PrintUnformatted(cjson_pars);

    printf("update before\n");
    if (p_json) {
        printf("%s\n", p_json);
    }

    cJSON* key_age = cJSON_GetObjectItem(cjson_pars, "age");
    if (NULL != key_age) {
        cJSON_SetNumberHelper(key_age, 18);
    }

    p_json = cJSON_PrintUnformatted(cjson_pars);

    printf("update after\n");
    if (p_json) {
        printf("%s\n", p_json);
    }

    /* cJSON_PrintUnformatted() and cJSON_Print() 函数的返回值char *
     * 需要用户释放，否则会造成内存泄露. */
    cJSON_free(p_json);
    /* cJSON_Parse() 函数的返回值cJSON * 需要用户自己释放，否则会造成内泄露. */
    cJSON_Delete(cjson_pars);
}

void cjson_find_item(void)
{
    char json[999]
        = { "{\"name\":\"ZhangSan\",\"age\":20,\"height\":180,\"weight\":60}" };
    cJSON* cjson_pars = cJSON_Parse(json);
    if (NULL == cjson_pars) {
        return;
    }

    char* p_json = NULL;
    p_json = cJSON_PrintUnformatted(cjson_pars);

    printf("cJONS key-value\n");
    if (p_json) {
        printf("%s\n", p_json);
    }

    cJSON* key_age = cJSON_GetObjectItem(cjson_pars, "age");
    printf("find age key value\n");
    if (NULL != key_age) {
        printf("find successful -- \"age\":%d\n", key_age->valueint);
    } else {
        printf("find failure\n");
    }

    /* cJSON_PrintUnformatted() and cJSON_Print() 函数的返回值char *
     * 需要用户释放，否则会造成内存泄露. */
    cJSON_free(p_json);
    /* cJSON_Parse() 函数的返回值cJSON * 需要用户自己释放，否则会造成内泄露. */
    cJSON_Delete(cjson_pars);
}

int main(void)
{
#if 0
    cjson_create_item();

    printf("-----------cjson_add_item------------\n");
    cjson_add_item();
    printf("-----------cjson_delete_item------------\n");
    cjson_delete_item();
    printf("-----------cjson_update_item------------\n");
    cjson_update_item();
    printf("-----------cjson_find_item------------\n");
    cjson_find_item();
#endif

#if 0
    cJSON* json = cJSON_CreateObject();

    // 输出JSON的字符串表示，由于是空JSON，所以字符串应该为空
    char* json_str = cJSON_Print(json);
    printf("JSON: %s\n", json_str);

    if (cJSON_GetObjectItem(json, "hello")) {
        printf("hello world\n");
    } else {
        printf("world hello\n");
    }

    // 释放内存
    cJSON_Delete(json);
#endif
    uint8_t nv_buffer[512] = { 0 };
    nv_init(CONFIG_NV_PATH, nv_buffer);

#if CONFIG_NV_DEBUG_MOCK_DATA
    cJSON* json = NULL;

    if (access(CONFIG_NV_PATH, F_OK) == 0) {
        json = cJSON_Parse((const char*)nv_buffer);
        if (!json) {
            nv_log("cJSON Parse fail %s\n", cJSON_GetErrorPtr());
        }
    }

    cJSON* tmp = cJSON_GetObjectItem(json, NV_KEY_NAME);
    if (tmp && tmp->valuestring) {
         memcpy(nv.name, tmp->valuestring, strlen(tmp->valuestring));
    } else {
        nv_log("%s not found\n", NV_KEY_NAME);
    }

    tmp = cJSON_GetObjectItem(json, NV_KEY_AGE);
    if (tmp && tmp->valueint) {
         nv.age = tmp->valueint;
    } else {
        nv_log("%s not found\n", NV_KEY_AGE);
    }

    tmp = cJSON_GetObjectItem(json, NV_KEY_HEIGHT);
    if (tmp && tmp->valueint) {
         nv.height = tmp->valueint;
    } else {
        nv_log("%s not found\n", NV_KEY_HEIGHT);
    }

    cJSON_Delete(json);

    uint8_t age = 30;
    uint16_t height = 175;
    uint32_t high = 140;
    uint64_t id = 88;
    char name[] = { "Bob" };
    float temp_float = 36.1;
    double temp_double = 36.2;

    uint32_t ip[]  = { 192, 168, 0, 1 };
    uint32_t mac[] = { 11, 22, 33, 44, 55, 66 };

    char* score_str[] = { "100", "150" };
    int score_int[] = { 100, 150 };
    float score_float[] = { 1.1, 1.2 };
    double score_double[] = { 2.1, 2.2 };

    /* clang-format off */
    nv_sync(CONFIG_NV_PATH, NV_KEY_AGE,    (char *)&age,    sizeof(age),    NV_DATA_U8);
    nv_sync(CONFIG_NV_PATH, NV_KEY_HEIGHT, (char *)&height, sizeof(height), NV_DATA_U16);
    nv_sync(CONFIG_NV_PATH, NV_KEY_HIGH,   (char *)&high,   sizeof(high),   NV_DATA_U32);
    nv_sync(CONFIG_NV_PATH, NV_KEY_ID,     (char *)&id,     sizeof(id),     NV_DATA_U64);
    nv_sync(CONFIG_NV_PATH, NV_KEY_NAME,   (char *)name,    strlen(name),   NV_DATA_STR);

    nv_sync(CONFIG_NV_PATH, NV_KEY_TEMP_FLOAT,  (char *)&temp_float,  sizeof(temp_float),  NV_DATA_FLOAT);
    nv_sync(CONFIG_NV_PATH, NV_KEY_TEMP_DOUBLE, (char *)&temp_double, sizeof(temp_double), NV_DATA_DOUBLE);

    nv_sync(CONFIG_NV_PATH, NV_KEY_SCORE_STR,    (char *)score_str,    ARRAY_SIZE(score_str),    NV_DATA_STRING_ARRAY);
    nv_sync(CONFIG_NV_PATH, NV_KEY_SCORE_INT,    (char *)score_int,    ARRAY_SIZE(score_int),    NV_DATA_INT_ARRAY);
    nv_sync(CONFIG_NV_PATH, NV_KEY_SCORE_FLOAT,  (char *)score_float,  ARRAY_SIZE(score_float),  NV_DATA_FLOAT_ARRAY);
    nv_sync(CONFIG_NV_PATH, NV_KEY_SCORE_DOUBLE, (char *)score_double, ARRAY_SIZE(score_double), NV_DATA_DOUBLE_ARRAY);

    nv_sync(CONFIG_NV_PATH, NV_KEY_IP,  (char *)ip,  ARRAY_SIZE(ip),  NV_DATA_IP);
    nv_sync(CONFIG_NV_PATH, NV_KEY_MAC, (char *)mac, ARRAY_SIZE(mac), NV_DATA_MAC);
    /* clang-format on */

//    nv_delete(CONFIG_NV_PATH, NV_KEY_NAME);

    memset(score_str, 0, sizeof(score_str));
    memset(score_int, 0, sizeof(score_int));
    memset(score_float, 0, sizeof(score_float));
    memset(score_double, 0, sizeof(score_double));

    /* clang-format off */
    nv_get(CONFIG_NV_PATH, NV_KEY_AGE,    (char*)&age,    sizeof(age),    NV_DATA_U8);
    nv_get(CONFIG_NV_PATH, NV_KEY_HEIGHT, (char*)&height, sizeof(height), NV_DATA_U16);
    nv_get(CONFIG_NV_PATH, NV_KEY_HIGH,   (char*)&high,   sizeof(high),   NV_DATA_U32);
    nv_get(CONFIG_NV_PATH, NV_KEY_ID,     (char*)&id,     sizeof(id),     NV_DATA_U64);
    nv_get(CONFIG_NV_PATH, NV_KEY_NAME,   (char*)name,    strlen(name),   NV_DATA_STR);

    nv_get(CONFIG_NV_PATH, NV_KEY_TEMP_FLOAT,  (char *)&temp_float,  sizeof(temp_float),  NV_DATA_FLOAT);
    nv_get(CONFIG_NV_PATH, NV_KEY_TEMP_DOUBLE, (char *)&temp_double, sizeof(temp_double), NV_DATA_DOUBLE);
    /* clang-format on */

    nv_log("age         = %d\n", age);
    nv_log("height      = %d\n", height);
    nv_log("high        = %" PRIu32 "\n", high);
    nv_log("id          = %" PRIu64 "\n", id);
    nv_log("name        = %s\n", name);
    nv_log("temp_float  = %f\n", temp_float);
    nv_log("temp_double = %f\n", temp_double);

    char* score_buf[16] = {0};
    for (size_t i = 0; i < ARRAY_SIZE(score_buf); i++) {
        score_buf[i] = calloc(1, ARRAY_SIZE(score_buf) * sizeof(char));
    }

    /* clang-format off */
    nv_get(CONFIG_NV_PATH, NV_KEY_SCORE_STR,    (char*)score_buf,    ARRAY_SIZE(score_buf),    NV_DATA_STRING_ARRAY);
    nv_get(CONFIG_NV_PATH, NV_KEY_SCORE_INT,    (char*)score_int,    ARRAY_SIZE(score_int),    NV_DATA_INT_ARRAY);
    nv_get(CONFIG_NV_PATH, NV_KEY_SCORE_FLOAT,  (char*)score_float,  ARRAY_SIZE(score_float),  NV_DATA_FLOAT_ARRAY);
    nv_get(CONFIG_NV_PATH, NV_KEY_SCORE_DOUBLE, (char*)score_double, ARRAY_SIZE(score_double), NV_DATA_DOUBLE_ARRAY);
    /* clang-format on */

    nv_log("score buf    \"%s\" \"%s\"\n", score_buf[0], score_buf[1]);
    nv_log("score int    %d %d\n", score_int[0], score_int[1]);
    nv_log("score float  %f %f\n", score_float[0], score_float[1]);
    nv_log("score double %f %f\n", score_double[0], score_double[1]);

    for (size_t i = 0; i < ARRAY_SIZE(score_buf); i++) {
        if (score_buf[i]) {
            free(score_buf[i]);
        }
    }

    memset(ip,  0, sizeof(ip));
    memset(mac, 0, sizeof(mac));

    /* clang-format off */
    nv_get(CONFIG_NV_PATH, NV_KEY_IP,  (char*)ip,  ARRAY_SIZE(ip),  NV_DATA_IP);
    nv_get(CONFIG_NV_PATH, NV_KEY_MAC, (char*)mac, ARRAY_SIZE(mac), NV_DATA_MAC);
    /* clang-format on */

    nv_log("ip : %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
    nv_log("mac: %d-%d-%d-%d-%d-%d\n", mac[0], mac[1], mac[2], mac[3], mac[4],
           mac[5]);
#endif

    return 0;
}
