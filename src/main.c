#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_OUTPUT_STR_LEN 30
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ARRAY_LEN(a) sizeof(a) / sizeof(a[0])

struct interval_t {
    double beg;
    double end;
};

void error_handle(const char* error_message) {
    fprintf(stderr, "%s", error_message);
    exit(1);
}

void free_array_of_strings(const size_t array_len, char** array) {
    for (int i = 0; i < array_len; i++) {
        free(array[i]);
    }
    free(array);
}

double rectangle_rule(const double interval_beg, const double step) {
    return step * sin(interval_beg + step / 2);
}

double simpsons_rule(const double interval_beg, const double step) {
    return step / 6 * (sin(interval_beg) + 4 * sin(interval_beg + step / 2) + sin(interval_beg + step));
}

double calculate_area(const struct interval_t interval, const unsigned int steps_num, double rule(double, double)) {
    const double step = (interval.end - interval.beg) / steps_num;
    double area = 0, current_interval_beg = interval.beg;
    for (int i = 0; i < steps_num; i++) {
        area += rule(current_interval_beg, step);
        current_interval_beg += step;
    }
    return area;
}

char** create_results(const struct interval_t interval, const size_t tests_num, const unsigned int steps_nums[]) {
    char** results_strings = (char**) malloc(tests_num * sizeof(char*));
    if (results_strings == NULL) {
        error_handle("Memory allocation for results_strings failed");
    }
    double area_rectangle, area_simpsons;
    for (int i = 0; i < tests_num; i++) {
        results_strings[i] = (char*) malloc(MAX_OUTPUT_STR_LEN * sizeof(char));
        if (results_strings[i] == NULL) {
            free_array_of_strings(tests_num, results_strings);
            error_handle("Memory allocation for a string in results_strings failed");
        }
        area_rectangle = calculate_area(interval, steps_nums[i], rectangle_rule);
        area_simpsons = calculate_area(interval, steps_nums[i], simpsons_rule);
        sprintf(results_strings[i], "%i %.5lf %.5lf\n", steps_nums[i], area_rectangle, area_simpsons);
    }
    return results_strings;
}

void read_interval(struct interval_t* interval) {
    if (printf("Enter interval's beginning: ") < 0) {
        error_handle("Printing input instructions to stdout failed");
    }
    if (scanf("%lf", &(interval -> beg)) != 1) {
        error_handle("Reading interval's beginning failed");
    }
    if (interval -> beg < 0 || interval -> beg > M_PI) {
        error_handle("Interval must begin in [0; pi]");
    }
    if (printf("Enter interval's ending: ") < 0) {
        error_handle("Printing input instructions to stdout failed");
    }
    if (scanf("%lf", &(interval -> end)) != 1) {
        error_handle("Reading interval's ending failed");
    }
    if (interval -> end <= interval -> beg || interval -> end > M_PI) {
        error_handle("Interval must end in (interval's beginning; pi]");
    }
}

void print_array_of_strings(const size_t array_len, char** array) {
    for (int i = 0; i < array_len; i++) {
        if (printf("%s", array[i]) < 0) {
            free_array_of_strings(array_len, array);
            error_handle("Printing array of strings to stdout failed");
        }
    }
}

int main() {
    const unsigned int steps_nums[] = {5, 10, 20, 100, 500, 1000};
    const size_t tests_num = ARRAY_LEN(steps_nums);
    struct interval_t interval = {0, 0};
    read_interval(&interval);
    char** results_strings = create_results(interval, tests_num, steps_nums);
    print_array_of_strings(tests_num, results_strings);
    free_array_of_strings(tests_num, results_strings);
    return 0;
}
