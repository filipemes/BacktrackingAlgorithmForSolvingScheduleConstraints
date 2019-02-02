#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "Schedule.h"

/**
 *
 * \author Filipe Mesquita
 *
 */
int main(int argc, const char * argv[])
{
    ALLCLASSES *allClasses=(ALLCLASSES*)calloc(1,sizeof(ALLCLASSES));
    load_all_classes_from_file(allClasses);
    list_all_classes(allClasses);
    ALLSCHEDULES *allSchedules=(ALLSCHEDULES*)calloc(1,sizeof(ALLSCHEDULES));
    SCHEDULE *schedule=(SCHEDULE*)calloc(1,sizeof(SCHEDULE));
    recursive_backtracking(allClasses,schedule,allSchedules,0);
    save_to_file(allSchedules);
    return 0;
}

/** \brief This method allows to list all classes available
 * \param allClasses ALLCLASSES*
 * \return void
 *
 */
void list_all_classes(ALLCLASSES *allClasses)
{
    int i=0;
    CLASS* currentClass=allClasses->firstClass;
    for(i=0; i<allClasses->numberOfClasses; i++)
    {
        printf("ClassName:%s Schedule Ref:%s day: %d hours: %.2d:%.2d-%.2d:%.2d Component: %d\n",
               currentClass->discipline,currentClass->scheduleReference,currentClass->day,currentClass->startTime->hour,
               currentClass->startTime->minute,currentClass->endTime->hour,
               currentClass->endTime->minute,currentClass->component);
        currentClass=currentClass->nextClass;
    }

}

/** \brief This method allows to save to the file all schedules
 * \param allSchedules ALLSCHEDULES*
 * \return void
 *
 */
void save_to_file(ALLSCHEDULES *allSchedules)
{
    int i;
    int j;
    FILE *fp=NULL;
    SCHEDULE * currentSchedule=NULL;
    CLASS * currentClass=NULL;
    fp=fopen(pathToFileAllSchedules,"w");
    currentSchedule=allSchedules->firstSchedule;
    if(fp!=NULL)
    {
        for(i=0; i<allSchedules->numberOfSchedules; i++)
        {
            fprintf(fp,"\n********************Schedule %d***************************",i+1);
            fprintf(fp,"\nNumber of Classes %d\n",currentSchedule->numberOfClasses);
            currentClass=currentSchedule->firstClass;
            for(j=0; j<currentSchedule->numberOfClasses; j++)
            {
                fprintf(fp,"%s|%s|%d|%.2d:%.2d-%.2d:%.2d|%d\n",
                        currentClass->discipline,currentClass->scheduleReference,currentClass->day,
                        currentClass->startTime->hour,currentClass->startTime->minute,
                        currentClass->endTime->hour,currentClass->endTime->minute,currentClass->component);
                currentClass=currentClass->nextClass;
            }
            currentSchedule=currentSchedule->nextSchedule;
        }
    }
    fclose(fp);
}
/** \brief
 *
 * \param allClasses ALLCLASSES*
 * \return void
 *
 */
