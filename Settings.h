#pragma once

#include "Profile.h"

#include <memory>
#include <string>
#include <vector>

class Settings final
{
public:
  static bool init();

  static std::string getPath();

  static std::vector<std::string> getProfileList();

  static bool saveProfileList(const std::vector<std::string>& profiles);

  static std::unique_ptr<Profile> getProfile(const std::string& profilePath);

  static bool createProfile();

  static bool destroyProfile(const std::string& profilePath);

  static bool renameProfile(const std::string& profilePath,
                            const std::string& newName);

private:
  static bool incrementNextProfileID();

  static std::string getNextProfilePath();

  static int getFileAsInt(const std::string& path, int fallbackValue);

  static bool setProfileList(const std::vector<std::string>& profileList);

private:
  static std::string appendPath(const std::string& parent,
                                const std::string& child);

  static bool createDirectory(const std::string& path);
};
