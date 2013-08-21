#include "../../JuceLibraryCode/JuceHeader.h"
#include "boost/program_options.hpp"
#include "../Common/GPLogger.h"
#include "../Common/GPRandom.h"
#include "../Synth/GPSynth.h"
#include "GPExperiment.h"

int main( int argc, const char* argv[] )
{
    namespace po = boost::program_options;

    // command line container variables
    std::string logger_cfg_file_path;
    std::string me_cfg_file_path;
    std::string synth_cfg_file_path;
    std::string primitives_file_path;
    std::string target_file_path;
    std::string output_dir_path;
    unsigned seed;
    std::vector<float> constants;

    // parse command line
    po::options_description cl_desc("Allowed options");
    cl_desc.add_options()
        ("help", "display help message")
        ("sys_info", "print info about machine")
        ("sanity", "run a sanity test of GP library")
        ("logger_cfg", po::value<std::string>(&logger_cfg_file_path), "logger config file")
        ("synth_cfg", po::value<std::string>(&synth_cfg_file_path), "synth config file")
        ("primitives", po::value<std::string>(&primitives_file_path), "list of primitives available to experiment")
        ("me_cfg", po::value<std::string>(&me_cfg_file_path), "matching experiment config file")
        ("target", po::value<std::string>(&target_file_path), "target wav file")
        ("output_dir", po::value<std::string>(&output_dir_path)->default_value("./"), "output file directory")
        ("seed", po::value<unsigned>(&seed)->default_value(time(NULL)), "experiment seed number")
        ("constants", po::value<std::vector<float> >(&constants), "special constant values that synthesis algorithms can use")
    ;
    po::variables_map cl_vm;
    po::store(po::parse_command_line(argc, argv, cl_desc), cl_vm);
    po::notify(cl_vm);

    // print help
    if (cl_vm.count("help")) {
        std::cout << cl_desc << std::endl;
        return 1;
    }

    // declare logger desc
    GPLoggerParams* logger_params = (GPLoggerParams*) malloc(sizeof(GPLoggerParams));
    po::options_description logger_desc("");
    logger_desc.add_options()
        ("save_precision", po::value<unsigned>(&(logger_params->save_precision))->default_value(20), "precision for saving/backing up synthesis algorithms")
        ("print_precision", po::value<unsigned>(&(logger_params->print_precision))->default_value(3), "precision for text-logging synthesis algorithms")

        ("log_to_file", po::value<bool>(&(logger_params->log_to_file))->default_value(false), "")
        ("log_to_cout", po::value<bool>(&(logger_params->log_to_cout))->default_value(true), "")
        ("log_to_cerr", po::value<bool>(&(logger_params->log_to_cerr))->default_value(false), "")
        ("log_verbose_to_file", po::value<bool>(&(logger_params->log_verbose_to_file))->default_value(false), "")
        ("log_verbose_to_cout", po::value<bool>(&(logger_params->log_verbose_to_cout))->default_value(false), "")
        ("log_verbose_to_cerr", po::value<bool>(&(logger_params->log_verbose_to_cerr))->default_value(false), "")
        ("debug_to_file", po::value<bool>(&(logger_params->debug_to_file))->default_value(false), "")
        ("debug_to_cout", po::value<bool>(&(logger_params->debug_to_cout))->default_value(false), "")
        ("debug_to_cerr", po::value<bool>(&(logger_params->debug_to_cerr))->default_value(true), "")
    ;

    // parse logger config file
    std::ifstream logger_cfg_file;
    logger_cfg_file.open(logger_cfg_file_path.c_str());
    po::variables_map logger_vm;
    po::store(po::parse_config_file(logger_cfg_file, logger_desc, true), logger_vm);
    po::notify(logger_vm);
    logger_cfg_file.close();

    // create logger
    GPLogger* logger = new GPLogger(logger_params, seed, output_dir_path);

    // print system info if requested
    if (cl_vm.count("sys_info")) {
        logger->log(logger->get_system_info());
    }

    // declare synth desc
    GPSynthParams* synth_params = (GPSynthParams*) malloc(sizeof(GPSynthParams));
    po::options_description synth_desc("");
    synth_desc.add_options()
        ("backup_all_networks", po::value<bool>(&(synth_params->backup_all_networks))->default_value(true), "save rendered audio of the champion from each generation")
        ("backup_precision", po::value<unsigned>(&(synth_params->backup_precision))->default_value(20), "precision for saving/backing up synthesis algorithms")

        ("population_size", po::value<unsigned>(&(synth_params->population_size))->default_value(10), "precision for saving/backing up synthesis algorithms")
        ("max_initial_height", po::value<unsigned>(&(synth_params->max_initial_height))->default_value(5), "precision for saving/backing up synthesis algorithms")
        ("max_height", po::value<unsigned>(&(synth_params->max_height))->default_value(10), "precision for saving/backing up synthesis algorithms")
        ("erc", po::value<bool>(&(synth_params->erc))->default_value(true), "save rendered audio of the champion from each generation")

        ("best_possible_fitness", po::value<double>(&(synth_params->best_possible_fitness))->default_value(0.0f), "fitness to reach to terminate evolution")
        ("lower_fitness_is_better", po::value<bool>(&(synth_params->lower_fitness_is_better))->default_value(true), "save rendered audio of the champion from each generation")

        ("nm_proportion", po::value<double>(&(synth_params->nm_proportion))->default_value(1.0f), "fitness to reach to terminate evolution")
        ("nm_temperature", po::value<double>(&(synth_params->nm_temperature))->default_value(0.9f), "fitness to reach to terminate evolution")
        ("nm_selection_type", po::value<unsigned>(&(synth_params->nm_selection_type))->default_value(0), "which fitness function to use")
        ("nm_selection_percentile", po::value<double>(&(synth_params->nm_selection_percentile))->default_value(0.0f), "fitness to reach to terminate evolution")

        ("mu_proportion", po::value<double>(&(synth_params->mu_proportion))->default_value(1.0f), "fitness to reach to terminate evolution")
        ("mu_type", po::value<unsigned>(&(synth_params->mu_type))->default_value(0), "which fitness function to use")
        ("mu_selection_type", po::value<unsigned>(&(synth_params->mu_selection_type))->default_value(0), "which fitness function to use")
        ("mu_selection_percentile", po::value<double>(&(synth_params->mu_selection_percentile))->default_value(0.0f), "fitness to reach to terminate evolution")

        ("x_proportion", po::value<double>(&(synth_params->x_proportion))->default_value(1.0f), "fitness to reach to terminate evolution")
        ("x_type", po::value<unsigned>(&(synth_params->x_type))->default_value(0), "which fitness function to use")
        ("x_selection_type", po::value<unsigned>(&(synth_params->x_selection_type))->default_value(0), "which fitness function to use")
        ("x_selection_percentile", po::value<double>(&(synth_params->x_selection_percentile))->default_value(0.0f), "fitness to reach to terminate evolution")

        ("re_proportion", po::value<double>(&(synth_params->re_proportion))->default_value(1.0f), "fitness to reach to terminate evolution")
        ("re_selection_type", po::value<unsigned>(&(synth_params->re_selection_type))->default_value(0), "which fitness function to use")
        ("re_selection_percentile", po::value<double>(&(synth_params->re_selection_percentile))->default_value(0.0f), "fitness to reach to terminate evolution")
    ;

    // parse synth config file
    std::ifstream synth_cfg_file;
    synth_cfg_file.open(synth_cfg_file_path.c_str());
    po::variables_map synth_vm;
    po::store(po::parse_config_file(synth_cfg_file, synth_desc, true), synth_vm);
    po::notify(synth_vm);
    synth_cfg_file.close();

    // create random instance
    GPRandom* rng = new GPRandom(seed);

    // parse primitives file
    std::ifstream primitives_file;
    primitives_file.open(primitives_file_path.c_str());
    std::string primitive;
    std::vector<GPNode*>* primitives = new std::vector<GPNode*>();
    while (std::getline(primitives_file, primitive)) {
        primitives->push_back(createNode(primitive, rng));
    }
    primitives_file.close();

    // create synth
    GPSynth* synth = new GPSynth(synth_params, rng, primitives);

    // check if this is a sanity test
    bool sanity = false;
    if (cl_vm.count("sanity")) {
        sanity = true;
    }

    // declare matching experiments desc
    GPMatchingExperimentParams* me_params = (GPMatchingExperimentParams*) malloc(sizeof(GPMatchingExperimentParams));
    std::string window_type;
    po::options_description me_desc("");
    me_desc.add_options()
        ("log_save_gen_champ_audio", po::value<bool>(&(me_params->log_save_gen_champ_audio))->default_value(false), "save rendered audio of the champion from each generation")
        ("log_save_overall_champ_audio", po::value<bool>(&(me_params->log_save_overall_champ_audio))->default_value(false), "save rendered audio of the champion from the entire run")
        ("log_save_target_spectrum", po::value<bool>(&(me_params->log_save_target_spectrum))->default_value(false), "save spectrum of target wav file")
        ("log_save_target_copy", po::value<bool>(&(me_params->log_save_target_copy))->default_value(false), "save copy of target wav file")

        ("aux_wav_file_buffer_size", po::value<unsigned>(&(me_params->aux_wav_file_buffer_size))->default_value(512), "buffer size for wav file IO")
        ("aux_render_block_size", po::value<unsigned>(&(me_params->aux_render_block_size))->default_value(512), "chunk size for rendering synthesis algorithms")
 
        ("exp_suboptimize_type", po::value<unsigned>(&(me_params->exp_suboptimize_type))->default_value(0), "which type of suboptimization to perform")
        ("exp_generations", po::value<unsigned>(&(me_params->exp_generations))->default_value(10), "maximum number of generations to evaluate")
        ("exp_threshold", po::value<double>(&(me_params->exp_threshold))->default_value(0.0f), "fitness to reach to terminate evolution")

        ("ff_type", po::value<unsigned>(&(me_params->ff_type))->default_value(1), "which fitness function to use")
        ("ff_spectrum_mag_weight", po::value<double>(&(me_params->ff_spectrum_mag_weight))->default_value(1.0f), "weight multiplier for spectrum magnitudes in fitness function")
        ("ff_spectrum_phase_weight", po::value<double>(&(me_params->ff_spectrum_phase_weight))->default_value(1.0f), "weight multiplier for spectrum phases in fitness function")
        
        ("ff_fft_window", po::value<std::string>(&window_type)->default_value("rect"), "fft window type")
        ("ff_fft_size", po::value<unsigned>(&(me_params->ff_fft_size))->default_value(1024), "fft size")
        ("ff_fft_overlap", po::value<unsigned>(&(me_params->ff_fft_overlap))->default_value(0), "fft overlap")

        ("ff_moving_average_type", po::value<unsigned>(&(me_params->ff_moving_average_type))->default_value(0), "type of moving average to use for fitness function comparison")
        ("ff_moving_average_past_radius", po::value<unsigned>(&(me_params->ff_moving_average_past_radius))->default_value(40), "# of feedback samples for moving average filter")
        ("ff_moving_average_future_radius", po::value<unsigned>(&(me_params->ff_moving_average_future_radius))->default_value(40), "# of feedforward samples for moving average filter")
        ("ff_moving_average_exponential_alpha", po::value<double>(&(me_params->ff_moving_average_exponential_alpha))->default_value(0.95), "importance of successive samples in the moving average")
        ("ff_weight_frames", po::value<bool>(&(me_params->ff_weight_frames))->default_value(false), "weight importance of target frames in comparison")
        ("ff_weight_frames_exponent", po::value<double>(&(me_params->ff_weight_frames_exponent))->default_value(1.0f), "exponent for target frames weighting")
        ("ff_phase_comparison_exponent", po::value<double>(&(me_params->ff_phase_comparison_exponent))->default_value(1.0f), "exponent for penalizing bad phase")
        ("ff_mag_base_comparison", po::value<double>(&(me_params->ff_mag_base_comparison))->default_value(1.0f), "penalty for comparing magnitudes of bins in each frame")
        ("ff_mag_good_comparison", po::value<double>(&(me_params->ff_mag_good_comparison))->default_value(0.0f), "penalty bonus for comparing bins which are on the correct side of the moving average")
        ("ff_mag_bad_comparison", po::value<double>(&(me_params->ff_mag_bad_comparison))->default_value(0.0f), "penalty bonus for comparing bins which are on the incorrect side of the moving average")
    ;

    // parse me config file
    std::ifstream me_cfg_file;
    me_cfg_file.open(me_cfg_file_path.c_str());
    po::variables_map me_vm;
    po::store(po::parse_config_file(me_cfg_file, me_desc, true), me_vm);
    po::notify(me_vm);
    me_cfg_file.close();

    if (me_vm.count("ff_fft_window")) {
        strncpy(me_params->ff_fft_window, window_type.c_str(), 5);
    }

    // create experiment
    GPExperiment* experiment = new GPExperiment(sanity, synth, me_params, seed, target_file_path, output_dir_path, constants);

    // run experiment
    GPNetwork* champion = experiment->evolve();

    return 1;
}
