/***************************************

	aLaw decompresser

	Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#include "bralaw.h"
#include "brendian.h"
#include "brfixedpoint.h"

#if !defined(DOXYGEN)
BURGER_CREATE_STATICRTTI_PARENT(Burger::DecompressALaw,Burger::DecompressAudio);
#endif

/*! ************************************

	\var const Burger::StaticRTTI Burger::DecompressALaw::g_StaticRTTI
	\brief The global description of the class

	This record contains the name of this class and a
	reference to the parent

***************************************/

/*! ************************************

	\brief ALaw decompression lookup table

***************************************/

const Int16 Burger::DecompressALaw::g_Table[256] = {
	-5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
	-7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
	-2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
	-3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
	-22016,-20992,-24064,-23040,-17920,-16896,-19968,-18944,
	-30208,-29184,-32256,-31232,-26112,-25088,-28160,-27136,
	-11008,-10496,-12032,-11520, -8960, -8448, -9984, -9472,
	-15104,-14592,-16128,-15616,-13056,-12544,-14080,-13568,
	-344,  -328,  -376,  -360,  -280,  -264,  -312,  -296,
	-472,  -456,  -504,  -488,  -408,  -392,  -440,  -424,
	-88,   -72,  -120,  -104,   -24,    -8,   -56,   -40,
	-216,  -200,  -248,  -232,  -152,  -136,  -184,  -168,
	-1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
	-1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
	-688,  -656,  -752,  -720,  -560,  -528,  -624,  -592,
	-944,  -912, -1008,  -976,  -816,  -784,  -880,  -848,
	5504,  5248,  6016,  5760,  4480,  4224,  4992,  4736,
	7552,  7296,  8064,  7808,  6528,  6272,  7040,  6784,
	2752,  2624,  3008,  2880,  2240,  2112,  2496,  2368,
	3776,  3648,  4032,  3904,  3264,  3136,  3520,  3392,
	22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
	30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
	11008, 10496, 12032, 11520,  8960,  8448,  9984,  9472,
	15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
	344,   328,   376,   360,   280,   264,   312,   296,
	472,   456,   504,   488,   408,   392,   440,   424,
	88,    72,   120,   104,    24,     8,    56,    40,
	216,   200,   248,   232,   152,   136,   184,   168,
	1376,  1312,  1504,  1440,  1120,  1056,  1248,  1184,
	1888,  1824,  2016,  1952,  1632,  1568,  1760,  1696,
	688,   656,   752,   720,   560,   528,   624,   592,
	944,   912,  1008,   976,   816,   784,   880,   848
};


/*! ************************************

	\class Burger::DecompressALaw
	\brief Process audio data compressed with ALaw

	Convert audio data compressed with ALaw to native
	16 bit format

	\sa DecompressULaw, \ref Decompress and \ref DecompressAudio

***************************************/

/*! ************************************

	\brief Default constructor

	Initializes the defaults

***************************************/

Burger::DecompressALaw::DecompressALaw() :
	DecompressAudio(SoundManager::TYPESHORT),
	m_uCacheSize(0),
	m_eState(STATE_INIT)
{
	m_uSignature = Signature;
}

/*! ************************************

	\brief Resets the decompresser to defaults

	\return \ref DECOMPRESS_OKAY if successful

***************************************/

Burger::Decompress::eError Burger::DecompressALaw::Reset(void)
{
	m_uTotalInput = 0;
	m_uTotalOutput = 0;
	// No worries!
	m_uCacheSize = 0;
	m_eState = STATE_INIT;
	return DECOMPRESS_OKAY;
}

/*! ************************************

	\brief Decompress audio data using alaw

	Input data is assumed to be alaw compressed bytes.

	\param pOutput Pointer to the buffer to accept the decompressed data
	\param uOutputChunkLength Number of bytes in the output buffer
	\param pInput Pointer to data to decompress
	\param uInputChunkLength Number of bytes in the data to decompress

	\return Decompress::eError code with zero if no failure, non-zero is an error code

***************************************/

