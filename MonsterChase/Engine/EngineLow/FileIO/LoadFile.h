#pragma once
#include "Dependencies/GLib/GLib.h"
#include <vector>
void* LoadFile(const char * i_pFilename, size_t & o_sizeFile);
GLib::Sprite* CreateSprite(const char * i_pFilename,unsigned int& i_Width, unsigned int& i_Height);
std::vector<uint8_t> LoadFileToBuffer(const char* i_pFilename);
