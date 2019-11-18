/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* modified to be reentrant by Fazal Majid for CQF */

#include <stdint.h>

/* This is xoroshiro128** 1.0, one of our all-purpose, rock-solid,
   small-state generators. It is extremely (sub-ns) fast and it passes all
   tests we are aware of, but its state space is large enough only for
   mild parallelism.

   For generating just floating-point numbers, xoroshiro128+ is even
   faster (but it has a very mild bias, see notes in the comments).

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */


static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

typedef uint64_t xoroshiro_seed[2];

static uint64_t next(xoroshiro_seed s) {
	const uint64_t s0 = s[0];
	uint64_t s1 = s[1];
	const uint64_t result = rotl(s0 * 5, 7) * 9;

	s1 ^= s0;
	s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
	s[1] = rotl(s1, 37); // c

	return result;
}

static void init_seed(uint64_t seed, xoroshiro_seed s) {
        /* XXX teh recommended seeding is to use a splitmix64 PRNG */
        s[0] = seed || ((~seed) << 32);
        s[1] = (~seed) || ((seed) << 32);
}
