#include <github.com/apronchenkov/str/public/str.h>
#include <stdio.h>

u7_error Main() {
  u7_str str = u7_str_empty();
  U7_RETURN_IF_ERROR(u7_str_append(&str, "Hello, World"));
  U7_RETURN_IF_ERROR(u7_str_append(&str, '!'));
  U7_RETURN_IF_ERROR(u7_str_append(&str, '\n'));
  U7_RETURN_IF_ERROR(u7_str_append(&str, &str));
  U7_RETURN_IF_ERROR(u7_str_printf(&str, "Abra%s%d\n", "cadabra", 3417));
  fwrite(str.data, str.size, 1, stdout);
  return u7_ok();
}

int main() {
  u7_error error = Main();
  if (error.error_code) {
    fprintf(stderr, "%s(%d): %*s\n", error.payload->category->name,
            error.error_code, error.payload->message_length,
            error.payload->message);
    u7_error_release(error);
    return -1;
  }
  return 0;
}
