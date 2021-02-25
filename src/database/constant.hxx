#ifndef AA8179A7_F57B_4110_97DC_CEBEDA7522AB
#define AA8179A7_F57B_4110_97DC_CEBEDA7522AB

#include <filesystem>
#include <string>

std::string const pathToTemplateDatabase = std::filesystem::path{ PATH_TO_BINARY }.parent_path ().append ("template_database").append ("empty_test_db");
std::string const pathToTestDatabase = std::filesystem::path{ PATH_TO_BINARY }.append ("database").append ("game_db");

#endif /* AA8179A7_F57B_4110_97DC_CEBEDA7522AB */
