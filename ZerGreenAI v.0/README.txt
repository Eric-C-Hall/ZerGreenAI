-----------------------------
ZerGreenAI

Written by Eric Hall

Using the BWAPI and BWEM libraries
-----------------------------

The AI requires that the files required for BWEM be placed in the folder Source/BWEM/ and the files for EasyBMP (used in BWEM) be placed in the folder Source/EasyBMP_1.06/

It also requires that the files required for BWAPI be placed in the files ../include and ../lib

The AI requires that the files required for justinhj's AStar implementation be placed in the folder Source/astar-algorithm-cpp-master/

Also, the read directory should be placed in bwapi-data in the Starcraft directory

If someone other than me wants to try to compile this AI they should expect to have to put in a little work.

Honestly, it isn't written particularly robustly, so it might be broken if you try on your setup if is different to mine.

-----------------------------

Sources:

BWAPI and BWEM (standard)

justinhj's AStar implementation (https://github.com/justinhj/astar-algorithm-cpp)

IMP scouting based on imp42's post here: http://www.teamliquid.net/blogs/514487-towards-a-good-sc-bot-p3-scouting

N00byEdge's FAP here: https://github.com/N00byEdge/Neohuman

OpenBW here: https://github.com/OpenBW/openbw

OpenBWAPI here: https://github.com/OpenBW/bwapi

General help from members of the SSCAIT discord
