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

int compare_short(short, short);
int compare_date(struct date *, struct date *);
int compare_time(struct time *, struct time *);
int compare_record(struct record *, struct record *);
int compare_day(struct day *, struct day *);

struct tags * add_tag(struct tags *, char *);
struct records * add_record(struct records *, struct record *);
struct journal * add_day(struct journal *, struct day *);

struct tags * new_tags();
struct record * new_record();
struct records * new_records();
struct day * new_day();
struct journal * new_journal();


char * append_note(const char *, char *);

struct date * set_date(struct date *, const char *);
struct time * set_time(struct time *, const char *);
struct time * set_begin(struct time *, const char *);
struct time * set_end(struct time *, const char *);

void print_date(int depth, struct date *);
void print_time(int depth, struct time *);
void print_tags(int depth, struct tags *);
void print_record(int depth, struct record *);
void print_records(int depth, struct records *);
void print_day(int depth, struct day *);
void print_journal(int depth, struct journal *);

#endif /* _logfile_h_ */
