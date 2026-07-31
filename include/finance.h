#ifndef FINANCE_H
#define FINANCE_H

typedef struct {
    char *protect_against;
    int premium;
    int compensation;
} InsuranceType;

typedef struct {
    char *name;
    InsuranceType *type;
} Insurance;

typedef struct {
    int abc;
} Bank;

typedef struct {
    int abc;
} Tax;

#endif /* FINANCE_H */
