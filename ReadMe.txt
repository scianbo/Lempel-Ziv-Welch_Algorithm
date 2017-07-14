/*Author Name: Advaith Auron Suresh*/
  
This program performs Lempel-Ziv-Welch (LWZ) algorithm. The program has been designed in such a way that the bit-length varies between 9 bits to 16 bits. The maximum table size would be 2^(bit-length). The first 256 codes (when using 8-bit characters) are by default assigned to the standard character set. The remaining codes are assigned to strings as the algorithm proceeds.

Program Design:

Compression:
After initializing the table, the algorithm keeps track of two things: a string current_string, representing the current sequence; and a character symbol, the new character in the input file. If a sequence (current_string + symbol) exists in the translation table, the algorithm keeps adding bytes to the sequence by setting current_string to (current_string + symbol) and reading the next byte of input, symbol. When a sequence (current_string + symbol) does not appear in the translation table, the algorithm knows that string current_string is in the table, so it appends the code for current_string to the output file, it adds the new sequence (current_string + symbol) to the table, and it resets the current sequence (current_string = symbol).

Decompression:
The decompression algorithm does not require the translation table that was given by the compressor; instead, it rebuilds it as it processes the compressed string. The compressed code which is stored in a binary file is then retrieved 16 bits at a time and the original message is rebuilt on 16 bits at a time. The code obtained from the encoded file would be in binary format. If the code is not defined in the table, a new string is obtained by concatenating the current_string and its substring and then output. The string and the new string substring is then added to the translation table. If the code is not defined in the table, the code is directly appended to the new string and then output it before adding it to the table. The new string is then made to be the current_string.

Data Structure:
The primary data structure used for LZW algorithm in this program is an unordered_map. Unordered_map is faster than map for single element access and since sorted traversal is not required, unordered_map is used. The search time of an unordered_map is O(1) if there are no hash collisions and up to O(n) if there are hash collisions. The insertion and deletion operations are same as search. 

Files:
The program is contained in a single C++ file called lzw.cpp which mainly contains two functions, compress and decompress which are executed based on the command line arguments given by the user.  

Program Content Summary:
The program efficiently performs LZW compression and de-compression but could be subjected to a few drawbacks. 
1. Since the encoded file stores input as 16 bits each, the size of the encoded file could sometimes be larger than the original file size.
2. If an input file consists of each character represented exactly once, then no compression would take place (in fact, the file would enlarge). LZW only compresses the input when it encounters repeated sequences. 
3. The program works only with the standard ASCII set as input.

Programming Language Used:
C++

Compiler Version:
GCC version: 5.4.0

Information for running the file:
1. The command line arguments for compression: encoder filename.txt bit_length (ex: encoder input.txt 14)
2. The command line arguments for de-compression: decoder filename.lzw bit_length (ex: decoder input.lzw 14)
3. Compressed file can be found on the same directory with the same filename and .lzw extension
4. De-compressed file can be found on the same directory with the same filename and _decoded.txt extension

The input file should be placed in the same directory as the .cpp file. The encoded and decoded outputs are also placed in the same folder.

EXIT Value Indicators:
Exit Value 0: Run Successful
Exit Value 1: Mismatch in number of command line arguments
Exit Value 2: File Not Found
Exit Value 3: Bit Length Error
Exit Value 4: Option Mismatch Error