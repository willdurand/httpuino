/* vim:ts=2 sw=2 sts=2 */
/*
 * httpuino - William Durand <will@drnd.me> - MIT License
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef __linux__
#include <bsd/string.h>
#endif

#include "httpuino.h"

/* ========================================================================= */

char * trim(char *);
char from_hex(char);

/* ========================================================================= */

http_request_t * parse_request(const char * string)
{
  char * line, * pos, * pos2, * data = strdup(string);

  int i, nbHeaders = 0;
  char * method , body[BUFFER_SIZE] = "";

  http_request_t * request = (http_request_t *)malloc(sizeof(http_request_t));

  /* parse `method` */
  line   = strtok_r(data, "\r\n", &pos);
  method = strtok_r(line, " ", &pos2);

  request->method = UNKNOWN;

  if (!strcmp("GET", method)) {
    request->method = GET;
  } else if (!strcmp("HEAD", method)) {
    request->method = HEAD;
  } else if (!strcmp("POST", method)) {
    request->method = POST;
  } else if (!strcmp("PUT", method)) {
    request->method = PUT;
  } else if (!strcmp("DELETE", method)) {
    request->method = DELETE;
  } else if (!strcmp("PATCH", method)) {
    request->method = PATCH;
  } else if (!strcmp("OPTIONS", method)) {
    request->method = OPTIONS;
  } else if (!strcmp("BREW", method)) {
    request->method = BREW;
  } else if (!strcmp("WHEN", method)) {
    request->method = WHEN;
  } else if (!strcmp("PROPFIND", method)) {
    request->method = PROPFIND;
  }

  /* parse `path` */
  strlcpy(request->path, strtok_r(NULL, " ", &pos2), sizeof(request->path));

  /* parse `headers` */
  if (nbHeaders == 0) {
    for (i = 0; i <= NB_HEADERS; i++) {
      request->headers[i].name[0]  = 0;
      request->headers[i].value[0] = 0;
    }
  }

  /*
   * use `\r` here to detect `\r\n\r\n` which
   * marks the end of the headers section.
   */
  line = strtok_r(NULL, "\r", &pos);

  while (line != NULL) {
    if (!strcmp("\n", line)) {
      break;
    }

    /* TODO: fixme */
    if (nbHeaders <= NB_HEADERS) {
      strlcpy(request->headers[nbHeaders].name, trim(strtok_r(line, ":", &pos2)), sizeof(request->headers[nbHeaders].name));
      strlcpy(request->headers[nbHeaders].value, trim(pos2), sizeof(request->headers[nbHeaders].value));
    }

    line = strtok_r(NULL, "\r", &pos);
    nbHeaders++;
  }

  /* parse `body` */
  line = strtok_r(NULL, "\r\n", &pos);

  while (line != NULL) {
    if (strlen(body) != 0) {
      strcat(body, "\r\n");
    }
    strlcat(body, line, sizeof(body));
    line = strtok_r(NULL, "\r\n", &pos);
  }

  strlcpy(request->body, body, sizeof(request->body));

  free(data);

  return request;
}

char * post_value(const char * body, const char * name)
{
  int i;

  char * value;
  char * occ = strstr(body, name);

  if (occ == NULL) {
    return NULL;
  }

  occ += strlen(name);

  if (*occ++ != '=') {
    return NULL;
  }

  value = (char *)malloc(BUFFER_SIZE * sizeof(char));

  i = 0;
  while (*occ != '\0' && i < BUFFER_SIZE) {
    if (*occ == '&') {
      break;
    }

    if (*occ == '%') {
      if (occ[1] && occ[2]) {
        value[i++] = from_hex(occ[1]) << 4 | from_hex(occ[2]);
        occ += 2;
      }
    } else if (*occ == '+') {
      value[i++] = ' ';
    } else {
      value[i++] = *occ;
    }

    occ++;
  }

  value[i] = '\0';

  return value;
}

/* ========================================================================= */

char * trim(char * input)
{
  char * end;

  /* trim leading space */
  while(isspace(*input)) {
    input++;
  }

  /* all spaces? */
  if(*input == 0) {
    return input;
  }

  /* trim trailing space */
  end = input + strlen(input) - 1;
  while(end > input && isspace(*end)) {
    end--;
  }

  /* write new null terminator */
  *(end + 1) = 0;

  return input;
}

char from_hex(char ch)
{
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}
