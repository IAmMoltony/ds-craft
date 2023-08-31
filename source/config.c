#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#define MAX_LINE_LEN 128

typedef struct cfgdict
{
    char *key;
    char *value;
    struct cfgdict *next;
} ConfigDict;

static ConfigDict *_cfgDict = NULL;

void configInit(void)
{
    FILE *configFile = fopen("nitro:/config.cfg", "r");
    if (!configFile)
    {
        perror("error opening nitro:/config.cfg");
        hang();
    }

    char buf[MAX_LINE_LEN];
    char keyBuf[MAX_LINE_LEN];
    char valueBuf[MAX_LINE_LEN];
    while (fgets(buf, MAX_LINE_LEN, configFile))
    {
        if (buf[0] == '#' || buf[0] == '\n')
            continue; // comment or empty line

        // extract key
        int i;
        int keyLen = 0;
        int valueLen = 0;
        for (i = 0; buf[i] != ' '; i++)
            keyBuf[i] = buf[i];
        keyBuf[i++] = 0;
        keyLen = i;

        // extract value
        int j;
        for (j = 0; buf[i] != '\n'; i++, j++)
            valueBuf[j] = buf[i];
        valueBuf[j] = 0;
        valueLen = i;

        if (_cfgDict == NULL)
        {
            // create 1st entry
            _cfgDict = malloc(sizeof(ConfigDict));
            _cfgDict->key = malloc(keyLen * sizeof(char));
            _cfgDict->value = malloc(valueLen * sizeof(char));
            _cfgDict->next = NULL;

            // copy values
            memcpy(_cfgDict->key, keyBuf, keyLen);
            memcpy(_cfgDict->value, valueBuf, valueLen);
        }
        else
        {
            // find last entry
            ConfigDict *current = _cfgDict;
            for (; current->next; current = current->next)
                ;

            // allocate next entry
            current->next = malloc(sizeof(ConfigDict));
            current = current->next;

            // allocate key and value
            current->key = malloc(keyLen * sizeof(char));
            current->value = malloc(valueLen * sizeof(char));
            current->next = NULL; // last entry (next is null)

            // copy values
            memcpy(current->key, keyBuf, keyLen);
            memcpy(current->value, valueBuf, valueLen);
        }
    }
    printf("config loaded\n");

    fclose(configFile);
}

const char *configGet(const char *key)
{
    for (ConfigDict *d = _cfgDict;; d = d->next)
    {
        if (!d)
            break;

        if (strcmp(key, d->key) == 0)
            return d->value;
    }
    return "";
}

int configGetInt(const char *key)
{
    const char *strValue = configGet(key);

    // return 0 if key not found
    if (strlen(strValue) == 0)
        return 0;

    return atoi(strValue);
}
