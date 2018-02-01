#ifndef LYRA2DC_H
#define LYRA2DC_H

#ifdef __cplusplus
extern "C" {
#endif

// input : fixed to 80 bytes data length
void lyra2dc_hash(const char* input, char* output);

#ifdef __cplusplus
}
#endif

#endif