void load_all_classes_from_file(ALLCLASSES *allClasses)
{
    FILE *fp=NULL;
    int i;
    int numberOfClasses=0;
    int component;
    char disciplineName[40];
    int startHour;
    int startMinutes;
    int endHour;
    int endMinutes;
    int day;
    char scheduleReference[20];
    fp=fopen(pathToFileClasses,"r");
    if(fp!=NULL)
    {
        fscanf(fp,"%*[^:] %*[:] %d",&numberOfClasses);
        allClasses->numberOfClasses=numberOfClasses;
        for(i=0; i<numberOfClasses; i++)
        {
            fscanf(fp,"%[^;] %*[;] %[^;] %*[;] %d %*[;] %d %*[:] %d %*[;] %d %*[:] %d %*[;] %d ",
                   disciplineName,scheduleReference,&day,&startHour,&startMinutes,&endHour,&endMinutes,&component);
            char *stringName=(char*)malloc(sizeof(char)*(strlen(disciplineName)+1));
            strcpy(stringName,disciplineName);
            char *stringReference=(char*)malloc(sizeof(char)*(strlen(scheduleReference)+1));
            strcpy(stringReference,scheduleReference);
            TIME* startTime=(TIME*)malloc(sizeof(TIME));
            TIME* endTime=(TIME*)malloc(sizeof(TIME));
            startTime->hour=startHour;
            startTime->minute=startMinutes;
            endTime->hour=endHour;
            endTime->minute=endMinutes;
            CLASS *newClass=(CLASS*)malloc(sizeof(CLASS));
            newClass->discipline=stringName;
            newClass->scheduleReference=stringReference;
            newClass->component=component;
            newClass->day=day;
            newClass->startTime=startTime;
            newClass->endTime=endTime;
            newClass->isSelected=0;
            newClass->nextClass=NULL;
            if(allClasses->firstClass==NULL)
            {
                allClasses->firstClass=newClass;
                newClass->prevClass=newClass;
            }
            else
            {
                allClasses->firstClass->prevClass->nextClass=newClass;
                allClasses->firstClass->prevClass=newClass;
            }
        }
    }
    fclose(fp);
}
/** \brief
 *
 * \param classToClone CLASS*
 * \return CLASS*
 *
 */
CLASS *clone_class(CLASS *classToClone)
{
    CLASS* newClass=(CLASS*)calloc(1,sizeof(CLASS));
    newClass->nextClass=NULL;
    newClass->prevClass=NULL;
    newClass->day=classToClone->day;
    newClass->discipline=classToClone->discipline;
    newClass->scheduleReference=classToClone->scheduleReference;
    newClass->startTime=classToClone->startTime;
    newClass->endTime=classToClone->endTime;
    newClass->component=classToClone->component;
    newClass->isSelected=0;
    return newClass;
}
/** \brief
 *
 * \param schedule SCHEDULE*
 * \return void
 *
 */
void list_schedule(SCHEDULE *schedule)
{
    int i;
    CLASS *currentClass=schedule->firstClass;
    printf("\nNumber of Classes %d\n",schedule->numberOfClasses);
    for(i=0; i<schedule->numberOfClasses; i++)
    {
        printf("ClassName:%s Schedule Ref:%s day:%d hours:%d:%d-%d:%d Component:%d\n",
               currentClass->discipline,currentClass->scheduleReference,currentClass->day,
               currentClass->startTime->hour,currentClass->startTime->minute,
               currentClass->endTime->hour,currentClass->endTime->minute,currentClass->component);
        currentClass=currentClass->nextClass;
    }
}
/** \brief
 *
 * \param schedule SCHEDULE*
 * \param allSchedules ALLSCHEDULES*
 * \return void
 *
 */
void save_sequence(SCHEDULE *schedule,ALLSCHEDULES *allSchedules)
{
    int i;
    CLASS *currentClass=schedule->firstClass;
    CLASS *newClass=NULL;
    CLASS *firstClass=NULL;
    SCHEDULE *newSchedule=(SCHEDULE*)calloc(1,sizeof(SCHEDULE));
    newSchedule->numberOfClasses=schedule->numberOfClasses;
    newSchedule->nextSchedule=NULL;
    for(i=0; i<schedule->numberOfClasses; i++)
    {
        if(firstClass==NULL)
        {
            firstClass=clone_class(currentClass);
            firstClass->prevClass=firstClass;
        }
        else
        {
            newClass=clone_class(currentClass);
            firstClass->prevClass->nextClass=newClass;
            newClass->prevClass=firstClass;
            firstClass->prevClass=newClass;
        }
        currentClass=currentClass->nextClass;
    }
    newSchedule->firstClass=firstClass;
    if(allSchedules->firstSchedule==NULL)
    {
        newSchedule->prevSchedule=newSchedule;
        allSchedules->firstSchedule=newSchedule;
    }
    else
    {
        newSchedule->prevSchedule=allSchedules->firstSchedule->prevSchedule;
        allSchedules->firstSchedule->prevSchedule->nextSchedule=newSchedule;
        allSchedules->firstSchedule->prevSchedule=newSchedule;
    }
    allSchedules->numberOfSchedules++;
}
/** \brief
 * \param schedule SCHEDULE*
 * \param newClass CLASS*
 * \param inserted int
 * \return int
 *
 */
