Arithmetic Compression (C++)

A lightweight arithmetic encoding and decoding implementation using fixed integer precision. This project demonstrates the core concepts of arithmetic coding — a form of entropy coding used in data compression — by mapping an entire message to a single number between 0 and 1 and then decoding it back without loss.

Features:
- 32-bit integer arithmetic coding
= Fixed symbol probability model
- Supports arbitrary symbol sets (can be configured in the code)
- Handles EOF markers ($) for precise decoding
- Clean separation of encoding and decoding logic


Includes handling of:
- E1 condition (MSB = 0)
- E2 condition (MSB = 1)
- E3 condition (underflow, MSBs don't match)


Model Definition
Each symbol is assigned a fixed integer frequency, with '$' designated as an end of file symbol.

model = {'a': 2, 'b': 1, '$': 1}


Encoding:
- Maintains a low and high bound representing the current coding interval.
- Narrows the range with each symbol based on cumulative frequencies.

  Outputs bits when:
    Range falls entirely in the lower half (E1)
    Range falls entirely in the upper half (E2)
    Range falls in the middle half (E3 underflow)


Decoding:
- Starts with the encoded bitstream, iteratively refines the interval to determine the original symbols until the EOF marker ($) is reached.


Precision:
- Uses a fixed number of bits (32-bit) for range arithmetic.
