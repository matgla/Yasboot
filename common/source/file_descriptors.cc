module;
/**
 * file_descriptors.cpp
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

#include <bitset>
#include <cstdint>

export module yasboot.filesystem.file_descriptors;

export namespace yasboot::fs {
class FileDescriptors {
public:
  static FileDescriptors &get();

  int8_t allocate();
  void release(int8_t fd);

private:
  FileDescriptors();

  std::bitset<8> file_descriptors_;
};
} // namespace yasboot::fs

namespace yasboot::fs {

FileDescriptors &FileDescriptors::get() {
  static FileDescriptors instance;
  return instance;
}

int8_t FileDescriptors::allocate() {
  for (int8_t i = 0; i < static_cast<int8_t>(file_descriptors_.size()); i++) {
    if (!file_descriptors_.test(i)) {
      file_descriptors_.set(i);
      return i;
    }
  }
  return -1;
}

void FileDescriptors::release(int8_t fd) { file_descriptors_.reset(fd); }

FileDescriptors::FileDescriptors() = default;

} // namespace yasboot::fs
