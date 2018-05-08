#include "bigint.h"
#include <stdio.h>

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
	for (int i = 0;  i < DEFAULT_LIMBS; i++ ) { 
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

	int bitar = a->limbs*WORDSIZE;
	char *s;
	s = malloc(sizeof(*s) * ((bitar + BLOCKSIZE - 1) / BLOCKSIZE + 2));

	if (a->sign < 0) {
		printf("-");
	}
	if (a->value[0] == 0 && a->limbs == 1) {
		printf("0");
	}
	else {
		int limb_index  = a->limbs - 1; //börja med mest signifikanta
		int num_blocks = bi_blocks(a);

		int n = 0;
		int j = num_blocks;
		int i = 1;
		while (n < limb_index) {
			for ( i = 1; i <= num_blocks - BLOCKS  ; i++) {
				int blocket = bi_getblk(a, i);
				// printf(" blocket %d ", blocket);
				char hexdecimal = int_to_hex(blocket);
				s[j--] = hexdecimal;
			}
			n++;
		}

		int last_index  = num_blocks;
		while( bi_getblk(a, last_index) == 0) {
			last_index--;
		}

		for (i  ; i <= last_index; i++ ) {
			int blocket = bi_getblk(a, i);
			// printf(" blocket %d ", blocket);
			char hexdecimal = int_to_hex(blocket);
			s[j--] = hexdecimal;
		}



		for (int i = j+1; i <= num_blocks; i++ ) {
		printf("%c", s[i]);
			
		}

	}
}


/**
 * Sets bi equal to the integer represented by the hexadecimal string
 * given as input.
 */
void bi_import(bi_t res, char *s, int num_characters) {
	int antal_chars = num_characters; //bytes * 8 bitar/bytes  /
	int last_index = antal_chars-1;
	int antal_limbs = antal_chars / BLOCKS;
	antal_limbs += 1;
	if (antal_chars % BLOCKS == 0) {
		antal_limbs-- ;
	}

	if (antal_chars == 0) {
		antal_limbs = 1;
	}

	bi_resize(res, antal_limbs );
	// printf("%c\n antal_chars: %d ", s[0], antal_chars);
	char noll = '0';
	if (s[0] == noll && antal_chars == 0) {
		 // printf("antal limbs: %d \n", res->limbs);
		res->sign = 0;
		res->value[0] = 0;

	} 
	else {

		if (s[0] == '-') {
			res->sign = -1;
			// last_index--;
		} else {
			res->sign = 1;
		}

		int j = 1;
		for (int i = last_index ; i >= 0; i--){
		 // printf("SUPRISE MUFACKA");
			int decimal_tal = hex_to_int(s[i]);
			// printf("Decimal talet: %d\n", decimal_tal);
			bi_setblk(res, j++, decimal_tal);
		}
	}
	// printf("limb1: %d limb2: %d \n", res->value[1], res->value[0]);
	bi_normalize(res);

}

/**
 * Writes a signed hexadecimal representation of the value of a to
 * res. It is assumed that res has sufficient space. The number of
 * characters written is returned.
 */
int bi_export(char *res, bi_t a) {
	int bitar = a->limbs*WORDSIZE;
	int is = 0;
	char *s = malloc(sizeof(*s) * ((bitar + BLOCKSIZE - 1) / BLOCKSIZE + 2));
	int num_blocks = bi_blocks(a);

	if (a->sign < 0) {
		char minus = '-';
		res[is++] = minus;

	}
	if (a->value[0] == 0 && a->limbs == 1) {
		char noll = '0';
		res[is] = noll;
	}
	else {
		int limb_index  = a->limbs - 1; //börja med mest signifikanta

		int n = 0;
		int j = num_blocks;
		int i = 1;
		while (n < limb_index) {
			for ( i = 1; i <= num_blocks - BLOCKS  ; i++) {
				int blocket = bi_getblk(a, i);
				// printf(" blocket %d ", blocket);
				char hexdecimal = int_to_hex(blocket);
				s[j--] = hexdecimal;
			}
			n++;
		}

		int last_index  = num_blocks;
		while( bi_getblk(a, last_index) == 0) {
			last_index--;
		}

		for (i  ; i <= last_index; i++ ) {
			int blocket = bi_getblk(a, i);
			// printf(" blocket %d ", blocket);
			char hexdecimal = int_to_hex(blocket);
			s[j--] = hexdecimal;
		}



		for (int i = j+1; i <= num_blocks; i++ ) {
			res[is++] = s[i];		
		}

	}




	// //ett block en hexadecimal siffra
	// int num_blocks = bi_blocks(a);

	// for (int i = num_blocks; i >= 0; i-- ) {
	// 	int block = bi_getblk (a, i);
	// 	char hex = int_to_hex(block);
	// 	res[i] = hex;
	// }
  return is;
}

