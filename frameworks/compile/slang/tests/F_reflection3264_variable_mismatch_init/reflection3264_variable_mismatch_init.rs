#pragma version(1)
#pragma rs java_package_name(foo)

float a = 1.23f;

float b = 4.56f + sizeof(void*);

double c = 7.89;

double d = 98.7 * sizeof(void*);

char e = 'e';

char f = sizeof(void*);

int2 g = { 1, 2 };

int2 h = { 3, sizeof(void*) };

float4 i = { 1.2f, 3.4f, 5.6f, 7.8f };

float4 j = { 1.2f, 3.4f,  // vector component initialized differently
#ifdef __LP64__
             5.6f,
#else
             6.5f,
#endif
             7.8f };

int k
#ifdef __LP64__
= 0  // we get an error even though explicit zero initialization is redundant
#endif
    ;

int l[10] = { 1, 2, 3 };

int m[10] = { 1, 2, 3
#ifdef __LP64__
              , 4  // array with a different number of initializers
#endif
              , 5
};

int2 n = { 1
#ifdef __LP64__
           , 2  // vector with different number of initializers
#endif
};

float4 jj = { 1.2f, 3.4f,  // vector component initialized differently
#ifndef __LP64__
             5.6f,
#else
             6.5f,
#endif
             7.8f };

int kk
#ifndef __LP64__
= 0  // we get an error even though explicit zero initialization is redundant
#endif
    ;

int mm[10] = { 1, 2, 3
#ifndef __LP64__
              , 4  // array with a different number of initializers
#endif
              , 5
};

int2 nn = { 1
#ifndef __LP64__
            , 2  // vector with a different number of initializers
#endif
};
