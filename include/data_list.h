#ifndef DATA_LIST_H
#define DATA_LIST_H

#include <stdint.h>
#include <malloc.h>

#include "list.h"


struct data_list_t {
    uint8_t *data;
    int size;
    int offset;

    struct list_head entry;
};

static void data_list_add(uint8_t *data, int len, struct list_head *head)
{
    struct data_list_t *pdata = NULL;

    if (head == NULL)
        return;

    if (data == NULL
            || len <= 0)
        return;

    pdata = (struct data_list_t *)malloc(sizeof(struct data_list_t));
    if (pdata == NULL)
    {
        printf("%s: error(%d - %s)\n", __func__, errno, strerror(errno));

        return;
    }

    pdata->data = (uint8_t *)malloc(len);
    if (pdata->data == NULL)
    {
        printf("%s: error(%d - %s)\n", __func__, errno, strerror(errno));

        return;
    }

    pdata->size = len;
    pdata->offset = 0;

    memmove(pdata->data, data, len);

    list_add(&pdata->entry, head);
}

static int data_list_len(struct list_head *head)
{
    struct data_list_t *pdata = NULL;
    int len = 0;

    if (head == NULL
        || list_empty(head))
        return 0;

    list_for_each_entry(pdata, head, entry)
    {
        if (pdata->size >= pdata->offset)
            len += pdata->size - pdata->offset;
    }

    return len;
}

static int data_list_dump(struct list_head *head, int offset, uint8_t *buf, int len)
{
    struct data_list_t *pdata = NULL;

    int slen;
    int last;
    int pos;

    list_for_each_entry(pdata, head, entry)
    {
        if (pdata->size > offset)
            break;

        offset -= pdata->size;
    }

    last = len;
    pos = 0;

    while((last > 0)
          && ((pdata != NULL) && (&pdata->entry != head))
          && (pdata->size > offset))
    {
        slen = last;

        if (slen > (pdata->size - offset))
            slen = pdata->size - offset;

        memmove(&buf[pos], &(pdata->data[offset]), slen);

        pos += slen;
        last -= slen;
        pdata = list_entry(pdata->entry._next, typeof(*pdata), entry);

        offset = 0;
    }

    return (len - last);
}

#endif // DATA_LIST_H
