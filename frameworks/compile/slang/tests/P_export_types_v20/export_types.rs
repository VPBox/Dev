// -target-api 20
#pragma version(1)
#pragma rs java_package_name(foo)

#include "rs_graphics.rsh"

float f = 9.9999f;
double d = 7.0;
char c = 'a';
unsigned char uc = 'a';
short s = 1;
unsigned short us = 1;
int i = 5;
unsigned int ui;
long l = 13;
unsigned long ul = 13;
long long ll = 34;
unsigned long long ull = 34;
bool b = true;

rs_element element;
rs_type type;
rs_allocation allocation;
rs_sampler sampler;
rs_script script;
rs_mesh mesh;
rs_program_fragment program_fragment;
rs_program_vertex program_vertex;
rs_program_raster program_raster;
rs_program_store program_store;
rs_font font;
rs_data_kind dk;
rs_data_type dt;

float *fp;
int *ip;

