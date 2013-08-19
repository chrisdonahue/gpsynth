#ifndef GPSYNTHPARAMS_H
#define GPSYNTHPARAMS_H

struct GPSynthParams {
    // synth evolution parameters
    unsigned population_size;
    bool backup_all_networks;
    unsigned max_initial_height;
    unsigned max_height;
    bool erc;

    // synth genetic parameters
    // numeric mutation
    double nm_proportion;
    double nm_temperature;
    unsigned nm_selection_type;
    double nm_selection_percentile;
    // mutation
    double mu_proportion;
    unsigned mu_type;
    unsigned mu_selection_type;
    double mu_selection_percentile;
    // crossover
    double x_proportion;
    unsigned x_type;
    unsigned x_selection_type;
    double x_selection_percentile;
    // reproduction
    double re_proportion;
    unsigned re_selection_type;
    double re_selection_percentile;
};

#endif
