#ifndef GPPARSER_H
#define GPPARSER_H

#include <string>
#include <sstream>
#include <vector>
#include "GPMutatableParam.h"
#include "../Synth/GPNode.h"
#include "../Synth/GPPrimitives.h"

// TOKENIZER
extern std::vector<std::string> &split(const std::string &s, std::vector<std::string> &elems, const char* delims);
extern std::vector<std::string> split(const std::string &s, const char* delims);

// S-EXPRESSION PARSING
// macro to "consume" a token when we observe it
#define consume (*currentIndex)++
extern GPNode* createNode(std::vector<std::string> tokens, unsigned* currentIndex, GPRandom* rng);
// macro for standard structure of vector of tokens/index
#define tokenizer tokens, currentIndex
// macro for calling createSubtree easily
#define subtreeArgs rng
extern GPMutatableParam* createMutatableParam(std::vector<std::string> tokens, unsigned* currentIndex, bool ismutatable, std::string type);

// EASY ACCESS
extern GPNode* createNode(std::string nodestring, GPRandom* rng);
extern GPMutatableParam* createMutatableParam(std::string paramstring, std::string type, bool ismutatable);

#endif
