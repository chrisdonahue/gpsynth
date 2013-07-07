/*
  ==============================================================================

    GPNetwork.cpp
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#include "GPParser.h"

/*
    =========
    TOKENIZER
    =========
*/

// http://stackoverflow.com/a/236803
std::vector<std::string> &split(const std::string &s, std::vector<std::string> &elems, const char* delims) {
    std::size_t prev = 0, pos;
    while ((pos = s.find_first_of(delims, prev)) != std::string::npos)
    {
        if (pos > prev)
            elems.push_back(s.substr(prev, pos-prev));
        prev = pos+1;
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, const char* delims) {
    std::vector<std::string> elems;
    split(s, elems, delims);
    return elems;
}

/*
    ====================
    S-EXPRESSION PARSING
    ====================
*/

std::vector<GPMutatableParam*> parseMutatableParams(tokenizerFunctionArgs) {
    std::vector<GPMutatableParam*> ret;
    unsigned tokens_remaining = tokens.size() - (*currentIndex);
    GPMutatableParam* current = NULL;

    // if we have at least 4 tokens remaining
    while (tokens_remaining >= 4) {
        // try to make a mutatable param
        current = createMutatableParam(tokenizerArgs, "", true);

        // if bad parse reset the tokenizer position and break
        if (current == NULL) {
            *currentIndex -= 4;
            break;
        }

        // otherwise add it to the return array
        else {
            ret.push_back(current);
        }
        
        // update number of tokens remaining
        tokens_remaining = tokens.size() - (*currentIndex);
    }

    // return the list of mutatable params
    return ret;
}

GPMutatableParam* createMutatableParam(tokenizerFunctionArgs, std::string type, bool ismutatable) {
    // get tokens
    std::string tag = tokens[consume];
    std::string minstr = tokens[consume];
    std::string valstr = tokens[consume];
    std::string maxstr = tokens[consume];

    // if the param is continuous
    if (tag.compare("c") == 0) {
        float min = (float) std::atof(minstr.c_str());
        float val = (float) std::atof(valstr.c_str());
        float max = (float) std::atof(maxstr.c_str());
        return new GPMutatableParam(type, ismutatable, val, min, max);
    }
    // else if the param is discrete
    else if (tag.compare("d") == 0) {
        int min = std::atoi(minstr.c_str());
        int val = std::atoi(valstr.c_str());
        int max = std::atoi(maxstr.c_str());
        return new GPMutatableParam(type, ismutatable, val, min, max);
    }
    // else something went wrong
    else {
        return NULL;
    }
}

/*
	returns true for a successful parse, false for an unsuccessful parse
*/

bool parseChild(tokenizerFunctionArgs, subtreeFunctionArgs, GPNode** child) {
	// make sure we're not out of tokens
    unsigned tokens_remaining = tokens.size() - (*currentIndex);
	if (tokens_remaining <= 0) {
		std::cerr << "Child expected but not found" << std::endl;
		*child = NULL;
		return false;
	}

	// cache the type of the upcoming child to check later if it was intentionally null
	std::string child_type = tokens[*currentIndex];

	// create the child and deref the return value
	*child = createNode(tokenizerArgs, subtreeArgs);
	
	// check if return child null and return successful parse if it was intentional
	if (*child == NULL) {
		return child_type.compare("null") == 0;
	}

	// child was non null representing a successful parse
	return true;
}

GPNode* createNode(tokenizerFunctionArgs, subtreeFunctionArgs) {
    // parse node string tag
    std::string type = tokens[consume];

    // parse mutatable params list
    std::vector<GPMutatableParam*> params = parseMutatableParams(tokenizerArgs);

    // radius around constant nodes (any nonzero value should be the same)
    // float constantRadius = 1;

    // ADSR nodes
    if (type.compare("adsr") == 0) {
        if (params.size() != 7) {
            std::cerr << "Incorrect number of mutatable params for an ADSR Terminal Node" << std::endl;
            return NULL;
        }
        params[0]->setType("adsr_terminal_delay");
        params[1]->setType("adsr_terminal_attack");
        params[2]->setType("adsr_terminal_attack_height");
        params[3]->setType("adsr_terminal_decay");
        params[4]->setType("adsr_terminal_sustain");
        params[5]->setType("adsr_terminal_sustain_height");
        params[6]->setType("adsr_terminal_release");

        return new ADSRTerminalNode(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
    }
    else if (type.compare("adsr*") == 0) {
        if (params.size() != 7) {
            std::cerr << "Incorrect number of mutatable params for an ADSR Envelope Node" << std::endl;
            return NULL;
        }
        params[0]->setType("adsr_envelope_delay");
        params[1]->setType("adsr_envelope_attack");
        params[2]->setType("adsr_envelope_attack_height");
        params[3]->setType("adsr_envelope_decay");
        params[4]->setType("adsr_envelope_sustain");
        params[5]->setType("adsr_envelope_sustain_height");
        params[6]->setType("adsr_envelope_release");

		GPNode* signal;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signal)) return NULL;

        return new ADSREnvelopeNode(params[0], params[1], params[2], params[3], params[4], params[5], params[6], signal);
    }
    // constant nodes
    else if (type.compare("pi") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Pi Node" << std::endl;
            return NULL;
        }

        return new ConstantNode(true, NULL);
    }
    else if (type.compare("const") == 0) {
        if (params.size() != 1) {
            std::cerr << "Incorrect number of mutatable params for a Constant Node" << std::endl;
            return NULL;
        }
        params[0]->setType("constant_value");

        return new ConstantNode(false, params[0]);
    }
    else if (type.compare("gain") == 0) {
        if (params.size() != 1) {
            std::cerr << "Incorrect number of mutatable params for a Gain Node" << std::endl;
            return NULL;
        }
        params[0]->setType("gain_value");

		GPNode* signal;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signal)) return NULL;

        return new GainNode(params[0], signal);
    }
    // function nodes
    else if (type.compare("+") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for an Add Node" << std::endl;
            return NULL;
        }

		GPNode* signalzero;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalzero)) return NULL;
        
		GPNode* signalone;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalone)) return NULL;

        return new AddNode(signalzero, signalone);
    }
    else if (type.compare("-") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Subtract Node" << std::endl;
            return NULL;
        }

		GPNode* signalzero;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalzero)) return NULL;
        
		GPNode* signalone;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalone)) return NULL;

        return new SubtractNode(signalzero, signalone);
    }
    else if (type.compare("*") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Multiply Node" << std::endl;
            return NULL;
        }

		GPNode* signalzero;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalzero)) return NULL;
        
		GPNode* signalone;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalone)) return NULL;

        return new MultiplyNode(signalzero, signalone);
    }
    else if (type.compare("sin") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Sine Node" << std::endl;
            return NULL;
        }

		GPNode* signalzero;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalzero)) return NULL;
       
        return new SineNode(signalzero);
    }
    else if (type.compare("cos") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Cosine Node" << std::endl;
            return NULL;
        }

		GPNode* signalzero;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalzero)) return NULL;
       
        return new CosineNode(signalzero);
    }
    // LFO nodes
    else if (type.compare("lfo") == 0) {
        if (params.size() != 1) {
            std::cerr << "Incorrect number of mutatable params for an LFO Terminal Node" << std::endl;
            return NULL;
        }
        params[0]->setType("lfo_terminal_rate");

        return new LFOTerminalNode(params[0]);
    }
    else if (type.compare("lfo*") == 0) {
        if (params.size() != 1) {
            std::cerr << "Incorrect number of mutatable params for an LFO Envelope Node" << std::endl;
            return NULL;
        }
        params[0]->setType("lfo_envelope_rate");

		GPNode* signal;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signal)) return NULL;

        return new LFOEnvelopeNode(params[0], signal);
    }
    // mixer nodes
    else if (type.compare("switch") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Switch Node" << std::endl;
            return NULL;
        }

		GPNode* control;
		if (!parseChild(tokenizerArgs, subtreeArgs, &control)) return NULL;

		GPNode* signalzero;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalzero)) return NULL;
        
		GPNode* signalone;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalone)) return NULL;

        return new SwitchNode(control, signalzero, signalone);
    }
    else if (type.compare("mix") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Mixer Node" << std::endl;
            return NULL;
        }

		GPNode* control;
		if (!parseChild(tokenizerArgs, subtreeArgs, &control)) return NULL;

		GPNode* signalzero;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalzero)) return NULL;
        
		GPNode* signalone;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signalone)) return NULL;

        return new MixerNode(control, signalzero, signalone);
    }
    // noise node
    else if (type.compare("whitenoise") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Noise Node" << std::endl;
            return NULL;
        }

        return new NoiseNode(rng);
    }
    // modulation nodes
    else if (type.compare("am") == 0) {
        if (params.size() != 4) {
            std::cerr << "Incorrect number of mutatable params for an AM Node" << std::endl;
            return NULL;
        }
        params[0]->setType("am_var_num");
        params[0]->setUnmutatable();
        params[1]->setType("am_partial");
        params[2]->setType("am_offset");
        params[3]->setType("am_alpha");

		GPNode* signal;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signal)) return NULL;

        return new AMNode(params[0], params[1], params[2], params[3], signal);
    }
    else if (type.compare("pm") == 0) {
        if (params.size() != 3) {
            std::cerr << "Incorrect number of mutatable params for a PM Node" << std::endl;
            return NULL;
        }
        params[0]->setType("pm_var_num");
        params[0]->setUnmutatable();
        params[1]->setType("pm_partial");
        params[2]->setType("pm_index");

		GPNode* signal;
		if (!parseChild(tokenizerArgs, subtreeArgs, &signal)) return NULL;

        return new PMNode(params[0], params[1], params[2], signal);
    }
    // silence node
    else if (type.compare("silence") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Silence Node" << std::endl;
            return NULL;
        }

        return new SilenceNode();
    }
    // spline node (already created)
    else if (type.compare("spline") == 0) {
        // if this is a primitive spline that we want to randomize
        if (params.size() % 2 == 0) {
            // make sure we have the right number of arguments
            if (params.size() != 4) {
                std::cerr << "Incorrect number of mutatable params for Spline Node primitive" << std::endl;
                return NULL;
            }
            params[0]->setType("spline_type");
            params[0]->setUnmutatable();
            params[1]->setType("spline_num_points");
            params[1]->setUnmutatable();
            params[2]->setType("spline_amp_range");
            params[3]->setType("spline_segment_length_range");

            std::vector<GPMutatableParam*> splinepoints;
            splinepoints.push_back(params[2]);
            splinepoints.push_back(params[3]);

            return new SplineTerminalNode(params[0], params[1], splinepoints);
        }
        // else interpret this is a previously instantiated spline
        else {
            // make sure we at least have a type, numPoints and final length
            if (params.size() < 3) {
                std::cerr << "Incorrect number of mutatable params for Spline Node" << std::endl;
                return NULL;
            }
            params[0]->setType("spline_type");
            params[0]->setUnmutatable();
            params[1]->setType("spline_num_points");
            params[1]->setUnmutatable();

            std::vector<GPMutatableParam*> splinepoints;
            unsigned currentParam = 2;
            while (currentParam + 2 < params.size()) {
                params[currentParam]->setType("spline_amp");
                splinepoints.push_back(params[currentParam]);
                currentParam++;
                params[currentParam]->setType("spline_segment_length");
                currentParam++;
                splinepoints.push_back(params[currentParam]);
            }
            params[currentParam]->setType("spline_amp_final");
            splinepoints.push_back(params[currentParam]);

            return new SplineTerminalNode(params[0], params[1], splinepoints);
        }
    }
    else if (type.compare("spline*") == 0) {
        // if this is a primitive spline that we want to randomize
        if (params.size() % 2 == 0) {
            // make sure we have the right number of arguments
            if (params.size() != 4) {
                std::cerr << "Incorrect number of mutatable params for Spline Node primitive" << std::endl;
                return NULL;
            }
            params[0]->setType("spline_type");
            params[0]->setUnmutatable();
            params[1]->setType("spline_num_points");
            params[1]->setUnmutatable();
            params[2]->setType("spline_amp_range");
            params[3]->setType("spline_segment_length_range");

            std::vector<GPMutatableParam*> splinepoints;
            splinepoints.push_back(params[2]);
            splinepoints.push_back(params[3]);

            GPNode* signal;
            if (!parseChild(tokenizerArgs, subtreeArgs, &signal)) return NULL;

            return new SplineEnvelopeNode(rng, true, params[0], params[1], splinepoints, signal);
        }
        // else interpret this is a previously instantiated spline
        else {
            // make sure we at least have a type, numPoints and final length
            if (params.size() < 3) {
                std::cerr << "Incorrect number of mutatable params for Spline Node" << std::endl;
                return NULL;
            }
            params[0]->setType("spline_type");
            params[0]->setUnmutatable();
            params[1]->setType("spline_num_points");
            params[1]->setUnmutatable();

            std::vector<GPMutatableParam*> splinepoints;
            unsigned currentParam = 2;
            while (currentParam + 2 < params.size()) {
                params[currentParam]->setType("spline_amp");
                splinepoints.push_back(params[currentParam]);
                currentParam++;
                params[currentParam]->setType("spline_segment_length");
                currentParam++;
                splinepoints.push_back(params[currentParam]);
            }
            params[currentParam]->setType("spline_amp_final");
            splinepoints.push_back(params[currentParam]);

            GPNode* signal;
            if (!parseChild(tokenizerArgs, subtreeArgs, &signal)) return NULL;

            return new SplineEnvelopeNode(rng, false, params[0], params[1], splinepoints, signal);
        }
    }
    // time node
    else if (type.compare("time") == 0) {
        if (params.size() != 0) {
            std::cerr << "Incorrect number of mutatable params for a Time Node" << std::endl;
            return NULL;
        }

        return new TimeNode();
    }
    // variable node
    else if (type.compare("var") == 0) {
        if (params.size() != 2) {
            std::cerr << "Incorrect number of mutatable params for a Variable Node" << std::endl;
            return NULL;
        }
        params[0]->setType("var_num");
        params[0]->setUnmutatable();
        params[1]->setType("var_range");

        return new VariableNode(params[0], params[1]);
    }
    // wave table oscillator nodes
    else if (type.compare("sinosc") == 0) {
        if (params.size() != 3) {
            std::cerr << "Incorrect number of mutatable params for a Sin Osc Node" << std::endl;
            return NULL;
        }
        params[0]->setType("sinosc_var_num");
        params[0]->setUnmutatable();
        params[1]->setType("sinosc_partial");
        params[2]->setType("sinosc_phase");

        return new SinOscNode(params[0], params[1], params[2]);
    }
    else if (type.compare("sawosc") == 0) {
        if (params.size() != 3) {
            std::cerr << "Incorrect number of mutatable params for a Saw Osc Node" << std::endl;
            return NULL;
        }
        params[0]->setType("sawosc_var_num");
        params[0]->setUnmutatable();
        params[1]->setType("sawosc_partial");
        params[2]->setType("sawosc_phase");

        return new SawOscNode(params[0], params[1], params[2]);
    }
    else if (type.compare("squareosc") == 0) {
        if (params.size() != 3) {
            std::cerr << "Incorrect number of mutatable params for a Square Osc Node" << std::endl;
            return NULL;
        }
        params[0]->setType("squareosc_var_num");
        params[0]->setUnmutatable();
        params[1]->setType("squareosc_partial");
        params[2]->setType("squareosc_phase");

        return new SquareOscNode(params[0], params[1], params[2]);
    }
    else if (type.compare("triangleosc") == 0) {
        if (params.size() != 3) {
            std::cerr << "Incorrect number of mutatable params for a Triangle Osc Node" << std::endl;
            return NULL;
        }
        params[0]->setType("triangleosc_var_num");
        params[0]->setUnmutatable();
        params[1]->setType("triangleosc_partial");
        params[2]->setType("triangleosc_phase");

        return new TriangleOscNode(params[0], params[1], params[2]);
    }
	// explicitly null node for primitives
	else if (type.compare("null") == 0) {
		return NULL;
	}
	// otherwise s-expression formatting is wrong
    else {
        std::cerr << "Incorrect node type string: " << type << std::endl;
        return NULL;
    }
}

/*
    ===========
    EASY ACCESS
    ===========
*/

GPNode* createNode(std::string nodestring, GPRandom* rng) {
	std::vector<std::string> tokens = split(nodestring, " }{)(");
    unsigned index = 0;
	std::string error;
	return createNode(tokens, &index, rng, error);
}

GPMutatableParam* createMutatableParam(std::string paramstring, std::string type, bool ismutatable) {
	std::vector<std::string> tokens = split(paramstring, " }{)(");
    unsigned index = 0;
	return createMutatableParam(tokens, &index, type, ismutatable);
}
