#include "../../JuceLibraryCode/JuceHeader.h"
#include "boost/program_options.hpp"
#include "../Common/GPLogger.h"
#include "../Common/GPRandom.h"
#include "../Common/GPParser.h"
#include "../Synth/GPSynth.h"
#include "GPExperiment.h"

int main( int argc, const char* argv[] )
{
    namespace po = boost::program_options;

    // command line container variables
    std::string logger_cfg_file_path;
    std::string synth_cfg_file_path;
    std::string primitives_file_path;
    std::string comparator_cfg_file_path;
    std::string me_cfg_file_path;
    std::string beagle_cfg_file_path;
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
        ("comparator_cfg", po::value<std::string>(&comparator_cfg_file_path), "comparator config file")
        ("beagle_cfg", po::value<std::string>(&beagle_cfg_file_path), "beagle suboptimization config file")
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

    // create output directory
    juce::File output_dir(output_dir_path);
    if (!output_dir.isDirectory()) {
        if (!output_dir.createDirectory().wasOk()) {
            std::cout << "Could not create or open the output directory path" << std::endl;
            return 1;
        }
    }

    // create random instance
    GPRandom* rng = new GPRandom(seed);

    // declare logger desc
    GPLoggerParams* logger_params = (GPLoggerParams*) malloc(sizeof(GPLoggerParams));
    logger_params->log_params = (GPLogParams*) malloc(sizeof(GPLogParams));
    logger_params->verbose_params = (GPLogParams*) malloc(sizeof(GPLogParams));
    logger_params->debug_params = (GPLogParams*) malloc(sizeof(GPLogParams));
    logger_params->error_params = (GPLogParams*) malloc(sizeof(GPLogParams));
    po::options_description logger_desc("");
    logger_desc.add_options()
        ("log_to_file", po::value<bool>(&(logger_params->log_params->to_file))->default_value(false), "")
        ("log_to_cout", po::value<bool>(&(logger_params->log_params->to_cout))->default_value(false), "")
        ("log_to_cerr", po::value<bool>(&(logger_params->log_params->to_cerr))->default_value(false), "")
        ("verbose_to_file", po::value<bool>(&(logger_params->verbose_params->to_file))->default_value(false), "")
        ("verbose_to_log", po::value<bool>(&(logger_params->verbose_to_log))->default_value(false), "")
        ("debug_to_file", po::value<bool>(&(logger_params->debug_params->to_file))->default_value(false), "")
        ("debug_to_cout", po::value<bool>(&(logger_params->debug_params->to_cout))->default_value(false), "")
        ("debug_to_cerr", po::value<bool>(&(logger_params->debug_params->to_cerr))->default_value(false), "")
        ("error_to_file", po::value<bool>(&(logger_params->error_params->to_file))->default_value(false), "")
        ("error_to_cout", po::value<bool>(&(logger_params->error_params->to_cout))->default_value(false), "")
        ("error_to_cerr", po::value<bool>(&(logger_params->error_params->to_cerr))->default_value(false), "")

        ("log_precision", po::value<unsigned>(&(logger_params->log_precision))->default_value(20), "precision for printing floats in the log")
        ("save_net_precision", po::value<unsigned>(&(logger_params->save_net_precision))->default_value(20), "precision for saving/backing up synthesis algorithms")
        ("print_net_precision", po::value<unsigned>(&(logger_params->print_net_precision))->default_value(3), "precision for text-logging synthesis algorithms")
    ;

    // parse logger config file
    std::ifstream logger_cfg_file;
    logger_cfg_file.open(logger_cfg_file_path.c_str());
    po::variables_map logger_vm;
    po::store(po::parse_config_file(logger_cfg_file, logger_desc, true), logger_vm);
    po::notify(logger_vm);
    logger_cfg_file.close();

    // create logger
    std::stringstream ss;
    ss << seed;
    logger_params->verbose_params->to_cout = false;
    logger_params->verbose_params->to_cerr = false;
    GPLogger* logger = new GPLogger(logger_params, ss.str(), output_dir_path);

    // print system info if requested
    if (cl_vm.count("sys_info")) {
        logger->log << logger->get_system_info() << std::flush;
    }

    // check if this is a sanity test
    if (cl_vm.count("sanity")) {
        GPExperiment* sanity_test = new GPExperiment(logger);
        int ret = sanity_test->sanityTest(rng);
        free(logger_params->log_params);
        free(logger_params->verbose_params);
        free(logger_params->debug_params);
        free(logger_params->error_params);
        free(logger_params);
        delete logger;
        delete rng;
        return ret;
    }

    // declare synth desc
    GPSynthParams* synth_params = (GPSynthParams*) malloc(sizeof(GPSynthParams));
    po::options_description synth_desc("");
    synth_desc.add_options()
        ("population_size", po::value<unsigned>(&(synth_params->population_size))->default_value(10), "precision for saving/backing up synthesis algorithms")
        ("max_initial_height", po::value<unsigned>(&(synth_params->max_initial_height))->default_value(5), "precision for saving/backing up synthesis algorithms")
        ("max_height", po::value<unsigned>(&(synth_params->max_height))->default_value(10), "precision for saving/backing up synthesis algorithms")
        ("erc", po::value<bool>(&(synth_params->erc))->default_value(true), "save rendered audio of the champion from each generation")
        ("backup_all_networks", po::value<bool>(&(synth_params->backup_all_networks))->default_value(true), "save rendered audio of the champion from each generation")
        ("backup_precision", po::value<unsigned>(&(synth_params->backup_precision))->default_value(20), "precision for saving/backing up synthesis algorithms")

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

        ("new_proportion", po::value<double>(&(synth_params->new_proportion))->default_value(1.0f), "fitness to reach to terminate evolution")
        ("new_type", po::value<unsigned>(&(synth_params->new_type))->default_value(0), "which fitness function to use")
    ;

    // parse synth config file
    std::ifstream synth_cfg_file;
    synth_cfg_file.open(synth_cfg_file_path.c_str());
    po::variables_map synth_vm;
    po::store(po::parse_config_file(synth_cfg_file, synth_desc, true), synth_vm);
    po::notify(synth_vm);
    synth_cfg_file.close();

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
    GPSynth* synth = new GPSynth(logger, synth_params, rng, primitives);

    // declare comparator desc
    GPAudioComparatorParams* comparator_params = (GPAudioComparatorParams*) malloc(sizeof(GPAudioComparatorParams));
    std::string window_type;
    po::options_description comparator_desc("");
    comparator_desc.add_options()
        ("aux_wav_file_buffer_size", po::value<unsigned>(&(comparator_params->aux_wav_file_buffer_size))->default_value(512), "")

        ("fft_window", po::value<std::string>(&window_type)->default_value("rect"), "")
        ("fft_size", po::value<unsigned>(&comparator_params->fft_size)->default_value(1024), "")
        ("fft_overlap", po::value<unsigned>(&(comparator_params->fft_overlap))->default_value(0), "")

        ("moving_average_type", po::value<unsigned>(&(comparator_params->moving_average_type))->default_value(0), "")
        ("moving_average_past_radius", po::value<unsigned>(&(comparator_params->moving_average_past_radius))->default_value(0), "")
        ("moving_average_future_radius", po::value<unsigned>(&(comparator_params->moving_average_future_radius))->default_value(0), "")
        ("moving_average_exponential_alpha", po::value<double>(&(comparator_params->moving_average_exponential_alpha))->default_value(0.95f), "")

        ("amp_comparison_p", po::value<double>(&(comparator_params->amp_comparison_p))->default_value(1.0f), "")
        ("mag_weight_multiplier", po::value<double>(&(comparator_params->mag_weight_multiplier))->default_value(1.0f), "")
        ("mag_comparison_p", po::value<double>(&(comparator_params->mag_comparison_p))->default_value(0.9f), "")
        ("mag_good_comparison_additional_p", po::value<double>(&(comparator_params->mag_good_comparison_additional_p))->default_value(0.1f), "")
        ("mag_good_comparison_additional_p", po::value<double>(&(comparator_params->mag_good_comparison_additional_p))->default_value(1.1f), "")
        ("phase_weight_multiplier", po::value<double>(&(comparator_params->phase_weight_multiplier))->default_value(1.0f), "")
        ("phase_comparison_p", po::value<double>(&(comparator_params->phase_comparison_p))->default_value(2.0f), "")
    ;

    // parse comparator config file
    std::ifstream comparator_cfg_file;
    comparator_cfg_file.open(comparator_cfg_file_path.c_str());
    po::variables_map comparator_vm;
    po::store(po::parse_config_file(comparator_cfg_file, comparator_desc, true), comparator_vm);
    po::notify(comparator_vm);
    comparator_cfg_file.close();

    if (comparator_vm.count("fft_window")) {
        strncpy(comparator_params->fft_window, window_type.c_str(), 5);
    }

    // create comparator
    GPAudioComparator* comparator = new GPAudioComparator(comparator_params, target_file_path);

    // declare matching experiments desc
    GPMatchingExperimentParams* me_params = (GPMatchingExperimentParams*) malloc(sizeof(GPMatchingExperimentParams));
    po::options_description me_desc("");
    me_desc.add_options()
        ("log_save_gen_summary_file", po::value<bool>(&(me_params->log_save_gen_summary_file))->default_value(false), "save rendered audio of the champion from each generation")
        ("log_save_gen_champ_audio", po::value<bool>(&(me_params->log_save_gen_champ_audio))->default_value(false), "save rendered audio of the champion from each generation")
        ("log_save_overall_champ_audio", po::value<bool>(&(me_params->log_save_overall_champ_audio))->default_value(false), "save rendered audio of the champion from the entire run")
        ("log_save_target_spectrum", po::value<bool>(&(me_params->log_save_target_spectrum))->default_value(false), "save spectrum of target wav file")
        ("log_save_target_copy", po::value<bool>(&(me_params->log_save_target_copy))->default_value(false), "save copy of target wav file")

        ("aux_render_block_size", po::value<unsigned>(&(me_params->aux_render_block_size))->default_value(512), "chunk size for rendering synthesis algorithms")
 
        ("exp_ff_type", po::value<unsigned>(&(me_params->exp_ff_type))->default_value(0), "which type of suboptimization to perform")
        ("exp_suboptimize_ff_type", po::value<unsigned>(&(me_params->exp_suboptimize_ff_type))->default_value(0), "which type of suboptimization to perform")
        ("exp_suboptimize_type", po::value<unsigned>(&(me_params->exp_suboptimize_type))->default_value(0), "which type of suboptimization to perform")
        ("exp_generations", po::value<unsigned>(&(me_params->exp_generations))->default_value(10), "maximum number of generations to evaluate")
        ("exp_threshold", po::value<double>(&(me_params->exp_threshold))->default_value(0.0f), "fitness to reach to terminate evolution")
    ;

    // parse me config file
    std::ifstream me_cfg_file;
    me_cfg_file.open(me_cfg_file_path.c_str());
    po::variables_map me_vm;
    po::store(po::parse_config_file(me_cfg_file, me_desc, true), me_vm);
    po::notify(me_vm);
    me_cfg_file.close();

    // create experiment
    GPExperiment* experiment = new GPExperiment(logger, me_params, seed, beagle_cfg_file_path, synth, comparator, output_dir_path, constants);

    // run experiment
    GPNetwork* champion = experiment->evolve();

    // delete rng
    delete rng;

    // delete logger
    free(logger_params->log_params);
    free(logger_params->verbose_params);
    free(logger_params->debug_params);
    free(logger_params->error_params);
    free(logger_params);
    delete logger;

    // delete synth
    free(synth_params);
    delete synth;

    // delete experiment
    free(me_params);
    delete experiment;

    return 1;
}
