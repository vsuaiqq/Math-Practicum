#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

typedef enum status_code 
{
    success,
    allocate_error,
    file_error,
} status_code;

typedef enum station_type 
{
    initial,
    intermediate,
    final,
} station_type;

typedef struct Datetime 
{
    char date[BUFSIZ];
    char time[BUFSIZ];
} Datetime;

typedef struct Station 
{
    char id[BUFSIZ];
    Datetime* arrive;
    Datetime* depart;
    station_type type;
    struct Station* next_station;
} Station;

typedef struct Stations 
{
    Station* head;
} Stations;

typedef struct Vehicle 
{
    char id[BUFSIZ];
    Stations* stations;
    struct Vehicle* next_vehicle;
} Vehicle;

typedef struct Vehicles 
{
    Vehicle* head;
} Vehicles;

typedef struct Storage 
{
    Vehicles* vehicles;
} Storage;

status_code create_storage(Storage** new_storage) 
{
    *new_storage = (Storage*)malloc(sizeof(Storage));
    if (!(*new_storage)) return allocate_error;
    (*new_storage)->vehicles = NULL;
    return success;
}

void delete_datetime(Datetime* datetime) 
{
    free(datetime);
}

void delete_station(Station* station) 
{
    if (!station) return;
    if (station->arrive) delete_datetime(station->arrive);
    if (station->depart) delete_datetime(station->depart);
    free(station);
}

void delete_stations(Stations* stations) 
{
    if (!stations) return;
    if (stations->head) 
    {
        Station* tmp = stations->head;
        while (tmp) 
        {
            Station* tmp_copy = tmp;
            tmp = tmp->next_station;
            delete_station(tmp_copy);
        }
    }
    free(stations);
}

void delete_vehicle(Vehicle* vehicle) 
{
    if (!vehicle) return;
    if (vehicle->stations) 
    {
        delete_stations(vehicle->stations);
    }
    free(vehicle);
}

void delete_vehicles(Vehicles* vehicles) 
{
    if (!vehicles) return;
    if (vehicles->head) 
    {
        Vehicle* tmp = vehicles->head;
        while (tmp) 
        {
            Vehicle* tmp_copy = tmp;
            tmp = tmp->next_vehicle;
            delete_vehicle(tmp_copy);
        }
    }
    free(vehicles);
}

void delete_storage(Storage* storage) 
{
    if (!storage) return;
    if (storage->vehicles) 
    {
        delete_vehicles(storage->vehicles);
    }
    free(storage);
}

time_t get_time_diff(Datetime* datetime_1, Datetime* datetime_2) 
{
    int day_1, month_1, year_1, hours_1, minutes_1, seconds_1;
    int day_2, month_2, year_2, hours_2, minutes_2, seconds_2;
    sscanf(datetime_1->date, "%d.%d.%d", &day_1, &month_1, &year_1);
    sscanf(datetime_1->time, "%d:%d:%d", &hours_1, &minutes_1, &seconds_1);
    sscanf(datetime_2->date, "%d.%d.%d", &day_2, &month_2, &year_2);
    sscanf(datetime_2->time, "%d:%d:%d", &hours_2, &minutes_2, &seconds_2);

    struct tm date1 = {0};
    struct tm date2 = {0};

    date1.tm_year = year_1 - 1900;
    date1.tm_mon = month_1;
    date1.tm_mday = day_1;
    date1.tm_hour = hours_1;
    date1.tm_min = minutes_1;
    date1.tm_sec = seconds_1;

    date2.tm_year = year_2 - 1900;
    date2.tm_mon = month_2;
    date2.tm_mday = day_2;
    date2.tm_hour = hours_2;
    date2.tm_min = minutes_2;
    date2.tm_sec = seconds_2;

    time_t time1 = mktime(&date1);
    time_t time2 = mktime(&date2);

    time_t diff = difftime(time2, time1);
    return diff;
}

