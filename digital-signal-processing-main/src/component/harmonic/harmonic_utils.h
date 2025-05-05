#ifndef HARMONICUTILS_H
#define HARMONICUTILS_H

class HarmonicUtils {
public:
    static std::vector<double> generateHarmonic(
        int numPoints,
        double amplitude, 
        double frequency,
        double phase, 
        double deltaT
    ) {

        std::vector<double> harmonic(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            double t = i * deltaT;
            harmonic[i] = amplitude * sin(2 * M_PI * frequency * t + phase);
        }

        return harmonic;
    }

    static std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> processSignal(
        const std::vector<double>& signal, 
        double deltaT, 
        double targetDistortion
    ) {
            
        int N    = signal.size();
        int half = N / 2;

        std::vector<std::complex<double>> fftResult(N);
        
        // Выполняем FFT
        fftw_plan plan = fftw_plan_dft_r2c_1d(
            N, 
            const_cast<double*>(signal.data()), 
            reinterpret_cast<fftw_complex*>(fftResult.data()), 
            FFTW_ESTIMATE
        );

        fftw_execute(plan);
        fftw_destroy_plan(plan);

        // Рассчитываем амплитуды и частоты
        std::vector<double> amplitudes(N);
        std::vector<double> freq(N);

        for(int i = 0; i < N; ++i) {
            amplitudes[i] = std::abs(fftResult[i]) / N;
            freq[i] = i / (deltaT * N);
        }

        // Алгоритм выбора компонент
        std::vector<int> indices(half + 1);
        for(int i = 0; i <= half; ++i) indices[i] = i;
        
        std::sort(indices.begin(), indices.end(), [&amplitudes](int a, int b) {
            return amplitudes[a] > amplitudes[b];
        });

        double totalEnergy = 0.0;
        for(int i = 0; i <= half; ++i) {
            
            int idx = indices[i];
            if (idx == 0 || idx == half)
                totalEnergy += pow(amplitudes[idx], 2);
            else
                totalEnergy += 2 * pow(amplitudes[idx], 2);
        }
        
        double requiredEnergy = (1 - pow(targetDistortion/100, 2)) * totalEnergy;
        
        std::vector<std::complex<double>> filteredFFT(N, 0.0);

        double currentEnergy = 0.0;
        for(int idx : indices) {

            double componentEnergy;
            if (idx == 0 || idx == half) {
                componentEnergy = pow(amplitudes[idx], 2);
            } else {
                componentEnergy = 2 * pow(amplitudes[idx], 2);
            }

            if(currentEnergy + componentEnergy > requiredEnergy) {

                double remaining = requiredEnergy - currentEnergy;
                double scale     = sqrt(remaining / componentEnergy);

                filteredFFT[idx] = fftResult[idx] * scale;
                if (idx != 0 && idx != half)
                    filteredFFT[N - idx] = fftResult[N - idx] * scale;
                currentEnergy += remaining;
                break;
            }
            
            filteredFFT[idx] = fftResult[idx];
            if (idx != 0 && idx != half)
                filteredFFT[N - idx] = fftResult[N - idx];
            currentEnergy += componentEnergy;
        }

        // Обратное FFT
        std::vector<double> reconstructed(N);

        fftw_plan inv_plan = fftw_plan_dft_c2r_1d(
            N, 
            reinterpret_cast<fftw_complex*>(filteredFFT.data()), 
            reconstructed.data(), 
            FFTW_ESTIMATE
        );
        
        fftw_execute(inv_plan);
        fftw_destroy_plan(inv_plan);
        
        // Нормализация
        for(auto& val : reconstructed) 
            val /= N;

        return {freq, amplitudes, reconstructed};
    }
    
    static void exportData(const SignalData& data, const std::string& filename) {
        const std::string dirPath = "../data";
    
        if (!std::filesystem::exists(dirPath)) {
            try {
                std::filesystem::create_directories(dirPath);
            } catch (const std::filesystem::filesystem_error& e) {
                throw std::runtime_error("Directory creation failed: " + std::string(e.what()));
            }
        }

        std::ofstream outFile(dirPath + "/" + filename);
        if (!outFile) {
            throw std::runtime_error("Error opening output file.");
        }

        outFile << std::fixed << std::setprecision(6);

        const std::vector<double>& timeValues   = SignalTime;
        const std::vector<double>& signalValues = SignalFrequency;
        
        if (timeValues.size() != signalValues.size()) {
            throw std::runtime_error("Data size mismatch.");
        }

        for (size_t i = 0; i < timeValues.size(); ++i) {
            outFile << timeValues[i] << "\t" << signalValues[i] << "\n";
        }

        outFile.close();
        std::cout << "Data successfully written to " + filename << "\n";
    }

    static SignalData importData(const std::string& filename) {

        std::ifstream inFile("../data/" + filename);
        if (!inFile) {
            throw std::runtime_error("Error opening input file.");
        }

        SignalData  data;
        std::string line;

        while (std::getline(inFile, line)) {
            double time, value;
            std::istringstream iss(line);

            if (iss >> time >> value) {
                data.addData(time, value);
            } else {
                continue;
            }
        }

        if (SignalTime.empty()) {
            throw std::runtime_error("No valid data found in file.");
        }

        std::cout << "Successfully imported " << SignalTime.size() 
                  << " data points from " << filename << "\n";
        
        return data;
    }
};

#endif // HARMONICUTILS_H
