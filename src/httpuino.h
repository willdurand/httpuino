/* vim:ts=2 sw=2 sts=2 */
/*
 * httpuino - William Durand <will@drnd.me> - MIT License
 */
#ifndef HTTPUINO_H
#define HTTPUINO_H

#define BUFFER_SIZE 32
#define NB_HEADERS  4

typedef enum {
  UNKNOWN,
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  PATCH,
  OPTIONS,
  /* HTCPCP */
  BREW,
  WHEN,
  PROPFIND
} http_method;

typedef struct http_header {
  char name[BUFFER_SIZE / 2];
  char value[BUFFER_SIZE];
} http_header_t;

typedef struct http_request {
  http_method method;
  char path[BUFFER_SIZE];
  http_header_t headers[NB_HEADERS];
  char body[BUFFER_SIZE];
} http_request_t;

/**
 * This function parses an HTTP message with a few limitations such as:
 *
 *   - Parse the first 4 HTTP headers only
 *   - Header's name cannot exceed (BUFFER_SIZE / 2) chars
 *   - Header's value cannot exceed BUFFER_SIZE chars
 *   - Return a path that cannot exceed BUFFER_SIZE chars
 *   - Return a body content that cannot exceed BUFFER_SIZE chars
 */
http_request_t * parse_request(const char *);

/**
 * This function parses an HTTP body content encoded with
 * `www-form-urlencoded`.
 *
 * @return the value if parameter is found, `NULL` otherwise.
 */
char * post_value(const char *, const char *);

#endif
