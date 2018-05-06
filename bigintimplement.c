#include "bigint.h"

/*********************************************************************
 * The following functions must be implemented to solve the          *
 * problems in the laboration.                                       *
 *********************************************************************/


/**
 * Initializes this input.
 */
void bi_init(bi_t bi) {
	bi->sign = 0;
	bi->value = malloc (sizeof(*(bi->value) * DEFAULT_LIMBS)); //@LAB3
	bi->limbs = DEFAULT_LIMBS;
	for (int i = 0;  i < DEFAULT_LIMBS; i++ ) { // Hur hänger detta ihop?
		bi->value[i] = 0;
	}

}

/**
 * Deallocates the memory allocated in the initialized input.
 */
void bi_clear(bi_t bi) {
	free (bi->value); //@LAB3
}

/**
 * Prints a to stdout as a hexadecimal integer.
 */
void bi_printf(bi_t a) {
}

/**
 * Sets bi equal to the integer represented by the hexadecimal string
 * given as input.
 */
void bi_import(bi_t res, char *s) {
}

/**
 * Writes a signed hexadecimal representation of the value of a to
 * res. It is assumed that res has sufficient space. The number of
 * characters written is returned.
 */
int bi_export(char *res, bi_t a) {
  return 0;
}

/**
 * Sets res = a.
 */
void bi_set(bi_t res, bi_t a) {
}

/**
 * Sets res = u, where u is an unsigned WORDSIZE-bit integer.
 */
void bi_setui(bi_t res, unsigned int u) {
}

/**
 * Sets res equal to a random non-negative integer with the given
 * nominal number of bits. Keep in mind that each limb must contain at
 * most WORDSIZE non-zero bits and they should be the least
 * significant bits.
 *
 * The most significant word requires special attention, since some of
 * its bits may have to be set to zero if bits is not divisible by
 * WORDSIZE.
 *
 * We stress that some (or even all!) leading bits in the most
 * significant word may be zero, since they are randomly chosen.
 *
 * We are using this random generator for testing purposes, so we do
 * not need to use any cryptographically strong source such as
 * /dev/urandom. It is perfectly fine to use rand() from stdlib.h, but
 * keep in mind that it outputs a number in a small interval.
 */
void bi_rand(bi_t res, int bits) {
// 1 Word 24 bit

//räkna ut hur många limbs vi behöver
	int num_limbs  = bits/WORDSIZE + 1;

//frigör utrymme, fyller också alla limbs med 0
	bi_resize(res, num_limbs);

//Fyll varje Limb med ett word tills vi använt upp alla bits
	int limb_index = res->limbs -1;
	int osatta_bitar = bits;
	//Börja med de minst signifikanta orden och fyll de
	for (int i = limb_index; i > 0 ; i--) {
		res->value[i] = bi_randword();
		osatta_bitar	-= WORDSIZE	; //räkna bort de bitar vi satt
	}

	//Specialfall för mest signifikanta limben
	int sig_limb = bi_randword(); 
	int mask = WORDMASK	;
	mask = mask >> (WORDSIZE - osatta_bitar); 	//gör en mask för att se till att vi har rätt antal bitar i mest signifikanta ordet
	res->value[0] = sig_limb & mask; 

}

/**
 * Returns the number of bits needed to represent a->value as an
 * unsigned integer.
 */
int bi_bitsize(bi_t a) {
	int num_bits ;
//Räkna limbs 
//Specialfall för mest signifikanta limben
	if (a->limbs > 1) {
		num_bits = (a->limbs - 1)*WORDSIZE; //Wordsize-bits * fulla limbs
	}

		//hantera mest signifikanta limben
	int sig_limb = a->value[0]; 
	int mask = TOPMASK; //Räkna bitar, börja med mest signifikanta och gå bakåt
	num_bits += WORDSIZE; //Utgå från att alla bitar i mest signifikanta ordet används 

	int i = WORDSIZE; //Redundans, se till att vi inte kör loopen i evighet	
	while (!(mask && sig_limb) || i == 0 ) {
		num_bits-- ; //Räkna bort en bit om biten = 0
		mask = mask >> 1; //Flytta biten i masken 
	}

  return num_bits;
}

/**
 * Returns the ith bit as an integer.
 */
//konvention: i = 1 är den minst signifikanta biten
//räknar bara bitar i word, ignorerar "nails"
int bi_tstbit(bi_t a, int i) {
	int num_bits = bi_bitsize(a); // Totala antalet bitar i a
	int bit_mask = 1; // 0001
	
	int limb_index = a->limbs - 1;
	//Vilken limb finns biten i:
	limb_index = limb_index - (i / WORDSIZE); 
	limb_index = (i % WORDSIZE == 0) ? (limb_index--) : limb_index ; //justera så att ett ord innehåller bitar 1-24. 
	//Skifta masken korrekt antal steg:
	bit_mask = bit_mask << (i - WORDSIZE*((a->limbs-1)-limb_index));
	//Hämta biten
	int biten = a->value[limb_index] & bit_mask;
	if (biten) {
		return 1;
	}	else {
		return 0;
	}

}

/**
 * Returns -1, 0, or 1 depending on if a is smaller, equal, or greater
 * than b as signed integers.
 */
int bi_cmp(bi_t a, bi_t b) {
  return 0;
}

/**
 * Sets res = -a.
 */
void bi_neg(bi_t res, bi_t a) {
}

/**
 * Sets res = a + b.
 */
void bi_add(bi_t res, bi_t a, bi_t b) {
}

/**
 * Sets res = a - b.
 */
void bi_sub(bi_t res, bi_t a, bi_t b) {
}

/**
 * Sets res = a * b.
 */
void bi_mul(bi_t res, bi_t a, bi_t b) {
}

/**
 * Sets res = a ^ b. If one input is longer then the other is
 * considered to contain the needed number of leading zero to do the
 * operation.
 */
void bi_xor(bi_t res, bi_t a, bi_t b) {

}

/**
 * Sets res = a & b. If one input is longer then the other is
 * considered to contain the needed number of leading zero to do the
 * operation.
 */
void bi_and(bi_t res, bi_t a, bi_t b) {
}

/**
 * Sets res = a | b. If one input is longer then the other is
 * considered to contain the needed number of leading zero to do the
 * operation.
 */
void bi_or(bi_t res, bi_t a, bi_t b) {
}
