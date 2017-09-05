//**********************************************************************************************************
// SWE Assignment 1 
// Margin Manipulation (Word Processing System)
//  This word processing program allocates words to lines in order to satisfy the margin requirements
//  Task: Process a text file DATA1.TXT and produce as output
//  Requirements:
//      i.  This word processing program allocates words to lines in order to satisfy the margin requirements
//
//  Assumptions:
//  1 char -> 12pt
//  72pt -> 1 inch
//  No word from the input file is greater in length than 80 - (leftmargin + rightmargin) characters
//
//  By Ayotunde Odejayi
//**********************************************************************************************************


#include <iostream>
#include <fstream>
#include <string>

void writeToOutfile(std::string, bool);
std::string appendNSpaces(std::string, int);

int leftmargin = 0, rightmargin = 0;
std::ofstream datawrite("out.txt");
bool isfirstline = true;

enum ascii{
    space = 32,
    newLine = 13,
    lineFeed = 10,
    fullStop = 46
};

int main()
{
    char val;
    std::string wordsToWrite = "", newWord = "";
    bool wordDetected = false, isSentenceComplete = false;
    const int kmaxlineChar = 80, kcharToInches = 12;
    
    std::ifstream reader;
    reader.open("DATA1.txt");
    
    // Parse input file
    reader >> leftmargin >> rightmargin;
    uint noLineCharLeft = kmaxlineChar*kcharToInches-(leftmargin+rightmargin);

    while (!reader.eof())
    {
        reader >> std::noskipws >> val;

        if (static_cast<int>(val) == newLine)  
        {
            wordDetected = false;
            noLineCharLeft = kmaxlineChar*kcharToInches-(leftmargin+rightmargin);
        }
        else if (static_cast<int>(val) == space)  
        {
            if (wordDetected) 
            {
                if (wordsToWrite.length()*kcharToInches < noLineCharLeft)
                {                  
                    if (isSentenceComplete)
                    {
                        wordsToWrite = appendNSpaces(wordsToWrite, 2);
                        newWord = appendNSpaces(newWord, 2);
                    } else {
                        wordsToWrite = appendNSpaces(wordsToWrite, 1);
                        newWord = appendNSpaces(wordsToWrite, 1);
                    }
                    writeToOutfile(newWord, false);   
                }
                else if (wordsToWrite.length()*kcharToInches == noLineCharLeft)
                {
                    writeToOutfile(newWord, false);  
                }
                else  // write to new line
                {
                    isSentenceComplete ? newWord += "  " : newWord += " ";
                    wordsToWrite = newWord;                    
                    writeToOutfile(newWord, true);      
                }
                newWord = "";
            }
            isSentenceComplete = false;
        }
        else if (static_cast<int>(val) == lineFeed)
        {
            // we have no work to do
        }
        else 
        {        
            wordDetected = true;
            wordsToWrite += val;
            newWord += val;
 
            if (static_cast<int>(val) == fullStop) 
            {
                isSentenceComplete = true;
            }
        }
    }

    // flush buffer to outfile
    std::cout << std::flush;
    datawrite.close();       

    return 0;
}

void writeToOutfile(std::string word, bool isNewLine)
{
    if (!isNewLine)
    {
        if (isfirstline)
        {
            for (int i=0; i<leftmargin; i++)
            datawrite << " ";        
        }
        datawrite << word;                  
    } 
    else 
    {
        datawrite<<"\r\n";
        for (int i=0; i<leftmargin; i++)
            datawrite << " ";
        datawrite << word;
    }
    isfirstline = false;    
}

std::string appendNSpaces(std::string word, int numSpaces)
{
    for (uint i=0; i<numSpaces; i++)
    {
        word += " ";
    }
    return word;
}
