#ifndef U7_STR_VIEW_H_
#define U7_STR_VIEW_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

// A struct for string_view representation.
typedef struct {
  const char* data;
  size_t size;
} u7_str_view;

static inline u7_str_view u7_str_view_make(const char* cstr) {
  u7_str_view result = {
      .data = cstr,
      .size = strlen(cstr),
  };
  return result;
}

static inline u7_str_view u7_str_view_remove_prefix(u7_str_view* str,
                                                    size_t n) {
  assert(n <= str->size);
  u7_str_view result = {
      .data = str->data,
      .size = n,
  };
  str->data += n;
  str->size -= n;
  return result;
}

static inline u7_str_view u7_str_view_prefix(u7_str_view str, size_t n) {
  assert(n <= str->size);
  u7_str_view result = {
      .data = str.data,
      .size = n,
  };
  return result;
}

bool u7_str_view_compare(u7_str_view lhs, u7_str_view rhs);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // U7_STR_VIEW_H_