/**
 * Sets res = a.
 */
void bi_set(bi_t res, bi_t a) {
	*res = *a;
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
	num_limbs = (bits % WORDSIZE == 0) ? num_limbs -1 : num_limbs; //undvik skapa en extra limb 

//frigör utrymme, fyller också alla limbs med 0
	bi_resize(res, num_limbs);

//Fyll varje Limb med ett word tills vi använt upp alla bits
	int limb_index = res->limbs -1;
	int osatta_bitar = bits;

	//Börja med de minst signifikanta orden och fyll de
	for (int i = 0; i < limb_index ; i++) {
		res->value[i] = (int) bi_randword();
		osatta_bitar	-= WORDSIZE	; //räkna bort de bitar vi satt
	}

	//Specialfall för mest signifikanta limben
	int sig_limb = (int)bi_randword(); 
	int mask = WORDMASK	;
	mask = mask >> (WORDSIZE - osatta_bitar); 	//gör en mask för att se till att vi har rätt antal bitar i mest signifikanta ordet
	int maskad = sig_limb & mask;
	// printf("sig_limb: %d  masken: %d maskad: %d ",sig_limb,  mask, maskad);
	res->value[limb_index] = mask & sig_limb  ; 
	bi_normalize(res); 


	//Tecknet
	if (res->limbs == 0 && res->value[0] == 0) {
		res->sign = 0;
	}
	else {
		res->sign = 1;
	}



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
	int sig_limb_index = a->limbs -1;
	int sig_limb = a->value[sig_limb_index]; 
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
	
	// int limb_index = a->limbs - 1;
	int limb_index = 0 ; 
	//Vilken limb finns biten i:
	limb_index =  (i / WORDSIZE);
	limb_index = (i % WORDSIZE == 0) ? (limb_index--) : limb_index ; //justera så att ett ord innehåller bitar 1-24. 
	
	//Skifta masken korrekt antal steg:
	bit_mask = bit_mask << (i - WORDSIZE*limb_index);
	
	int biten = a->value[limb_index] & bit_mask; //Hämta biten
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
	if (a->sign > b->sign){ //a positiv, b antingen = 0 eller negativ
		return 1;
	} 
	else if (a->sign < b->sign) {
		return -1;
	}
	else { // a->sign == b->sign
		if (bi_ucmp(a,b) == 0) { //a == b 
			return 0;
		}
		else if (a->sign == -1) {
			if (bi_ucmp(a,b) == 1) { // a > b (unsigned) , men tecken negativt 
				return -1; 
			}
			else if (bi_ucmp (a,b) == -1) { // a < b, negativt tecken
				return 1;
			}
		}
		else if (a->sign == 0) { //a = b = 0
			return 0;
			}
		else if (a->sign == 1){
			if (bi_ucmp(a,b) == 1) { //a > b 
				return 1;
			} 
			else if (bi_ucmp(a,b) == -1) {
				return -1;
			}
		}
	}
}


/**
 * Sets res = -a.
 */
void bi_neg(bi_t res, bi_t a) {
	res->value = a->value;
	res->limbs = a->limbs;
	res->sign = a->sign * -1;
	
}

/**
 * Sets res = a + b.
 */
void bi_add(bi_t res, bi_t a, bi_t b) {

	if (a->sign == 0) { //a är = 0
		bi_set(res, b) ;
	} else if (b->sign == 0) { 
		bi_set(res, a);
	} else {

		if (a->sign == b->sign) {
			bi_resize(res, MAX(a->limbs, b->limbs)+1);
			bi_uadd (res, a, b);
			res->sign = a->sign;
		} else {
			bi_resize (res, MAX(a->limbs, b->limbs));
			bi_uabsdiff(res, a , b);
			if (a->sign < 1) {
				res->sign *= -1;
			}

		
		}

	}	

}

/**
 * Sets res = a - b.
 */
void bi_sub(bi_t res, bi_t a, bi_t b) {

	if (a->sign == b->sign ) {
		if (bi_ucmp(a, b) == 1) {
			bi_usub(res, a,b);
		} 
		else if (bi_ucmp(a,b) == 0)
			res->value = 0;
		else if (bi_ucmp(a,b) == -1)
		{
			bi_usub(res, b,a);
		}
	}
}

/**
 * Sets res = a * b.
 */
void bi_mul(bi_t res, bi_t a, bi_t b) {

	if (a->limbs == b->limbs) {

	}

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
