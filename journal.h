#ifndef _logfile_h_
#define _logfile_h_


struct date {
    short year;
    short month;
    short day;
};

struct time {
    short hour;
    short minute;
    short second;
};

struct tags {
    int length;
    char * name;
    struct tags * l;
    struct tags * r;
};

struct record {
    struct time * recorded_at;
    struct time * begin;
    struct time * end;
    struct tags * tags;
    char * notes;
};

struct records {
    struct record * data;
    struct records * l;
    struct records * r;
};

struct day {
    struct date * date;
    struct records * records;
};

struct journal {
    struct day * data;
    struct journal * l;
    struct journal * r;
};

int compare_short(short l, short r);
int compare_date(struct date * l, struct date * r);
int compare_time(struct time * l, struct time * r);
int compare_record(struct record * l, struct record * r);
int compare_day(struct day * l, struct day * r);

struct tags * add_tag(struct tags * tree, char * name);
struct records * add_record(struct records * tree, struct record * data);
struct journal * add_day(struct journal * tree, struct day * data);

struct tags * new_tags();
struct record * new_record();
struct records * new_records();
struct day * new_day();
struct journal * new_journal();

#endif /* _logfile_h_ */
