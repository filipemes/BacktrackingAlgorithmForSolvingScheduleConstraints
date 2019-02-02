#ifndef SCHEDULE_H_INCLUDED
#define SCHEDULE_H_INCLUDED
#define TRUE 1
#define FALSE 0
#define IS_CONSISTENT 5
#define NOT_CONSISTENT 6

typedef enum { sunday=1, monday, tuesday, wednesday,thursday, friday, saturday } DAY;
typedef enum { pl=0, tp } COMPONENT;

static const char *pathToFileClasses= "data/classes.txt";
static const char *pathToFileAllSchedules="data/AllSchedules.txt";

typedef struct Time
{
    short hour;
    short minute;
} TIME;


typedef struct Class
{
    DAY day;
    TIME *startTime;
    TIME *endTime;
    COMPONENT component;
    char *discipline;
    short isSelected;
    struct Class *nextClass;
    struct Class *prevClass;
    char *scheduleReference;
} CLASS;

typedef struct AllClasses
{
    CLASS *firstClass;
    short numberOfClasses;
} ALLCLASSES;

typedef struct Schedule
{
    CLASS *firstClass;
    short numberOfClasses;
    struct Schedule *nextSchedule;
    struct Schedule *prevSchedule;
} SCHEDULE;

typedef struct AllSchedule
{
    SCHEDULE *firstSchedule;
    short numberOfSchedules;
} ALLSCHEDULES;

void recursive_backtracking(ALLCLASSES *allClasses,SCHEDULE *schedule,ALLSCHEDULES *allSchedules,int inserted);
void list_all_classes(ALLCLASSES *allClasses);
void load_all_classes_from_file(ALLCLASSES *allClasses);
int check_assignment_consistent(SCHEDULE *schedule,CLASS* newClass,int inserted);
void save_to_file(ALLSCHEDULES *allSchedules);
void save_sequence(SCHEDULE *schedule,ALLSCHEDULES *allSchedules);
CLASS *clone_class(CLASS *classToClone);
int exists_class(SCHEDULE *schedule,CLASS* newClass);
void list_schedule(SCHEDULE *schedule);
#endif // SCHEDULE_H_INCLUDED
