//***********************************************************************************************************************************
// SWE Assignment 1 
// Margin Manipulation (Word Processing System)
//  This word processing program allocates words to lines in order to satisfy the margin requirements
//  Task: Process a command line specified input text file  and produce as output DAT1.TXT
//
//  Requirements:
//      i.  This word processing program allocates words to lines in order to satisfy the margin requirements
//  
//  Notes:
//  Using a 12pt. font:
//       12 char -> 1 inch
//
//  Assumptions:
//       i) 1 inch = 5 characters (Round figure selected against 12 for ease of computation)
//       ii) The leftmargin + rightmargin value (converted to characters) is no greater than the max. number of characters per line
//           i.e leftmargin + rightmargin <= [80 - (leftmargin + rightmargin)*5]
//
//  By Ayotunde Odejayi
//*************************************************************************************************************************************


#include <iostream>
#include <fstream>
#include <cstdlib> 

void writeToOutfile(std::string, bool);
std::string appendNSpaces(std::string, int);

// 1-time configurable vars
const int kMaxlineChar = 80, kInchToChar = 5;
const bool consoleOutputListing = false;

bool isfirstline = true, nlflag = false;
uint leftmargin = 0, rightmargin = 0;
std::ofstream writeToTxt("DAT1.txt");

enum ascii{
    lineFeed = 10,
    newLine = 13,
    space = 32,
    parensClose = 41,
    fullStop = 46
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
	    std::cerr << "Input text file missing...\n\n";
	    exit(EXIT_FAILURE);
    }
    
    std::ifstream reader(argv[1]);
    if(!reader){
        std::cerr << "Unable to open file \"" << argv[1] << "\"\n\n";
	    exit(EXIT_FAILURE);
    }
    
    // 1. init vars
    char val;
    std::string wordsToWrite = "", newWord = "";
    bool wordDetected = false, isSentenceComplete = false;
    std::ofstream writeToTxt2("stuff.txt");
    
    // 2. parse input file
    reader >> leftmargin >> rightmargin;
    uint noLineCharLeft = kMaxlineChar - (leftmargin+rightmargin)*kInchToChar;

    while (!reader.eof())
    {
        reader >> std::noskipws >> val;
        writeToTxt2 << val << " " << static_cast<int>(val) << "\n";

        if (static_cast<int>(val) == newLine)  
        {
            if (!nlflag && newWord != "")  // helps identify if immediate last wasn't new line + if space(wordDetect) has already inserted the corr. spaces
            {
                wordsToWrite = appendNSpaces(wordsToWrite,2);
                newWord = appendNSpaces(newWord,2);
            }
        
            noLineCharLeft = kMaxlineChar - (leftmargin+rightmargin)*kInchToChar;
            wordDetected = false;
            isSentenceComplete = false;
            nlflag = true;            
        }
        else if (static_cast<int>(val) == space)  
        {
            if (wordDetected) 
            {
                if (wordsToWrite.length() < noLineCharLeft)
                {                  
                    if (isSentenceComplete)
                    {
                        wordsToWrite = appendNSpaces(wordsToWrite,2);
                        newWord = appendNSpaces(newWord,2);
                    } else {
                        wordsToWrite = appendNSpaces(wordsToWrite,1);
                        newWord = appendNSpaces(newWord,1);
                    }
                    writeToOutfile(newWord, false);   
                }
                else if (wordsToWrite.length() == noLineCharLeft)
                {
                    writeToOutfile(newWord, false);  
                }
                else  // write to new line
                {
                    isSentenceComplete ? newWord = appendNSpaces(newWord,2) : newWord = appendNSpaces(newWord,1);
                    wordsToWrite = newWord;                    
                    writeToOutfile(newWord, true);      
                }
                newWord = "";
            }
            isSentenceComplete = false;
            wordDetected = false;
        }
        else if (static_cast<int>(val) == lineFeed)
        {
            // we have no work to do except we don't include it in output listing or outfile 
        }
        else 
        {        
            wordDetected = true;
            wordsToWrite += val;
            newWord += val;
            nlflag = false;                                    

            isSentenceComplete = false;            
            if (static_cast<int>(val) == fullStop) 
            {
                isSentenceComplete = true;
            }
        }
    }

    // 3. flush buffer to outfile
    writeToOutfile(newWord, false);     
    std::cout << std::flush;
    writeToTxt.close();       

    return 0;
}

// Helper functions
void writeToOutfile(std::string word, bool isNewLine)
{
    if (!isNewLine)
    {
        if (isfirstline)
        {
            for (uint i=0; i<leftmargin*kInchToChar; i++)
            {
                writeToTxt << " ";        
                consoleOutputListing ? std::cout << " " : std::cout << "";
            }
        }
        writeToTxt << word;  
        consoleOutputListing ? std::cout << word : std::cout << "";
    } 
    else 
    {
        writeToTxt<<"\r\n";
        for (uint i=0; i<leftmargin*kInchToChar; i++)
        {
            writeToTxt << " ";
            consoleOutputListing ? std::cout << " " : std::cout << "";
        }
        writeToTxt << word;
        consoleOutputListing ? std::cout << word : std::cout << "";                        
    }
    isfirstline = false;    
}

std::string appendNSpaces(std::string word, int numSpaces)
{
    for (uint i=0; i<numSpaces; i++)
        word += " ";
    return word;
}
