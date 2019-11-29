#pragma once
#ifndef ENDFNUMBERS
#define ENDFNUMBERS

#include <map>
#include <iostream> 
#include <iterator> 
#include <algorithm>    // std::find_if

void elToMap(std::map<int, int>* fmap);
std::pair<int, int> reverseLookup(std::map<int, int>* map, int key);

#endif // ENDFNUMBERS