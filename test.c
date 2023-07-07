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

#include "nv.h"

#include <stdio.h>
#include <string.h>

#include "cJSON.h"

static void cjson_create_item(void)
{
    cJSON* root = cJSON_CreateObject();

#if CONFIG_NV_DEBUG_MOCK_DATA
    cJSON_AddStringToObject(root, NV_KEY_NAME, "zhansan");
    cJSON_AddStringToObject(root, NV_KEY_SEX, "male");
    cJSON_AddNumberToObject(root, NV_KEY_AGE, 20);
    cJSON_AddNumberToObject(root, NV_KEY_HEIGHT, 175);
#endif

    char* json_str = cJSON_Print(root);
    nv_write(json_str);

    cJSON_free(json_str);
    cJSON_Delete(root);
}

static void cjson_add_item(void)
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

    nv_init();

#if CONFIG_NV_DEBUG_MOCK_DATA
    nv_debug();

    nv_sync(NV_KEY_NAME, "xiaomi", strlen("xiaomi"));
    nv_sync(NV_KEY_SEX, "male", strlen("male"));
    nv_sync("UNUSED", "unused", strlen("unused"));

    char age = 30;
    nv_sync(NV_KEY_AGE, &age, sizeof(age));

    char height = 120;
    nv_sync(NV_KEY_HEIGHT, &height, sizeof(height));

    nv_debug();
#endif

    return 0;
}