int check_assignment_consistent(SCHEDULE *schedule,CLASS* newClass,int inserted)
{
    if(inserted==0)
        return IS_CONSISTENT;

    CLASS *lastClass=schedule->firstClass->prevClass;
    if(exists_class(schedule,newClass)==FALSE)
    {
        if(lastClass->day==newClass->day)
        {
            if((lastClass->endTime->hour==newClass->startTime->hour&&
                lastClass->endTime->minute<=newClass->startTime->minute)
               ||(lastClass->endTime->hour<newClass->startTime->hour))
                return IS_CONSISTENT;
        }
        else if(lastClass->day<newClass->day)
            return IS_CONSISTENT;
    }
    return NOT_CONSISTENT;
}

/** \brief
 *
 * \param schedule SCHEDULE*
 * \param newClass CLASS*
 * \return int
 *
 */
int exists_class(SCHEDULE *schedule,CLASS* newClass)
{
    int i;
    CLASS *current=schedule->firstClass;
    for(i=0; i<schedule->numberOfClasses; i++)
    {
        if(strcmp(newClass->discipline,current->discipline)==0
           &&newClass->component==current->component)
            return TRUE;
        current=current->nextClass;
    }
    return FALSE;
}
/** \brief
 *
 * \param allClasses ALLCLASSES*
 * \param schedule SCHEDULE*
 * \param allSchedules ALLSCHEDULES*
 * \param inserted int
 * \return void
 *
 */
void recursive_backtracking(ALLCLASSES *allClasses,SCHEDULE *schedule,ALLSCHEDULES *allSchedules,int inserted)
{
    int i;
    CLASS *currentClass = NULL;
    currentClass = allClasses->firstClass;
    for (i = 0; i < allClasses->numberOfClasses; i++)
    {
        if (currentClass->isSelected == 0)
        {
            if (check_assignment_consistent(schedule, currentClass, inserted) == IS_CONSISTENT)
            {
                CLASS *newClass = (CLASS *) calloc(1,sizeof(CLASS));
                newClass->discipline=currentClass->discipline;
                newClass->scheduleReference=currentClass->scheduleReference;
                newClass->isSelected=0;
                newClass->day=currentClass->day;
                newClass->startTime=currentClass->startTime;
                newClass->endTime=currentClass->endTime;
                newClass->component=currentClass->component;
                newClass->nextClass = NULL;

                if(inserted == 0)
                {
                    schedule->firstClass = newClass;
                    schedule->firstClass->prevClass = newClass;
                }
                else
                {
                    newClass->prevClass = schedule->firstClass->prevClass;
                    schedule->firstClass->prevClass->nextClass=newClass;
                    schedule->firstClass->prevClass = newClass;
                }
                inserted++;
                schedule->numberOfClasses++;
                currentClass->isSelected = 1;

                if(schedule->numberOfClasses)
                    save_sequence(schedule,allSchedules);

                recursive_backtracking(allClasses,schedule,allSchedules,inserted);

                currentClass->isSelected = 0;
                schedule->firstClass->prevClass = schedule->firstClass->prevClass->prevClass;
                schedule->firstClass->prevClass->nextClass=NULL;
                schedule->numberOfClasses--;
                inserted--;
            }
        }
        currentClass = currentClass->nextClass;
    }
}
