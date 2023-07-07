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

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"

#if CONFIG_NV_DEBUG_LOG
#define nv_log(...)      \
    printf(__VA_ARGS__); \
    fflush(stdout);
#else
#define nv_log(...)
#endif

static char nv_data[512] = { 0 };

static nv_t nv;

#define NV_KEY_CHECK(src, des) (strncmp(src, des, strlen(des)) == 0)

#if CONFIG_NV_DEBUG_MOCK_DATA
void nv_debug(void)
{
    nv_log("nv.name   %s\n", nv_get()->name);
    nv_log("nv.sex    %s\n", nv_get()->sex);
    nv_log("nv.age    %d\n", nv_get()->age);
    nv_log("nv.height %d\n", nv_get()->height);
}
#endif

int nv_str2hex(char* str, unsigned char* out, unsigned int* outlen)
{
    char* p = str;
    char high = 0, low = 0;
    int tmplen = strlen(p), cnt = 0;
    while (cnt < (tmplen / 2)) {
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7
                                                            : *p - 48;
        low = (*(++p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p)-48 - 7
                                                             : *(p)-48;
        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p++;
        cnt++;
    }

    if (tmplen % 2 != 0) {
        out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7
                                                                : *p - 48;
    }

    if (outlen != NULL) {
        *outlen = tmplen / 2 + tmplen % 2;
    }
    return tmplen / 2 + tmplen % 2;
}

bool nv_read(char* data)
{
    FILE* fp = fopen(NV_PATH, "r");
    if (fp == NULL) {
        nv_log("nv read open fail\n");
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    fread(data, 1, file_size, fp);
    fclose(fp);

    nv_log("nv read success\n");
    return true;
}

bool nv_write(char* data)
{
    FILE* fp = fopen(NV_PATH, "w");
    if (fp == NULL) {
        nv_log("nv write open fail\n");
        return false;
    }

    fprintf(fp, "%s", data);
    fclose(fp);

    nv_log("nv write success\n");
    return true;
}

nv_t* nv_get(void)
{
    return &nv;
}

void nv_sync(char* key, char* value, uint8_t len)
{
    cJSON* json = NULL;

    if (access(NV_PATH, F_OK) == 0) {
        if (nv_read(nv_data)) {
            json = cJSON_Parse(nv_data);
        } else {
            nv_log("nv sync, nv read fail\n");
            return;
        }
    } else {
        cJSON* root = cJSON_CreateObject();
        char* str = cJSON_Print(root);
        json = cJSON_Parse(str);

        cJSON_free(str);
        cJSON_Delete(root);
    }

    if (!json) {
        nv_log("cJSON Parse fail: %s\n", cJSON_GetErrorPtr());
        return;
    }

#if CONFIG_NV_DEBUG_MOCK_DATA
    cJSON* tmp = cJSON_GetObjectItem(json, key);
    if (NV_KEY_CHECK(key, NV_KEY_NAME)) {
        memset(nv.name, 0, sizeof(nv.name));
        memcpy(nv.name, value, len);

        if (tmp && tmp->valuestring) {
            nv_log("replace name from %s to %s\n", tmp->valuestring, nv.name);
            cJSON_SetValuestring(tmp, (const char*)nv.name);
        } else {
            nv_log("%s not found, add it\n", key);
            cJSON_AddStringToObject(json, key, value);
        }
    } else if (NV_KEY_CHECK(key, NV_KEY_SEX)) {
        memset(nv.sex, 0, sizeof(nv.sex));
        memcpy(nv.sex, value, len);

        if (tmp && tmp->valuestring) {
            nv_log("replace sex from %s to %s\n", tmp->valuestring, nv.sex);
            cJSON_SetValuestring(tmp, (const char*)nv.sex);
        } else {
            nv_log("%s not found, add it\n", key);
            cJSON_AddStringToObject(json, key, value);
        }
    } else if (NV_KEY_CHECK(key, NV_KEY_HEIGHT)) {
        nv.height = *value;

        if (tmp) {
            nv_log("replace height from %d to %d\n", tmp->valueint, nv.height);
            cJSON_SetNumberHelper(tmp, nv.height);
        } else {
            nv_log("%s not found, add it\n", key);
            cJSON_AddNumberToObject(json, key, nv.height);
        }
    } else if (NV_KEY_CHECK(key, NV_KEY_AGE)) {
        nv.age = *value;

        if (tmp) {
            nv_log("replace age from %d to %d\n", tmp->valueint, nv.age);
            cJSON_SetNumberHelper(tmp, nv.age);
        } else {
            nv_log("%s not found, add it\n", key);
            cJSON_AddNumberToObject(json, key, nv.age);
        }
    } else {
        nv_log("sync check %s key fail\n", key);
        goto err;
    }
#endif

    char* p_json = cJSON_Print(json);
    if (nv_write(p_json) == false) {
        nv_log("nv write fail\n");
    }
    cJSON_free(p_json);
err:
    cJSON_Delete(json);
}

void nv_init(void)
{
    nv_log("nv init\n");

    memset(&nv, 0, sizeof(nv_t));

    if (access(NV_PATH, F_OK) != 0) {
        nv_log("%s not exist\n", NV_PATH);
        return;
    }

    if (nv_read(nv_data) == false) {
        return;
    }

    cJSON* json = cJSON_Parse(nv_data);
    if (!json) {
        nv_log("cJSON Parse %s\n", cJSON_GetErrorPtr());
        return;
    }

#if CONFIG_NV_DEBUG_MOCK_DATA
    cJSON* tmp = cJSON_GetObjectItem(json, NV_KEY_NAME);
    if (tmp && tmp->valuestring) {
        memcpy(nv.name, tmp->valuestring, strlen(tmp->valuestring));
    } else {
        nv_log("%s not found\n", NV_KEY_NAME);
    }

    tmp = cJSON_GetObjectItem(json, NV_KEY_SEX);
    if (tmp && tmp->valuestring) {
        memcpy(nv.sex, tmp->valuestring, strlen(tmp->valuestring));
    } else {
        nv_log("%s not found\n", NV_KEY_SEX);
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
#endif

    cJSON_Delete(json);
}
