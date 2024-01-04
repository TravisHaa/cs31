//
//  render.cpp
//  proj5
//
//  Created by Travis Ha on 11/14/23.
//
#include <string.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

//b
#include <iostream>
  #include <sstream>
  #include <streambuf>
  #include <cstring>
  #include <cassert>
int hyphen(char s[]){
    for(int i=0; i<strlen(s); i++){
        if(s[i] == '-'){
            return i;
        }
    }
    return -1;
}

int updatelen(char word[]){
    size_t wordLength = 0;
    if (word != nullptr) {
        wordLength = strlen(word);
        return wordLength;
    }
    else{
        return 0;
    }
}

void strincopy(char dest[], const char copy[], int numChars) {
    for (int i = 0; i < numChars; ++i) {
        dest[i] = copy[i];
    }
    dest[numChars] = '\0'; // Null-terminate the destination string
}
bool isStreamEmpty(istream& stream) {
    return stream.peek() == EOF;
}
              
int render(int lineLength, istream& inf, ostream& outf){
    
    char line[250];
    int out = 0;
    
    int currentLength = 0;
    char prev[180] = {};
    int sameword = 0;//0 when we aren't on the sameword, 1 when we are on the sameword, for when we split hyphenated word and want to add its segments without including spaces
    if(isStreamEmpty(inf)){
        return 0;
    }
        while (inf.getline(line, 250)) {
          
        
        if(lineLength < 1){
            out = 2;
            break;
        }
        
        char* word = strtok(line, " \t");//find first occurence of " " and replaces it with "\0"
            if(word == NULL){//if we start with no word, meaning spaces
                if(currentLength != 0){//if we reach a line or word that is null, have to check if the most recent output line is a newline or not. if it isnt, then make a newline
                    outf <<'\n';
                }
                return out;
                break;
            }
        int spaces = 1;
                while (word != nullptr) {
                    if(strcmp(word, "@P@") == 0 && strcmp(prev, "@P@") != 0){
                        if(currentLength > 0){
                            outf << '\n';//have to move to next line if our current line has something in it
                        }
                        if(prev[0] != '\0'){
                            outf << '\n';
                        }
                        word = strtok(nullptr, " \t");
                        strcpy(prev, "@P@");
                        currentLength = 0;
                        spaces = 1;
                        while(word != NULL && strcmp(word, "@P@") == 0 && strcmp(prev, "@P@") == 0){//if we have a paragraph break, and the lat word wasn't a character break, print a newline and move onto next word
                            word = strtok(nullptr, " \t");
                            strcpy(prev, "@P@");
                            currentLength = 0;
                            spaces = 1;
                        }
                       
                        if(word == NULL){
                            break;
                        }
                    }
                    
                    if(strcasecmp(word, "\t") == 0){
                        word = strtok(nullptr, "  \t");
                    }
                    
                    size_t wordLength = strlen(word);
                    
                    if(wordLength == lineLength){
                        spaces = 0; //when our word is the perfect length to fit in the line, we dont need a space
                    }
                    
                    if (currentLength + wordLength + spaces <= lineLength) {
                        if (currentLength > 0) {//signifies that we aren't at the start of a paragraph, so add a space
                           
                            if(spaces == 2){
                                outf << "  ";
                                currentLength += 2;
                                spaces = 1;
                            }
                            else if(spaces == 1){
                                outf << " ";
                                currentLength++;
                            }
                        }
                         outf << word;
                        currentLength += wordLength;
                        
                        strcpy(prev, word);
                        word = strtok(nullptr, " \t");//tells strtok to continue from where it left off in the previous call, meaning word refers to the next word in the line.
                        wordLength = updatelen(word);
                        
                        
                    } else {
                        if(strlen(word) < lineLength && strchr(word, '-') == NULL){//if the word doesnr't fit on the line, and we dont have hyphens or a super long word, then we have a normal word, so create a newline and add the word there
                            sameword = 0; //if word has no hypehns, we finished the hypentaed word, so sameword = 0
                            outf << "\n" << word;
                            currentLength = wordLength;
                            strcpy(prev, word);
                            word = strtok(nullptr, " \t");//move to next word
                            wordLength = updatelen(word);
                        }
                        
                        else if(strchr(word, '-') != NULL){//if the word has hyphens
                            
                           
                            char temp[180];
                            int h = hyphen(word);
                                strincopy(temp, word, h+1);//temp contains first half including hyphen
                        
                            if(h == 0){//if the hyphen was at the start of our word, we have to rewrite word differently
                                char* newString = word + 1;
                                strcpy(word, newString);
                                wordLength = updatelen(word);
                                sameword = 1;
                            }
                            else{
                                char *substring = word + (h+1);
                                strincopy(word, substring, strlen(substring)); // update word with second half of word
                                if(strcmp(word, "") == 0){//if the second half is empty, that means we finished the hypenated word, so we arent on the smae word anymore
                                    sameword = 0;
                                    word = strtok(nullptr, " \t");
                                    
                                }
                                wordLength = updatelen(word);
                                sameword = 1;
                              
                                
                            }
                            //we now have a half stored in our word, so dont update the word at the end of the function
                            int x =strlen(temp);
                            if(sameword == 1){
                                spaces = 0;
                            }
                                if( x +  currentLength + spaces <= lineLength){//if the half that contains the hyphen fits in the remainder of the line, add it along with the spaces behind it
                                    if (currentLength > 0) {//signifies that we aren't at the start of a paragraph, so add a space
                                        if(spaces == 2){
                                            outf << "  ";
                                            currentLength += 2;
                                            spaces = 1;
                                        }
                                        else if(spaces == 1){
                                            outf << " ";
                                            currentLength++;
                                        }
                                    }
                                    outf << temp;
                                    currentLength += strlen(temp);
                                    sameword = 1;
    
                                }
                               
                                else{//if the half with the hyphen doesnt fit in the line
                                    if(strlen(temp) > lineLength){//if the half is bigger than a line
                                        while(strlen(temp) > lineLength){//if the wordLength is greater than the line,
                                            //must be a word longer than the lineLength, so continue the word on the following lines and return 1, then move onto next word
                                            out = 1;
                                            char part[180];
                                            strincopy(part, temp, lineLength);//temp cstring that contains the part of the word that fits on the line
                                            part[lineLength] = '\0'; //add nll character to end of temp
                                            int s = strlen(prev);
                                            if(s != 0 && strcmp(prev, "@P@") != 0){
                                                outf << '\n';
                                            }
                                            outf << part;
                                            strcpy(prev, part);
                                            
                                            char *substring = temp + lineLength;//pointer that contains second half of word
                                            strcpy(temp, substring); //rewriting word to contain the second half of the word
                                            temp[strlen(temp)] = '\0';
                                           
                                            
                                        }
                                        outf << "\n" << temp;
                                        currentLength = strlen(temp);
                                        strcpy(prev, temp);
                                        //still have other half to deal with, so dont update word
                                    }
                                    else{//if it fits in a new line, then print it, and update word and wordlength, if the word still has a hyphen, let it run through our entire render function again and let them deal with it
                                        outf << '\n' << temp;
                                        currentLength = strlen(temp);
                                        strcpy(prev, temp);
                                        
                                      
                                    }
                                    
                                }
                                
                            
                        }
                        else{//the only other scenario is if the word is larger than the line
                             while(wordLength > lineLength){//if the wordLength is greater than the line,
                                //must be a word longer than the lineLength, so continue the word on the following lines and return 1, then move onto next word
                                
                                //if word that surpasses lineLength contains hyphens, then break down by hyphens
                                
                                out = 1;
                                char temp[180];
                                strincopy(temp, word, lineLength);//temp cstring that contains the part of the word that fits on the line
                                temp[lineLength] = '\0'; //add nll character to end of temp
                                int s =strlen(prev);
                                if(s != 0 && strcmp(prev, "@P@") != 0){
                                    outf << '\n';
                                }
                                outf << temp;
                                 strcpy(prev, temp);
                                
                                //always will be a full line, so dont need to update currentlength
                                char *substring = word + lineLength;//pointer that contains second half of word
                                strcpy(word, substring); //rewriting word to contain the second half of the word
                                wordLength = updatelen(word);
                               
                                
                            }
                            outf << '\n' << word;
                            currentLength = strlen(word);
                            strcpy(prev, word);
                            
                            
                            
                            word = strtok(nullptr, " \t");//move to next word
                            wordLength = updatelen(word);
                            
                        }
                        
                        
                       
                        }
                    
                    if(word != nullptr){//check if the word had punc, if it did, then on the next word we will add two spaces
                        int lastchar = strlen(prev)-1;
                        if(prev[lastchar] == '.' || prev[lastchar] == '?' ||prev[lastchar] == '!' ||prev[lastchar] == ':' ){
                            spaces = 2;
                        }
                        else if(sameword == 1){
                            spaces = 0;
                        }
                        else{
                            spaces = 1;
                        }
                    }

                }
        
                
            }
    if(out != 2 &&  strcmp(prev, "@P@") != 0 && strcmp(prev, "") != 0){
        outf << '\n';
    }
   
    return out;
    }

