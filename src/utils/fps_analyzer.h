#pragma once

#include "utils/cu_misc.h"

class FpsAnalyzer {
    public:
        FpsAnalyzer();
        ~FpsAnalyzer();

        typedef struct {
            int fps;
            int jank;
            int bigJank;
            int maxFrameTime;
        } FpsData;

        void StartAnalysis();
        FpsData GetFpsData();

    private:
        std::string GetWindowName();
};