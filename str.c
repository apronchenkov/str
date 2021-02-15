#include "@/public/str.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

u7_str u7_str_empty() {
  u7_str result = {.data = "", .size = 0, .capacity = 0};
  return result;
}

u7_error* u7_str_reserve(u7_str* self, size_t capacity) {
  char* data;
  if (capacity <= self->capacity) {
    return NULL;
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
      return u7_error_out_of_memory();
    }
    memcpy(data, self->data, self->size);
  } else {
    if (capacity < (self->capacity * 3 + 1) / 2) {
      capacity = (self->capacity * 3 + 1) / 2;
    }
    data = realloc(self->data, capacity);
    if (!data) {
      return u7_error_out_of_memory();
    }
  }
  self->data = data;
  self->capacity = capacity;
  return NULL;
}

static u7_error* u7_error_bad_format_string() {
#define u7_error_bad_format_string_message "bad format string"
  static struct u7_error_base result = {
      .ref_count = 1,
      .destroy_fn = &u7_error_destroy_noop_fn,
      .error_code = EINVAL,
      .message = u7_error_bad_format_string_message,
      .message_length = sizeof(u7_error_bad_format_string_message) - 1,
  };
#undef u7_error_out_of_memory_message
  if (!result.category) {  // Cannot be statically initialized.
    result.category = u7_error_errno_category();
  }
  return u7_error_acquire(&result);
}

u7_error* u7_str_vprintf(u7_str* self, const char* format, va_list arg) {
  size_t n = 0;
  if (self->capacity > 0) {
    n = self->capacity - self->size;
  }
  va_list arg_copy;
  va_copy(arg_copy, arg);
  int m = vsnprintf(self->data + self->size, n, format, arg_copy);
  va_end(arg_copy);
  if (m < 0) {
    return u7_error_bad_format_string();
  }
  if (((size_t)m) >= n) {
    U7_RETURN_IF_ERROR(u7_str_reserve(self, self->size + m + 1));
    m = vsnprintf(self->data + self->size, m + 1, format, arg);
    if (m < 0) {
      return u7_error_bad_format_string();
    }
    assert(((size_t)m) < self->capacity - self->size);
  }
  self->size += m;
  return NULL;
}

u7_error* u7_str_printf(u7_str* self, const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  u7_error* error = u7_str_vprintf(self, format, arg);
  va_end(arg);
  return error;
}
