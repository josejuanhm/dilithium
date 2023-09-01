#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "../randombytes.h"
#include "../sign.h"
#include <string.h>

#include "../fpau_switches.h"

#define MLEN 59
#define NTESTS 100

#ifdef UART
#include "../uart.h"
#endif

#ifdef RISCV_PROCESSOR
int main(void)
{
  size_t i;
  int ret;
  size_t mlen, smlen;
  uint8_t m[MLEN + CRYPTO_BYTES];
  uint8_t m2[MLEN + CRYPTO_BYTES];
  uint8_t sm[MLEN + CRYPTO_BYTES];
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];

#ifdef UART
  uart_send_string("\n\rHi Dilithium");
#endif

#ifdef PROFILING_STAGES
  register uint32_t cycle_start  asm("s2");
  register uint32_t cycle_end    asm("s3");
  register uint32_t cycle_start2 asm("s4");
  register uint32_t cycle_end2   asm("s5");
  register uint32_t cycle_start3 asm("s6");
  register uint32_t cycle_end3   asm("s7");

  static float keys_avg   = 0;
  static float sign_avg   = 0;
  static float verify_avg = 0;
#endif

  for(i = 0; i < NTESTS; ++i) {    
#ifdef UART
    uart_send_string("\n\rCreate keys");
#endif
    randombytes(m, MLEN);
    
#ifdef PROFILING_STAGES
    asm("csrrs s2, time, zero");
#endif
    crypto_sign_keypair(pk, sk);
#ifdef PROFILING_STAGES
    asm("csrrs s3, time, zero");
    print_runtime(cycle_start, cycle_end);
    keys_avg = ((cycle_end - cycle_start) + i*keys_avg) / (i+1); // obtaining average per new sample
#endif

#ifdef UART
    uart_send_string("\nSign message");
#endif
#ifdef PROFILING_STAGES
    asm("csrrs s4, time, zero");
#endif
    crypto_sign(sm, &smlen, m, MLEN, sk);
#ifdef PROFILING_STAGES
    asm("csrrs s5, time, zero");
    print_runtime(cycle_start2, cycle_end2);
    sign_avg = ((cycle_end2 - cycle_start2) + i*sign_avg) / (i+1); 
#endif

#ifdef UART
    uart_send_string("\nVerify signature");
#endif
#ifdef PROFILING_STAGES
    asm("csrrs s6, time, zero");
#endif
    ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);
#ifdef PROFILING_STAGES
    asm("csrrs s7, time, zero");
    print_runtime(cycle_start3, cycle_end3);
    verify_avg = ((cycle_end3 - cycle_start3) + i*verify_avg) / (i+1); 
#endif

    if(ret) {
      //fprintf(stderr, "Verification failed\n");
#ifdef UART
      uart_send_string("\n\rVerification failed");
#endif
      return -1;
    }
    //if(smlen != MLEN + CRYPTO_BYTES) {
    //  //fprintf(stderr, "Signed message lengths wrong\n");
    //  uart_send_string("\n\rSigned message lengths wrong");
    //  return -1;
    //}
    //if(mlen != MLEN) {
    //  //fprintf(stderr, "Message lengths wrong\n");
    //  uart_send_string("\n\rMessage lengths wrong");
    //  return -1;
    //}
    //for(j = 0; j < MLEN; ++j) {
    //  if(m2[j] != m[j]) {
    //    //fprintf(stderr, "Messages don't match\n");
    //    uart_send_string("\n\rMessages don't match");
    //    return -1;
    //  }
    //}
    //randombytes((uint8_t *)&j, sizeof(j));
    //do {
    //  randombytes(&b, 1);
    //} while(!b);
    //sm[j % (MLEN + CRYPTO_BYTES)] += b;
    //ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);
    //if(!ret) {
    //  fprintf(stderr, "Trivial forgeries possible\n");
    //  return -1;
    //}
  }

  //printf("CRYPTO_PUBLICKEYBYTES = %d\n", CRYPTO_PUBLICKEYBYTES);
  //printf("CRYPTO_SECRETKEYBYTES = %d\n", CRYPTO_SECRETKEYBYTES);
  //printf("CRYPTO_BYTES = %d\n", CRYPTO_BYTES);

  // print average runtimes
#ifdef PROFILING_STAGES
  itoa(pbuf, (unsigned int)keys_avg, 10);
  uart_send_string("\n\rKeys average: ");
  uart_send_string(str);
  
  itoa(pbuf, (unsigned int)sign_avg, 10);
  uart_send_string("\nSign average: ");
  uart_send_string(str);

  itoa(pbuf, (unsigned int)verify_avg, 10);
  uart_send_string("\nVerify average: ");
  uart_send_string(str);
#endif
  
  // Busy wait
  for(;;);
}

#else

int main(void)
{
  size_t i, j;
  int ret;
  size_t mlen, smlen;
  uint8_t b;
  uint8_t m[MLEN + CRYPTO_BYTES];
  uint8_t m2[MLEN + CRYPTO_BYTES];
  uint8_t sm[MLEN + CRYPTO_BYTES];
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];

  for(i = 0; i < NTESTS; ++i) {
    randombytes(m, MLEN);

    crypto_sign_keypair(pk, sk);
    crypto_sign(sm, &smlen, m, MLEN, sk);
    ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);

    if(ret) {
      fprintf(stderr, "Verification failed\n");
      return -1;
    }
    if(smlen != MLEN + CRYPTO_BYTES) {
      fprintf(stderr, "Signed message lengths wrong\n");
      return -1;
    }
    if(mlen != MLEN) {
      fprintf(stderr, "Message lengths wrong\n");
      return -1;
    }
    for(j = 0; j < MLEN; ++j) {
      if(m2[j] != m[j]) {
        fprintf(stderr, "Messages don't match\n");
        return -1;
      }
    }

    randombytes((uint8_t *)&j, sizeof(j));
    do {
      randombytes(&b, 1);
    } while(!b);
    sm[j % (MLEN + CRYPTO_BYTES)] += b;
    ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);
    if(!ret) {
      fprintf(stderr, "Trivial forgeries possible\n");
      return -1;
    }
  }

  printf("CRYPTO_PUBLICKEYBYTES = %d\n", CRYPTO_PUBLICKEYBYTES);
  printf("CRYPTO_SECRETKEYBYTES = %d\n", CRYPTO_SECRETKEYBYTES);
  printf("CRYPTO_BYTES = %d\n", CRYPTO_BYTES);

  return 0;
}
#endif
