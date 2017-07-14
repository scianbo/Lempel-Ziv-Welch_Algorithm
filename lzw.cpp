/*Author: Advaith Auron Suresh
 UNCC Id: 800943085*/

/*Standard Header File Inclusions*/
#include <iostream>
#include <sstream>
#include <fstream>
#include <bitset>
#include <string>
#include <unordered_map>
#include <utility>
#include <cstring>
#include <vector>

/*User defined Table Size parameter N*/
std::size_t BIT_LENGTH;
std::size_t MAX_TABLE_SIZE;

using namespace std;

/*Sets Maximum Table Size based on User Defined Input*/
void initialize_global_variables(int bit_len)
{
    BIT_LENGTH = bit_len;
    MAX_TABLE_SIZE = 1 << BIT_LENGTH;
}

/*Removes file extension (i.e removes .txt from input.txt)*/
std::string remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

/*This function contains the compression algorithm
 Return type: void
 Arguments: input string, input file size, filename*/
void compress(string input, int size, string filename) {
    unordered_map<string, int> compression(MAX_TABLE_SIZE);
    //Dictionary initializing with ASCII
    unsigned int next_entry;
    for ( next_entry = 0 ; next_entry < 256 ; next_entry++ ){
        compression[string(1,next_entry)] = next_entry;
    }
    std::string current_string;
    std::string rawfile;
    short result;
    /*Contains encoded filename*/
    rawfile = remove_extension(filename)+".lzw";
    /*Opens file in binary mode for writing*/
    std::ofstream outputFile(rawfile,ios::binary | ios::out);
    /*Iterates through the input set*/
    for(char& symbol:input)
    {
        current_string = current_string + symbol;
        /*Checks if string is already present in the dictionary or not*/
        if ( compression.find(current_string) == compression.end() )
        {
            /*Adds content to the dictionary if the maximum table size is not exceeded*/
            if (next_entry <= MAX_TABLE_SIZE)
            {
                compression.insert(make_pair(current_string, next_entry++));
            }
            current_string.erase(current_string.size()-1);
            result = (compression[current_string]);
            /*Writing content into a binary file*/
            outputFile.write(reinterpret_cast<const char*>(&result),sizeof(result));
            current_string = symbol;
        }   
    }   
    if (current_string.size())
    {
            result = (compression[current_string]);
            outputFile.write(reinterpret_cast<const char*>(&result),sizeof(result));
    }
    outputFile.close();
}

/*This function contains the de-compression algorithm
 Return type: void
 Arguments: input string, input file size, filename*/
void decompress(string input, int size, string filename) {
    unordered_map<unsigned int, string> updated_dictionary(MAX_TABLE_SIZE);
    //Dictionary initializing with ASCII
    unsigned int next_code;
    for ( next_code = 0 ; next_code < 256 ; next_code++ ){
    updated_dictionary[next_code] = string(1,next_code);
    }
    string previous_string;
    /*Read file in binary mode*/
    ifstream infile(filename, ios::in|ios::binary);
    ofstream outputFile;
    std::string rawfile;
    rawfile = remove_extension(filename);
    outputFile.open(rawfile + "_decoded.txt");
    int iteration;
    short code;
    /*Extracting 16 bits and converting binary to Integer*/
    for (iteration=0; iteration<size; iteration+=2){
        /*Reads buffer and appends data into code*/
        infile.read((char*)&code,sizeof(code));
        /*Iterate till end of file*/
        if(infile.eof())
            break;
        /*Checks if string is already present in the dictionary or not*/
        if ( updated_dictionary.find(code) == updated_dictionary.end() ) 
        {
            /*Update dictionary for a new string found*/
            updated_dictionary.insert(make_pair(code,(previous_string + previous_string.substr(0,1))));
        }
        outputFile<<updated_dictionary[code];
        if ( previous_string.size())
        {
            updated_dictionary.insert(make_pair(next_code++,previous_string + updated_dictionary[code][0])); 
        }
        /*Updates previous string*/
        previous_string = updated_dictionary[code];
    }
    outputFile.close();
}

/*Obtains character stream and converts it into a string*/
string char_to_string(const char *ch, int Size){
    string converted_string;
    if (ch[Size-1] == '\0') 
        converted_string.append(ch);
    else 
    {
        for(int iteration=0; iteration<Size; iteration++) 
            converted_string.append(1,ch[iteration]);
    }
    return converted_string;
}

/*Flags an error and displays help information*/
static void display_error()
{
    cerr  <<"\n\t\t\t\t How to run the file "
          <<"\nThe command line arguments for compression: encoder filename.txt bit_length (bit_length between 9 and 16)"
          <<"\nThe command line arguments for de-compression: decoder filename.lzw bit_length (bit_length between 9 and 16)"
          <<"\nCompressed file to be found on the same directory with the same filename and .lzw extension"
          <<"\nDe-compressed file to be found on the same directory with the same filename and _decoded.txt extension\n";
    
}

int main (int argc, char* argv[]) 
{     
    /*Checks if the number of command line arguments are as expected or flags an error */
    if (argc < 3)
    {
        display_error();   
        return(1);
    }
    ifstream file (argv[2], ios::in|ios::binary|ios::ate);
    /*Displays error if file is not found*/
    if(!file) 
    {
        cout <<"\nFile Not Found";
        display_error();
        return(2);
    }
    char * filemem;  
    int bit_len;
    streampos file_size;
    bit_len = atoi(argv[3]);
    /*Checks if the input bit length is between 9 and 16, if not, flags an error*/
    if(bit_len>=9 && bit_len<=16)
        initialize_global_variables(bit_len);
    else
    {
        display_error();
        return(3);
    }
    
    if (file.is_open())
    {
        file_size = file.tellg();
        filemem = new char[file_size];
        file.seekg (0, ios::beg);
        file.read (filemem, file_size);
        file.close();
        string input_stream;
        input_stream = char_to_string(filemem,file_size);
        /*Checks for command line argument "encoder"*/
        if (string( "encoder" ) == argv[1] )
        {
            compress( input_stream, file_size, argv[2]);
            /*Memory release*/
            delete []filemem;
        }
        /*Checks for command line argument "decoder"*/
        else if (string( "decoder" ) == argv[1] )
        {
            decompress( input_stream, file_size, argv[2]);
            /*Memory release*/
            delete []filemem;
        }
        else
        {
            display_error();
            return (4);
        }
        
    }
    return 0;
}
