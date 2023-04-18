/* nettle-benchmark.c
 *
 * Tries the performance of the various algorithms.
 *
 */
 
/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2001, 2010 Niels Möller
 *  
 * The nettle library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 * 
 * The nettle library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with the nettle library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02111-1301, USA.
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "timing.h"

#include "aes.h"
#include "arcfour.h"
#include "blowfish.h"
#include "cast128.h"
#include "cbc.h"
#include "ctr.h"
#include "des.h"
#include "gcm.h"
#include "memxor.h"
#include "salsa20.h"
#include "serpent.h"
#include "sha1.h"
#include "sha2.h"
#include "sha3.h"
#include "twofish.h"
#include "umac.h"

#include "nettle-meta.h"
#include "nettle-internal.h"

#include "getopt.h"

static double frequency = 0.0;

/* Process BENCH_BLOCK bytes at a time, for BENCH_INTERVAL seconds. */
#define BENCH_BLOCK 10240
#define BENCH_INTERVAL 0.1

/* FIXME: Proper configure test for rdtsc? */
#ifndef WITH_CYCLE_COUNTER
# if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#  define WITH_CYCLE_COUNTER 1
# else
#  define WITH_CYCLE_COUNTER 0
# endif
#endif

#if WITH_CYCLE_COUNTER
# if defined(__i386__)
#define GET_CYCLE_COUNTER(hi, lo)		\
  __asm__("xorl %%eax,%%eax\n"			\
	  "movl %%ebx, %%edi\n"			\
	  "cpuid\n"				\
	  "rdtsc\n"				\
	  "movl %%edi, %%ebx\n"			\
	  : "=a" (lo), "=d" (hi)		\
	  : /* No inputs. */			\
	  : "%edi", "%ecx", "cc")
# elif defined(__x86_64__)
#define GET_CYCLE_COUNTER(hi, lo)		\
  __asm__("xorl %%eax,%%eax\n"			\
	  "mov %%rbx, %%r10\n"			\
	  "cpuid\n"				\
	  "rdtsc\n"				\
	  "mov %%r10, %%rbx\n"			\
	  : "=a" (lo), "=d" (hi)		\
	  : /* No inputs. */			\
	  : "%r10", "%rcx", "cc")
# endif
#define BENCH_ITERATIONS 10
#endif

static void NORETURN PRINTF_STYLE(1,2)
die(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  exit(EXIT_FAILURE);
}

static double overhead = 0.0; 

/* Returns second per function call */
static double
time_function(void (*f)(void *arg), void *arg)
{
  unsigned ncalls;
  double elapsed;

  for (ncalls = 10 ;;)
    {
      unsigned i;

      time_start();
      for (i = 0; i < ncalls; i++)
	f(arg);
      elapsed = time_end();
      if (elapsed > BENCH_INTERVAL)
	break;
      else if (elapsed < BENCH_INTERVAL / 10)
	ncalls *= 10;
      else
	ncalls *= 2;
    }
  return elapsed / ncalls - overhead;
}

static void
bench_nothing(void *arg UNUSED)
{
  return;
}

struct bench_memxor_info
{
  uint8_t *dst;
  const uint8_t *src;
  const uint8_t *other;  
};

static void
bench_memxor(void *arg)
{
  struct bench_memxor_info *info = arg;
  memxor (info->dst, info->src, BENCH_BLOCK);
}

static void
bench_memxor3(void *arg)
{
  struct bench_memxor_info *info = arg;
  memxor3 (info->dst, info->src, info->other, BENCH_BLOCK);
}

struct bench_hash_info
{
  void *ctx;
  nettle_hash_update_func *update;
  const uint8_t *data;
};

static void
bench_hash(void *arg)
{
  struct bench_hash_info *info = arg;
  info->update(info->ctx, BENCH_BLOCK, info->data);
}

struct bench_cipher_info
{
  void *ctx;
  nettle_crypt_func *crypt;
  uint8_t *data;
};

