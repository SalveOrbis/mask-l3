#include "bigint.h"

/*********************************************************************
 * The following can be implemented as subroutines to help implement *
 * the above. This gives a naive implementation.                     *
 *********************************************************************/

/**
 * Reduces the number of limbs in bi such that the leading limb of
 * bi->value is non-zero, except if bi == 0 in which case we set
 * bi->limbs = 1 to be able to store zero.
 */
void bi_normalize(bi_t bi) {

  int length;
  int i;

  i = bi->limbs -1; //Börja med den mest sign. delen
  while (i > 0 && bi->value[i] == 0) {
    i--;
  }
  length = i + 1 ; //vi har hittat där vi har en nollskild lim


  if (length < bi->limbs) {
    //Kopiera till ny med rätt storlek
    int *copy = malloc(length * LIMBBYTES); // Gör plats för kopian 
    copy = bi->value; //kopiera

    bi_resize (bi, length ); 
    bi->value = copy; 

    free (copy); //frigör minnet från kopian.

    //släng bort gamla
  }


}

/**
 * Set all limbs in res->value starting at index i to zero without
 * changing res->limbs.
 */
void bi_setzero(bi_t res, int i) {

  for (int j = i; j < res->limbs; j++){
    res->value[j]  = 0;
  }

}

/**
 * Generates a pseudo-random word.
 */
int bi_randword() {
  unsigned int rand_word = arc4random();
  rand_word = rand_word & WORDMASK ; //Säkerhetsåtgärd för att se till att alla bitar i en "nail" är = 0 
  return rand_word;
}

/**
 * Resizes bi->value to have limbs number of limbs and sets a->value
 * to the all-zero array and the sign to zero.
 */
void bi_resize(bi_t bi, int limbs) {
  
  free(bi->value); //frigör gamla utrymmet

  bi->value = malloc( (limbs*LIMBBYTES)); //omallokerar minnet för ny storlek;
  bi->sign = 0;
  bi->limbs = limbs;

    //nollställer values
  for (int i = 0; i < limbs ; i++){
    bi->value[i] = 0;
  }

}

/**
 * Applies op limb-wise to limbs of a and b and stores the result in
 * res. Here op is a function pointer, so any function with the given
 * signature can be used. (This is convenient for this problem, but
 * far too slow in a real implementation.)
 */
void bi_map(bi_t res, bi_t a, bi_t b, int (*op)(int, int)) {
}

/**
 * xor as a function.
 */
int bi_ixor(int x, int y) {
  return x ^ y;
}

/**
 * and as a function.
 */
int bi_iand(int x, int y) {
  return x & y;
}

/**
 * or as a function.
 */
int bi_ior(int x, int y) {
  return x | y;
}

/**
 * Returns the integer value of the input hexadecimal character.
 */
int hex_to_int(char c) {
   int val;
    if (c == '1') {
      val = 1; 
    } else if (c == '2') {
      val = 2;
    } else if (c == '3') {
      val = 3;
    } else if (c == '4') {
      val = 4;
    } else if (c == '5') {
      val = 5;
    } else if (c == '6') {
      val = 6;
    } else if (c == '7') {
      val = 7;
    } else if (c == '8') {
      val = 8;
    } else if (c == '9') {
      val = 9;
    } else if (c == 'a' || c == 'A') {
      val = 10;
    } else if (c == 'b' || c == 'B') {
      val = 11;
    } else if (c == 'c' || c == 'C') {
      val = 12;
    } else if (c == 'd' || c == 'D') {
      val = 13;
    } else if (c == 'e' || c == 'E') {
      val = 14;
    } else if (c == 'f' || c == 'F') {
      val = 15;
    } else {
      val = 0;
    }
    
  return val;
}

/**
 * Returns the hexadecimal character corresponding to the integer
 * input.
 */
char int_to_hex(int h) {
   char  hex;
    if (h == 1) {
      hex = '1'; 
    } else if (h == 2) {
      hex = '2';
    } else if (h == 3) {
      hex = '3';
    } else if (h == 4) {
      hex = '4';
    } else if (h == 5) {
      hex = '5';
    } else if (h == 6) {
      hex = '6';
    } else if (h == 7) {
      hex = '7';
    } else if (h ==8) {
      hex = '8';
    } else if (h == 9) {
      hex = '9';
    } else if (h == 10) {
      hex = 'a';
    } else if (h == 11) {
      hex = 'b';
    } else if (h == 12) {
      hex = 'c';
    } else if (h == 13) {
      hex = 'd';
    } else if (h == 14) {
      hex = 'e';
    } else if (h == 15) {
      hex = 'f';
    } else {
      hex = '0';
    }
    
  return hex;
}

