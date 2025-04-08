#pragma once
#include <string>

bool ValidateCredentials(const std::string& username, const std::string& password);

std::string GetAccountID(const std::string& username);