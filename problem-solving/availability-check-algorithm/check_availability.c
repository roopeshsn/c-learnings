#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int hour;   // 0-23
    int minute; // 0-59
} Time;

Time new_time(int hour, int minute) {
    Time time;
    time.hour = hour;
    time.minute = minute;
    return time;
}

typedef struct {
    Time start;
    Time end;
} BusinessHours;

typedef struct {
    BusinessHours monday;
    BusinessHours tuesday;
    BusinessHours wednesday;
    BusinessHours thursday;
    BusinessHours friday;
    BusinessHours saturday;
    BusinessHours sunday;
} BusinessAvailability;

typedef struct {
    int day;
    int month;
    int year;
} Date;

Date new_date(int day, int month, int year) {
    Date date;
    date.day = day;
    date.month = month;
    date.year = year;
    return date;
}

typedef struct {
    // appointments should contain date, time, name
    char name[10];
    Time start;
    Date date;
} Appointment;

Appointment new_appointment(char name[10], Date date, Time time) {
    Appointment new_appointment;
    strncpy(new_appointment.name, name, sizeof(new_appointment.name) - 1);
    new_appointment.name[9] = '\0';
    new_appointment.date = date;
    new_appointment.start = time;
    return new_appointment;
}

void push(Appointment *appointments, int *size, int capacity, Appointment appointment) {
    if (*size >= capacity) {
        printf("Array full!\n");
        return;
    }

    appointments[*size] = appointment;
    (*size)++;
}

bool create_new_appointment(char name[10], Date date, Time time, Appointment *appointments, int *size, int capacity) {
    Appointment appointment = new_appointment(name, date, time);
    push(appointments, size, capacity, appointment);
    return true;
}

Time find_next_available_slot(Time time, int slot_length) {
    // one solution could be we can increment the requested time with slot_length and run the check_availabilty function again.
}

Time find_previous_available_slot() {
    
}

// Working days and time will be given something similar to the following and the assumption would be saturday and sunday would be holiday. Also the time will be 24-hour format.
// Monday: {start: 09:00, end: 21:00}
// Tuesday: {start: 09:00, end: 21:00}
// Wednesday: {start: 09:00, end: 21:00}
// Thursday: {start: 09:00, end: 21:00}
// Friday: {start: 09:00, end: 21:00}
// appointment slot is 30 minutes
// so say if the business availability is 9am to 6pm then, the hours should split up into each 30 min slot, which is 9/0.5 which is 20 slots.
// say if a client says that they want to book an appointment at 10 am then the function should check if there is a slot which is already booked at that time.
bool check_availability(char *name, Date date, Time time, Appointment *appointments, int size, int capacity) {
    printf("Appointment requested on %d-%d-%d at %d:%d\n", date.day, date.month, date.year, time.hour, time.minute);

    // TODO: Need to check whether the requested time is with in the business availability window.

    // With date and time check if there are any existing appointments. If not create a new appointment else the function should return next available slot.
    if(size == 0) {
        return true;
    } 

    // iterate the appointments array and check if there is already an appointment for the requested date and time
    // if there is no matching appointments then we can create a new one
    // otherwise we need to return the next available slot
    for(int i = 0; i < size; i++) {
        Appointment appointment = appointments[i];
        if(appointment.date.day == date.day && appointment.date.month == date.month && appointment.date.year == date.year) {
            // match found - we cannot create appointment for the requested date and time
            // we need to check for next available slot - before or after
            // find_next_available_slot()
            // find_previous_available_slot()
            printf("Requested date and time is already booked!\n");
            return false;
        }
    }

    return true;
}

int main(void) {
    BusinessAvailability wellness_spa_availability = {
        .monday = {.start = {9, 0}, .end = {18, 0}},
        .tuesday = {.start = {9, 0}, .end = {18, 0}},
        .wednesday = {.start = {9, 0}, .end = {18}},
        .thursday = {.start = {9, 0}, .end = {18, 0}},
        .friday = {.start = {9, 0}, .end = {18, 0}},
        .saturday = {.start = {0, 0}, .end = {0, 0}},
        .sunday = {.start = {0, 0}, .end = {0, 0}},
    };

    Appointment appointments[10];
    // fill appointments array
    int size = 0;
    create_new_appointment("ajay", new_date(21, 3, 2026), new_time(16, 0), appointments, &size, 10);
    create_new_appointment("vijay", new_date(21, 3, 2026), new_time(10, 0), appointments, &size, 10);
    create_new_appointment("ajith", new_date(21, 3, 2026), new_time(9, 0), appointments, &size, 10);
    create_new_appointment("sujith", new_date(21, 3, 2026), new_time(1, 0), appointments, &size, 10);

    check_availability("roopesh", new_date(21, 3, 2026), new_time(10, 0), appointments, size, 10);
}