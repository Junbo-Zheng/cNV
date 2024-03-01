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

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif /* UNUSED */

#ifndef CONFIG_NV_DATA_BUFFER_SIZE
#define CONFIG_NV_DATA_BUFFER_SIZE 512    // TODO, 1024 Bytes enough?
#endif /* CONFIG_NV_DATA_BUFFER_SIZE */

/**
 * nv_read from file
 * @param file nv file path
 * @param data data buffer
 * @return     boolean
 */
bool nv_read(const char* file, void* data)
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        nv_log("nv read open %s fail, errno %d %s\n", file, errno,
               strerror(errno));
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    fread(data, 1, size, fp);
    fclose(fp);

    return true;
}

/**
 * nv_write to file
 * @param file nv file path
 * @param data data buffer
 * @return     boolean
 */
bool nv_write(const char* file, void* data)
{
    FILE* fp = fopen(file, "w");
    if (fp == NULL) {
        nv_log("nv write open %s fail, errno %d %s\n", file, errno,
               strerror(errno));
        return false;
    }

    int fd = fileno(fp);
    if (fd != -1) {
        fsync(fd);
    } else {
        nv_log("fileno fail, errno %d %s\n", errno, strerror(errno));
    }

    fprintf(fp, "%s", (char*)data);
    fclose(fp);

    return true;
}

/**
 * nv_sync to file
 * @param file  nv file path
 * @param key   nv key
 * @param value data buffer
 * @param len   data buffer length
 * @param type  data type
 */
