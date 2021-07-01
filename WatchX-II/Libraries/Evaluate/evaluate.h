#ifndef __EVALUATE_H
#define __EVALUATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void evaluate_run_expression(const char* expression, bool persent_flag);
bool evaluate_get_is_decimal_point(void);
void evaluate_get_result(char* res, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
