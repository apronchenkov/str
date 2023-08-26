#include "@/public/str.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void u7_str_release(u7_str* self) {
  if (self->capacity > 0) {
    assert(self->data != NULL);
    free(self->data);
  }
  *self = u7_str_empty();
}

u7_error u7_str_reserve(u7_str* self, size_t capacity) {
  char* data;
  if (capacity <= self->capacity) {
    return u7_ok();
  }
  if (capacity < 24) {
    capacity = 24;
  }
  if (self->capacity == 0) {
    if (capacity < (self->size * 3 + 1) / 2) {
      capacity = (self->size * 3 + 1) / 2;
    }
    data = malloc(capacity);
    if (!data) {
      return u7_errorf(u7_errno_category(), ENOMEM,
                       "cannot allocate memory: malloc(size=%zu) returned NULL",
                       capacity);
    }
    memcpy(data, self->data, self->size);
  } else {
    if (capacity < (self->capacity * 3 + 1) / 2) {
      capacity = (self->capacity * 3 + 1) / 2;
    }
    data = realloc(self->data, capacity);
    if (!data) {
      return u7_errorf(
          u7_errno_category(), ENOMEM,
          "cannot allocate memory: realloc(new_size=%zu) returned NULL",
          capacity);
    }
  }
  self->data = data;
  self->capacity = capacity;
  return u7_ok();
}

u7_error u7_str_vappendf(u7_str* self, const char* format, va_list arg) {
  size_t n = 0;
  if (self->capacity > 0) {
    n = self->capacity - self->size;
  }
  va_list arg_copy;
  va_copy(arg_copy, arg);
  int m = vsnprintf(self->data + self->size, n, format, arg_copy);
  va_end(arg_copy);
  if (m < 0) {
    goto bad_format_string;
  }
  if (((size_t)m) >= n) {
    U7_RETURN_IF_ERROR(u7_str_reserve(self, self->size + m + 1));
    m = vsnprintf(self->data + self->size, m + 1, format, arg);
    if (m < 0) {
      goto bad_format_string;
    }
    assert(((size_t)m) < self->capacity - self->size);
  }
  self->size += m;
  return u7_ok();
bad_format_string:
  return u7_errorf(u7_errno_category(), EINVAL, "bad format string: %s",
                   format);
}

u7_error u7_str_appendf(u7_str* self, const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  u7_error error = u7_str_vappendf(self, format, arg);
  va_end(arg);
  return error;
}

u7_error u7_vstrf(u7_str* self, const char* format, va_list arg) {
  u7_str_clear(self);
  return u7_str_vappendf(self, format, arg);
}

u7_error u7_strf(u7_str* self, const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  u7_error error = u7_vstrf(self, format, arg);
  va_end(arg);
  return error;
}

u7_error u7_str_append_error(u7_str* self, u7_error error) {
  return u7_str_appendf(self, "%" U7_ERROR_FMT, U7_ERROR_FMT_PARAMS(error));
}