void nv_sync(const char* file, char* key, void* value, uint32_t len,
             nv_data_type_t type)
{
    cJSON* json = NULL;

    uint8_t nv_buffer[CONFIG_NV_DATA_BUFFER_SIZE] = { 0 };

    if (access(file, F_OK) == 0) {
        if (nv_read(file, nv_buffer)) {
            json = cJSON_Parse((const char*)nv_buffer);
        } else {
            nv_log("nv sync, nv read %s fail, error %d %s\n", file, errno,
                   strerror(errno));
            return;
        }
    }

    if (json == NULL) {
        nv_log("cJSON Parse fail: %s\n", cJSON_GetErrorPtr());

        cJSON* root = cJSON_CreateObject();
        char* str = cJSON_Print(root);
        json = cJSON_Parse(str);

        cJSON_free(str);
        cJSON_Delete(root);

        if (!json) {
            nv_log("cJSON Parse fail again: %s\n", cJSON_GetErrorPtr());
            return;
        }
    }

    cJSON* key_item = cJSON_GetObjectItem(json, key);
    if (key_item == NULL) {
        nv_log("cJSON_GetObjectItem %s key %s fail: %s\n", key, file,
               cJSON_GetErrorPtr());
    }

    switch (type) {
    case NV_DATA_U8:
    case NV_DATA_S8:
        if (key_item) {
            if (type == NV_DATA_U8) {
                cJSON_SetNumberHelper(key_item, *(uint8_t*)value);
            } else {
                cJSON_SetNumberHelper(key_item, *(int8_t*)value);
            }
        } else {
            if (type == NV_DATA_U8) {
                cJSON_AddNumberToObject(json, key, *(uint8_t*)value);
            } else {
                cJSON_AddNumberToObject(json, key, *(int8_t*)value);
            }
        }
        break;
    case NV_DATA_U16:
    case NV_DATA_S16:
        if (key_item) {
            if (type == NV_DATA_U16) {
                cJSON_SetNumberHelper(key_item, *(uint16_t*)value);
            } else {
                cJSON_SetNumberHelper(key_item, *(uint16_t*)value);
            }
        } else {
            if (type == NV_DATA_U16) {
                cJSON_AddNumberToObject(json, key, *(uint16_t*)value);
            } else {
                cJSON_AddNumberToObject(json, key, *(int16_t*)value);
            }
        }
        break;
    case NV_DATA_U32:
    case NV_DATA_S32:
        if (key_item) {
            if (type == NV_DATA_U32) {
                cJSON_SetNumberHelper(key_item, *(uint32_t*)value);
            } else {
                cJSON_SetNumberHelper(key_item, *(int32_t*)value);
            }
        } else {
            if (type == NV_DATA_U32) {
                cJSON_AddNumberToObject(json, key, *(uint32_t*)value);
            } else {
                cJSON_AddNumberToObject(json, key, *(int32_t*)value);
            }
        }
        break;
    case NV_DATA_U64:
    case NV_DATA_S64:
        if (key_item) {
            if (type == NV_DATA_U64) {
                cJSON_SetNumberHelper(key_item, *(uint64_t*)value);
            } else {
                cJSON_SetNumberHelper(key_item, *(int64_t*)value);
            }
        } else {
            if (type == NV_DATA_U64) {
                cJSON_AddNumberToObject(json, key, *(uint64_t*)value);
            } else {
                cJSON_AddNumberToObject(json, key, *(int64_t*)value);
            }
        }
        break;
    case NV_DATA_FLOAT:
    case NV_DATA_DOUBLE:
        if (key_item) {
            if (type == NV_DATA_FLOAT) {
                cJSON_SetNumberHelper(key_item, *(float*)value);
            } else {
                cJSON_SetNumberHelper(key_item, *(double*)value);
            }
        } else {
            if (type == NV_DATA_FLOAT) {
                cJSON_AddNumberToObject(json, key, *(float*)value);
            } else {
                cJSON_AddNumberToObject(json, key, *(double*)value);
            }
        }
        break;
    case NV_DATA_STR:
        if (key_item) {
            cJSON_SetValuestring(key_item, value);
        } else {
            cJSON_AddStringToObject(json, key, value);
        }
        break;
    case NV_DATA_STRING_ARRAY:
    case NV_DATA_INT_ARRAY:
    case NV_DATA_FLOAT_ARRAY:
    case NV_DATA_DOUBLE_ARRAY: {
        if (key_item) {
            cJSON* tmp = cJSON_DetachItemFromObject(json, key);
            cJSON_Delete(tmp);
        }

        if (type == NV_DATA_STRING_ARRAY) {
            cJSON_AddItemToObject(
                json, key, cJSON_CreateStringArray((const char**)value, len));
        } else if (type == NV_DATA_INT_ARRAY) {
            cJSON_AddItemToObject(json, key,
                                  cJSON_CreateIntArray((const int*)value, len));
        } else if (type == NV_DATA_FLOAT_ARRAY) {
            cJSON_AddItemToObject(
                json, key, cJSON_CreateFloatArray((const float*)value, len));
        } else if (type == NV_DATA_DOUBLE_ARRAY) {
            cJSON_AddItemToObject(
                json, key, cJSON_CreateDoubleArray((const double*)value, len));
        }
        break;
    }
    case NV_DATA_IP:
        memset(nv_buffer, 0, sizeof(nv_buffer));
        sprintf((char*)nv_buffer, "%d.%d.%d.%d", *((uint32_t*)value),
                *((uint32_t*)value + 1), *((uint32_t*)value + 2),
                *((uint32_t*)value + 3));
        if (key_item) {
            cJSON_SetValuestring(key_item, (const char*)nv_buffer);
        } else {
            cJSON_AddStringToObject(json, key, (const char* const)nv_buffer);
        }
        break;
    case NV_DATA_MAC:
        memset(nv_buffer, 0, sizeof(nv_buffer));
        sprintf((char*)nv_buffer, "%d-%d-%d-%d-%d-%d", *((uint32_t*)value),
                *((uint32_t*)value + 1), *((uint32_t*)value + 2),
                *((uint32_t*)value + 3), *((uint32_t*)value + 4),
                *((uint32_t*)value + 5));

        if (key_item) {
            cJSON_SetValuestring(key_item, (const char*)nv_buffer);
        } else {
            cJSON_AddStringToObject(json, key, (const char* const)nv_buffer);
        }
        break;
    default:
        nv_log("unknown %d type\n", type);
        break;
    }

    char* p_json = cJSON_Print(json);
    if (nv_write(file, p_json) == false) {
        nv_log("nv write %s fail, errno %d %s\n", file, errno, strerror(errno));
    }
    cJSON_free(p_json);

    cJSON_Delete(json);
}

/**
 * nv_get from file
 * @param file  nv file path
 * @param key   nv key
 * @param value data buffer
 * @param len   data buffer length
 * @param type  data type
 * @return      boolean
 */
