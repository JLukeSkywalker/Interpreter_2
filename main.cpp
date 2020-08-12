/*
    a lot of the code in add/sub/mul/div/mod is the same,
    probs a way to condense/combine

*/

#include <stdio.h>
#include <string.h>
#include <boost/variant.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <unordered_map>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <cfenv>

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
        args[1] = "hello.txt";
    }
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
        return NULL;
    }
    fseek(fileData,0L,SEEK_END);  // Find the end of the file
    byteSize = ftell(fileData);   // Store the file size for later
    fseek(fileData,0L,SEEK_SET);  // Move iterator back to the start

    char* codeIn = (char*)calloc(byteSize,CHAR_SIZE);
    if(codeIn == NULL){
        return NULL;
    }

    fread(codeIn, CHAR_SIZE, byteSize, fileData);
    fclose(fileData);

    // Remove all spaces from the file
    // LATER, PROBABLY EXTRACT FUNCTION POSITIONS HERE TOO
    int spaces = 0;
    int openQuote = 0;
    for(int i=0;i<byteSize;i++){
        if(codeIn[i] == '"')
            openQuote ^= 1;
        if(openQuote || codeIn[i] != ' ')
            codeIn[spaces++] = codeIn[i];
    }
    if(openQuote){
        printf("Unmatched Quote\n");
        return NULL;
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
        // printf("Command: %s, Params: %s\n",command,params);

        switch(*(int*)command){
        case 7368041:
            // imp (import)
            break;
        case 7561577:
            //ias (import as)
            break;
        case 7632239:{
            // out (output)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            // loop through the vector of params
            for (auto & out : inputs) {
                // if its a variable, print its value
                if(variables.count(out)){
                    std::cout << variables[out];
                }else{
                    // not a variable, remove quotes if any and print
                    out.erase(remove(out.begin(), out.end(), '\"'),out.end());
                    boost::replace_all(out, "\\n","\n");
                    boost::replace_all(out, "\\c",",");
                    boost::replace_all(out, "\\s"," ");
                    boost::replace_all(out, "\\t","\t");
                    std::cout << out;
                }
            }
            break;
        }case 7368297:{
            // inp (input)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 1){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << std::endl;
                return 1;
            }if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:{
                    std::string userIn;
                    std::getline(std::cin,userIn);
                    variables[inputs[0]] = userIn;
                    break;
                }case 1:{
                    int userIn;
                    if(scanf("%d", &userIn)<0){
                        printf("Error on Line: %d, Invalid user input for type int",lineNum);
                        return 1;
                    }
                    variables[inputs[0]] = userIn;
                    break;
                }case 2:
                    double userIn;
                    if(scanf("%lf", &userIn)<0){
                        printf("Error on Line: %d, Invalid user input for type double",lineNum);
                        return 1;
                    }
                    variables[inputs[0]] = userIn;
                }
                undeclared.erase(inputs[0]);
            }else{
                std::string userIn;
                std::getline(std::cin,userIn);
                variables[inputs[0]] = userIn;
            }

            break;
        }case 6514035:
            // sec (seconds)
            break;
        case 6579297:{
            // add (addition)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << std::endl;
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                case 1:
                    a = boost::get<int>(variables[inputs[1]]);
                    break;
                case 2: 
                    a = boost::get<double>(variables[inputs[1]]);
                }
            }else{
                try{
                    a = std::stod(inputs[1]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    b = boost::get<int>(variables[inputs[2]]);
                    break;
                case 2: 
                    b = boost::get<double>(variables[inputs[2]]);
                }
            }else{
                try{
                    b = std::stod(inputs[2]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    variables[inputs[0]] = (int)(a+b);
                    break;
                case 2:
                    variables[inputs[0]] = a+b;
                undeclared.erase(inputs[0]);
                }
            }else{
                variables[inputs[0]] = a+b;
            }

            break;
        }case 6452595:{
            // sub (subtract)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << std::endl;
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                case 1:
                    a = boost::get<int>(variables[inputs[1]]);
                    break;
                case 2: 
                    a = boost::get<double>(variables[inputs[1]]);
                }
            }else{
                try{
                    a = std::stod(inputs[1]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    b = boost::get<int>(variables[inputs[2]]);
                    break;
                case 2: 
                    b = boost::get<double>(variables[inputs[2]]);
                }
            }else{
                try{
                    b = std::stod(inputs[2]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    variables[inputs[0]] = (int)(a-b);
                    break;
                case 2:
                    variables[inputs[0]] = a-b;
                undeclared.erase(inputs[0]);
                }
            }else{
                variables[inputs[0]] = a-b;
            }

            break;
        }case 7107949:{
            // mul (multiply)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << std::endl;
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                case 1:
                    a = boost::get<int>(variables[inputs[1]]);
                    break;
                case 2: 
                    a = boost::get<double>(variables[inputs[1]]);
                }
            }else{
                try{
                    a = std::stod(inputs[1]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    b = boost::get<int>(variables[inputs[2]]);
                    break;
                case 2: 
                    b = boost::get<double>(variables[inputs[2]]);
                }
            }else{
                try{
                    b = std::stod(inputs[2]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    variables[inputs[0]] = (int)(a*b);
                    break;
                case 2:
                    variables[inputs[0]] = a*b;
                undeclared.erase(inputs[0]);
                }
            }else{
                variables[inputs[0]] = a*b;
            }

            break;
        }case 7760228:{
            // div (divide)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << std::endl;
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                case 1:
                    a = boost::get<int>(variables[inputs[1]]);
                    break;
                case 2: 
                    a = boost::get<double>(variables[inputs[1]]);
                }
            }else{
                try{
                    a = std::stod(inputs[1]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    b = boost::get<int>(variables[inputs[2]]);
                    break;
                case 2: 
                    b = boost::get<double>(variables[inputs[2]]);
                }
            }else{
                try{
                    b = std::stod(inputs[2]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    variables[inputs[0]] = (int)(a/b);
                    break;
                case 2:
                    variables[inputs[0]] = a/b;
                undeclared.erase(inputs[0]);
                }
            }else{
                variables[inputs[0]] = a/b;
            }

            break;
        }case 6582125:{
            // mod (modulus)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << std::endl;
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                case 1:
                    a = boost::get<int>(variables[inputs[1]]);
                    break;
                case 2: 
                    a = boost::get<double>(variables[inputs[1]]);
                }
            }else{
                try{
                    a = std::stod(inputs[1]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << std::endl;
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    b = boost::get<int>(variables[inputs[2]]);
                    break;
                case 2: 
                    b = boost::get<double>(variables[inputs[2]]);
                }
            }else{
                try{
                    b = std::stod(inputs[2]);
                }catch(std::exception& e){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << std::endl;
                    return 1;
                case 1:
                    std::feclearexcept(FE_ALL_EXCEPT);
                    variables[inputs[0]] = (int)std::fmod(a,b);
                    if(std::fetestexcept(FE_INVALID)){
                        printf("Error on Line: %d, divide by zero\n",lineNum);
                        return 1;
                    }
                    break;
                case 2:
                    std::feclearexcept(FE_ALL_EXCEPT);
                    variables[inputs[0]] = std::fmod(a,b);
                    if(std::fetestexcept(FE_INVALID)){
                        printf("Error on Line: %d, divide by zero\n",lineNum);
                        return 1;
                    }
                undeclared.erase(inputs[0]);
                }
            }else{
                std::feclearexcept(FE_ALL_EXCEPT);
                variables[inputs[0]] = std::fmod(a,b);
                if(std::fetestexcept(FE_INVALID)){
                    printf("Error on Line: %d, divide by zero\n",lineNum);
                    return 1;
                }
            }

            break;   
        }case 7630441:{
            // int (make int)
            char* index = strchr(params,',');
            if(index != NULL){
                char varName[index-params];
                strncpy(varName, params,index-params);
                int value;
                try{
                    value = std::stoi(index+1);
                }catch(std::exception& e){
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
                std::string value = index+1;
                value.erase(remove(value.begin(), value.end(), '\"'),value.end());
                variables[varName] = value;
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
                double value;
                try{
                    value = std::stod(index+1);
                }catch(std::exception& e){
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
        case 7103844:{
            // del (delete variable)
            
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            // loop through the vector of params
            for (auto & param : inputs)
                variables.erase(param);
            
            break;
        }case 7102838:{
            // val (check if variable available)

            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 2){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << std::endl;
                return 1;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    printf("Error on Line: %d, cannot store bool into string\n",lineNum);
                    return 1;
                case 1:
                case 2:
                    variables[inputs[0]] = (int)variables.count(inputs[1]);
                undeclared.erase(inputs[0]);
                }
            }else{
                variables[inputs[0]] = (int)variables.count(inputs[1]);
            }

            break;
        }case 7103858:
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