static void
bench_cipher(void *arg)
{
  struct bench_cipher_info *info = arg;
  info->crypt(info->ctx, BENCH_BLOCK, info->data, info->data);
}

struct bench_cbc_info
{
  void *ctx;
  nettle_crypt_func *crypt;
 
  uint8_t *data;
  
  unsigned block_size;
  uint8_t *iv;
};

static void
bench_cbc_encrypt(void *arg)
{
  struct bench_cbc_info *info = arg;
  cbc_encrypt(info->ctx, info->crypt,
	      info->block_size, info->iv,
	      BENCH_BLOCK, info->data, info->data);
}

static void
bench_cbc_decrypt(void *arg)
{
  struct bench_cbc_info *info = arg;
  cbc_decrypt(info->ctx, info->crypt,
	      info->block_size, info->iv,
	      BENCH_BLOCK, info->data, info->data);
}

static void
bench_ctr(void *arg)
{
  struct bench_cbc_info *info = arg;
  ctr_crypt(info->ctx, info->crypt,
	    info->block_size, info->iv,
	    BENCH_BLOCK, info->data, info->data);
}

/* Set data[i] = floor(sqrt(i)) */
static void
init_data(uint8_t *data)
{
  unsigned i,j;
  for (i = j = 0; i<BENCH_BLOCK;  i++)
    {
      if (j*j < i)
	j++;
      data[i] = j;
    }
}

static void
init_key(unsigned length,
         uint8_t *key)
{
  unsigned i;
  for (i = 0; i<length; i++)
    key[i] = i;
}

static void
header(void)
{
  printf("%18s %11s Mbyte/s%s\n",
	 "Algorithm", "mode", 
	 frequency > 0.0 ? " cycles/byte cycles/block" : "");  
}

static void
display(const char *name, const char *mode, unsigned block_size,
	double time)
{
  printf("%18s %11s %7.2f",
	 name, mode,
	 BENCH_BLOCK / (time * 1048576.0));
  if (frequency > 0.0)
    {
      printf(" %11.2f", time * frequency / BENCH_BLOCK);
      if (block_size > 0)
	printf(" %12.2f", time * frequency * block_size / BENCH_BLOCK);
    }
  printf("\n");
}

static void *
xalloc(size_t size)
{
  void *p = malloc(size);
  if (!p)
    die("Virtual memory exhausted.\n");

  return p;
}

static void
time_overhead(void)
{
  overhead = time_function(bench_nothing, NULL);
  printf("benchmark call overhead: %7f us", overhead * 1e6);
  if (frequency > 0.0)
    printf("%7.2f cycles\n", overhead * frequency);
  printf("\n");  
}



static void
time_memxor(void)
{
  struct bench_memxor_info info;
  uint8_t src[BENCH_BLOCK + sizeof(long)];
  uint8_t other[BENCH_BLOCK + sizeof(long)];
  uint8_t dst[BENCH_BLOCK];

  info.src = src;
  info.dst = dst;

  display ("memxor", "aligned", sizeof(unsigned long),
	   time_function(bench_memxor, &info));
  info.src = src + 1;
  display ("memxor", "unaligned", sizeof(unsigned long),
	   time_function(bench_memxor, &info));

  info.src = src;
  info.other = other;
  display ("memxor3", "aligned", sizeof(unsigned long),
	   time_function(bench_memxor3, &info));

  info.other = other + 1;
  display ("memxor3", "unaligned01", sizeof(unsigned long),
	   time_function(bench_memxor3, &info));
  info.src = src + 1;
  display ("memxor3", "unaligned11", sizeof(unsigned long),
	   time_function(bench_memxor3, &info));
  info.other = other + 2;
  display ("memxor3", "unaligned12", sizeof(unsigned long),
	   time_function(bench_memxor3, &info));  
}

