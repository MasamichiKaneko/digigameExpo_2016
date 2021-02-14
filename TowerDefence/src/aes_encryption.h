#pragma once

#include <string>

/* ˆÃ†‰» */
void encrypt( const char* input_filename, const unsigned char* password );

void encrypt( const char* input_filename, const char* password );

/* •œ†‰» */
void decrypt( const char* input_filename, const unsigned char* password );

void decrypt( const char* input_filename, const char* password );