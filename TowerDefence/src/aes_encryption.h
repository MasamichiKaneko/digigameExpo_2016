#pragma once

#include <string>

/* �Í��� */
void encrypt( const char* input_filename, const unsigned char* password );

void encrypt( const char* input_filename, const char* password );

/* ������ */
void decrypt( const char* input_filename, const unsigned char* password );

void decrypt( const char* input_filename, const char* password );