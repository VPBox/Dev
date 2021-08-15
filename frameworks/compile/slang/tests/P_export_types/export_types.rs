#pragma version(1)
#pragma rs java_package_name(foo)

half h = (half) 1.2f;
half2 h2 = {(half) 1.2f, (half) 2.3f};
half3 h3 = {(half) 1.2f, (half) 2.3f, (half) 3.4f};
half4 h4 = {(half) 1.2f, (half) 2.3f, (half) 3.4f, (half) 4.5f};
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
rs_data_kind dk;
rs_data_type dt;

float *fp;
int *ip;

