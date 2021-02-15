#ifndef U7_STR_H_
#define U7_STR_H_

#include <github.com/apronchenkov/error/public/error.h>
#include <stdarg.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct {
  const char* data;
} u7_str_literal;

// A struct for string representation.
//
// NOTE: capacity=0 has a special meaning, that the string is a literal.
typedef struct {
  char* data;
  size_t size;
  size_t capacity;
} u7_str;

void u7_str_release(u7_str* self);

// Returns an empty string.n
u7_str u7_str_empty();

u7_error u7_str_reserve(u7_str* self, size_t capacity);

static inline void u7_str_clear(u7_str* self) { self->size = 0; }

static inline u7_error u7_str_append_char(u7_str* self, char c) {
  if (self->size >= self->capacity) {
    U7_RETURN_IF_ERROR(u7_str_reserve(self, self->size + 1));
  }
  self->data[self->size] = c;
  self->size += 1;
  return u7_ok();
}

static inline u7_error u7_str_append_cstr(u7_str* self, const char* cstr) {
  size_t n = strlen(cstr);
  if (self->size + n >= self->capacity) {
    U7_RETURN_IF_ERROR(u7_str_reserve(self, self->size + n));
  }
  memcpy(self->data + self->size, cstr, n);
  self->size += n;
  return u7_ok();
}

static inline u7_error u7_str_append_str(u7_str* self, const u7_str* str) {
  if (self->size + str->size >= self->capacity) {
    U7_RETURN_IF_ERROR(u7_str_reserve(self, self->size + str->size));
  }
  memcpy(self->data + self->size, str->data, str->size);
  self->size += str->size;
  return u7_ok();
}

#define u7_str_append(self, arg) \
  _Generic((arg),                               \
           int: u7_str_append_char,             \
           char*: u7_str_append_cstr,           \
           const char*: u7_str_append_cstr,     \
           u7_str*: u7_str_append_str,          \
           const u7_str*: u7_str_append_str     \
           )(self, arg)

u7_error u7_str_vprintf(u7_str* self, const char* format, va_list arg);

u7_error u7_str_printf(u7_str* self, const char* format, ...);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // U7_STR_H_
