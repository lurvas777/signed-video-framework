/**
 * MIT License
 *
 * Copyright (c) 2021 Axis Communications AB
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next paragraph) shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __SIGNED_VIDEO_INTERFACES_H__
#define __SIGNED_VIDEO_INTERFACES_H__

#include <stdbool.h>  // bool
#include <stdint.h>  // uint8_t
#include <string.h>  // size_t

#include "signed_video_common.h"

typedef struct _signature_info_t signature_info_t;

/**
 * @brief Signing algorithm
 *
 * The following signing algorithms are supported and has to be set when creating the signed video
 * session on the signing side.
 *
 * NOTE: The algorithms are currently fixed to SHA-256, which needs to be addressed when
 * implementing the interfaces.
 */
typedef enum { SIGN_ALGO_RSA = 0, SIGN_ALGO_ECDSA = 1, SIGN_ALGO_NUM } sign_algo_t;

/**
 * Struct for storing necessary information to generate a signature
 *
 * It is used by the signing plugins and also to validated the authenticity.
 */
struct _signature_info_t {
  uint8_t *hash;  // The hash to be signed, or to verify the signature.
  size_t hash_size;  // The size of the |hash|. For now with a fixed size of HASH_DIGEST_SIZE.
  sign_algo_t algo;  // The algorithm used to sign the |hash|.
  void *private_key;  // The private key used for signing in a pem file format.
  size_t private_key_size;  // The size of the |private_key|.
  void *public_key;  // The public key used for validation in a pem file format.
  size_t public_key_size;  // The size of the |public_key|.
  uint8_t *signature;  // The signature of the |hash|.
  size_t signature_size;  // The size of the |signature|.
  size_t max_signature_size;  // The allocated size of the |signature|.
};

/**
 * Cryptography library calling interface APIs are declared here.
 */

/**
 * @brief Signs a hash with a private key
 *
 * This function should sign the |hash|, using the |private_key|, and store the result in
 * |signature|. See new_signature_info_t for information on members.
 *
 * @param signature_info A pointer to the object holding all information needed for signing.
 *
 * @returns Should return SV_OK upon success and an adequate value upon failure.
 */
SignedVideoReturnCode
sv_interface_sign_hash(signature_info_t *signature_info);

/**
 * @brief Gets the signature
 *
 * This function should write the |signature| to output if a new signature is available, and return
 * True if the output has been updated.
 *
 * @param signature The memory slot to which the signature is copied if present. It is assumed that
 *   enough memory has been allocated by the user.
 *
 * @returns True if signing is completed, else False
 */
bool
sv_interface_get_signature(uint8_t *signature);

/**
 * @brief Sets up the signing plugin
 *
 * This function is called when the Signed Video session is created. For example, useful to initiate
 * member variables of the plugin.
 *
 * @returns Should return SV_OK upon success and an adequate value upon failure.
 */
SignedVideoReturnCode
sv_interface_setup();

/**
 * @brief Tears down the signing plugin
 *
 * This function is called when the Signed Video session is terminated.
 */
void
sv_interface_teardown();

/**
 * @brief Data allocation
 *
 * This function is called to allocate memory for signatures.
 *
 * @param data_size Maximum size of a signature.
 *
 * @returns Pointer to the allocated memory
 */
uint8_t *
sv_interface_malloc(size_t data_size);

/**
 * @brief Free data
 *
 * Free the allocated data memory.
 *
 * @param data Pointer to the data.
 */
void
sv_interface_free(uint8_t *data);

#endif  // __SIGNED_VIDEO_INTERFACES_H__