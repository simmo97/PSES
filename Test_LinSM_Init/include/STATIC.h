#ifndef STATIC_H
#define STATIC_H_

#ifdef UNIT_TEST
#define STATIC  
#else
#define STATIC static
#endif // UNIT_TEST

#endif // STATIC_H