static void
time_hash(const struct nettle_hash *hash)
{
  static uint8_t data[BENCH_BLOCK];
  struct bench_hash_info info;

  info.ctx = xalloc(hash->context_size); 
  info.update = hash->update;
  info.data = data;

  init_data(data);
  hash->init(info.ctx);

  display(hash->name, "update", hash->block_size,
	  time_function(bench_hash, &info));

  free(info.ctx);
}

static void
time_umac(void)
{
  static uint8_t data[BENCH_BLOCK];
  struct bench_hash_info info;
  struct umac32_ctx ctx32;
  struct umac64_ctx ctx64;
  struct umac96_ctx ctx96;
  struct umac128_ctx ctx128;
  
  uint8_t key[16];

  umac32_set_key (&ctx32, key);
  info.ctx = &ctx32;
  info.update = (nettle_hash_update_func *) umac32_update;
  info.data = data;

  display("umac32", "update", UMAC_DATA_SIZE,
	  time_function(bench_hash, &info));

  umac64_set_key (&ctx64, key);
  info.ctx = &ctx64;
  info.update = (nettle_hash_update_func *) umac64_update;
  info.data = data;

  display("umac64", "update", UMAC_DATA_SIZE,
	  time_function(bench_hash, &info));

  umac96_set_key (&ctx96, key);
  info.ctx = &ctx96;
  info.update = (nettle_hash_update_func *) umac96_update;
  info.data = data;

  display("umac96", "update", UMAC_DATA_SIZE,
	  time_function(bench_hash, &info));

  umac128_set_key (&ctx128, key);
  info.ctx = &ctx128;
  info.update = (nettle_hash_update_func *) umac128_update;
  info.data = data;

  display("umac128", "update", UMAC_DATA_SIZE,
	  time_function(bench_hash, &info));
}

static void
time_gcm(void)
{
  static uint8_t data[BENCH_BLOCK];
  struct bench_hash_info hinfo;
  struct bench_cipher_info cinfo;
  struct gcm_aes_ctx ctx;

  uint8_t key[16];
  uint8_t iv[GCM_IV_SIZE];

  gcm_aes_set_key(&ctx, sizeof(key), key);
  gcm_aes_set_iv(&ctx, sizeof(iv), iv);

  hinfo.ctx = &ctx;
  hinfo.update = (nettle_hash_update_func *) gcm_aes_update;
  hinfo.data = data;
  
  display("gcm-aes", "update", GCM_BLOCK_SIZE,
	  time_function(bench_hash, &hinfo));
  
  cinfo.ctx = &ctx;
  cinfo.crypt = (nettle_crypt_func *) gcm_aes_encrypt;
  cinfo.data = data;

  display("gcm-aes", "encrypt", GCM_BLOCK_SIZE,
	  time_function(bench_cipher, &cinfo));

  cinfo.crypt = (nettle_crypt_func *) gcm_aes_decrypt;

  display("gcm-aes", "decrypt", GCM_BLOCK_SIZE,
	  time_function(bench_cipher, &cinfo));
}

static int
prefix_p(const char *prefix, const char *s)
{
  size_t i;
  for (i = 0; prefix[i]; i++)
    if (prefix[i] != s[i])
      return 0;
  return 1;
}

static int
block_cipher_p(const struct nettle_cipher *cipher)
{
  /* Don't use nettle cbc and ctr for openssl ciphers. */
  return cipher->block_size > 0 && !prefix_p("openssl", cipher->name);
}

