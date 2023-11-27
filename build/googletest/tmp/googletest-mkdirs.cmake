# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/src/googletest"
  "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/src/googletest-build"
  "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest"
  "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/tmp"
  "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/src/googletest-stamp"
  "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/src"
  "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/gandhi/Projects/ZespolowyGit/tachion-engine/build/googletest/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