/**
 * Returns the ith block of a->value.
  block [0] är det mest signifikanta blocket.
 */
int bi_getblk(bi_t a, int i) {
  // //Räkna ut vilken limb blocket finns i:

  // int limb;
  // limb = i / BLOCKS ;//a->limbs[0] mest signifikanta limben

  // if (limb >= a->limbs ) {
  //   printf("Block index exceeds the number of blocks in a");
  //   return 0;
  // }

  // //Hämta blocket
  // //Ställ in masken på rätt position:
  // int block_mask ;
  // if limb == 0 {
  //     block_mask = (BLOCKMASK << (i*BLOCKSIZE)); //shift block i steps
  //  } 
  // else {
  //     i - limb*BLOCKS 
  //  }
  // int block = bi->value[limb]

  // return 0;
}

/**
 * Sets the ith block of a->value.
 */
int bi_setblk(bi_t a, int i, int blk) {
  return 0;
}

/**
 * Returns the number of blocks in a->value.
 */
int bi_blocks(bi_t a) {
  int num_blocks = a->limbs * BLOCKS; //Ignorerar "nails" 

  return num_blocks ;
}

/**
 * Returns -1, 0, or 1 depending on if a->value is smaller, equal, or
 * greater than b->value as unsigned integers.
 */
int bi_ucmp(bi_t a, bi_t b) {
  return 0;
}

/**
 * Sets res->value = a->value + b->value without setting res->sign or
 * changing res->limbs.
 *
 * ASSUMES: res->limbs >= max(a->limbs, b->limbs). If equality holds,
 * then the overflow is discarded.
 */

//Om vi har en siffra i nails, vi har fått carry-over. Returnera 1, annars returnera 0
int get_carry (int limb) {
  if ((limb / SIGFIGURES*10) != 0) {
    return 1
  } else {
    return 0;
  }
}

void bi_uadd(bi_t res, bi_t a, bi_t b) {
  final_limb_index = res->limbs-1 ; //Hämta index för de mest signifikanta limben i resultatet, används bara om vi får en carry-over

  int limb_index = 0;
  int add_limit = MIN(a->limbs, b->limbs); //Vi kommer bara räkna med minst antal words 
  int carry = 0;

  while (limb_index < add_limit) {
    res->value[limb_index] = a->value[limb_index] + b->value[limb_index] + carry;
    //kolla om vi får carry-over
    carry = get_carry(res->value[limb_index]);
    // if ((res->value[limb_index] / (SIGFIGURES*10) ) != 0) {
    //   carry = 1;
    // }
    res->value = res->value & WORDMASK; //Ta bort carry-over från limben
    limb_index++;
  }

  if (a->limbs == MAX(a->limbs, b->limbs)) { // a hade mer limbs än bt
    while ( limb_index < a->limbs) {
      res->value[limb_index] = a->value[limb_index] + carry;
      if ((res->value[limb_index] / (SIGFIGURES*10) ) != 0) {
         carry = 1;
      } else {
        carry = 0;
      }

      limb_index++;
    } else if (b->limbs == MAX(a->limbs, b->limbs)){ //b hade mer limbs än a 
        while ( limb_index < b->limbs ) {
        res->value[limb_index] = b->value[limb_index] + carry;
        if ((res->value[limb_index] / (SIGFIGURES*10) ) != 0) {
        carry = 1;
      } else {
        carry = 0;
      }
        limb_index++;
    } 

    //vi har en slutlig carry-over. 
    if (carry == 1) {
      res->value[final_limb_index] = carry; //Stoppa in en 1:a i sista platsen
      carry = 0;
    }

  }

}

/**
 * Sets res->value = - a->value in two's complement for integers with
 * res->limbs * WORDSIZE bits.
 */
void bi_pneg(bi_t res, bi_t a) {
}

/**
 * Sets res->value = a->value - b->value. This function assumes that
 * a->value > b->value.
 */
void bi_usub(bi_t res, bi_t a, bi_t b) {
}

/**
 * Sets res->value = |a->value - b->value| and sets res->sign equal to
 * 1, 0, or -1 depending on if a->value is greater, equal, or smaller
 * than b->value.
 */
void bi_uabsdiff(bi_t res, bi_t a, bi_t b) {
}

/**
 * Sets res->value = a->value + b->value * scalar * 2^(shift * WORDSIZE)
 * mod 2^((a->limbs + shift) * WORDSIZE).
 */
void bi_umuladd(bi_t res, bi_t a, int scalar, int shift) {
}
