add_executable(CContainersSpeedCheck Main.cpp PVector_vs_stdvector.cpp PVector_vs_stdvector.hpp PHashMap_vs_stdunorderedmap.cpp PHashMap_vs_stdunorderedmap.hpp PHeap_vs_stdpriorityqueue.cpp PHeap_vs_stdpriorityqueue.hpp PVectorHeap_vs_stdpriorityqueue.cpp PVectorHeap_vs_stdpriorityqueue.hpp)
target_link_libraries(CContainersSpeedCheck CContainers)