void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if ( ! dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if ( ! oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}




int main()
  {
    ifstream infile("/Users/travisha/Desktop/CS31/proj5/proj5/proj5/test.txt");
   cerr << render(8, infile, cerr);

    testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0);
           testRender(8, "  This is a test  \n", "This is\na test\n", 0);
           testRender(6, "Testing it\n", "Testin\ng it\n", 1);
           testRender(-5, "irrelevant", "irrelevant", 2);
    testRender(10, "--------------------", "----------\n----------\n", 0);
    testRender(6, "bruno-is-a-dog", "bruno-\nis-a-\ndog\n", 0);
    testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0); //multiple tabs and newlines
        testRender(6, "Testing it\n\n", "Testin\ng it\n", 1); //ending with a multiple newlin
        testRender(2, "---so", "--\n-\nso\n", 0); //multiple dashes at once
        testRender(7, "            ", "", 0); //multiple spaces
        testRender(7, "", "", 0); //empty string
        testRender(7, "\n\n\n\n\n", "", 0); //multiple newlines
        testRender(7, "@P@ @P@ @P@ @P@", "", 0); //multiple paragraph breaks
        testRender(7, "@P@R", "@P@R\n", 0); //invalid paragraph break
        testRender(7, "\n\n  \n \n ", "", 0); //multiple spaces and newlines
        testRender(0, "\n\n  \n \n ", "", 2); //invalid linelength
        testRender(8, "@P@ @P@ @P@ @P@  This is a test  \n", "This is\na test\n", 0); //paragraphs at start of input
        testRender(8, "@P@ @P@ @P@ @P@  This is a test  @P@ @P@ @P@ @P@", "This is\na test\n", 0); //paragraphs at end of input
        testRender(8, "@P@ @P@ @P@ @P@  This is a test  \n @P@ @P@ @P@ @P@", "This is\na test\n", 0); //paragraphs at end of input after newline
        testRender(5, "hello hello hello", "hello\nhello\nhello\n", 0); //word just fits the line
        testRender(5, ".", ".\n", 0); //just punctuation
                testRender(5, "hi @P@ hi", "hi\n\nhi\n", 0); //new paragraph
        testRender(5, "tree---leaf", "tree-\n--\nleaf\n", 0); //multiple word parts
        testRender(5, "longerthanlinelength", "longe\nrthan\nlinel\nength\n", 1); //returns 1 when longer than word length    testRender(7,
        testRender(5, "longerthanlinelength", "longe\nrthan\nlinel\nength\n", 1); //returns 1 when longer than word length
        testRender(5, "treee---leaf", "treee\n---\nleaf\n", 1); //returns 1 when word parts longer than word length
        testRender(251, "asdfalskhflskfhksdhfksjdhfksdhfksdhfksdhfksdhfksdjhfksdhfkjsdhfksdfhkdsjhfkdjshfkjdshfkdfhskjsdhfksdfhksdfhksjdhfkfdshkjfhskjdhfkjsdfhkjdshfskjdfhskjdhfkjsdfhkjsdhfsdfa;lskdjf;sjdf asdfalskhflskfhksdhfksjdhfksdhfksdhfksdhfksdhfksdjhfksdhfkjsdhfksdfhkdsjhfkdjshfkjdshfkdfhskjsdhfksdfhksdfhksjdhfkfdshkjfhskjdhfkjsdfhkjdshfskjdfhskjdhfkjsdfhkjsdhfsdfa;lskdjf;sjdf", "asdfalskhflskfhksdhfksjdhfksdhfksdhfksdhfksdhfksdjhfksdhfkjsdhfksdfhkdsjhfkdjshfkjdshfkdfhskjsdhfksdfhksdfhksjdhfkfdshkjfhskjdhfkjsdfhkjdshfskjdfhskjdhfkjsdfhkjsdhfsdfa;lskdjf;sjdf\nasdfalskhflskfhksdhfksjdhfksdhfksdhfksdhfksdhfksdjhfksdhfkjsdhfksdfhkdsjhfkdjshfkjdshfkdfhskjsdhfksdfhksdfhksjdhfkfdshkjfhskjdhfkjsdfhkjdshfskjdfhskjdhfkjsdfhkjsdhfsdfa;lskdjf;sjdf\n", 0); //over 250 lines for extra credit
        testRender(5, "hi. hi. hi.", "hi.\nhi.\nhi.\n", 0); //punction but new lines
        testRender(10, "hi. hi. hi.", "hi.  hi.\nhi.\n", 0); //two spaces after punctuation
        testRender(10, "hi? hi? hi?", "hi?  hi?\nhi?\n", 0); //two spaces after punctuation
        testRender(10, "hi! hi! hi!", "hi!  hi!\nhi!\n", 0); //two spaces after punctuation
        testRender(10, "hi: hi: hi:", "hi:  hi:\nhi:\n", 0); //two spaces after punctuation
        testRender(3, "hi. @P@ hi.", "hi.\n\nhi.\n", 0); //paragraph break after word fits line
        testRender(3, "hi. @p@ hi.", "hi.\n@p@\nhi.\n", 0); //lowercase paragraph break doesn't work
        testRender(7, "@P@@P@", "@P@@P@\n", 0); //multiple paragraph breaks together
        testRender(30, "... ??? !!! ::: .?!:", "...  ???  !!!  :::  .?!:\n", 0); //multiple punctuation together
        testRender(5, "-- -", "-- -\n", 0); //dashes with space
        testRender(4, "-- -", "-- -\n", 0); //dashes with space
        testRender(4, "- --", "- --\n", 0); //dashes with space fit exactly
        testRender(5, "-!- -", "-!- -\n", 0); //dashes with punctuation
        testRender(5, "!-- -", "!-- -\n", 0); //punctuation before dashes
        testRender(5, "!-- --", "!-- -\n-\n", 0); //punctuation then multiple dashes
        testRender(5, "!--. --", "!--.\n--\n", 0); //punctuation space then dash
        testRender(7, "!--. --", "!--.  -\n-\n", 0); //punctuation space dash with dash that doesn't fit
        testRender(7, "!--. -hi-", "!--.  -\nhi-\n", 0); //punctuation space dash with dash that doesn't fit with word
        testRender(8, "!--. -hi-", "!--.  -\nhi-\n", 0); //punctuation space dash with dash that doesn't fit with word
        testRender(7, "hi -hi -", "hi -hi\n-\n", 0); //dash before and after word
        testRender(7, "hi- hi -", "hi- hi\n-\n", 0); //dash after word
        testRender(7, "---.---", "---.---\n", 0); //punctuation within dashes
        testRender(10, "---.--- .", "---.--- .\n", 0); //dash space punctuation
        testRender(10, "--- .--- .", "--- .--- .\n", 0); //dash space punctuation
        testRender(10, "- --- .--- .", "- --- .---\n.\n", 0); //multiple dashes and punctuation
        testRender(10, "-. --- .--- .", "-.  --- .-\n-- .\n", 0); //multiple dashes and punctuation
        testRender(10, "-. -Hi", "-.  -Hi\n", 0); //punctuation space dash word
        testRender(10, "- -Hi", "- -Hi\n", 0); //dash space dash word
        testRender(2, "- -Hi", "-\n-\nHi\n", 0); //dash space dash word that doesn't fit in line
        testRender(2, "- -Hii", "-\n-\nHi\ni\n", 1); //dash spaces and words that extends line
        testRender(2, "- @P@", "-\n", 0); //paragraph break after dash
        testRender(3, "- @P@", "-\n", 0); //paragraph break after dash
        testRender(2, "-. @P@ -", "-.\n\n-\n", 0); //paragraph break after dash
        testRender(3, "-. @P@ -", "-.\n\n-\n", 0); //paragraph break after dash
        testRender(1, "- @P@ -", "-\n\n-\n", 0); //paragraph break after dash
        testRender(1, "a @P@ @P@ a", "a\n\na\n", 0); //paragraph break after dash
        testRender(1, "aa @P@ @P@ a", "a\na\n\na\n", 1); //paragraph break after dash
        testRender(1, "a- @P@ @P@ a", "a\n-\n\na\n", 1); //paragraph break after dash
        testRender(1, "-a @P@ @P@ a", "-\na\n\na\n", 0); //paragraph break after dash
        testRender(2, " - @P@ @P@ -", "-\n\n-\n", 0); //paragraph break after dash
        testRender(2, " a @P@ @P@ a", "a\n\na\n", 0); //paragraph break after dash
        testRender(2, "@P@R", "@P\n@R\n", 1); //paragraph break after dash
        testRender(2, "@P@R a a", "@P\n@R\na\na\n", 1); //paragraph break after dash
        testRender(1, "@P@R a a", "@\nP\n@\nR\na\na\n", 1); //paragraph break after dash
        testRender(1, "@P@R a a", "@\nP\n@\nR\na\na\n", 1); //paragraph break after dash
        testRender(1, "@Pa a", "@\nP\na\na\n", 1); //paragraph break after dash
        testRender(3, "hi. pi", "hi.\npi\n", 0); //paragraph break after dash
        testRender(2, "@Pa a", "@P\na\na\n", 1);
    testRender(2, "----@P@", "----\n@P@\n", 0); // dashes before break code, @P@ starting on own line
    testRender(7, "test-@P@", "test-\n@P@\n", 0); // break code is part of longer word portion but on its own after a dash
    testRender(7, "tests@P@", "tests@P\n@\n", 1); // break code part is out of bounds
    testRender(4, "@P@t", "@P@t\n", 0); // normal character after break code
    testRender(7, "@P@: x", "@P@:  x\n", 0); // extra space character after break code
    testRender(4, "@P@-", "@P@-\n", 0); // dash after break code
    testRender(5, "@P@-tests", "@P@-\ntests\n", 0); // break code is part of longer word portion but on its own before (and including) a dash
    testRender(7, "@P@tests", "@P@test\ns\n", 1); // dash before break code, and break code part is out of bounds
    testRender(5, "x@P@x", "x@P@x\n", 0); // base case
    testRender(4, "x@P@x", "x@P@\nx\n", 1); // word portion longer than line length
    testRender(5, "-@P@-", "-@P@-\n", 0); // sandwich with dashes
    testRender(5, "-----@P@-", "-----\n@P@-\n", 0); // sandwich with dashes, @P@ starting on its own line
    testRender(2, "", "", 0);
    testRender(1, "a", "a\n", 0); // size 1 that fits
    testRender(3, "a", "a\n", 0); // 1 full word that is shorter than linelength
    testRender(2, "am", "am\n", 0); // 1 full word that just fits
    testRender(2, " am", "am\n", 0); // 1 full word that just fits, with a leading space
    testRender(2, " \n\n am", "am\n", 0); // 1 full word that fits, with multiple leading newlines/whitespaces
    testRender(2, "am ", "am\n", 0); // 1 full word that fits, followed by a whitespace
    testRender(2, "am \n\n ", "am\n", 0); // 1 full word that fits, followed by mutiple newlines/whitespaces
    testRender(1, "ab", "a\nb\n", 1); // size 1 that doesn't fit
    testRender(2, "hello", "he\nll\no\n", 1); // 1 full word that doesn't fit
    testRender(3, "way-", "way\n-\n", 1); // 1 full word that doesn't fit, and the character that wraps around is a dash
    testRender(3, "a b", "a b\n", 0); // multiple words that fit on a line
    testRender(3, "a\n\n b", "a b\n", 0); // multiple words that fit on a line, but given in multiple lines
    testRender(3, "be a", "be\na\n", 0); // two words whose combined length is longer than lineLength
    testRender(3, "a- a", "a-\na\n", 0); // whitespace/newline after a dash means new word
    testRender(3, "am-a", "am-\na\n", 0); // 1 word portion that perfectly fits the line, ending in a dash
    testRender(3, "a-a", "a-a\n", 0); // 1 full word made of multiple word portions that fits on line
    testRender(3, "a-str", "a-\nstr\n", 0); // word cut into two by dash (two portions, one word)
    testRender(2, "a------a", "a-\n--\n--\n-a\n", 0); // consecutive dashes
    testRender(3, ".     ! : ? .", ".\n!\n:\n?\n.\n", 0); // characters with extra spaces needed (test all at once)
    testRender(2, "a:", "a:\n", 0); // 1 full word that just fits on line, with last character needing extra space --> basically checking that
    testRender(3, "a:a", "a:a\n", 0); // make sure no extra space printed when special character is within word
    testRender(3, "? xx", "?\nxx\n", 0); // make sure needing an extra space doesn't cause trailing space at the end of line
    testRender(1, "@P@", "", 0); // break code is the only word portion in text
    testRender(3, "@P@ \n\n ", "", 0); // break code is the only word portion in text, but several newlines/whitespaces following it
    testRender(2, " \n\n @P@", "", 0); // break code is the only word portion in text, but several newlines/whitespaces preceding it
    testRender(1, " @P@  \t  \n \n    @P@ @P@", "", 0); // break codes are only non-whitespace characters in file
    testRender(2, " @P@ x", "x\n", 0); // break code preceding first printable char
    testRender(1, "x @P@ ", "x\n", 0); // break code following last printable char
    testRender(2, "te @P@ te", "te\n\nte\n", 0); // base test for break
    testRender(2, "te\n@P@\tte", "te\n\nte\n", 0); // other types of whitespaces around break, like newlines or tabs
    testRender(2, "te @P@ @P@ \t @P@ @P@ \n @P@ te", "te\n\nte\n", 0); // make sure that consecutive breaks treated as one
    testRender(2, "t@P@", "t@\nP@\n", 1); // normal character before break code
    testRender(2, ":@P@", ":@\nP@\n", 1); // extra space character before break code
    testRender(3, "-@P@", "-\n@P@\n", 0); // dash before break code
    testRender(2, "----@P@", "----\n@P@\n", 0); // dashes before break code, @P@ starting on own line
    testRender(7, "test-@P@", "test-\n@P@\n", 0); // break code is part of longer word portion but on its own after a dash
    testRender(7, "tests@P@", "tests@P\n@\n", 1); // break code part is out of bounds
    testRender(4, "@P@t", "@P@t\n", 0); // normal character after break code
    testRender(7, "@P@: x", "@P@:  x\n", 0); // extra space character after break code
    testRender(4, "@P@-", "@P@-\n", 0); // dash after break code
    testRender(5, "@P@-tests", "@P@-\ntests\n", 0); // break code is part of longer word portion but on its own before (and including) a dash
    testRender(7, "@P@tests", "@P@test\ns\n", 1); // dash before break code, and break code part is out of bounds
    testRender(5, "x@P@x", "x@P@x\n", 0); // base case
    testRender(4, "x@P@x", "x@P@\nx\n", 1); // word portion longer than line length
    testRender(5, "-@P@-", "-@P@-\n", 0); // sandwich with dashes
    testRender(5, "-----@P@-", "-----\n@P@-\n", 0);
//           cerr << "Tests complete" << endl;
    
  }
