#include "testutils.h"

static void
ref_redc (mp_limb_t *rp, const mp_limb_t *ap, const mp_limb_t *mp, mp_size_t mn)
{
  mpz_t t;
  mpz_t m, a;
  mp_size_t an;

  mpz_init (t);
  mpz_setbit (t, mn * GMP_NUMB_BITS);

  mpz_roinit_n (m, mp, mn);

  an = 2*mn;
  while (an > 0 && ap[an-1] == 0)
    an--;

  mpz_roinit_n (a, ap, an);
  
  mpz_invert (t, t, m);
  mpz_mul (t, t, a);
  mpz_mod (t, t, m);

  mpz_limbs_copy (rp, t, mn);

  mpz_clear (t);
}

#define MAX_ECC_SIZE (1 + 521 / GMP_NUMB_BITS)
#define MAX_SIZE (2*MAX_ECC_SIZE)
#define COUNT 50000

void
test_main (void)
{
  gmp_randstate_t state;
  mp_limb_t a[MAX_SIZE];
  mp_limb_t m[MAX_SIZE];
  mp_limb_t ref[MAX_SIZE];
  unsigned i;
  mpz_t r;

  gmp_randinit_default (state);
  
  mpz_init (r);
  
  for (i = 0; ecc_curves[i]; i++)
    {
      const struct ecc_curve *ecc = ecc_curves[i];
      unsigned j;
      if (!ecc->redc)
	continue;

      for (j = 0; j < COUNT; j++)
	{
	  if (j & 1)
	    mpz_rrandomb (r, state, 2*ecc->size * GMP_NUMB_BITS);
	  else
	    mpz_urandomb (r, state, 2*ecc->size * GMP_NUMB_BITS);

	  mpz_limbs_copy (a, r, 2*ecc->size);

	  ref_redc (ref, a, ecc->p, ecc->size);

	  mpn_copyi (m, a, 2*ecc->size);
	  ecc->redc (ecc, m);
	  if (mpn_cmp (m, ecc->p, ecc->size) >= 0)
	    mpn_sub_n (m, m, ecc->p, ecc->size);

	  if (mpn_cmp (m, ref, ecc->size))
	    {
	      fprintf (stderr, "ecc->redc failed: bit_size = %u\n",
		       ecc->bit_size);
	      gmp_fprintf (stderr, "a   = %Nx\n", a, 2*ecc->size);
	      gmp_fprintf (stderr, "m   = %Nx (bad)\n", m, ecc->size);
	      gmp_fprintf (stderr, "ref = %Nx\n", ref, ecc->size);
	      abort ();
	    }

	  mpn_copyi (m, a, 2*ecc->size);
	  ecc_generic_redc (ecc, m);
	  if (mpn_cmp (m, ecc->p, ecc->size) >= 0)
	    mpn_sub_n (m, m, ecc->p, ecc->size);

	  if (mpn_cmp (m, ref, ecc->size))
	    {
	      fprintf (stderr, "ecc_generic_redc failed: bit_size = %u\n",
		       ecc->bit_size);
	      gmp_fprintf (stderr, "a   = %Nx\n", a, 2*ecc->size);
	      gmp_fprintf (stderr, "m   = %Nx (bad)\n", m, ecc->size);
	      gmp_fprintf (stderr, "ref = %Nx\n", ref, ecc->size);
	      abort ();
	    }
	}
    }

  mpz_clear (r);
  gmp_randclear (state);
}
