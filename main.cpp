#include <stdio.h>
#include <string.h>
#include <boost/variant.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <unordered_map>
#include <stdlib.h>
#include <iostream>

#define CHAR_SIZE sizeof(char)

std::unordered_map<std::string, boost::variant<std::string,int,double>> variables;
std::unordered_map<std::string, int> undeclared;

// Function definitions
char* readFile(char* fileName);
int runCode(char* codeIn);

int main(int numArgs, char* args[]) {
    // Make sure only a fileName was given from command line
    if(numArgs < 2 || numArgs > 2){
        //printf("Invalid or no filename given\n");
        //return 1;
    }
    args[1] = "hello.txt";
    char* codeIn = readFile(args[1]);
    if(codeIn == NULL){
        printf("Unable to read file: %s\n",args[1]);
        return 1;
    }

    int codeRan = runCode(codeIn);
    if(codeRan != 0)  return codeRan;
}// END FUNCTION main

char* readFile(char* fileName){
    FILE *fileData;
    long byteSize;

    // read in entire file
    fileData = fopen(fileName,"r");
    if(fileData == NULL){
        printf("Unable to read file: %s\n",fileName);
        return NULL;
    }
    fseek(fileData,0L,SEEK_END);  // Find the end of the file
    byteSize = ftell(fileData);   // Store the file size for later
    fseek(fileData,0L,SEEK_SET);  // Move iterator back to the start

    char* codeIn = (char*)calloc(byteSize,CHAR_SIZE);
    if(codeIn == NULL){
        printf("Unable to read file: %s\n",fileName);
        return NULL;
    }

    fread(codeIn, CHAR_SIZE, byteSize, fileData);
    fclose(fileData);

    // Remove all spaces from the file
    // LATER, PROBABLY EXTRACT FUNCTION POSITIONS HERE TOO
    int spaces = 0;
    for(int i=0;i<byteSize;i++){
        if(codeIn[i] != ' '){
            codeIn[spaces++] = codeIn[i]!='_'?codeIn[i]:' ';
        }
    }
    codeIn[spaces] = '\0';

    return codeIn;
}// END FUNCTION readFile

int runCode(char* codeIn){
    // Create something to store all variables

    // Copy over the inputted code into new array to not mutate the old one
    char runningCode[strlen(codeIn)];
    strcpy(runningCode,codeIn);

    // Loop through each line of the new changeable array
    int lineNum = 0;
    int charactersRead = 0;
    // Use strtok as an iterator for each line
    char *line = strtok(runningCode, "\n");
    while(line != NULL){
        printf("Line %d: %s\n",++lineNum,line);
        charactersRead += strlen(line);
        char *command = (char*)calloc(3, CHAR_SIZE);
        char *params = (char*)calloc(strlen(line)-3, CHAR_SIZE);
        strcpy(params,line+3);
        strncpy(command, line, 3);
        printf("Command: %s, Params: %s\n",command,params);

        switch(*(int*)command){
        case 7368041:
            // imp (import)
            break;
        case 7561577:
            //ias (import as)
            break;
        case 7632239:{
            // out (output)
            std::vector<std::string> outputs;
            boost::split(outputs, params, [](char c){return c == ',';});
            for (auto & out : outputs) {
                if(variables.count(out)){
                    switch(variables[out].which()){
                    case 0:
                        std::cout << boost::get<std::string>(variables[out]);
                        break;
                    case 1:
                        std::cout << boost::get<int>(variables[out]);
                        break;
                    case 2:
                        std::cout << boost::get<double>(variables[out]);
                        break;
                    }
                }else{
                    std::cout << out;
                }
            }
            break;
        }case 7368297:
            // inp (input)
            break;
        case 6514035:
            // sec (seconds)
            break;
        case 6579297:
            // add (addition)
            break;
        case 6452595:
            // sub (subtraction)
            break;
        case 7107949:
            // mul (multiplication)
            break;
        case 7760228:
            // div (division)
            break;
        case 7630441:{
            // int (make int)
            char* index = strchr(params,',');
            if(index != NULL){
                char varName[index-params];
                strncpy(varName, params,index-params);
                int value = atoi(index+1);
                if(strcmp(index+1,"0")!=0 && value==0){
                    printf("Error on Line: %d, Invalid parameter: %s\n",lineNum, index+1);
                    return 1;
                }
                variables[varName] = value;
                if(undeclared.count(varName)){
                    undeclared.erase(varName);
                }
            }else{
                undeclared[params] = 1;
            }
            break;
        }case 7500915:{
            // str (make string)
            char* index = strchr(params,',');
            if(index != NULL){
                char varName[index-params];
                strncpy(varName, params,index-params);
                variables[varName] = index+1;
                if(undeclared.count(varName)){
                    undeclared.erase(varName);
                }
            }else{
                undeclared[params] = 0;
            }
            break;
        }case 7103076:{
            // dbl (make double)
            char* index = strchr(params,',');
            if(index != NULL){
                char varName[index-params];
                strncpy(varName, params,index-params);
                double value = atof(index+1);
                if(strncmp(index+1,"0",1)!=0 && value==0){
                    printf("Error on Line: %d, Invalid parameter: %s\n",lineNum, index+1);
                    return 1;
                }
                variables[varName] = value;
                if(undeclared.count(varName)){
                    undeclared.erase(varName);
                }
            }else{
                undeclared[params] = 2;
            }
            break;
        }case 7564393:
            // ils (make int list)
            break;
        case 7564403:
            // sls (make string list)
            break;
        case 7564388:
            // dls (make double list)
            break;
        case 7564406:
            // vls (make variable list)
            break;
        case 7368560:
            // pop (pop from list)
            break;
        case 7632240:
            // put (push to list)
            break;
        case 7890025:
            // idx (get element ad index)
            break;
        case 7234924:
            // len (get length of list or string)
            break;
        case 7958627:
            // cpy (copy data from one var to another)
            break;
        case 7103844:
            // del (delete variable)
            break;
        case 7102838:
            // val (check if variable available)
            break;
        case 7103858:
            // rel (perform boolean operation)
            break;
        case 7500389:
            // err (throw manual error)
            break;
        case 7171939:
            // com (comment, do nothing)
            break;
        case 6711666:
            // rif (run function if true)
            break;
        case 7239026:
            // run (run function)
            break;
        default:
            printf("Invalid command found on line %d: %s\n",lineNum,line);
            return 1;
        }
        // Move iterator to next line
        line=strtok(NULL,"\n");
    }// END WHILE
    return 0;
}// END FUNCTION runCode