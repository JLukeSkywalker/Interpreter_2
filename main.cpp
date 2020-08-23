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
#include <stack>
#include <time.h>

#define CHAR_SIZE sizeof(char)

std::unordered_map<std::string, boost::variant<std::string,int,double>> variables;
std::unordered_map<std::string, int> undeclared;
std::stack<boost::variant<std::string,int,double>> theStack;

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
    std::string runningCode(codeIn);

    // Loop through each line of the new changeable array
    int lineNum = 0;
    
    std::vector<std::string> lines;
    boost::split(lines, runningCode, [](char c){return c == '\n';});

    int lastLine = lines.size();

    while(lineNum < lastLine){
        char line[lines[lineNum].length()+1];
        strcpy(line, lines[lineNum].c_str());

        printf("Line %d: %s\n",++lineNum,line);
        char *command = (char*)calloc(3, CHAR_SIZE);
        char *params = (char*)calloc(strlen(line)-3, CHAR_SIZE);
        strcpy(params,line+3);
        strncpy(command, line, 3);
        // printf("Command: %s, Params: %s\n",command,params);

        switch(*(int*)command){
        // run (run function)
        case 7239026:{
            
            break;
        }
        // imp (import)
        case 7368041:{

            break;
        }
        // ias (import as)
        case 7561577:{

            break;
        }
        // not (logic not)
        case 7630702:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 2){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[1] << '\n';
                return 1;
            }
            double value;
            switch(variables[inputs[1]].which()){
            case 1:
                value = boost::get<int>(variables[inputs[1]]);
                break;
            case 2:
                value = boost::get<double>(variables[inputs[1]]);
            }
            
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)!value;
                    break;
                case 2:
                    variables[inputs[0]] = (double)!value;
                }
            }else{
                variables[inputs[0]] = (int)!value;
            }
            break;
        }
        // and (logic and)
        case 6581857:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[1] << '\n';
                return 1;
            }if(!variables.count(inputs[2]) || variables[inputs[2]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[2] << '\n';
                return 1;
            }
            double first, second;
            switch(variables[inputs[1]].which()){
            case 1:
                first = boost::get<int>(variables[inputs[1]]);
                break;
            case 2:
                first = boost::get<double>(variables[inputs[1]]);
            }
            switch(variables[inputs[2]].which()){
            case 1:
                second = boost::get<int>(variables[inputs[2]]);
                break;
            case 2:
                second = boost::get<double>(variables[inputs[2]]);
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)(first && second);
                    break;
                case 2:
                    variables[inputs[0]] = (double)(first && second);
                }
            }else{
                variables[inputs[0]] = (int)(first && second);
            }
            break;
        }
        // orr (logic or)
        case 0000000:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[1] << '\n';
                return 1;
            }if(!variables.count(inputs[2]) || variables[inputs[2]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[2] << '\n';
                return 1;
            }
            double first, second;
            switch(variables[inputs[1]].which()){
            case 1:
                first = boost::get<int>(variables[inputs[1]]);
                break;
            case 2:
                first = boost::get<double>(variables[inputs[1]]);
            }
            switch(variables[inputs[2]].which()){
            case 1:
                second = boost::get<int>(variables[inputs[2]]);
                break;
            case 2:
                second = boost::get<double>(variables[inputs[2]]);
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)(first || second);
                    break;
                case 2:
                    variables[inputs[0]] = (double)(first || second);
                }
            }else{
                variables[inputs[0]] = (int)(first || second);
            }
            break;
        }
        // cmp (compare/test equality)
        case 7368035:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1])){
                std::cout << "Error on Line: " << lineNum << ", No variable named: " << inputs[1] << '\n';
                return 1;
            }if(variables.count(inputs[2]) && !variables[inputs[1]].which() != !variables[inputs[2]].which()){
                printf("Error on Line: %d, Cannot compare strings and numbers\n",lineNum);
                return 1;
            }
            double result;
            switch(variables[inputs[1]].which()){
                case 0:{
                    inputs[2].erase(remove(inputs[2].begin(), inputs[2].end(), '\"'),inputs[2].end());
                    result = boost::get<std::string>(variables[inputs[1]]) == (variables.count(inputs[2]) ? boost::get<std::string>(variables[inputs[2]]) : inputs[2]);
                    break;
                }
                case 1:{
                    double value;
                    try{
                        value = std::stod(inputs[2]);
                    }catch(std::exception& e){
                        if(!variables.count(inputs[2])){
                            std::cout << "Error on Line: " << lineNum << ", No numeric variable names: " << inputs[2] << '\n';
                            return 1;
                        }
                        switch(variables[inputs[2]].which()){
                        case 1:
                            value = boost::get<int>(variables[inputs[2]]);
                            break;
                        case 2:
                            value = boost::get<double>(variables[inputs[2]]);
                        }
                    }
                    result = boost::get<int>(variables[inputs[1]]) == value;
                    break;
                }
                case 2:{
                    double value;
                    try{
                        value = std::stod(inputs[2]);
                    }catch(std::exception& e){
                        if(!variables.count(inputs[2])){
                            std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for index: " << inputs[2] << '\n';
                            return 1;
                        }
                        switch(variables[inputs[2]].which()){
                        case 1:
                            value = boost::get<int>(variables[inputs[2]]);
                            break;
                        case 2:
                            value = boost::get<double>(variables[inputs[2]]);
                        }
                    }
                    result = boost::get<double>(variables[inputs[1]]) == value;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)result;
                    break;
                case 2:
                    variables[inputs[0]] = (double)result;
                }
            }else{
                variables[inputs[0]] = (int)result;
            }
            break;
        }
        // gth (greater than)
        case 6845543:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[1] << '\n';
                return 1;
            }
            double value;
            try{
                value = std::stod(inputs[2]);
            }catch(std::exception& e){
                if(!variables.count(inputs[2]) || variables[inputs[2]].which() == 0){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for index: " << inputs[2] << '\n';
                    return 1;
                }
                switch(variables[inputs[2]].which()){
                    case 1:
                        value = boost::get<int>(variables[inputs[2]]);
                        break;
                    case 2:
                        value = boost::get<double>(variables[inputs[2]]);
                }
            }
            double result;
            switch(variables[inputs[1]].which()){
                case 1:
                    result = boost::get<int>(variables[inputs[1]]) > value;
                    break;
                case 2:
                    result = boost::get<double>(variables[inputs[1]]) > value;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)result;
                    break;
                case 2:
                    variables[inputs[0]] = (double)result;
                }
            }else{
                variables[inputs[0]] = (int)result;
            }
            break;
        }
        // lth (less than)
        case 6845548:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[1] << '\n';
                return 1;
            }
            double value;
            try{
                value = std::stod(inputs[2]);
            }catch(std::exception& e){
                if(!variables.count(inputs[2]) || variables[inputs[2]].which() == 0){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for index: " << inputs[2] << '\n';
                    return 1;
                }
                switch(variables[inputs[2]].which()){
                    case 1:
                        value = boost::get<int>(variables[inputs[2]]);
                        break;
                    case 2:
                        value = boost::get<double>(variables[inputs[2]]);
                }
            }
            double result;
            switch(variables[inputs[1]].which()){
                case 1:
                    result = boost::get<int>(variables[inputs[1]]) < value;
                    break;
                case 2:
                    result = boost::get<double>(variables[inputs[1]]) < value;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)result;
                    break;
                case 2:
                    variables[inputs[0]] = (double)result;
                }
            }else{
                variables[inputs[0]] = (int)result;
            }
            break;
        }
        // gte (greater than or equal)
        case 6648935:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[1] << '\n';
                return 1;
            }
            double value;
            try{
                value = std::stod(inputs[2]);
            }catch(std::exception& e){
                if(!variables.count(inputs[2]) || variables[inputs[2]].which() == 0){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for index: " << inputs[2] << '\n';
                    return 1;
                }
                switch(variables[inputs[2]].which()){
                    case 1:
                        value = boost::get<int>(variables[inputs[2]]);
                        break;
                    case 2:
                        value = boost::get<double>(variables[inputs[2]]);
                }
            }
            double result;
            switch(variables[inputs[1]].which()){
                case 1:
                    result = boost::get<int>(variables[inputs[1]]) >= value;
                    break;
                case 2:
                    result = boost::get<double>(variables[inputs[1]]) >= value;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)result;
                    break;
                case 2:
                    variables[inputs[0]] = (double)result;
                }
            }else{
                variables[inputs[0]] = (int)result;
            }
            break;
        }
        // lte (less than or equal)
        case 6648940:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(undeclared.count(inputs[0]) && undeclared[inputs[0]]==0){
                printf("Error on Line: %d, cannot store boolean into string\n",lineNum);
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() == 0){
                std::cout << "Error on Line: " << lineNum << ", No numeric variable named: " << inputs[1] << '\n';
                return 1;
            }
            double value;
            try{
                value = std::stod(inputs[2]);
            }catch(std::exception& e){
                if(!variables.count(inputs[2]) || variables[inputs[2]].which() == 0){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for index: " << inputs[2] << '\n';
                    return 1;
                }
                switch(variables[inputs[2]].which()){
                    case 1:
                        value = boost::get<int>(variables[inputs[2]]);
                        break;
                    case 2:
                        value = boost::get<double>(variables[inputs[2]]);
                }
            }
            double result;
            switch(variables[inputs[1]].which()){
                case 1:
                    result = boost::get<int>(variables[inputs[1]]) <= value;
                    break;
                case 2:
                    result = boost::get<double>(variables[inputs[1]]) <= value;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 1:
                    variables[inputs[0]] = (int)result;
                    break;
                case 2:
                    variables[inputs[0]] = (double)result;
                }
            }else{
                variables[inputs[0]] = (int)result;
            }
            break;
        }
        // out (output)
        case 7632239:{
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
        }
        // inp (input)
        case 7368297:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 1){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
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
                        printf("Error on Line: %d, Invalid user input for type int.\n",lineNum);
                        return 1;
                    }
                    variables[inputs[0]] = userIn;
                    break;
                }case 2:
                    double userIn;
                    if(scanf("%lf", &userIn)<0){
                        printf("Error on Line: %d, Invalid user input for type double.\n",lineNum);
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
        }
        // sec (seconds)
        case 6514035:{
            char* index = strchr(params,',');
            if(index != NULL){
                printf("Error on Line: %d, Too many parameters.\n",lineNum);
                return 1;
            }if(variables.count(params)){
                printf("Error on Line: %d, Variable already in use: %s\n",lineNum, params);
                return 1;
            }
            if(undeclared.count(params)){
                switch(undeclared[params]){
                case 0:
                    printf("Error on Line: %d, cannot store time into string\n",lineNum);
                    return 1;
                case 1:
                    variables[params] = (int)time(NULL);
                    break;
                case 2:
                    variables[params] = (double)time(NULL);
                }
            }else{
                variables[params] = (double)time(NULL);
            }
            break;
        }
        // add (addition)
        case 6579297:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
        }
        // sub (subtract)
        case 6452595:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
        }
        // mul (multiply)
        case 7107949:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
        }
        // div (divide)
        case 7760228:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
        }
        // mod (modulus)
        case 6582125:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }
            double a=0, b = 0;
            if(variables.count(inputs[1])){
                switch(variables[inputs[1]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[1] << '\n';
                    return 1;
                }
            }if(variables.count(inputs[2])){
                switch(variables[inputs[2]].which()){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
                    return 1;
                }
            }

            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for add: " << inputs[2] << '\n';
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
        }
        // int (make int)
        case 7630441:{
            char* index = strchr(params,',');
            if(index != NULL){
                char varName[index-params];
                strncpy(varName, params,index-params);
                if(variables.count(varName)){
                    printf("Error on Line: %d, Variable already in use: %s\n",lineNum, varName);
                    return 1;
                }
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
            }else if(!variables.count(params)){
                undeclared[params] = 1;
            }else{
                printf("Error on Line: %d, Variable already in use: %s\n",lineNum, params);
                return 1;
            }
            break;
        }
        // str (make string)
        case 7500915:{
            char* index = strchr(params,',');
            if(index != NULL){
                char varName[index-params];
                strncpy(varName, params,index-params);
                if(variables.count(varName)){
                    printf("Error on Line: %d, Variable already in use: %s\n",lineNum, varName);
                    return 1;
                }
                std::string value = index+1;
                value.erase(remove(value.begin(), value.end(), '\"'),value.end());
                variables[varName] = value;
                if(undeclared.count(varName)){
                    undeclared.erase(varName);
                }
            }else if(!variables.count(params)){
                undeclared[params] = 0;
            }else{
                printf("Error on Line: %d, Variable already in use: %s\n",lineNum, params);
                return 1;
            }
            break;
        }
        // dbl (make double)
        case 7103076:{
            char* index = strchr(params,',');
            if(index != NULL){
                char varName[index-params];
                strncpy(varName, params,index-params);
                if(variables.count(varName)){
                    printf("Error on Line: %d, Variable already in use: %s\n",lineNum, varName);
                    return 1;
                }
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
            }else if(!variables.count(params)){
                undeclared[params] = 2;
            }else{
                printf("Error on Line: %d, Variable already in use: %s\n",lineNum, params);
                return 1;
            }
            break;
        }
        // put (push to theStack)
        case 7632240:{
            char* index = strchr(params,',');
            if(index != NULL){
                printf("Error on Line: %d, Too many parameters.",lineNum);
                return 1;
            }if(!variables.count(params)){
                printf("Error on Line: %d, Undefined variable: %s.\n",lineNum, params);
                return 1;
            }
            theStack.push(variables[params]);
            break;
        }
        // pop (pop from theStack)
        case 7368560:{
            theStack.pop();
            break;
        }
        // top (read the top of theStack)
        case 7368564:{
            char* index = strchr(params,',');
            if(index != NULL){
                printf("Error on Line: %d, Too many parameters.",lineNum);
                return 1;
            }if(variables.count(params)){
                printf("Error on Line: %d, Variable already in use: %s\n",lineNum, params);
                return 1;
            }
            if(undeclared.count(params)){
                switch(undeclared[params]){
                case 0:
                    if(theStack.top().which() != 0){
                        printf("Error on Line: %d, Invalid variable type: %s\n",lineNum, params);
                        return 1;
                    }
                    variables[params] = theStack.top();
                    break;
                case 1:
                    switch(theStack.top().which()){
                    case 0:
                        printf("Error on Line: %d, Invalid variable type: %s\n",lineNum, params);
                        return 1;
                    case 1:
                        variables[params] = theStack.top();
                        break;
                    case 2:
                        variables[params] = (int)boost::get<double>(theStack.top());
                    }
                    break;
                case 2:
                    switch(theStack.top().which()){
                    case 0:
                        printf("Error on Line: %d, Invalid variable type: %s\n",lineNum, params);
                        return 1;
                    case 1:
                        variables[params] = (double)boost::get<int>(theStack.top());
                        break;
                    case 2:
                        variables[params] = theStack.top();
                    }
                undeclared.erase(params);
                }
            }
            variables[params] = theStack.top();
            
            break;
        }
        // clr (clear theStack)
        case 7498851:{
            while (!theStack.empty())
                theStack.pop();
            break;
        }
        // idx (get element at index of string)
        case 7890025:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 3){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() != 0){
                std::cout << "Error on Line: " << lineNum << ", No string named: " << inputs[1] << '\n';
                return 1;
            }
            int index;
            try{
                index = std::stoi(inputs[2]);
            }catch(std::exception& e){
                if(!variables.count(inputs[2]) || variables[inputs[2]].which() != 1){
                    std::cout << "Error on Line: " << lineNum << ", Invalid parameter type for index: " << inputs[2] << '\n';
                    return 1;
                }
                index = boost::get<int>(variables[inputs[2]]);
            }
            std::string text = boost::get<std::string>(variables[inputs[1]]);
            if(index < 0 || index >= text.length()){
                printf("Error on Line: %d, Index out of range: %d\n",lineNum, index);
                return 1;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:{
                    std::string s(1,text[index]);
                    variables[inputs[0]] = s;
                    break;
                }
                case 1:
                    variables[inputs[0]] = (int)text[index];
                    break;
                case 2:
                    variables[inputs[0]] = (double)text[index];
                    break;
                undeclared.erase(inputs[0]);
                }
            }else{
                std::string s(1,text[index]);
                variables[inputs[0]] = s;
            }

            break;
        }
        // len (get length of string)
        case 7234924:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 2){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }if(!variables.count(inputs[1]) || variables[inputs[1]].which() != 0){
                std::cout << "Error on Line: " << lineNum << ", No string named: " << inputs[1] << '\n';
                return 1;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    printf("Error on Line: %d, cannot store number into string\n",lineNum);
                    return 1;
                case 1:
                    variables[inputs[0]] = (int)boost::get<std::string>(variables[inputs[1]]).length();
                    break;
                case 2:
                    variables[inputs[0]] = (double)boost::get<std::string>(variables[inputs[1]]).length();
                    break;
                undeclared.erase(inputs[0]);
                }
            }else{
                variables[inputs[0]] = (int)boost::get<std::string>(variables[inputs[1]]).length();
            }

            break;
        }
        // del (delete variable)
        case 7103844:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});
            // loop through the vector of params
            for (auto & param : inputs)
                variables.erase(param);
            
            break;
        }
        // val (check if variable available)
        case 7102838:{
            // tokenize the params and store into a vector
            std::vector<std::string> inputs;
            boost::split(inputs, params, [](char c){return c == ',';});

            if(inputs.size() != 2){
                printf("Error on Line: %d, Invalid number of parameters: %s\n",lineNum, params);
                return 1;
            }if(variables.count(inputs[0])){
                std::cout << "Error on Line: " << lineNum << ", Variable already in use: " << inputs[0] << '\n';
                return 1;
            }
            if(undeclared.count(inputs[0])){
                switch(undeclared[inputs[0]]){
                case 0:
                    printf("Error on Line: %d, cannot store bool into string\n",lineNum);
                    return 1;
                case 1:
                    variables[inputs[0]] = (int)variables.count(inputs[1]);
                    break;
                case 2:
                    variables[inputs[0]] = (double)variables.count(inputs[1]);
                undeclared.erase(inputs[0]);
                }
            }else{
                variables[inputs[0]] = (int)variables.count(inputs[1]);
            }

            break;
        }
        // err (throw manual error)
        case 7500389:{
            printf("Error on Line: %d, User Error: %s\n",lineNum, params);
            return 1;
            break;
        }
        // com (comment, do nothing)
        case 7171939:
            break;
        // default, no valid command found
        default:
            printf("Error on Line: %d, Invalid Command: %s\n",lineNum,line);
            return 1;
        }
    }// END WHILE
    return 0;
}// END FUNCTION runCode