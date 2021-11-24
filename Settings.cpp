#include "Settings.h"

#include <sago/platform_folders.h>

#include <algorithm>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

bool
Settings::init()
{
  const std::string root = getPath();

  if (!createDirectory(root))
    return false;

  if (!createDirectory(appendPath(root, "profiles")))
    return false;

  return true;
}

std::string
Settings::getPath()
{
  return appendPath(sago::getConfigHome(), PROJECT_NAME);
}

std::vector<std::string>
Settings::getProfileList()
{
  const std::string profilesPath = appendPath(getPath(), "profiles.txt");

  std::vector<std::string> profileList;

  std::ifstream file(profilesPath.c_str());

  if (!file.good())
    return profileList;

  std::string line;

  while (std::getline(file, line))
    profileList.emplace_back(std::move(line));

  return profileList;
}

std::unique_ptr<Profile>
Settings::getProfile(const std::string& profilePath)
{
  std::unique_ptr<Profile> profile(new Profile());

  if (!profile->open(profilePath))
    return nullptr;

  return profile;
}

bool
Settings::createProfile()
{
  std::vector<std::string> profileList = getProfileList();

  std::string nextProfilePath = getNextProfilePath();

  if (!incrementNextProfileID())
    return false;

  Profile newProfile;

  if (!newProfile.save(nextProfilePath))
    return false;

  profileList.emplace_back(std::move(nextProfilePath));

  return setProfileList(profileList);
}

bool
Settings::renameProfile(const std::string& profilePath, const std::string& name)
{
  Profile profile;

  if (!profile.open(profilePath))
    return false;

  profile.name = name;

  return profile.save(profilePath);
}

bool
Settings::destroyProfile(const std::string& profilePath)
{
  std::vector<std::string> profileList = getProfileList();

  auto newEnd = std::remove(profileList.begin(), profileList.end(), profilePath);

  if (newEnd == profileList.end())
    return false;

  profileList.erase(newEnd, profileList.end());

  if (!setProfileList(profileList))
    return false;

  return true;
}

bool
Settings::incrementNextProfileID()
{
  const std::string path = appendPath(getPath(), "next_profile_id.txt");

  const int id = getFileAsInt(path, 0) + 1;

  std::ofstream output(path.c_str());

  if (!output.good())
    return false;

  if (!(output << id))
    return false;

  output << std::endl;

  return true;
}

bool
Settings::setProfileList(const std::vector<std::string>& profileList)
{
  std::ofstream file(appendPath(getPath(), "profiles.txt"));

  if (!file.good())
    return false;

  for (const std::string& profile : profileList)
    file << profile << std::endl;

  return true;
}

std::string
Settings::getNextProfilePath()
{
  const int id = getFileAsInt(appendPath(getPath(), "next_profile_id.txt"), 0);
  std::ostringstream tmpStream;
  tmpStream << id;
  tmpStream << ".txt";
  return appendPath(getPath(), appendPath("profiles", tmpStream.str()));
}

int
Settings::getFileAsInt(const std::string& path, int fallbackValue)
{
  std::ifstream file(path.c_str());

  if (!file.good())
    return fallbackValue;

  int value = 0;

  if (!(file >> value))
    return fallbackValue;

  return value;
}

std::string
Settings::appendPath(const std::string& parent, const std::string& child)
{
#ifdef _WIN32
  return parent + "\\" + child;
#else
  return parent + "/" + child;
#endif
}

bool
Settings::createDirectory(const std::string& path)
{
#ifdef _WIN32
  CreateDirectoryA(path.c_str(), nullptr);
  return true;
#elif __unix__
  mkdir(path.c_str(), 0700);
  return true;
#else
  (void)path;
  return false;
#endif
}
