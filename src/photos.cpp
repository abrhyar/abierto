#include "photos.h"
#include "ui.h"

#include <algorithm>
#include <filesystem>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

static bool isImage(const fs::path& path) {
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

    return ext == ".jpg" ||
           ext == ".jpeg" ||
           ext == ".png" ||
           ext == ".gif" ||
           ext == ".bmp" ||
           ext == ".webp";
}

std::vector<MenuItem> getPhotos() {
    std::vector<MenuItem> photos;

    const fs::path photoDir = fs::path(std::getenv("HOME")) / "media" / "photos";

    if (!fs::exists(photoDir)) {
        return photos;
    }

    for (const auto& entry : fs::directory_iterator(photoDir)) {
        if (isImage(entry.path())) {
            photos.push_back({
                entry.path().filename().string()
            });
        }
    }

    std::sort(
        photos.begin(),
        photos.end(),
        [](const MenuItem& a, const MenuItem& b) {
            return a.name < b.name;
        }
    );

    return photos;
}

bool openPhoto(const std::string& path) {
    fs::path fullPath =
        fs::path(std::getenv("HOME")) / "media" / "photos" / path;

    shutdownUI();

    pid_t pid = fork();

    if (pid == 0) {
        execlp(
            "fbi",
            "fbi",
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