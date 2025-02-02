#include <DHG/Parser.h>
#include <DHG/Reflector.h>
#include <DHG/Generator.h>
#include <DHG/MetaData.h>
#include <Platform/FileStream.h>
#include <Core/Map.h>
#include <Platform/FileSystem.h>
#include <future>
#include <Platform/Mutex.h>
#include <Platform/LockGuard.h>
#include <Core/HashMap.h>
#include <Core/Timer.h>
#include <iostream>
#include <mutex>

#undef CreateDirectory

DJINN_NS2(DHG);

class Semaphore {
public:
    Semaphore(int n) : count{n} {}

    void notify() {
        std::unique_lock<std::mutex> l(m);
        ++count;
        cv.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [this] { return count != 0; });
        --count;
    }

private:
    std::mutex m;
    std::condition_variable cv;
    int count;
};
class Critical_section {
public:
    Critical_section(Semaphore &ss) : s{ss} {
        s.wait();
    }

    ~Critical_section() {
        s.notify();
    }

private:
    Semaphore &s;
};

int main(int argc, char *argv[]) {
    const char *configPath = nullptr;
#ifdef DJINN_PLATFORM_WINDOWS
    if (argc != 2) {
        std::cerr << "[ DHG] Djinn header generator expects one argument but got " << argc << " arguments\n";
        return 1;
    }
    configPath = argv[1];
#else
    if (argc != 2) {
        std::cerr << "[ DHG] Djinn header generator expects one argument but got " << argc << " arguments\n";
        return 1;
    }
    configPath = argv[1];
#endif

    // Record time
    Core::Timer timer;

    // Attempt to open config file
    Platform::FileStream configStream(Platform::FileMode::eRead, configPath);
    if (!configStream.IsGood()) {
        std::cerr << "[ DHG] Failed to open configuration file '" << configPath << "'\n";
        return 1;
    }

    // Stream helper
    Core::StringStream config(&configStream);

    constexpr UInt32 kCMakeVersion    = 1;
    constexpr UInt32 kInternalVersion = 18;

    // Read version UID
    UInt32 version_uid = std::atoi(config.ReadLine().AsCharArray());
    if (version_uid != kCMakeVersion) {
        std::cerr << "[ DHG] Buildsystem generated an incompatible DHG version, expected " << kCMakeVersion << "\n";
        return kCMakeVersion;
    }

    // Read header
    Core::String projectName = config.ReadLine();
    Core::String outputDirectory = config.ReadLine();

    // Common directories
    Core::String cacheDirectory = outputDirectory + "/" + projectName + ".Cache";
    Core::String diagnosticDirectory = outputDirectory + "/" + projectName + ".Diagnostic";
    Core::String stampPath = outputDirectory + "/" + projectName + ".stamps";

    // Read output directory
    if (!Platform::FileSystem::Exists(cacheDirectory)) Platform::FileSystem::CreateDirectory(cacheDirectory);
    if (!Platform::FileSystem::Exists(diagnosticDirectory)) Platform::FileSystem::CreateDirectory(diagnosticDirectory);

    // Describe parser job
    ParserConfig job;
    Core::Array<Core::String> headers;

    // Read headers
    Core::Array<Core::String> *entry = nullptr;
    while (!configStream.IsEOS()) {
        auto line = config.ReadLine();

        if (line == "#include") {
            entry = &job.includes;
        } else if (line == "#header") {
            entry = &headers;
        } else if (line == "#preprocessor") {
            entry = &job.preprocessors;
        } else if (line == "#args") {
            entry = &job.args;
        } else {
            if (!entry) {
                std::cerr << "[ DHG] " << projectName.AsCharArray() << " - Expected entry, invalid config\n";
                return 1;
            }

            if (!line.EmptyWhitespace())
                entry->Add(line);
        }
    }

    // Current file stamps
    Core::HashMap<Core::String, Int64> stamps;

    UInt32 lastVersion = 0;

    // Attempt to open stamp file
    Platform::FileStream stampConfigStream(Platform::FileMode::eRead, stampPath);
    if (stampConfigStream.IsGood()) {
        Core::StringStream stampConfig(&stampConfigStream);

        lastVersion = std::atoll(stampConfig.ReadLine().AsCharArray());

        // Read all stamps
        while (!stampConfigStream.IsEOS()) {
            // Read file
            Core::String file = stampConfig.ReadLine();

            // ...
            stamps[file] = std::atoll(stampConfig.ReadLine().AsCharArray());
        }
    }
    stampConfigStream.Close();

    bool stampsOutOfDate = false;

    if (lastVersion != kInternalVersion)
    {
        stamps.Clear();
        stampsOutOfDate = true;
    }

    // Pre-visit stamp map
    for (auto &header : headers) {
        if (!stamps.Contains(header)) {
            stampsOutOfDate = true;
        }
    }

    // Header removed?
    stamps.RemoveAll([&](auto&& kv) {
        if (!headers.Contains(kv.first)) {
            std::cerr << "[ DHG] " << projectName.AsCharArray() << " - Header removed: '" << kv.first.AsCharArray() << "'\n";
            stampsOutOfDate = false;
            return true;
        }
        return false;
    });

    // General mutex
    Platform::Mutex mutex;

    // Number of skipped headers
    Core::AtomicCounter skippedHeaderCount{0};

    // Error code
    bool isError = false;

    // ...
    Semaphore task_limiter(std::thread::hardware_concurrency() / 2 + 1 );

    // Process all headers
    Core::Array<std::shared_future<void>> tasks;
    for (auto &header : headers) {
        tasks.Add(std::async([&, header] {
            Critical_section _(task_limiter);

            // Record time
            Core::Timer headerTimer;

            // Skip regeneration if needed
            Int64 stamp = Platform::FileSystem::GetFileTimeStamp(header);
            if (stamps.GetOrDefault(header, -1) == stamp) {
                skippedHeaderCount++;
                return;
            } else {
                stamps[header] = stamp;
            }

            // Prepare parser
            Parser parser;

            // Parse
            auto parserBlob = parser.Parse(job, header);
            if (!parserBlob) {
                Platform::LockGuard guard(mutex);
                std::cerr << "[ DHG] " << projectName.AsCharArray() << " - Failed to parse: '" << header.AsCharArray() << "' (Did you forget to reload cmake?)\n";
                isError = true;
                return;
            }

            // Temporary md
            MetaData md;

            // Reflect data into md
            Reflector reflector(&md);
            reflector.Reflect(parserBlob, header);

            // Open cache
            Platform::FileStream fs(Platform::FileMode::eWrite, cacheDirectory + "/" + Core::ComputeCRC64(header.AsCharArray()) + ".hmd");
            if (!fs.IsGood()) {
                Platform::LockGuard guard(mutex);
                std::cerr << "[ DHG] " << projectName.AsCharArray() << " - Failed open cache for: '" << header.AsCharArray() << "'\n";
                isError = true;
                return;
            }

            // Serialize metadata
            Core::BinaryStream out(&fs);
            md >> out;

            // Diagnostic
            {
                Platform::LockGuard guard(mutex);
                std::cout << "[ DHG] " << projectName.AsCharArray() << " - Built meta data for " << header.AsCharArray() << " (" << headerTimer.Delta() << "s)\n";
                std::cout.flush();
            }
        }));
    }

    // Wait for all tasks
    for (auto &task : tasks) {
        task.wait();
    }

    // Error?
    if (isError) {
        return 1;
    }

    // Needs regeneration?
    if (skippedHeaderCount != static_cast<long long>(headers.Size()) || stampsOutOfDate || !Platform::FileSystem::Exists(cacheDirectory + "/dummy")) {
        Platform::FileStream dummy(Platform::FileMode::eWrite, cacheDirectory + "/dummy");

        // Deserialize all metadata as a single collection
        MetaData md;
        for (auto &header : headers) {
            // Open cache
            Platform::FileStream fs(Platform::FileMode::eRead, cacheDirectory + "/" + Core::ComputeCRC64(header.AsCharArray()) + ".hmd");
            if (!fs.IsGood()) {
                Platform::LockGuard guard(mutex);
                std::cerr << "[ DHG] " << projectName.AsCharArray() << " - Failed open cache for: '" << header.AsCharArray() << "'\n";
                return 1;
            }

            // Deserialize metadata
            Core::BinaryStream in(&fs);
            md << in;
        }

        // Describe generator job
        GeneratorJob genJob;
        genJob.md = &md;
        genJob.output = (outputDirectory + "/" + projectName + ".DHG.cpp").AsCharArray();
        genJob.graphOutput = (diagnosticDirectory + "/" + "include.graph").AsCharArray();
        genJob.graphShortOutput = (diagnosticDirectory + "/" + "include.short.graph").AsCharArray();
        genJob.headers = headers;

        // Generate reflected data
        Generator generator;
        if (!generator.Generate(genJob)) {
            Platform::LockGuard guard(mutex);
            std::cerr << "[ DHG] " << projectName.AsCharArray() << " - Failed to generate metadata\n";
        }

        // Open stamp file
        Platform::FileStream outStampsStream(Platform::FileMode::eWrite, stampPath);
        if (!outStampsStream.IsGood()) {
            std::cerr << "[ DHG] " << projectName.AsCharArray() << " - Failed to open stamp file for writing: '" << stampPath.AsCharArray() << "'\n";
            return 1;
        }

        // Write stamps
        Core::StringStream outStamps(&outStampsStream);
        outStamps << kInternalVersion << "\n";
        for (const auto &kv : stamps) {
            outStamps << kv.first << "\n" << kv.second << "\n";
        }

        // ...
        if (skippedHeaderCount > 0) {
            std::cout << "[ DHG] Reflected target " << projectName.AsCharArray() << " - " << skippedHeaderCount << " of " << headers.Size() << " up to date (" << timer.Delta() << "s)\n";
        } else {
            std::cout << "[ DHG] Reflected target " << projectName.AsCharArray() << " (" << timer.Delta() << "s)\n";
        }
    } else {
        std::cout << "[ DHG] Reflected target " << projectName.AsCharArray() << " - Up to date (" << timer.Delta() << "s)\n";
    }
    return 0;
}