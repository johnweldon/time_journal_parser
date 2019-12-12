#include <stdlib.h>

#include "journal.h"


int
compare_short(short l, short r)
{
    if (l < r) {
        return -1;
    }

    if (l > r) {
        return 1;
    }

    return 0;
}

int
compare_time(struct time * l, struct time * r)
{
    if (l == r) {
        return 0;
    }

    if (0 == l) {
        return -1;
    }

    if (0 == r) {
        return 1;
    }

    int cmp = compare_short(l->hour, r->hour);

    if (0 == cmp) {
        cmp = compare_short(l->minute, r->minute);
    }

    if (0 == cmp) {
        cmp = compare_short(l->second, r->second);
    }

    return cmp;
}

int
compare_date(struct date * l, struct date * r)
{
    if (l == r) {
        return 0;
    }

    if (0 == l) {
        return -1;
    }

    if (0 == r) {
        return 1;
    }

    int cmp = compare_short(l->year, r->year);

    if (0 == cmp) {
        cmp = compare_short(l->month, r->month);
    }

    if (0 == cmp) {
        cmp = compare_short(l->day, r->day);
    }

    return cmp;
}

int
compare_record(struct record * l, struct record * r)
{
    if (l == r) {
        return 0;
    }

    if (0 == l) {
        return -1;
    }

    if (0 == r) {
        return 1;
    }

    int cmp = compare_time(l->recorded_at, r->recorded_at);

    if (0 == cmp) {
        cmp = compare_time(l->begin, r->begin);
    }

    if (0 == cmp) {
        cmp = compare_time(l->end, r->end);
    }

    return cmp;
}

int
compare_day(struct day * l, struct day * r)
{
    if (l == r) {
        return 0;
    }

    if (0 == l) {
        return -1;
    }

    if (0 == r) {
        return 1;
    }

    return compare_date(l->date, r->date);
}

struct records *
add_record(struct records * tree, struct record * data)
{
    if (0 == data) {
        data = new_record();
    }

    if (0 == tree) {
        tree = new_records();
    }

    if (0 == tree->data) {
        tree->data = data;
        return tree;
    }

    switch(compare_record(data, tree->data)) {
    case -1:
    case 0:
        return add_record(tree->l, data);

    case 1:
        return add_record(tree->r, data);

    default:
        return 0;
    }
}

struct journal *
add_day(struct journal * tree, struct day * data)
{
    if (0 == data) {
        data = new_day();
    }

    if (0 == tree) {
        tree = new_journal();
    }

    if (0 == tree->data) {
        tree->data = data;
        return tree;
    }

    switch(compare_day(data, tree->data)) {
    case -1:
    case 0:
        return add_day(tree->l, data);

    case 1:
        return add_day(tree->r, data);

    default:
        return 0;
    }
}

struct tags * new_tags()
{
    struct tags * data = malloc(sizeof(struct tags));
    return data;
}

struct record * new_record()
{
    struct record * data = malloc(sizeof(struct record));
    return data;
}

struct records * new_records()
{
    struct records * data = malloc(sizeof(struct records));
    return data;
}

struct day * new_day()
{
    struct day * data = malloc(sizeof(struct day));
    return data;
}
struct journal * new_journal()
{
    struct journal * data = malloc(sizeof(struct journal));
    return data;
}
