#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "journal.h"

extern int MAX_LEN;
const char * indent_s = "\t";

char *
append_note(const char * p, char * v )
{
    if (0 == v) {
        return (char *) p;
    }

    char * buf;

    if (0 == p) {
        buf = strndup(v, MAX_LEN);
        return buf;
    }

    buf = malloc(strnlen(p, MAX_LEN) + strnlen(v, MAX_LEN) + 2);
    sprintf(buf, "%s %s", p, v);
    return buf;
}

struct date *
set_date(struct date * d, const char * v)
{
    if (0 == v) {
        return d;
    }

    if (0 == d) {
        d = malloc(sizeof(struct date));
    }

    char buf[12];
    strncpy(buf, v, 10);
    buf[4] = 0;
    d->year = atoi(buf);
    buf[7] = 0;
    d->month = atoi(buf + 5);
    buf[10] = 0;
    d->day = atoi(buf + 8);
    return d;
}

struct time * set_time(struct time * t, const char * v)
{
    if (0 == v) {
        return t;
    }

    if (0 == t) {
        t = malloc(sizeof(struct time));
    }

    char buf[12];
    strncpy(buf, v, 10);
    buf[3] = 0;
    t->hour = atoi(buf + 1);
    buf[6] = 0;
    t->minute = atoi(buf + 4);
    buf[9] = 0;
    t->second = atoi(buf + 7);
    return t;
}

struct time * set_begin(struct time * t, const char * v)
{
    if (0 == v) {
        return t;
    }

    if (0 == t) {
        t = malloc(sizeof(struct time));
    }

    char buf[5];
    strncpy(buf, v, 4);
    t->minute = atoi(buf + 3);
    buf[2] = 0;
    t->hour = atoi(buf);
    return t;
}

struct time * set_end(struct time * t, const char * v)
{
    if (0 == v) {
        return t;
    }

    if (0 == t) {
        t = malloc(sizeof(struct time));
    }

    char buf[5];
    strncpy(buf, v + 5, 4);
    t->minute = atoi(buf + 3);
    buf[2] = 0;
    t->hour = atoi(buf);
    return t;
}



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

struct tags * add_tag(struct tags * t, char * v)
{
    if (0 == v) {
        return t;
    }

    if (0 == t) {
        t = new_tags();
    }

    if (0 == t->name) {
        t->name = strndup(v, MAX_LEN);
        return t;
    }

    if(0 >= strncmp(t->name, v, MAX_LEN)) {
        return add_tag(t->l, v);
    }

    return add_tag(t->r, v);
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
        tree->l = add_record(tree->l, data);
        return tree;

    case 1:
        tree->r = add_record(tree->r, data);
        return tree;

    default:
        fprintf(stderr, "BANG!\n");
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
        tree->l = add_day(tree->l, data);
        return tree;

    case 1:
        tree->r = add_day(tree->r, data);
        return tree;

    default:
        fprintf(stderr, "BANG!\n");
        return 0;
    }
}

struct tags * new_tags()
{
    struct tags * data = malloc(sizeof(struct tags));
    data->name = 0;
    data->l = 0;
    data->r = 0;
    return data;
}

struct record * new_record()
{
    struct record * data = malloc(sizeof(struct record));
    data->recorded_at = 0;
    data->begin = 0;
    data->end = 0;
    data->tags = 0;
    data->notes = 0;
    return data;
}

struct records * new_records()
{
    struct records * data = malloc(sizeof(struct records));
    data->data = 0;
    data->l = 0;
    data->r = 0;
    return data;
}

struct day * new_day()
{
    struct day * data = malloc(sizeof(struct day));
    data->date = 0;
    data->records = 0;
    return data;
}
struct journal * new_journal()
{
    struct journal * data = malloc(sizeof(struct journal));
    data->data = 0;
    data->l = 0;
    data->r = 0;
    return data;
}

void
print_date(int depth, struct date * d)
{
    if (0 == d) {
        return;
    }

    fprintf(stdout, "\n%*s", depth, indent_s);
    fprintf(stdout, "Date: %04d-%02d-%02d\n", d->year, d->month, d->day);
}

void
print_time(int depth, struct time * t)
{
    if (0 == t) {
        return;
    }

    fprintf(stdout, "%02d-%02d-%02d", t->hour, t->minute, t->second);
}

void
print_tags(int depth, struct tags * t)
{
    if(0 == t) {
        return;
    }

    fprintf(stdout, "\n%*s tags %p %p %p\n", depth, indent_s, t->l, t->name,
            t->r);
    print_tags(depth + 1, t->l);
    fprintf(stdout, "#%s ", t->name);
    print_tags(depth + 1, t->r);
}

void
print_record(int depth, struct record * d)
{
    if(0 == d) {
        return;
    }

    fprintf(stdout, "\n%*s record %p %p %p %p %p\n", depth, indent_s,
            d->recorded_at,
            d->begin, d->end, d->notes, d->tags);

    if (0 != d->recorded_at) {
        fprintf(stdout, "[");
        print_time(depth + 1, d->recorded_at);
        fprintf(stdout, "] ");
    }

    if (0 != d->begin) {
        print_time(depth + 1, d->begin);
        fprintf(stdout, "-");
    }

    if (0 != d->end) {
        print_time(depth + 1, d->end);
        fprintf(stdout, " ");
    }

    if (0 != d->notes) {
        fprintf(stdout, "%s ", d->notes);
    }

    print_tags(depth + 1, d->tags);
}

void
print_records(int depth, struct records * d)
{
    if(0 == d) {
        return;
    }

    fprintf(stdout, "\n%*s records %p %p %p\n", depth, indent_s, d->l, d->data,
            d->r);
    print_records(depth + 1, d->l);
    print_record(depth + 1, d->data);
    print_records(depth + 1, d->r);
}

void
print_day(int depth, struct day * d)
{
    if (0 == d) {
        return;
    }

    fprintf(stdout, "\n%*s day %p %p\n", depth, indent_s, d->date, d->records);
    print_date(depth + 1, d->date);
    print_records(depth + 1, d->records);
}

void
print_journal(int depth, struct journal * j)
{
    if (0 == j) {
        return;
    }

    fprintf(stdout, "\n%*s journal %p %p %p\n", depth, indent_s, j->l, j->data,
            j->r);
    print_journal(depth + 1, j->l);
    print_day(depth + 1, j->data);
    print_journal(depth + 1, j->r);
}
