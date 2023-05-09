#include "fps_analyzer.h"

FpsAnalyzer::FpsAnalyzer() { }

FpsAnalyzer::~FpsAnalyzer() { }

void FpsAnalyzer::StartAnalysis() 
{
    system("/system/bin/dumpsys SurfaceFlinger --latency-clear");
}

FpsAnalyzer::FpsData FpsAnalyzer::GetFpsData()
{
    FpsData data{};

    std::vector<std::string> frameBuffers{};
    {
        char buffer[16 * 1024] = { 0 };
        std::string command = "/system/bin/dumpsys SurfaceFlinger --latency \"" + GetWindowName() + "\"";
        FILE* fp = popen(command.c_str(), "r");
        if (fp) {
            fread(buffer, sizeof(char), 16 * 1024, fp);
            pclose(fp);
        }
        frameBuffers = StrSplit(buffer, "\n");
    }

    std::vector<uint64_t> desiredPresentTimes{};
    std::vector<uint64_t> actualPresentTimes{};
    std::vector<uint64_t> frameReadyTimes{};
    for (const std::string &frameBuffer : frameBuffers) {
        unsigned long long desiredPresentTime = 0, actualPresentTime = 0, frameReadyTime = 0;
        sscanf(frameBuffer.c_str(), "%llu %llu %llu", &desiredPresentTime, &actualPresentTime, &frameReadyTime);
        if (desiredPresentTime > 0 && actualPresentTime > 0 && frameReadyTime > 0) {
            desiredPresentTimes.emplace_back(desiredPresentTime);
            actualPresentTimes.emplace_back(actualPresentTime);
            frameReadyTimes.emplace_back(frameReadyTime);
        }
    }
    if (desiredPresentTimes.size() < 3) {
        return data;
    }

    std::sort(desiredPresentTimes.begin(), desiredPresentTimes.end());
    std::sort(actualPresentTimes.begin(), actualPresentTimes.end());
    std::sort(frameReadyTimes.begin(), frameReadyTimes.end());

    {
        int frameCount = desiredPresentTimes.size();
        int timeMs = (desiredPresentTimes.back() - desiredPresentTimes.front()) / 1000000;
        data.fps = frameCount * 1000 / timeMs;
    }
    {
        constexpr int bigJankThresMs = 1000 / 30 * 3;
        constexpr int jankThresMs = 1000 / 30 * 2;

        int jankCount = 0;
        int bigJankCount = 0;
        int maxFrameTime = 0;
        for (int idx = 1; idx < frameReadyTimes.size(); idx++) {
            int frameTimeMs = (frameReadyTimes[idx] - frameReadyTimes[idx - 1]) / 1000000;
            if (frameTimeMs > maxFrameTime) {
                maxFrameTime = frameTimeMs;
            }
            if (frameTimeMs > bigJankThresMs) {
                bigJankCount++;
            } else if (frameTimeMs > jankThresMs) {
                jankCount++;
            }
        }
        data.jank = jankCount;
        data.bigJank = bigJankCount;
        data.maxFrameTime = maxFrameTime;
    }

    return data;
}

std::string FpsAnalyzer::GetWindowName()
{
    std::string windowName = "";
    FILE* fp = popen("/system/bin/dumpsys SurfaceFlinger --list", "r");
    if (fp) {
        char buffer[1024] = { 0 };
        while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            if (CheckRegex(buffer, "^SurfaceView")) {
                windowName = GetPrevString(buffer, '\n');
            }
        }
        pclose(fp);
    }

    return windowName;
}
