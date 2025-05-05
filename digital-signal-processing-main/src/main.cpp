#include "component/necessary_headers.h"
#include "component/signal_data.h"
#include "component/harmonic/harmonic_utils.h"
#include "component/harmonic/harmonic_params.h"

namespace SignalConstants {
    constexpr int DEFAULT_POINT_COUNT = 10000;
    constexpr double TIME_INCREMENT = 1e-4;
    constexpr char OUTPUT_FILENAME[] = "merged_signal_data.txt";
}

class HarmonicSignalGenerator {
public:
    HarmonicSignalGenerator() 
        : point_count(SignalConstants::DEFAULT_POINT_COUNT),
          time_step(SignalConstants::TIME_INCREMENT) {
        initialize_containers();
    }

    void generate_composite_signal() {
        for (const auto& [amplitude, frequency, phase] : HarmonicParams::harmonicParams) {
            add_harmonic_component(amplitude, frequency, phase);
        }
        prepare_time_axis();
    }

    void export_results() const {
        HarmonicUtils::exportData(signal_data, SignalConstants::OUTPUT_FILENAME);
    }

private:
    int point_count;
    double time_step;
    SignalData signal_data;
    std::vector<double> composite_signal;
    std::vector<double> time_values;

    void initialize_containers() {
        composite_signal.resize(point_count, 0.0);
        time_values.resize(point_count);
    }

    void add_harmonic_component(double amplitude, double frequency, double phase) {
        auto component = HarmonicUtils::generateHarmonic(
            point_count, amplitude, frequency, phase, time_step);

        #pragma omp parallel for
        for (int i = 0; i < point_count; ++i) {
            composite_signal[i] += component[i];
        }
    }

    void prepare_time_axis() {
        #pragma omp parallel for
        for (int i = 0; i < point_count; ++i) {
            time_values[i] = i * time_step;
            signal_data.addData(time_values[i], composite_signal[i]);
        }
    }
};

int main() {
    try {
        HarmonicSignalGenerator generator;
        generator.generate_composite_signal();
        generator.export_results();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Signal generation failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}#include "component/necessary_headers.h"
#include "component/signal_data.h"
#include "component/harmonic/harmonic_utils.h"
#include "component/harmonic/harmonic_params.h"

namespace SignalConstants {
    constexpr int DEFAULT_POINT_COUNT = 10000;
    constexpr double TIME_INCREMENT = 1e-4;
    constexpr char OUTPUT_FILENAME[] = "merged_signal_data.txt";
}

class HarmonicSignalGenerator {
public:
    HarmonicSignalGenerator()
        : point_count(SignalConstants::DEFAULT_POINT_COUNT),
        time_step(SignalConstants::TIME_INCREMENT) {
        initialize_containers();
    }

    void generate_composite_signal() {
        for (const auto& [amplitude, frequency, phase] : HarmonicParams::harmonicParams) {
            add_harmonic_component(amplitude, frequency, phase);
        }
        prepare_time_axis();
    }

    void export_results() const {
        HarmonicUtils::exportData(signal_data, SignalConstants::OUTPUT_FILENAME);
    }

private:
    int point_count;
    double time_step;
    SignalData signal_data;
    std::vector<double> composite_signal;
    std::vector<double> time_values;

    void initialize_containers() {
        composite_signal.resize(point_count, 0.0);
        time_values.resize(point_count);
    }

    void add_harmonic_component(double amplitude, double frequency, double phase) {
        auto component = HarmonicUtils::generateHarmonic(
            point_count, amplitude, frequency, phase, time_step);

#pragma omp parallel for
        for (int i = 0; i < point_count; ++i) {
            composite_signal[i] += component[i];
        }
    }

    void prepare_time_axis() {
#pragma omp parallel for
        for (int i = 0; i < point_count; ++i) {
            time_values[i] = i * time_step;
            signal_data.addData(time_values[i], composite_signal[i]);
        }
    }
};

int main() {
    try {
        HarmonicSignalGenerator generator;
        generator.generate_composite_signal();
        generator.export_results();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        std::cerr << "Signal generation failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}