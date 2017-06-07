/**
@file funutils.h
@brief 全局函数定义
@author lujiashun 
@version 1.0.0
@date 2016-10-23
*/
#ifndef FUNUTILS_H
#define FUNUTILS_H


#include <QColor>
#include "complexs.h"

double time_to_float(const QString& dataTime);
QString cut_char(const QString& cStr, int index, char sub = ',');
int cut_char_to_int(const QString& cStr, int index);
double cut_char_to_float(const QString& cStr, int index);
int last_delimiter(QString Delimiters, const QString& s);
QString change_file_ext(const QString& fileName, QString ext);
QString extract_file_name(const QString& fileName);
QString possible_phase(const QString& s);
bool is_bit_on(ushort value, int bit);
void get_range(int& m1, int& m2);
QString format_complex(const Complex& data);
QString format_vector(const Complex& data);
QString switch_state(bool state);
QString get_format_str();
QString relative_path(const QString& path);


#endif 