status_code add_data(Storage* storage, const char* station_id, const char* vehicle_id, const char* date_arrive, const char* time_arrive, const char* date_depart, const char* time_depart, const station_type type) 
{
    if (!storage->vehicles) 
    {
        Vehicles* new_vehicles = (Vehicles*)malloc(sizeof(Vehicles));
        if (!new_vehicles) return allocate_error;
        new_vehicles->head = NULL;
        storage->vehicles = new_vehicles;
    }

    Vehicle* tmp_vehicle = storage->vehicles->head;
    Vehicle* prev_tmp_vehicle = storage->vehicles->head;
    bool is_found = false;
    while (tmp_vehicle) 
    {
        if (!strcmp(tmp_vehicle->id, vehicle_id)) 
        {
            is_found = true;
            break;
        }
        prev_tmp_vehicle = tmp_vehicle;
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
    if (!is_found) 
    {
        Vehicle* new_vehicle = (Vehicle*)malloc(sizeof(Vehicle));
        if (!new_vehicle) return allocate_error;
        strcpy(new_vehicle->id, vehicle_id);
        new_vehicle->next_vehicle = NULL;
        new_vehicle->stations = NULL;

        if (storage->vehicles->head) prev_tmp_vehicle->next_vehicle = new_vehicle;
        else storage->vehicles->head = new_vehicle;

        tmp_vehicle = new_vehicle;
    }

    if (!tmp_vehicle->stations) 
    {
        Stations* new_stations = (Stations*)malloc(sizeof(Stations));
        if (!new_stations) return allocate_error;
        tmp_vehicle->stations = new_stations;
    }

    Station* new_station = (Station*)malloc(sizeof(Station));
    if (!new_station) return allocate_error;
    strcpy(new_station->id, station_id);
    new_station->type = type;
    new_station->next_station = NULL;

    Datetime* new_datetime_arrive = (Datetime*)malloc(sizeof(Datetime));
    if (!new_datetime_arrive) 
    {
        delete_station(new_station);
        return allocate_error;
    }
    strcpy(new_datetime_arrive->date, date_arrive);
    strcpy(new_datetime_arrive->time, time_arrive);
    new_station->arrive = new_datetime_arrive;

    Datetime* new_datetime_depart = (Datetime*)malloc(sizeof(Datetime));
    if (!new_datetime_depart) 
    {
        delete_station(new_station);
        return allocate_error;
    }
    strcpy(new_datetime_depart->date, date_depart);
    strcpy(new_datetime_depart->time, time_depart);
    new_station->depart = new_datetime_depart;

    Station* tmp_station = tmp_vehicle->stations->head;
    Station* prev_tmp_station = tmp_vehicle->stations->head;
    while (tmp_station)
    {
        if (get_time_diff(new_datetime_arrive, tmp_station->arrive) >= 0) 
        {
            if (tmp_station == tmp_vehicle->stations->head) 
            {
                new_station->next_station = tmp_vehicle->stations->head;
                tmp_vehicle->stations->head = new_station;
            }
            else 
            {
                prev_tmp_station->next_station = new_station;
                new_station->next_station = tmp_station;
            }
            return success;
        }
        prev_tmp_station = tmp_station;
        tmp_station = tmp_station->next_station;
    }
    if (!prev_tmp_station) tmp_vehicle->stations->head = new_station;
    else prev_tmp_station->next_station = new_station;
    return success;
}

status_code read_data_from_files(const int num_of_files, char* files_pathes[], Storage* storage) 
{
    for (int i = 1; i < num_of_files; ++i) 
    {
        FILE* input_file = fopen(files_pathes[i], "r");
        if (!input_file) 
        {
            delete_storage(storage);
            return file_error;
        }

        char* line = NULL;
        size_t line_size = 0;
        char station_id[BUFSIZ];

        int getline_res;
        bool is_first_line = true;
        while ((getline_res = getline(&line, &line_size, input_file)) != -1) 
        {
            if (getline_res == -1) 
            {
                delete_storage(storage);
                fclose(input_file);
                return allocate_error;
            }

            if (is_first_line) 
            {
                sscanf(line, "%s", station_id);
                is_first_line = false;
                continue;
            }

            char vehicle_id[BUFSIZ];
            char date_arrive[BUFSIZ];
            char time_arrive[BUFSIZ];
            char date_depart[BUFSIZ];
            char time_depart[BUFSIZ];
            char type_str[BUFSIZ];
            sscanf(line, "%s %s %s %s %s %s", vehicle_id, date_arrive, time_arrive, date_depart, time_depart, type_str);

            station_type type;
            if (!strcmp(type_str, "initial")) type = initial;
            else if (!strcmp(type_str, "intermediate")) type = intermediate;
            else type = final;

            if (add_data(storage, station_id, vehicle_id, date_arrive, time_arrive, date_depart, time_depart, type) == allocate_error) 
            {
                delete_storage(storage);
                free(line);
                fclose(input_file);
                return allocate_error;
            }

            free(line);
            line = NULL;
        }
        free(line);
        fclose(input_file);
    }
    return success;
}

void max_routes_num_vehicle(Storage* storage, Vehicle** res) 
{
    int max_routes = 0;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        int cur_routes = 0;
        while (tmp_station) 
        {
            if (tmp_station->type == initial) ++cur_routes;
            tmp_station = tmp_station->next_station;
        }
        if (cur_routes > max_routes) 
        {
            max_routes = cur_routes;
            *res = tmp_vehicle;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
} 

void min_routes_num_vehicle(Storage* storage, Vehicle** res) 
{
    int min_routes = INT_MAX;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        int cur_routes = 0;
        while (tmp_station) 
        {
            if (tmp_station->type == initial) ++cur_routes;
            tmp_station = tmp_station->next_station;
        }
        if (cur_routes < min_routes) 
        {
            min_routes = cur_routes;
            *res = tmp_vehicle;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
} 

void max_distance_vehicle(Storage* storage, Vehicle** res) 
{
    int max_distance = 0;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        int cur_distance = 0;
        while (tmp_station) 
        {
            ++cur_distance;
            tmp_station = tmp_station->next_station;
        }
        if (cur_distance > max_distance) 
        {
            max_distance = cur_distance;
            *res = tmp_vehicle;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
}

void min_distance_vehicle(Storage* storage, Vehicle** res) 
{
    int min_distance = INT_MAX;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        int cur_distance = 0;
        while (tmp_station) 
        {
            ++cur_distance;
            tmp_station = tmp_station->next_station;
        }
        if (cur_distance < min_distance) 
        {
            min_distance = cur_distance;
            *res = tmp_vehicle;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
}

void max_route_length_vehicle(Storage* storage, Vehicle** res) 
{
    int max_route_length = 0;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        int cur_route_length = 0;
        while (tmp_station) 
        {
            ++cur_route_length;
            if (tmp_station->type == final) 
            {
                if (cur_route_length > max_route_length) 
                {
                    max_route_length = cur_route_length;
                    *res = tmp_vehicle;
                }
                cur_route_length = 0;
            }
            tmp_station = tmp_station->next_station;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
}

void min_route_length_vehicle(Storage* storage, Vehicle** res) 
{
    int min_route_length = INT_MAX;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        int cur_route_length = 0;
        while (tmp_station) 
        {
            ++cur_route_length;
            if (tmp_station->type == final) 
            {
                if (cur_route_length < min_route_length) 
                {
                    min_route_length = cur_route_length;
                    *res = tmp_vehicle;
                }
                cur_route_length = 0;
            }
            tmp_station = tmp_station->next_station;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
}

void max_stop_time_vehicle(Storage* storage, Vehicle** res) 
{
    time_t max_stop_time = 0;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        while (tmp_station) 
        {
            time_t cur_stop_time = get_time_diff(tmp_station->arrive, tmp_station->depart);
            if (cur_stop_time > max_stop_time) 
            {
                max_stop_time = cur_stop_time;
                *res = tmp_vehicle;
            }
            tmp_station = tmp_station->next_station;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
}

void min_stop_time_vehicle(Storage* storage, Vehicle** res) 
{
    time_t min_stop_time = LONG_MAX;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        while (tmp_station) 
        {
            time_t cur_stop_time = get_time_diff(tmp_station->arrive, tmp_station->depart);
            if (cur_stop_time < min_stop_time) 
            {
                min_stop_time = cur_stop_time;
                *res = tmp_vehicle;
            }
            tmp_station = tmp_station->next_station;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
}

void max_downtime_vehicle(Storage* storage, Vehicle** res) 
{
    time_t max_downtime = 0;
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        Station* tmp_station = tmp_vehicle->stations->head;
        time_t cur_downtime = 0;
        while (tmp_station) 
        {
            cur_downtime += get_time_diff(tmp_station->arrive, tmp_station->depart);
            tmp_station = tmp_station->next_station;
        }
        if (cur_downtime > max_downtime) 
        {
            max_downtime = cur_downtime;
            *res = tmp_vehicle;
        }
        tmp_vehicle = tmp_vehicle->next_vehicle;
    }
}

void print_vehicle_id(Vehicle* vehicle) 
{
    printf("%s\n", vehicle->id);
}

void print_menu() 
{
    printf("----------Available commands:----------\n");
    printf("1 - Find vehicle with max num of routes\n");
    printf("2 - Find vehicle with min num of routes\n");
    printf("3 - Find vehicle with max distance\n");
    printf("4 - Find vehicle with min distance\n");
    printf("5 - Find vehicle with max length of route\n");
    printf("6 - Find vehicle with min length of route\n");
    printf("7 - Find vehicle with max stop time\n");
    printf("8 - Find vehicle with min stop time\n");
    printf("9 - Find vehicle with max downtime\n");
    printf("0 - Finish the programm\n");
    printf("---------------------------------------\n");
}

void print_storage(Storage* storage) 
{
    Vehicle* tmp_vehicle = storage->vehicles->head;
    while (tmp_vehicle) 
    {
        printf("---%s---\n", tmp_vehicle->id);
        Station* tmp_station = tmp_vehicle->stations->head;
        while (tmp_station) 
        {
            printf("%s %s %s %s %s\n", tmp_station->id, tmp_station->arrive->date, tmp_station->arrive->time, tmp_station->depart->date, tmp_station->depart->time);
            tmp_station=tmp_station->next_station;
        }
        printf("\n");
        printf("----------\n");
        tmp_vehicle=tmp_vehicle->next_vehicle;
    }
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    Storage* storage;
    if (create_storage(&storage) == allocate_error) 
    {
        printf("Allocate error detected!\n");
        return 1;
    }
    if (read_data_from_files(argc, argv, storage) == allocate_error) 
    {
        delete_storage(storage);
        printf("Allocate error detected!\n");
        return 1;
    }
    print_storage(storage);
    int cmd;
    while (true) 
    {
        if (cmd == 10) while (getchar() != '\n');

        print_menu();

        printf("Enter command: ");
        if (scanf("%d", &cmd) != 1) cmd = 10;

        Vehicle* res;
        switch (cmd)
        {
            case 1:
                max_routes_num_vehicle(storage, &res);
                break;
            case 2:
                min_routes_num_vehicle(storage, &res);
                break;
            case 3:
                max_distance_vehicle(storage, &res);
                break;
            case 4:
                min_distance_vehicle(storage, &res);
                break;
            case 5:
                max_route_length_vehicle(storage, &res);
                break;
            case 6:
                min_route_length_vehicle(storage, &res);
                break;
            case 7:
                max_stop_time_vehicle(storage, &res);
                break;
            case 8:
                min_stop_time_vehicle(storage, &res);
                break;
            case 9:
                max_downtime_vehicle(storage, &res);
                break;
            case 0:
                delete_storage(storage);
                return 0;
            default:
                printf("Invalid command!\n");
        }

        if (cmd == 1 || cmd == 2 || cmd == 3 || cmd == 4 || cmd == 5 || cmd == 6 || cmd == 7 || cmd == 8 || cmd == 9) 
        {
            print_vehicle_id(res);
        }
    }

    delete_storage(storage);
    return 0;
}