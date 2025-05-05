#include "component/necessary_headers.h"
#include "component/signal_data.h"
#include "component/matplotlibcpp.h"
#include "component/harmonic/harmonic_utils.h"

namespace plt = matplotlibcpp;
namespace SignalParams {
    constexpr int NUM_POINTS = 10000;
    constexpr double TIME_STEP = 1e-4;
    constexpr double TARGET_THD = 5.0;  // Target Total Harmonic Distortion (%)
    constexpr int FIGURE_WIDTH = 1200;
    constexpr int FIGURE_HEIGHT = 600;
}

class SignalAnalyzer {
public:
    SignalAnalyzer(const std::string& filename)
        : data(HarmonicUtils::importData(filename)),
        time(SignalParams::NUM_POINTS),
        signal(SignalParams::NUM_POINTS) {
        initialize_vectors();
    }

    void process_signal() {
        auto [frequencies, amps, recon] = HarmonicUtils::processSignal(
            signal, SignalParams::TIME_STEP, SignalParams::TARGET_THD
        );

        reconstructed = std::move(recon);
        calculate_distortion_metrics();
        prepare_frequency_components(frequencies, amps);
    }

    void visualize_results() {
        plot_time_domain();
        plot_frequency_domain();
    }

private:
    SignalData data;
    std::vector<double> time;
    std::vector<double> signal;
    std::vector<double> reconstructed;
    std::vector<double> frequency_components;
    std::vector<double> amplitude_components;
    double thd = 0.0;  // Total Harmonic Distortion

    void initialize_vectors() {
        std::copy(data.getTimeVector().begin(),
            data.getTimeVector().end(),
            time.begin());
        std::copy(data.getAmplitudeVector().begin(),
            data.getAmplitudeVector().end(),
            signal.begin());
    }

    void calculate_distortion_metrics() {
        double mse = 0.0, power = 0.0;

#pragma omp parallel for reduction(+:mse, power)
        for (int i = 0; i < SignalParams::NUM_POINTS; ++i) {
            double error = signal[i] - reconstructed[i];
            mse += error * error;
            power += signal[i] * signal[i];
        }

        thd = std::sqrt(mse / power) * 100.0;
    }

    void prepare_frequency_components(
        const std::vector<double>& freqs,
        const std::vector<double>& amps)
    {
        size_t half_size = SignalParams::NUM_POINTS / 2;
        frequency_components.assign(freqs.begin(), freqs.begin() + half_size);
        amplitude_components.assign(amps.begin(), amps.begin() + half_size);

        // Adjust amplitudes for one-sided spectrum
        std::transform(amplitude_components.begin(), amplitude_components.end(),
            amplitude_components.begin(),
            [](double a) { return 2.0 * a; });
    }

    void plot_time_domain() {
        plt::figure_size(SignalParams::FIGURE_WIDTH, SignalParams::FIGURE_HEIGHT);

        plt::plot(time, signal, {
            {"label", "Original Signal"},
            {"color", "blue"},
            {"alpha", "0.7"},
            {"linewidth", "1.5"}
            });

        plt::plot(time, reconstructed, {
            {"label", "Reconstructed"},
            {"color", "red"},
            {"linestyle", "--"},
            {"linewidth", "1.2"}
            });

        plt::title("Signal Comparison (THD: " + std::to_string(thd) + "%)");
        plt::xlabel("Time (s)");
        plt::ylabel("Amplitude");
        plt::legend({
            {"loc", "upper right"},
            {"fontsize", "10"}
            });
        plt::grid({
            {"linestyle", "--"},
            {"alpha", "0.5"}
            });

        plt::show();
    }

    void plot_frequency_domain() {
        plt::figure_size(SignalParams::FIGURE_WIDTH, SignalParams::FIGURE_HEIGHT);

        plt::stem(frequency_components, amplitude_components, {
            {"basefmt", " "},
            {"linefmt", "b-"},
            {"markerfmt", "bo"},
            {"label", "Frequency Components"}
            });

        plt::title("Amplitude Spectrum");
        plt::xlabel("Frequency (Hz)");
        plt::ylabel("Amplitude");
        plt::legend();
        plt::grid({
            {"linestyle", ":"},
            {"alpha", "0.3"}
            });

        plt::show();
    }
};

int main() {
    try {
        SignalAnalyzer analyzer("merged_signal_data.txt");
        analyzer.process_signal();
        analyzer.visualize_results();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        std::cerr << "Analysis failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}