Burger::Decompress::eError Burger::DecompressALaw::Process(void *pOutput,WordPtr uOutputChunkLength,const void *pInput,WordPtr uInputChunkLength)
{
	//
	// Handle data "decompression"
	//

	// Save the pointers to determine consumption
	void *pOldOutput = pOutput;
	const void *pOldInput = pInput;

	//
	// Process based on state
	//

	eState uState = m_eState;
	Word bAbort = FALSE;
	do {
		switch (uState) {

		//
		// Cache has not been used, just copy
		//

		case STATE_INIT:
			{
				// Copy the data while converting the endian
				WordPtr uPacketCount = Min(uInputChunkLength,uOutputChunkLength/2);
				uInputChunkLength -= uPacketCount;
				uOutputChunkLength -= uPacketCount*2;

				WordPtr uLength = uPacketCount;
				if (uLength) {
					// Is it aligned?
					if (!(reinterpret_cast<WordPtr>(pOutput)&1)) {
						do {
							// Convert to native endian quickly
							static_cast<Int16 *>(pOutput)[0] = g_Table[static_cast<const Word8 *>(pInput)[0]];
							pInput = static_cast<const Word8 *>(pInput)+1;
							pOutput = static_cast<Word16 *>(pOutput)+1;
						} while (--uLength);
					} else {

						// You monster.

						do {
							// Convert to endian with unaligned data
							NativeEndian::StoreAny(static_cast<Int16 *>(pOutput),g_Table[static_cast<const Word8 *>(pInput)[0]]);
							pInput = static_cast<const Word8 *>(pInput)+1;
							pOutput = static_cast<Word16 *>(pOutput)+1;
						} while (--uLength);

					}
				}

				//
				// One extra byte?
				//
				if (uInputChunkLength) {
					// Put it in the cache and go into cache mode
					m_iCache = g_Table[static_cast<const Word8 *>(pInput)[0]];
					pInput = static_cast<const Word8 *>(pInput)+1;
					--uInputChunkLength;
					m_uCacheSize = 2;
					uState = STATE_CACHEFULL;
				} else {
					bAbort = TRUE;
				}
			}
			break;

		//
		// Cache is full, output the data
		//

		case STATE_CACHEFULL:
			if (uOutputChunkLength) {

				// Output 1 or 2 bytes

				WordPtr uCacheSize = m_uCacheSize;
				WordPtr uSteps = Min(uOutputChunkLength,static_cast<WordPtr>(uCacheSize));

				// Mark the byte(s) as consumed
				uOutputChunkLength -= uSteps;

				// Start copying where it left off
				const Word8 *pSrc = &reinterpret_cast<const Word8 *>(&m_iCache)[2-uCacheSize];

				// Update the cache size
				uCacheSize = static_cast<Word>(uCacheSize-uSteps);
				if (uCacheSize) {
					// Number of bytes remaining in cache
					m_uCacheSize = static_cast<Word>(uCacheSize);
				} else {
					// Cache will be empty, so switch to normal mode
					uState = STATE_INIT;
				}

				//
				// Copy out the cache data
				//
				do {
					static_cast<Word8 *>(pOutput)[0] = pSrc[0];
					++pSrc;
					pOutput = static_cast<Word8 *>(pOutput)+1;
				} while (--uSteps);
			} else {
				bAbort = TRUE;
			}
			break;

		}
	} while (!bAbort);

	// Save the state
	m_eState = uState;

	// Return the number of bytes actually consumed
	WordPtr uInputConsumed = static_cast<WordPtr>(static_cast<const Word8 *>(pInput)-static_cast<const Word8 *>(pOldInput));
	WordPtr uOutputConsumed = static_cast<WordPtr>(static_cast<const Word8 *>(pOutput)-static_cast<const Word8 *>(pOldOutput));

	// Store the amount of data that was processed

	m_uInputLength = uInputConsumed;
	m_uOutputLength = uOutputConsumed;

	// Add the decompressed data to the totals
	m_uTotalInput += uInputConsumed;
	m_uTotalOutput += uOutputConsumed;

	// Output buffer not big enough?
	if (uOutputChunkLength!=uOutputConsumed) {
		return DECOMPRESS_OUTPUTUNDERRUN;
	}

	// Input data remaining?
	if (uInputChunkLength!=uInputConsumed) {
		return DECOMPRESS_OUTPUTOVERRUN;
	}
	// Decompression is complete
	return DECOMPRESS_OKAY;
}

/*! ************************************

	\brief Allocate and initialize a DecompressALaw

	\return A pointer to a default DecompressALaw class or \ref NULL if out of memory
	\sa Delete(const T *)

***************************************/

Burger::DecompressALaw * BURGER_API Burger::DecompressALaw::New(void)
{
	// Allocate the memory
	return new (Alloc(sizeof(DecompressALaw))) DecompressALaw();
}