static void
time_cipher(const struct nettle_cipher *cipher)
{
  void *ctx = xalloc(cipher->context_size);
  uint8_t *key = xalloc(cipher->key_size);

  static uint8_t data[BENCH_BLOCK];

  printf("\n");
  
  init_data(data);

  {
    /* Decent initializers are a GNU extension, so don't use it here. */
    struct bench_cipher_info info;
    info.ctx = ctx;
    info.crypt = cipher->encrypt;
    info.data = data;
    
    init_key(cipher->key_size, key);
    cipher->set_encrypt_key(ctx, cipher->key_size, key);

    display(cipher->name, "ECB encrypt", cipher->block_size,
	    time_function(bench_cipher, &info));
  }
  
  {
    struct bench_cipher_info info;
    info.ctx = ctx;
    info.crypt = cipher->decrypt;
    info.data = data;
    
    init_key(cipher->key_size, key);
    cipher->set_decrypt_key(ctx, cipher->key_size, key);

    display(cipher->name, "ECB decrypt", cipher->block_size,
	    time_function(bench_cipher, &info));
  }

  if (block_cipher_p(cipher))
    {
      uint8_t *iv = xalloc(cipher->block_size);
      
      /* Do CBC mode */
      {
        struct bench_cbc_info info;
	info.ctx = ctx;
	info.crypt = cipher->encrypt;
	info.data = data;
	info.block_size = cipher->block_size;
	info.iv = iv;
    
        memset(iv, 0, sizeof(iv));
    
        cipher->set_encrypt_key(ctx, cipher->key_size, key);

	display(cipher->name, "CBC encrypt", cipher->block_size,
		time_function(bench_cbc_encrypt, &info));
      }

      {
        struct bench_cbc_info info;
	info.ctx = ctx;
	info.crypt = cipher->decrypt;
	info.data = data;
	info.block_size = cipher->block_size;
	info.iv = iv;
    
        memset(iv, 0, sizeof(iv));

        cipher->set_decrypt_key(ctx, cipher->key_size, key);

	display(cipher->name, "CBC decrypt", cipher->block_size,
		time_function(bench_cbc_decrypt, &info));
      }

      /* Do CTR mode */
      {
        struct bench_cbc_info info;
	info.ctx = ctx;
	info.crypt = cipher->encrypt;
	info.data = data;
	info.block_size = cipher->block_size;
	info.iv = iv;
    
        memset(iv, 0, sizeof(iv));
    
        cipher->set_encrypt_key(ctx, cipher->key_size, key);

	display(cipher->name, "CTR", cipher->block_size,
		time_function(bench_ctr, &info));	
      }
      
      free(iv);
    }
  free(ctx);
  free(key);
}

/* Try to get accurate cycle times for assembler functions. */
#if WITH_CYCLE_COUNTER
static int
compare_double(const void *ap, const void *bp)
{
  double a = *(const double *) ap;
  double b = *(const double *) bp;
  if (a < b)
    return -1;
  else if (a > b)
    return 1;
  else
    return 0;
}

#define TIME_CYCLES(t, code) do {				\
  double tc_count[5];						\
  uint32_t tc_start_lo, tc_start_hi, tc_end_lo, tc_end_hi;	\
  unsigned tc_i, tc_j;						\
  for (tc_j = 0; tc_j < 5; tc_j++)				\
    {								\
      tc_i = 0;							\
      GET_CYCLE_COUNTER(tc_start_hi, tc_start_lo);		\
      for (; tc_i < BENCH_ITERATIONS; tc_i++)			\
	{ code; }						\
								\
      GET_CYCLE_COUNTER(tc_end_hi, tc_end_lo);			\
								\
      tc_end_hi -= (tc_start_hi + (tc_start_lo > tc_end_lo));	\
      tc_end_lo -= tc_start_lo;					\
								\
      tc_count[tc_j] = ldexp(tc_end_hi, 32) + tc_end_lo;	\
    }								\
  qsort(tc_count, 5, sizeof(double), compare_double);		\
  (t) = tc_count[2] / BENCH_ITERATIONS;				\
} while (0)

static void
bench_sha1_compress(void)
{
  uint32_t state[_SHA1_DIGEST_LENGTH];
  uint8_t data[SHA1_DATA_SIZE];
  double t;

  TIME_CYCLES (t, _nettle_sha1_compress(state, data));

  printf("sha1_compress: %.2f cycles\n", t);  
}

