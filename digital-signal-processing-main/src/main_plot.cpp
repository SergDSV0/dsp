#include "component/necessary_headers.h"
#include "component/signal_data.h"
#include "component/harmonic/harmonic_utils.h"
#include "component/matplotlibcpp.h"

namespace plt = matplotlibcpp;
namespace PlotSettings {
    constexpr int FIGURE_WIDTH = 1200;
    constexpr int FIGURE_HEIGHT = 600;
    constexpr const char* PLOT_TITLE = "Signal Visualization";
    constexpr const char* X_LABEL = "Time, s";
    constexpr const char* Y_LABEL = "Amplitude";
    constexpr const char* INPUT_FILENAME = "merged_signal_data.txt";
}

class SignalVisualizer {
public:
    explicit SignalVisualizer(const std::string& filename)
        : signal_data(HarmonicUtils::importData(filename)) {
    }

    void configure_plot() {
        plt::figure_size(PlotSettings::FIGURE_WIDTH, PlotSettings::FIGURE_HEIGHT);

        plt::plot(
            signal_data.getTimeVector(),
            signal_data.getAmplitudeVector(),
            {
                {"label", "Composite Signal"},
                {"color", "#1f77b4"},  // Matplotlib blue
                {"linewidth", "1.5"}
            }
        );

        plt::xlabel(PlotSettings::X_LABEL);
        plt::ylabel(PlotSettings::Y_LABEL);
        plt::title(PlotSettings::PLOT_TITLE);

        plt::legend({
            {"loc", "upper right"},
            {"fontsize", "10"}
            });

        plt::grid({
            {"linestyle", "--"},
            {"alpha", "0.7"}
            });
    }

    void display() {
        plt::show();
    }

private:
    SignalData signal_data;
};

int main() {
    try {
        SignalVisualizer visualizer(PlotSettings::INPUT_FILENAME);
        visualizer.configure_plot();
        visualizer.display();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        std::cerr << "Plotting error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}