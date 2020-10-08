#include <iostream>
#include <memory>
#include <vector>

#include "../adventure.h"
#include "../utils.h"

const int smallTestSize = 100;
const int bigTestSize = 1000000;

const int smallThreadsGroupSize = 4;
const int bigThreadsGroupSize = 16;

void runAndVerify(Adventure &adventure, std::vector<Crystal> &crystals,
                  Crystal result) {
  Crystal crystal = adventure.selectBestCrystal(crystals);
  assert_msg(crystal == result, "Wrong crystal selection");
}

void runAndVerifyAndLog(Adventure &adventure, std::vector<Crystal> &crystals,
                        Crystal result, const int testSize) {
  std::cout << "[n = " << testSize << "] - ";

  runAndPrintDuration([&adventure, &crystals, &result]() {
    runAndVerify(adventure, crystals, result);
  });
}

void runTests(Adventure &adventure, std::vector<Crystal> &smallCrystals,
              Crystal smallResult, std::vector<Crystal> &bigCrystals,
              Crystal bigResult, const std::string &adventureType,
              const int groupSize) {
  std::cout << "=== " << adventureType << " (team size = " << groupSize
            << ") ===" << std::endl;

  runAndVerifyAndLog(adventure, smallCrystals, smallResult, smallTestSize);

  runAndVerifyAndLog(adventure, bigCrystals, bigResult, bigTestSize);
}

int main(int argc, char **argv) {
  auto lonesomeAdventure = std::shared_ptr<Adventure>(new LonesomeAdventure{});
  auto teamAdventureSmall =
      std::shared_ptr<Adventure>(new TeamAdventure(smallThreadsGroupSize));
  auto teamAdventureBig =
      std::shared_ptr<Adventure>(new TeamAdventure(bigThreadsGroupSize));

  std::vector<Crystal> smallTestVector(smallTestSize);
  std::generate(smallTestVector.begin(), smallTestVector.end(), std::rand);
  Crystal smallTestResult =
      *std::max_element(smallTestVector.begin(), smallTestVector.end());

  std::vector<Crystal> bigTestVector(bigTestSize);
  std::generate(bigTestVector.begin(), bigTestVector.end(), std::rand);
  Crystal bigTestResult =
      *std::max_element(bigTestVector.begin(), bigTestVector.end());

  runTests(*lonesomeAdventure, smallTestVector, smallTestResult, bigTestVector,
           bigTestResult, "lonesome", 1);

  runTests(*teamAdventureSmall, smallTestVector, smallTestResult, bigTestVector,
           bigTestResult, "team small", smallThreadsGroupSize);

  runTests(*teamAdventureBig, smallTestVector, smallTestResult, bigTestVector,
           bigTestResult, "team big", bigThreadsGroupSize);

  return 0;
}
