#include "lxc-container.h"
#include <sys/stat.h>
LxcContainer::LxcContainer(const char *name, const Method action) {
    this->m_action = action;
    this->m_name = name;

}

LxcContainer::~LxcContainer() {}

void LxcContainer::run() {
    if (m_action == Method::ENABLE) {
        create("busybox");
        start();
    } else if (m_action == Method::DISABLE) {
        stop();
    } else if (m_action == Method::RECONFIGURE) {
        reconfigure();
        stop();
        start();
    } else if (m_action == Method::RESET) {
        stop();
        destroy();
        create("busybox");
        start();
    } else if (m_action == Method::DESTROY) {
        destroy();
    } else {
        throw std::runtime_error("Invalid action");
    }

    if (m_callback != nullptr) {
        m_callback(m_data);
    }
}

void LxcContainer::create(std::string type) {
    container = lxc_container_new(m_name.c_str(), nullptr);
    if (!container) {
        throw std::runtime_error("Failed to setup lxc_container struct");
    }

    if (container->is_defined(container)) {
        throw std::runtime_error("Container already exists");
    }

// // Set up the overlay storage
// std::string overlay_dir = "/tmp/overlay/dir";
// std::string overlay_lower = "/tmp/lower/dir";
// std::string overlay_upper = "/tmp/upper/dir";
// std::string overlay_workdir = "/tmp/work/dir";
// if (mkdir(overlay_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
//     throw std::runtime_error("Failed to create overlay directory");
// }
// if (mkdir(overlay_lower.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
//     throw std::runtime_error("Failed to create overlay lower directory");
// }
// if (mkdir(overlay_upper.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
//     throw std::runtime_error("Failed to create overlay upper directory");
// }
// if (mkdir(overlay_workdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
//     throw std::runtime_error("Failed to create overlay work directory");
// }

// // Set up the squashfs storage
// std::string squashfs_file = "/tmp/squashfs/file";
// std::string squashfs_mount = "/tmp/squashfs/mount";
// if (mkdir(squashfs_mount.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
//     throw std::runtime_error("Failed to create squashfs mount directory");
// }
// if (system(("mksquashfs " + overlay_upper + " " + squashfs_file).c_str()) == -1) {
//     throw std::runtime_error("Failed to create squashfs file");
// }

 // Create the container
 if (!container->createl(container, "busybox", nullptr, nullptr, LXC_CREATE_QUIET, nullptr)) {
     throw std::runtime_error("Failed to create container");
 }

// // Set up the overlay and squashfs as the container's storage
// if (!container->set_config_item(container, "lxc.rootfs.path", overlay_upper.c_str())) {
//     throw std::runtime_error("Failed to set container rootfs path");
// }
// if (!container->set_config_item(container, "lxc.rootfs.backend", "overlayfs")) {
//     throw std::runtime_error("Failed to set container rootfs backend");
// }
// if (!container->set_config_item(container, "lxc.rootfs.options", ("lowerdir=" + overlay_lower + ",upperdir=" + overlay_upper + ",workdir=" + overlay_workdir).c_str())) {
//     throw std::runtime_error("Failed to set container rootfs options");
// }
// if (!container->set_config_item(container, "lxc.rootfs.mount", squashfs_mount.c_str())) {
//     throw std::runtime_error("Failed to set container rootfs mount");
// }
// if (!container->set_config_item(container, "lxc.rootfs.backend", "squashfs")) {
//     throw std::runtime_error("Failed to set container rootfs backend");
// }
// if (!container->set_config_item(container, "lxc.rootfs.options", ("loop=" + squashfs_file).c_str())) {
//     throw std::runtime_error("Failed to set container rootfs options");
// }

}

void LxcContainer::start() {
    container = lxc_container_new(m_name.c_str(), nullptr);
    if (!container) {
        throw std::runtime_error("Failed to setup lxc_container struct");
    }

    if (!container->start(container, 0, nullptr)) {
        throw std::runtime_error("Failed to start the container");
    }
}

void LxcContainer::stop() {
    container = lxc_container_new(m_name.c_str(), nullptr);
    if (!container) {
        throw std::runtime_error("Failed to setup lxc_container struct");
    }

    if (!container->shutdown(container, 30)) {
        if (!container->stop(container)) {
            throw std::runtime_error("Failed to kill the container.");
        }
    }
}

void LxcContainer::reconfigure() {
}

void LxcContainer::destroy() {
    container = lxc_container_new(m_name.c_str(), nullptr);
    if (!container) {
        throw std::runtime_error("Failed to setup lxc_container struct");
    }

    if (!container->destroy(container)) {
        throw std::runtime_error("Failed to destroy the container.");
    }
}
