#include <chrono>

class Chronometer {
   public:
    Chronometer() : start_time(std::chrono::high_resolution_clock::now()) {}

    void reset() { start_time = std::chrono::high_resolution_clock::now(); }

    double elapsed() const {
        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        return elapsed_seconds.count();
    }

    void print_elapsed(std::string checkpoint) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = end_time - start_time;

        auto hours = std::chrono::duration_cast<std::chrono::hours>(elapsed_time).count();
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(elapsed_time).count() % 60;
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count() % 60;
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() % 1000;
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count() % 1000;
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_time).count() % 1000;

        std::cout << "✅ [" << checkpoint << "] elapsed time: ";
        int printed = 0;

        auto print = [&] (auto quantity, std::string unit_of_measure) {
            if (++printed > 2) {
                return;
            } 
            std::cout << quantity << unit_of_measure << " ";
        } ;

        if (hours > 0) print(hours, "h");
        if (minutes > 0) print(minutes, "m");
        if (seconds > 0) print(seconds, "s");
        if (milliseconds > 0) print(milliseconds, "ms");
        if (microseconds > 0) print(microseconds, "us");
        if (nanoseconds > 0) print(nanoseconds, "ns");
        std::cout << std::endl << std::endl;
    }

   private:
    std::chrono::high_resolution_clock::time_point start_time;
};