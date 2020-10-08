#include <iostream>
#include <memory>
#include <vector>

#include "../adventure.h"
#include "../utils.h"

const int smallTestSize = 100;
const int bigTestSize = 100000;

const int smallThreadsGroupSize = 4;
const int bigThreadsGroupSize = 16;

void runAndVerify(Adventure &adventure, std::vector<GrainOfSand> &grains,
                  std::vector<GrainOfSand> &result) {
  adventure.arrangeSand(grains);
  assert_msg(grains == result, "Wrong sand arrangement");
}

void runAndVerifyAndLog(Adventure &adventure, std::vector<GrainOfSand> &grains,
                        std::vector<GrainOfSand> &result, const int testSize,
                        const bool sorted) {
  std::cout << "[n = " << testSize << ", sorted = " << sorted << "] - ";

  runAndPrintDuration([&adventure, &grains, &result]() {
    runAndVerify(adventure, grains, result);
  });
}

void runTests(Adventure &adventure, std::vector<GrainOfSand> &sGrains,
              std::vector<GrainOfSand> &sResult,
              std::vector<GrainOfSand> &sGrains2,
              std::vector<GrainOfSand> &sResult2,
              std::vector<GrainOfSand> &bGrains,
              std::vector<GrainOfSand> &bResult,
              std::vector<GrainOfSand> &bGrains2,
              std::vector<GrainOfSand> &bResult2,
              const std::string &adventureType, const int groupSize) {
  std::cout << "=== " << adventureType << " (team size = " << groupSize
            << ") ===" << std::endl;

  runAndVerifyAndLog(adventure, sGrains, sResult, smallTestSize, false);

  runAndVerifyAndLog(adventure, sGrains2, sResult2, smallTestSize, true);

  runAndVerifyAndLog(adventure, bGrains, bResult, bigTestSize, false);

  runAndVerifyAndLog(adventure, bGrains2, bResult2, bigTestSize, true);
}

int main(int argc, char **argv) {
  auto lonesomeAdventure = std::shared_ptr<Adventure>(new LonesomeAdventure{});
  auto teamAdventureSmall =
      std::shared_ptr<Adventure>(new TeamAdventure(smallThreadsGroupSize));
  auto teamAdventureBig =
      std::shared_ptr<Adventure>(new TeamAdventure(bigThreadsGroupSize));

  std::vector<GrainOfSand> smallTestVector(smallTestSize);
  std::generate(smallTestVector.begin(), smallTestVector.end(), std::rand);

  std::vector<GrainOfSand> smallTestVectorResult = smallTestVector;
  std::sort(smallTestVectorResult.begin(), smallTestVectorResult.end());

  std::vector<GrainOfSand> bigTestVector(bigTestSize);
  std::generate(bigTestVector.begin(), bigTestVector.end(), std::rand);

  std::vector<GrainOfSand> bigTestVectorResult = bigTestVector;
  std::sort(bigTestVectorResult.begin(), bigTestVectorResult.end());

  std::vector<GrainOfSand> smallTestVectorReverseSorted(smallTestSize);
  std::generate(smallTestVectorReverseSorted.begin(),
                smallTestVectorReverseSorted.end(), std::rand);
  std::sort(smallTestVectorReverseSorted.begin(),
            smallTestVectorReverseSorted.end());
  std::reverse(smallTestVectorReverseSorted.begin(),
               smallTestVectorReverseSorted.end());

  std::vector<GrainOfSand> smallTestVectorReverseSortedResult =
      smallTestVectorReverseSorted;
  std::sort(smallTestVectorReverseSortedResult.begin(),
            smallTestVectorReverseSortedResult.end());

  std::vector<GrainOfSand> bigTestVectorReverseSorted(bigTestSize);
  std::generate(bigTestVectorReverseSorted.begin(),
                bigTestVectorReverseSorted.end(), std::rand);
  std::sort(bigTestVectorReverseSorted.begin(),
            bigTestVectorReverseSorted.end());
  std::reverse(bigTestVectorReverseSorted.begin(),
               bigTestVectorReverseSorted.end());

  std::vector<GrainOfSand> bigTestVectorReverseSortedResult =
      bigTestVectorReverseSorted;
  std::sort(bigTestVectorReverseSortedResult.begin(),
            bigTestVectorReverseSortedResult.end());

  runTests(*lonesomeAdventure, smallTestVector, smallTestVectorResult,
           smallTestVectorReverseSorted, smallTestVectorReverseSortedResult,
           bigTestVector, bigTestVectorResult, bigTestVectorReverseSorted,
           bigTestVectorReverseSortedResult, "lonesome", 1);

  runTests(*teamAdventureSmall, smallTestVector, smallTestVectorResult,
           smallTestVectorReverseSorted, smallTestVectorReverseSortedResult,
           bigTestVector, bigTestVectorResult, bigTestVectorReverseSorted,
           bigTestVectorReverseSortedResult, "team small",
           smallThreadsGroupSize);

  runTests(*teamAdventureBig, smallTestVector, smallTestVectorResult,
           smallTestVectorReverseSorted, smallTestVectorReverseSortedResult,
           bigTestVector, bigTestVectorResult, bigTestVectorReverseSorted,
           bigTestVectorReverseSortedResult, "team big", bigThreadsGroupSize);

  return 0;
}
