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


struct PrinterDate {
    int day;
    int month;
    int year;
};

struct PrinterTime {
    int hour;
    int min;
    int sec;
};

#define SMFPTR_OK                0
#define SMFPTR_TIMEOUT_ERROR    -1
#define SMFPTR_INVALID_CRC      -2


#endif /* PRINTERTYPES_H */
