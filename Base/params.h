// #define PROTECTIONENABLED
/*************************************
NOTE - even if protectionenabled is defined we still
need to set the USEPROTECTION flag in critter.
this is to allow me to compare runs of enabling protection
and not enabling it.
Finally I will also need to check that the change in codon size and masks
do not upset the system.
******************************************/

#ifdef PROTECTIONENABLED
// A few lines of explanation is probably relevant
// I am going to try out allowing parts of genome
// to be "protected" this requires two additional
// bits of info (literally bits) in our codons
// just now we have 8-bit codon, 8-bit grammar LHS, 16-bit Expression length in each
// 32-bit long.
// if we enable protection each codon is really only 7-bits long the eigth bit is the protection
// bit, so the "value" needs to be masked with 0x7f not 0xff.
// the shilft is still 8 bits though (CODON = 8).
// Finally we will use the last bit (bit 16 of the expression length) as a temporary marker when
// we are trying to combine critters. so the expr mask needs to change. (and we need to make sure we apply it!)




#define CODON  8 // codon size in bits
#define CODONMASK 0x7f
#define FCODONMASK 0xff
#define PROTECTEDBITMASK 0x80

#define LHSINFO 8 // grammar LHS size in bits
#define LHSMASK 0xff

#define EXPRLENGTH 16 // expression length size in bits
#define EXPRMASK 0x7fff
#define EXPRPROTMASK 0x7fffffff

#define INPROTECTIONMASK 0x80000000 

#else

#define CODON  12 // codon size in bits
#define CODONMASK 0xfff
#define FCODONMASK 0xfff

#define LHSINFO 6 // grammar LHS size in bits
#define LHSMASK 0x3f


#define EXPRLENGTH 14 // expression length size in bits
#define EXPRMASK 0x3fff
#define CLEAREXPRMASK 0x3ffff
#endif


#define LENGTH_OF_EXPRESSION 10000 // max. no. of elements in mapped expr
#define MAX_LENGTH_OF_EXPRESSION 10000 // max. no. of elements in mapped expr
#define NUMBITS 30000 // the initial dna length when a random critter is generated.
// #define VERBOSE