bool nv_get(const char* file, char* key, char* value, uint32_t len,
            nv_data_type_t type)
{
    UNUSED(len);

    uint8_t nv_buffer[CONFIG_NV_DATA_BUFFER_SIZE] = { 0 };
    if (nv_read(file, nv_buffer) == false) {
        return false;
    }

    cJSON* json = cJSON_Parse((const char*)nv_buffer);
    if (json == NULL) {
        nv_log("cJSON_Parse fail %s\n", cJSON_GetErrorPtr());
        return false;
    }

    cJSON* key_item = cJSON_GetObjectItem(json, key);
    if (key_item == NULL) {
        cJSON_Delete(json);
        return false;
    }

    switch (type) {
    case NV_DATA_U8:
        *(uint8_t *)value = key_item->valueint;
        break;
    case NV_DATA_S8:
        *(int8_t *)value = key_item->valueint;
        break;
    case NV_DATA_U16:
        *(uint16_t *)value = key_item->valueint;
        break;
    case NV_DATA_S16:
        *(int16_t *)value = key_item->valueint;
        break;
    case NV_DATA_U32:
        *(uint32_t *)value = key_item->valueint;
        break;
    case NV_DATA_S32:
        *(int32_t *)value = key_item->valueint;
        break;
    case NV_DATA_U64:
        *(uint64_t *)value = key_item->valuedouble;
        break;
    case NV_DATA_S64:
        *(int64_t *)value = key_item->valuedouble;
        break;
    case NV_DATA_FLOAT:
        *(float *)value = key_item->valuedouble;
        break;
    case NV_DATA_DOUBLE:
        *(double *)value = key_item->valuedouble;
        break;
    case NV_DATA_STR:
        strcpy((char*)value, key_item->valuestring);
        break;
    case NV_DATA_STRING_ARRAY:
    case NV_DATA_INT_ARRAY:
    case NV_DATA_FLOAT_ARRAY:
    case NV_DATA_DOUBLE_ARRAY: {
        int size = cJSON_GetArraySize(key_item);
        for (int i = 0; i < size; i++) {
            cJSON* array_json = cJSON_GetArrayItem(key_item, i);
            if (array_json) {
                if (type == NV_DATA_STRING_ARRAY) {
                    memcpy(((char**)value)[i], array_json->valuestring,
                           strlen(array_json->valuestring) + 1);
                } else if (type == NV_DATA_INT_ARRAY) {
                    *((int32_t*)value + i) = array_json->valueint;
                } else if (type == NV_DATA_FLOAT_ARRAY) {
                    *((float*)value + i) = array_json->valuedouble;
                } else if (type == NV_DATA_DOUBLE_ARRAY) {
                    *((double*)value + i) = array_json->valuedouble;
                }
            }
        }
        break;
    }
    case NV_DATA_IP:
    case NV_DATA_MAC: {
        int* data = (int*)value;
        if (type == NV_DATA_IP) {
            sscanf(key_item->valuestring, "%d.%d.%d.%d", &data[0], &data[1],
                   &data[2], &data[3]);
        } else {

            sscanf(key_item->valuestring, "%d-%d-%d-%d-%d-%d", &data[0],
                   &data[1], &data[2], &data[3], &data[4], &data[5]);
        }
        break;
    }
    default:
        nv_log("unknown %d type\n", type);
        break;
    }

    cJSON_Delete(json);
    return true;
}

/**
 * nv_delete
 * @param file nv file path
 * @param key  nv key
 * @return     boolean
 */
bool nv_delete(const char* file, char* key)
{
    uint8_t nv_buffer[CONFIG_NV_DATA_BUFFER_SIZE] = { 0 };
    if (nv_read(file, nv_buffer) == false) {
        return false;
    }

    cJSON* json = cJSON_Parse((const char*)nv_buffer);
    if (json == NULL) {
        nv_log("cJSON_Parse fail %s\n", cJSON_GetErrorPtr());
        return false;
    }

    cJSON_DeleteItemFromObject(json, key);

    char* str = cJSON_Print(json);
    if (str) {
        if (nv_write(file, str) == false) {
            nv_log("nv delete write fail, error %d %s\n", errno,
                   strerror(errno));
        }
        cJSON_free(str);
    }

    cJSON_Delete(json);
    return true;
}

/**
 * nv_init
 * @param file nv file path
 * @param data nv json format data from file
 */
void nv_init(const char* file, void* data)
{
    if (access(file, F_OK) != 0) {
        nv_log("%s not exist, errno %d %s\n", file, errno, strerror(errno));
        return;
    }

    if (nv_read(file, (char*)data) == false) {
        return;
    }

    nv_log("nv init, file %s\n", file);
}
