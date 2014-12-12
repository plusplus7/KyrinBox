#ifndef KYRINBOX_SRC_COMMON_KYRIN_MACROS_H_
#define KYRINBOX_SRC_COMMON_KYRIN_MACROS_H_

#define kyrin_likely(x)       __builtin_expect(!!(x), 1)
#define kyrin_unlikely(x)     __builtin_expect(!!(x), 0)


#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_MACROS_H_ */
