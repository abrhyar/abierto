#include "videos.h"
#include "ui.h"

#include <algorithm>
#include <filesystem>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

static bool isVideo(const fs::path& path) {
    if (!fs::is_regular_file(path)) {
        return false;
    }

    std::string ext = path.extension().string();

    std::transform(
        ext.begin(),
        ext.end(),
        ext.begin(),
        ::tolower
    );

    return ext == ".mp4" ||
           ext == ".mkv" ||
           ext == ".avi" ||
           ext == ".mov" ||
           ext == ".webm";
}

std::vector<MenuItem> getVideos() {
    std::vector<MenuItem> videos;

    const fs::path videoDir =
        fs::path(std::getenv("HOME")) / "media" / "videos";

    if (!fs::exists(videoDir)) {
        return videos;
    }

    for (const auto& entry : fs::directory_iterator(videoDir)) {
        if (isVideo(entry.path())) {
            videos.push_back({
                entry.path().filename().string()
            });
        }
    }

    std::sort(
        videos.begin(),
        videos.end(),
        [](const MenuItem& a, const MenuItem& b) {
            return a.name < b.name;
        }
    );

    return videos;
}

bool openVideo(const std::string& path) {
    fs::path fullPath =
        fs::path(std::getenv("HOME")) / "media" / "videos" / path;

    shutdownUI();

    pid_t pid = fork();

    if (pid == 0) {
        execlp(
            "mpv",
            "mpv",
            "--vo=drm",
            fullPath.c_str(),
            static_cast<char*>(nullptr)
        );

        _exit(1);
    }

    if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
    }

    initUI();

    return true;
}