#ifndef GPPARSER_H
#define GPPARSER_H

#include <string>
#include <sstream>
#include <vector>
#include "../Synth/GPNode.h"
#include "../Synth/GPPrimitives.h"

// TOKENIZER
extern std::vector<std::string> &split(const std::string &s, std::vector<std::string> &elems, const char* delims);
extern std::vector<std::string> split(const std::string &s, const char* delims);

// S-EXPRESSION PARSING
// macro to "consume" a token when we observe it
#define consume (*currentIndex)++
#define tokenizerFunctionArgs std::vector<std::string> tokens, unsigned* currentIndex
#define tokenizerArgs tokens, currentIndex
#define subtreeFunctionArgs GPRandom* rng, std::string& error
#define subtreeArgs rng, error
extern std::vector<GPMutatableParam*> parseMutatableParams(tokenizerFunctionArgs);
extern GPMutatableParam* createMutatableParam(tokenizerFunctionArgs, std::string type, bool ismutatable);
extern bool parseChild(tokenizerFunctionArgs, subtreeFunctionArgs, GPNode** child);
extern GPNode* createNode(tokenizerFunctionArgs, subtreeFunctionArgs);

// EASY ACCESS
extern GPNode* createNode(std::string nodestring, GPRandom* rng);
extern GPMutatableParam* createMutatableParam(std::string paramstring, std::string type, bool ismutatable);

#endif
