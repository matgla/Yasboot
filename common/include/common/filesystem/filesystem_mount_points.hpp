/**
 * filesystem_mount_points.hpp
 *
 * Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General
 * Public License along with this program. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "common/filesystem/filesystem.hpp"

namespace yasboot::fs
{

class FilesystemMountPoints
{
public:
  using MountPoints = std::map<std::string, std::unique_ptr<Filesystem>>;

  int register_mount_point(std::string_view path,
                           std::unique_ptr<Filesystem> filesystem);

  std::pair<Filesystem *, std::string_view> get_mount_point(
    std::string_view pathname);
  Filesystem *get_filesystem_for_fd(int fd);

  static FilesystemMountPoints &get();

private:
  FilesystemMountPoints() = default;
  MountPoints mount_points_;
};

} // namespace yasboot::fs
