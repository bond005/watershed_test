#ifndef RANDLIB_H
#define RANDLIB_H

extern long g_seedForRandom;

double generate_random_value();
double generate_random_value(double min_val, double max_val);
void initialize_random_generator(long value);

#endif // RANDLIB_H