static void
bench_salsa20_core(void)
{
  uint32_t state[_SALSA20_INPUT_LENGTH];
  double t;

  TIME_CYCLES (t, _nettle_salsa20_core(state, state, 20));
  printf("salsa20_core: %.2f cycles\n", t);  
}

static void
bench_sha3_permute(void)
{
  struct sha3_state state;
  double t;

  TIME_CYCLES (t, sha3_permute (&state));
  printf("sha3_permute: %.2f cycles (%.2f / round)\n", t, t / 24.0);
}
#else
#define bench_sha1_compress()
#define bench_salsa20_core()
#define bench_sha3_permute()
#endif

#if WITH_OPENSSL
# define OPENSSL(x) x,
#else
# define OPENSSL(x)
#endif

int
main(int argc, char **argv)
{
  unsigned i;
  int c;
  const char *alg;

  const struct nettle_hash *hashes[] =
    {
      &nettle_md2, &nettle_md4, &nettle_md5,
      OPENSSL(&nettle_openssl_md5)
      &nettle_sha1, OPENSSL(&nettle_openssl_sha1)
      &nettle_sha224, &nettle_sha256,
      &nettle_sha384, &nettle_sha512,
      &nettle_sha3_224, &nettle_sha3_256,
      &nettle_sha3_384, &nettle_sha3_512,
      &nettle_ripemd160, &nettle_gosthash94,
      NULL
    };

  const struct nettle_cipher *ciphers[] =
    {
      &nettle_aes128, &nettle_aes192, &nettle_aes256,
      OPENSSL(&nettle_openssl_aes128)
      OPENSSL(&nettle_openssl_aes192)
      OPENSSL(&nettle_openssl_aes256)
      &nettle_arcfour128, OPENSSL(&nettle_openssl_arcfour128)
      &nettle_blowfish128, OPENSSL(&nettle_openssl_blowfish128)
      &nettle_camellia128, &nettle_camellia192, &nettle_camellia256,
      &nettle_cast128, OPENSSL(&nettle_openssl_cast128)
      &nettle_des, OPENSSL(&nettle_openssl_des)
      &nettle_des3,
      &nettle_serpent256,
      &nettle_twofish128, &nettle_twofish192, &nettle_twofish256,
      &nettle_salsa20, &nettle_salsa20r12,
      NULL
    };

  enum { OPT_HELP = 300 };
  static const struct option options[] =
    {
      /* Name, args, flag, val */
      { "help", no_argument, NULL, OPT_HELP },
      { "clock-frequency", required_argument, NULL, 'f' },
      { NULL, 0, NULL, 0 }
    };
  
  while ( (c = getopt_long(argc, argv, "f:", options, NULL)) != -1)
    switch (c)
      {
      case 'f':
	frequency = atof(optarg);
	if (frequency > 0.0)
	  break;

      case OPT_HELP:
	printf("Usage: nettle-benchmark [-f clock frequency] [alg]\n");
	return EXIT_SUCCESS;

      case '?':
	return EXIT_FAILURE;

      default:
	abort();
    }

  alg = argv[optind];

  time_init();
  bench_sha1_compress();
  bench_salsa20_core();
  bench_sha3_permute();
  printf("\n");
  time_overhead();

  header();

  if (!alg || strstr ("memxor", alg))
    {
      time_memxor();
      printf("\n");
    }
  
  for (i = 0; hashes[i]; i++)
    if (!alg || strstr(hashes[i]->name, alg))
      time_hash(hashes[i]);

  if (!alg || strstr ("umac", alg))
    time_umac();

  for (i = 0; ciphers[i]; i++)
    if (!alg || strstr(ciphers[i]->name, alg))
      time_cipher(ciphers[i]);

  if (!alg || strstr ("gcm", alg))
    {
      printf("\n");
      time_gcm();
    }

  return 0;
}
