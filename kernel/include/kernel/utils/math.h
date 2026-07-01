#ifndef KERNEL_UTILS_MATH_H
#define KERNEL_UTILS_MATH_H

#define DIV_UP(a, b) (((a) + (b) - 1) / (b))
#define ALIGN_UP(a, b) (DIV_UP((a), (b)) * (b))
#define ALIGN_DOWN(a, b) (((a) / (b)) * (b))

#endif
