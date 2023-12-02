// Jumbo of all external C files.
#if 0
#define UMKA_STATIC
#define IS_EMSCRIPTEN
#include "umka_api.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(IS_EMSCRIPTEN)
#include <emscripten.h>
#endif

// fn localstorage_get(): str
UMKA_EXPORT void
localstorage_get(UmkaStackSlot *p, UmkaStackSlot *r)
{
#if defined(IS_EMSCRIPTEN)
  void *dat = calloc(1, 1024);

  EM_ASM({
    var str = localStorage.getItem('localstorage');
    if (str === null)
      str = '';
    var dat = (new TextEncoder()).encode(str);
    setValue($0, dat, 'i32');
  }, dat);
  p->ptrVal = umkaMakeStr(umka, buf);
  free(buf);
#else
  FILE *f = fopen("localstorage.txt", "rb");
  void *umka = r->ptrVal;

  if (f == NULL)
  {
    r->ptrVal = umkaMakeStr(umka, "");
    return;
  }
 
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buf = calloc(size + 1, 1);
  fread(buf, 1, size, f);

  fclose(f);

  r->ptrVal = umkaMakeStr(umka, buf);
  free(buf);
#endif
}

// fn localstorage_set(s: str)
UMKA_EXPORT void
localstorage_set(UmkaStackSlot *p, UmkaStackSlot *r)
{
#if defined(IS_EMSCRIPTEN)
  EM_ASM({
    localStorage.setItem('localstorage', $0);
  }, p->ptrVal);
#else
  FILE *f = fopen("localstorage.txt", "wb");
  char *buf = p->ptrVal;

  fwrite(buf, 1, strlen(buf), f);
  fclose(f);
#endif
}
#endif
