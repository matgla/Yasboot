/**
 * filesystem_mount_points.cpp
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

#include "common/filesystem/filesystem_mount_points.hpp"

namespace yasboot::fs
{

int FilesystemMountPoints::register_mount_point(
  std::string_view path, std::unique_ptr<Filesystem> filesystem)
{
  if (mount_points_.count(std::string(path)))
  {
    return -1;
  }

  mount_points_.emplace(std::string(path), std::move(filesystem));
  return 0;
}

FilesystemMountPoints &FilesystemMountPoints::get()
{
  static FilesystemMountPoints instance;
  return instance;
}

std::pair<Filesystem *, std::string_view> FilesystemMountPoints::get_mount_point(
  std::string_view pathname)
{
  std::pair<Filesystem *, std::string_view> result{};
  for (const auto &[path, filesystem] : mount_points_)
  {
    if (pathname.starts_with(path) && path.size() > result.second.size())
    {
      result = std::make_pair(filesystem.get(), pathname.substr(path.size()));
    }
  }

  return result;
}

Filesystem *FilesystemMountPoints::get_filesystem_for_fd(int fd)
{
  for (const auto &[path, fs] : mount_points_)
  {
    if (fs->has_fd(fd))
    {
      return fs.get();
    }
  }
  return nullptr;
}

} // namespace yasboot::fs
