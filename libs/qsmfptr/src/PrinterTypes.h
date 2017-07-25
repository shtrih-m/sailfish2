/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PrinterTypes.h
 * Author: V.Kravtsov
 *
 * Created on June 28, 2016, 10:51 PM
 */

#ifndef PRINTERTYPES_H
#define PRINTERTYPES_H
#include <stdint.h>

struct PrinterDate {
    uint8_t day;
    uint8_t month;
    uint8_t year;
};

struct PrinterTime {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
};

#endif /* PRINTERTYPES_H */
