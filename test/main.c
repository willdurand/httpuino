/* vim:ts=2 sw=2 sts=2 */
#include <seatest.h>
#include <stdlib.h>

#include "httpuino.h"

http_request_t request1, request2, request3, request4;

void setup() {
  const char msg1[128] = "GET /path/file.html HTTP/1.1\r\nHost: www.host1.com:80\r\n\r\nbody\r\n<br>\r\n\r\nfoo";
  const char msg2[128] = "POST /foo HTTP/1.1\r\nHost: 192.168.1.17\r\nAccept-Encoding: gzip, deflate\r\nAccept: */*\r\nUser-Agent: HTTPie/0.8.0\r\n\r\nbody=babar";
  const char msg3[140] = "BREW / HTTP/1.1\r\nReally-Really-Long-Header-Name: And-Really-Really-Really-Really-Really-Long-Value\r\n\r\nthis=is&a=buffer&overflow=foobarbaz";
  const char msg4[128] = "POST / HTTP/1.1\r\n\r\ncontent";

  request1 = parse_request(msg1);
  request2 = parse_request(msg2);
  request3 = parse_request(msg3);
  request4 = parse_request(msg4);
}

void test_method() {
  assert_int_equal(GET, request1.method);
  assert_int_equal(POST, request2.method);
  assert_int_equal(BREW, request3.method);
  assert_int_equal(POST, request4.method);
}

void test_path() {
  assert_string_equal("/path/file.html", request1.path);
  assert_string_equal("/foo", request2.path);
  assert_string_equal("/", request3.path);
}

void test_headers() {
  assert_string_equal("Host", request1.headers[0].name);
  assert_string_equal("www.host1.com:80", request1.headers[0].value);
}

void test_header_name_is_limited_to_16_chars() {
  assert_int_equal(16, sizeof(request3.headers[0].name));
  assert_string_equal("Really-Really-L", request3.headers[0].name);
}

void test_header_value_is_limited_to_32_chars() {
  assert_int_equal(32, sizeof(request3.headers[0].value));
  assert_string_equal("And-Really-Really-Really-Really", request3.headers[0].value);
}

void test_no_headers() {
  assert_string_equal("", request4.headers[0].name);
  assert_string_equal("", request4.headers[0].value);
}

void test_body() {
  assert_string_equal("body\r\n<br>\r\nfoo", request1.body);
  assert_string_equal("body=babar", request2.body);

  assert_string_equal("this=is&a=buffer&overflow=fooba", request3.body);
  assert_int_equal(32, sizeof(request3.body));

  assert_string_equal("content", request4.body);
}

void test_post_value_simple()
{
  char * ret = post_value("var=babar", "var");

  assert_string_equal("babar", ret);
  free(ret);
}

void test_post_value_with_multiple_params()
{
  char * ret;

  ret = post_value("var=babar&foo=bar", "var");
  assert_string_equal("babar", ret);
  free(ret);

  ret = post_value("var=babar&foo=bar", "foo");
  assert_string_equal("bar", ret);
  free(ret);
}

void test_post_value_with_url_encoded_values()
{
  char * ret;

  ret = post_value("var=%23", "var");
  assert_string_equal("#", ret);
  free(ret);

  ret = post_value("var=a+t+%26+t", "var");
  assert_string_equal("a t & t", ret);
  free(ret);

  ret = post_value("var=this+is+it", "var");
  assert_string_equal("this is it", ret);
  free(ret);
}

void test_post_value_param_not_present()
{
  char * ret;

  ret = post_value("var=babar", "foo");
  assert_true(ret == NULL);

  ret = post_value("", "foo");
  assert_true(ret == NULL);
}

void test_suite() {
  run_test(test_method);
  run_test(test_path);
  run_test(test_headers);
  run_test(test_header_name_is_limited_to_16_chars);
  run_test(test_header_value_is_limited_to_32_chars);
  run_test(test_no_headers);
  run_test(test_body);
  run_test(test_post_value_simple);
  run_test(test_post_value_with_multiple_params);
  run_test(test_post_value_with_url_encoded_values);
  run_test(test_post_value_param_not_present);
}

int main(int argc, char ** argv) {
  suite_setup(setup);
  run_tests(test_suite);

  return 0;
}
