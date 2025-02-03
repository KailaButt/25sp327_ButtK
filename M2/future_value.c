#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_YEARS 50
#define MIN_YEARS 1
#define MIN_INVESTMENT 1
#define MAX_INVESTMENT 1000
#define MIN_INTEREST_RATE 1.0
#define MAX_INTEREST_RATE 15.0

int is_valid_number(const char *str) {
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str) && *str != '.' && *str != '-') return 0;
        str++;
    }
    return 1;
}

int parse_input(int argc, char *argv[], double *monthly_investment, double *annual_interest_rate, int *years) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./future_value <monthly-investment> <annual-interest-rate> <number-of-years>\n");
        return 1;
    }

    char *endptr;

    if (!is_valid_number(argv[1])) {
        fprintf(stderr, "Error: Invalid input for monthly investment.\n");
        return 1;
    }
    *monthly_investment = strtod(argv[1], &endptr);
    if (*endptr != '\0' || *monthly_investment < MIN_INVESTMENT || *monthly_investment > MAX_INVESTMENT) {
        fprintf(stderr, "Error: Invalid input for monthly investment.\n");
        return 1;
    }

    if (!is_valid_number(argv[2])) {
        fprintf(stderr, "Error: Invalid input for annual interest rate.\n");
        return 1;
    }
    *annual_interest_rate = strtod(argv[2], &endptr);
    if (*endptr != '\0' || *annual_interest_rate < MIN_INTEREST_RATE || *annual_interest_rate > MAX_INTEREST_RATE) {
        fprintf(stderr, "Error: Invalid input for annual interest rate.\n");
        return 1;
    }

    if (!is_valid_number(argv[3])) {
        fprintf(stderr, "Error: Invalid input for number of years.\n");
        return 1;
    }
    *years = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0' || *years < MIN_YEARS || *years > MAX_YEARS) {
        fprintf(stderr, "Error: Invalid input for number of years.\n");
        return 1;
    }

    return 0;
}

void calculate_future_value(double monthly_investment, double annual_interest_rate, int years, double results[MAX_YEARS][2]) {
    double monthly_interest_rate = annual_interest_rate / 12 / 100;
    int months = years * 12;
    double future_value = 0.0;

    for (int i = 0; i < months; i++) {
        future_value += monthly_investment;
        double monthly_interest_amount = future_value * monthly_interest_rate;
        future_value += monthly_interest_amount;

        if ((i + 1) % 12 == 0) {
            int year = (i + 1) / 12;
            results[year - 1][0] = future_value - (monthly_investment * 12 * year);
            results[year - 1][1] = future_value;
        }
    }
}

void display_results(double monthly_investment, double annual_interest_rate, int years, double results[MAX_YEARS][2]) {
    printf("Monthly Investment: %.2f\n", monthly_investment);
    printf("Yearly Interest Rate: %.2f%%\n", annual_interest_rate);
    printf("Years: %d\n", years);

    double future_value = results[years - 1][1];
    printf("\nFuture value after %d years is $%.2f\n", years, future_value);

    printf("\nYear   Accum Interest    Value\n");
    printf("====   ==============    ==========\n");

    for (int i = 0; i < years; i++) {
        printf("%4d   %15.2f   %10.2f\n", i + 1, results[i][0], results[i][1]);
    }
}

int main(int argc, char *argv[]) {
    double monthly_investment, annual_interest_rate;
    int years;

    if (parse_input(argc, argv, &monthly_investment, &annual_interest_rate, &years) != 0) {
        return 1;
    }

    double results[MAX_YEARS][2] = {0};

    calculate_future_value(monthly_investment, annual_interest_rate, years, results);
    display_results(monthly_investment, annual_interest_rate, years, results);

    return 0;
}

