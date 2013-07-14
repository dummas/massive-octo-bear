#include "main.h"
#include "utils.h"

/**
* Append the character to the `string`
**/
char *append(const char *o, char s) {
  /* Get the length */
  int len = strlen(o);
  /* Create new buffer +2 spaces */
  char buf[len+2];
  /* Copy the original read-only `string` */
  strcpy(buf, o);
  /* Append the character */
  buf[len] = s;
  /* Add the end to the character */
  buf[len+1] = 0;
  /* Return the constructed character */
  return strdup(buf);

}

/**
* Long
***/
char *append_long(const char *o, const char *s) {
  uint8_t o_len = strlen(o);
  uint8_t s_len = strlen(s);
  uint8_t i_cpy = 0;
  char buf[o_len+s_len+2];
  for (i_cpy = 0; i_cpy < o_len; i_cpy++) {
    buf[i_cpy] = o[i_cpy];
  }
  for (i_cpy = 0; i_cpy < s_len; i_cpy++) {
    buf[i_cpy+o_len] = s[i_cpy];
  }
  buf[s_len + o_len + 1] = 0;
  // return strdup(s);
  return strdup(buf);
}

/**
* Message compare
**/
uint8_t msgcmp(char *s1, char *s2, uint8_t n) {
  if (strlen(s1) >= n && strlen(s2) >= n) {
    uint8_t i = 0;
    for (i = 0; i < n; i++) {
      if (s1[i] != s2[i]) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}