/*
 version:0.1
 This is a tiny library written by @ponkux.The purpose is redefine the standard string class.
 Maybe it's not efficient enough but it's easy to use.
 */

#ifndef __Tstring__TString__
#define __Tstring__TString__

#include <string>
#include <iostream>
#include <cmath>

class TString{
public:
    TString();
    TString(std::string s);
    ~TString(){};
    void clear();
    TString &operator << (const char *s);
    TString &operator << (const std::string &s);
    TString &operator << (int s);
    TString &operator ++();
    TString operator ++(int);
    TString &operator --();
    TString operator --(int);
    TString operator +(TString _second);
    TString &operator +=(TString _second);
public:
    std::string str;
    int index;
private:
    bool ispp;
    std::string data,constant_data,data2;
};

#endif /* defined(__Tstring__TString__) */
